# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for matrices.
"""

# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=invalid-name, redefined-outer-name

import pytest

from _libsemigroups_pybind11 import (
    BMat,
    IntMat,
    MaxPlusMat,
    MinPlusMat,
    ProjMaxPlusMat,
    MaxPlusTruncMat,
    MinPlusTruncMat,
    NTPMat,
)

from libsemigroups_pybind11 import (  # pylint: disable=unused-import
    Matrix,
    MatrixKind,
)


@pytest.fixture
def matrix_types():
    return (
        BMat,
        IntMat,
        MaxPlusMat,
        MinPlusMat,
        ProjMaxPlusMat,
        MaxPlusTruncMat,
        MinPlusTruncMat,
        NTPMat,
    )


def make_mat(T, *args):
    try:
        return T(*args)
    except TypeError:
        try:
            return T(11, *args)
        except TypeError:
            return T(5, 7, *args)


def make_id_mat(T, *args):
    try:
        return T.make_identity(*args)
    except TypeError:
        try:
            return T.make_identity(11, *args)
        except TypeError:
            return T.make_identity(5, 7, *args)


def test_constructors(matrix_types):
    for T in matrix_types:
        # number rows/cols
        x = make_mat(T, 10, 10)
        assert x.number_of_rows() == 10
        assert x.number_of_cols() == 10

        # copy
        y = T(x)
        assert not y is x
        assert not x is y
        assert y.number_of_rows() == 10
        assert y.number_of_cols() == 10

        # from list of lists
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])

        assert x.number_of_rows() == 3
        assert x.number_of_cols() == 3
        assert make_mat(T, [[x[i, j] for i in range(3)] for j in range(3)]) == x

        # T.make_identity (static)
        l = x.one()
        o = x.zero()
        assert make_id_mat(T, 3) == make_mat(
            T, [[l, o, o], [o, l, o], [o, o, l]]
        )


def test_comparison_ops(matrix_types):
    for T in matrix_types:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = make_mat(T, [[1, 1, 1], [0, 0, 0], [1, 1, 1]])
        assert x < y
        assert y > x
        assert y != x
        assert x != y


def test_add_ops(matrix_types):
    for T in matrix_types:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = make_mat(T, [[0, 0, 0], [0, 1, 0], [1, 1, 1]])
        z = x + y
        x += y
        assert x == z


def test_mul_ops(matrix_types):
    for T in matrix_types:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = make_mat(T, [[0, 0, 0], [0, 1, 0], [1, 1, 1]])
        z = x * y
        t = make_id_mat(T, 3)
        t.product_inplace(x, y)
        assert z == t


def test_transpose(matrix_types):
    for T in matrix_types:
        x = make_mat(T, [[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        y = make_mat(T, x)
        x.transpose()
        assert x == make_mat(T, [[0, 0, 0], [1, 0, 0], [1, 1, 0]])
        x.transpose()
        assert x == y


def test_swap(matrix_types):
    for T in matrix_types:
        x = make_mat(T, [[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        y = make_mat(T, x)
        x.transpose()
        x.swap(y)
        assert x == make_mat(T, [[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        assert y == make_mat(T, [[0, 0, 0], [1, 0, 0], [1, 1, 0]])
        x.transpose()
        assert x == y


def test_rows(matrix_types):
    for T in matrix_types:
        if T is ProjMaxPlusMat:
            # Rows of ProjMaxPlusMat's are MaxPlusMat's so these tests don't
            # work (there's a good reason for this, read the comment in the
            # code in libsemigroups/matrix.hpp
            continue
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        assert x.row(0) == make_mat(T, [[0, 1, 1]])
        assert x.row(1) == make_mat(T, [[1, 0, 1]])
        assert x.row(2) == make_mat(T, [[1, 1, 1]])
        assert x.rows() == [
            make_mat(T, [[0, 1, 1]]),
            make_mat(T, [[1, 0, 1]]),
            make_mat(T, [[1, 1, 1]]),
        ]


def test_repr(matrix_types):
    for T in matrix_types:
        x = make_mat(T, [[0, 1], [1, 0]])
        assert eval(str(x)) == x  # pylint: disable=eval-used
