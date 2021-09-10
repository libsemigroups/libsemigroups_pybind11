# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for matrices.
"""

import pytest

from libsemigroups_pybind11 import (
    BMat,
    IntMat,
    MaxPlusMat,
    MinPlusMat,
    ProjMaxPlusMat,
    MaxPlusTruncSemiring,
    MinPlusTruncSemiring,
)


@pytest.fixture
def matrix_types():
    return (BMat, IntMat, MaxPlusMat, MinPlusMat, ProjMaxPlusMat)


@pytest.fixture
def semiring_types():
    return (MaxPlusTruncSemiring, MinPlusTruncSemiring)


def test_constructors(matrix_types):
    for T in matrix_types:
        # number rows/cols
        x = T(10, 10)
        x.number_of_rows() == 10
        x.number_of_cols() == 10

        # copy
        y = T(x)
        assert not y is x
        assert not x is y
        assert y.number_of_rows() == 10
        assert y.number_of_cols() == 10

        # from list of lists
        x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        assert x.number_of_rows() == 3
        assert x.number_of_cols() == 3
        assert T([[x[i, j] for i in range(3)] for j in range(3)]) == x

        # T.make_identity (static)
        l = x.one()
        o = x.zero()
        assert T.make_identity(3) == T([[l, o, o], [o, l, o], [o, o, l]])


def test_comparison_ops(matrix_types):
    for T in matrix_types:
        x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = T([[1, 1, 1], [0, 0, 0], [1, 1, 1]])
        assert x < y
        assert x == x
        assert y == y
        assert y > x
        assert y != x
        assert x != y


def test_add_ops(matrix_types):
    for T in matrix_types:
        x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = T([[0, 0, 0], [0, 1, 0], [1, 1, 1]])
        z = x + y
        x += y
        assert x == z


def test_mul_ops(matrix_types):
    for T in matrix_types:
        x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = T([[0, 0, 0], [0, 1, 0], [1, 1, 1]])
        z = x * y
        t = T.make_identity(3)
        t.product_inplace(x, y)
        assert z == t


def test_transpose(matrix_types):
    for T in matrix_types:
        x = T([[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        y = T(x)
        x.transpose()
        assert x == T([[0, 0, 0], [1, 0, 0], [1, 1, 0]])
        x.transpose()
        assert x == y


def test_swap(matrix_types):
    for T in matrix_types:
        x = T([[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        y = T(x)
        x.transpose()
        x.swap(y)
        assert x == T([[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        assert y == T([[0, 0, 0], [1, 0, 0], [1, 1, 0]])
        x.transpose()
        assert x == y


def test_rows(matrix_types):
    for T in matrix_types:
        if T is ProjMaxPlusMat:
            # Rows of ProjMaxPlusMat's are MaxPlusMat's so these tests don't
            # work (there's a good reason for this, read the comment in the
            # code in libsemigroups/matrix.hpp
            continue
        x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        assert x.row(0) == T([[0, 1, 1]])
        assert x.row(1) == T([[1, 0, 1]])
        assert x.row(2) == T([[1, 1, 1]])
        assert x.rows() == [T([[0, 1, 1]]), T([[1, 0, 1]]), T([[1, 1, 1]])]


def test_repr(matrix_types):
    for T in matrix_types:
        x = T([[0, 1], [1, 0]])
        n = T.__name__
        assert str(x)[: len(n)] == n


def test_semiring(semiring_types):
    for T in semiring_types:
        R = T(11)
        assert R.threshold() == 11
        assert R.plus(R.zero(), R.zero()) == R.zero()
        assert R.plus(R.zero(), R.one()) == R.one()
        assert R.plus(R.one(), R.zero()) == R.one()
        assert R.prod(R.zero(), R.zero()) == R.zero()
        assert R.prod(R.zero(), R.one()) == R.zero()
        assert R.prod(R.one(), R.zero()) == R.zero()
        assert R.prod(R.zero(), R.one()) == R.zero()
        assert R.prod(R.one(), R.one()) == R.one()
