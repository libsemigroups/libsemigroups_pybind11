# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for matrices.
"""

# pylint: disable=missing-function-docstring, invalid-name

import copy
import pytest

from libsemigroups_pybind11 import Matrix, MatrixKind


@pytest.fixture(name="matrix_kinds")
def fixture_matrix_kinds():
    return tuple(
        getattr(MatrixKind, x)
        for x in dir(MatrixKind)
        if not x.startswith("__")
    )


def make_mat(kind, *args):
    try:
        return Matrix(kind, *args)
    except TypeError:
        try:
            return Matrix(kind, 11, *args)
        except TypeError:
            return Matrix(kind, 5, 7, *args)


def make_id_mat(x, *args):
    return x.one(*args)


def test_constructors(matrix_kinds):
    for T in matrix_kinds:
        # number rows/cols
        x = make_mat(T, 10, 10)
        assert x.number_of_rows() == 10
        assert x.number_of_cols() == 10

        # copy
        y = x.copy()
        assert y is not x
        assert x is not y
        assert y.number_of_rows() == 10
        assert y.number_of_cols() == 10

        y = copy.copy(x)
        assert y is not x
        assert x is not y
        assert y.number_of_rows() == 10
        assert y.number_of_cols() == 10

        # from list of lists
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])

        assert x.number_of_rows() == 3
        assert x.number_of_cols() == 3
        assert make_mat(T, [[x[i, j] for i in range(3)] for j in range(3)]) == x

        # T.make_identity (static)
        one = x.scalar_one()
        zero = x.scalar_zero()
        assert x.one(3) == make_mat(
            T, [[one, zero, zero], [zero, one, zero], [zero, zero, one]]
        )


def test_comparison_ops(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = make_mat(T, [[1, 1, 1], [0, 0, 0], [1, 1, 1]])
        assert x < y
        assert y > x
        assert y != x
        assert x != y


def test_add_ops(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = make_mat(T, [[0, 0, 0], [0, 1, 0], [1, 1, 1]])
        z = x + y
        x += y
        assert x == z


def test_mul_ops(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = make_mat(T, [[0, 0, 0], [0, 1, 0], [1, 1, 1]])
        z = x * y
        t = x.one(3)
        t.product_inplace(x, y)
        assert z == t


def test_transpose(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        y = make_mat(T, x)
        x.transpose()
        assert x == make_mat(T, [[0, 0, 0], [1, 0, 0], [1, 1, 0]])
        x.transpose()
        assert x == y


def test_swap(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        y = make_mat(T, x)
        x.transpose()
        x.swap(y)
        assert x == make_mat(T, [[0, 1, 1], [0, 0, 1], [0, 0, 0]])
        assert y == make_mat(T, [[0, 0, 0], [1, 0, 0], [1, 1, 0]])
        x.transpose()
        assert x == y


def test_rows(matrix_kinds):
    for T in matrix_kinds:
        if T is MatrixKind.ProjMaxPlus:
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


def test_repr(matrix_kinds):
    # Note that the following won't work if POSITIVE_INFINITY or
    # NEGATIVE_INFINITY are entries in the matrix
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1], [1, 0]])
        assert eval(str(x)) == x  # pylint: disable=eval-used


def test_hash(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        d = {x: True}
        assert x in d


def test_matrix_return_policy(matrix_kinds):
    for T in matrix_kinds:
        x = make_mat(T, [[0, 1, 1], [1, 0, 1], [1, 1, 1]])
        y = x.copy()
        assert x is not y

        x *= 2
        assert x == 2 * y

        x += x
        y = 2 * y
        assert x == y + y

        x += 2
        y = y + y
        assert x == y + 2

        assert x.transpose() is not x
        assert x.row(0) is not x.row(0)
        assert x.rows() is not x.rows()
