# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

"""
This module contains some tests for transformations, partial perms, and
permutations.
"""

import pytest

from _libsemigroups_pybind11 import (
    Transf16,
    Transf1,
    Transf2,
    Transf4,
    PPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    Perm16,
    Perm1,
    Perm2,
    Perm4,
)


def check_identity_ops(T, x):
    # T.identity, operator==, and operator!=
    assert x.identity() == T.make(range(x.degree()))
    assert x.identity() != x
    assert x > x.identity()
    assert x.identity() < x
    assert x.identity() <= x
    assert x <= x  # pylint: disable=comparison-with-itself
    assert x.identity() <= x.identity()
    assert x >= x.identity()

    # T.make_identity (static)
    assert T.make_identity(x.degree()) == x.identity()


def check_product_inplace(x):
    y = x.identity()
    z = x.identity()
    z.product_inplace(x, y)
    assert z == x
    z.product_inplace(y, x)
    assert z == x
    z.product_inplace(x, x)
    assert z == x * x


def check_transf(T):

    # T.make
    with pytest.raises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T.make([1, 1, 2, 3] + list(range(4, 16)))
    assert x[0] == 1
    assert x[1] == 1
    assert x[2] == 2
    assert x[3] == 3

    # T.identity, T.make_identity, operator==, and operator!=
    check_identity_ops(T, x)

    # T.rank()
    assert x.rank() == 15
    assert x.identity().rank() == 16
    assert T.make([0] * 16).rank() == 1

    # T.degree()
    assert x.degree() == 16
    assert x.identity().degree() == 16
    assert T.make([0] * 16).degree() == 16

    # Product in-place
    check_product_inplace(x)

    # T.images
    assert list(x.images()) == [
        1,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
    ]

    if T is not Transf16:
        x = T.make(
            [
                15,
                5,
                2,
                10,
                17,
                8,
                13,
                15,
                1,
                9,
                4,
                0,
                15,
                5,
                14,
                11,
                15,
                4,
                7,
                3,
            ]
        )
        assert x.degree() == 20
        assert x.rank() == 15
        assert x[5] == 8
        assert x * x * x == T.make(
            [
                0,
                1,
                2,
                17,
                17,
                5,
                8,
                0,
                8,
                9,
                4,
                11,
                0,
                1,
                14,
                15,
                0,
                4,
                11,
                4,
            ]
        )


def check_pperm(T):

    # T.make
    with pytest.raises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T.make([1, 2, 3], [4, 7, 6], 16)
    assert x[0] == T.undef()
    assert x[1] == 4
    assert x[2] == 7
    assert x[3] == 6
    assert x[4] == T.undef()

    # T.identity, T.make_identity, operator==, and operator!=
    check_identity_ops(T, x)

    assert x * x.right_one() == x
    assert x.left_one() * x == x
    assert x * x.inverse() == x.left_one()
    assert x.inverse() * x == x.right_one()
    # assert list(x.left_one().images()) ==  x

    # T.rank()
    assert x.rank() == 3
    assert x.identity().rank() == 16

    # T.degree()
    assert x.degree() == 16
    assert x.identity().degree() == 16

    # Product in-place
    check_product_inplace(x)

    # T.images
    assert list(x.images()) == [T.undef(), 4, 7, 6] + [T.undef()] * 12


def check_perm(T):
    # T.make
    with pytest.raises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T.make([1, 2, 3, 0, 6, 5, 4] + list(range(7, 16)))
    assert x[0] == 1
    assert x[1] == 2
    assert x[2] == 3
    assert x[3] == 0
    assert x[4] == 6

    # T.identity, T.make_identity, operator==, and operator!=
    check_identity_ops(T, x)

    assert x.inverse() * x == x.identity()
    assert x * x.inverse() == x.identity()

    # T.rank()
    assert x.rank() == 16
    assert x.identity().rank() == 16

    # T.degree()
    assert x.degree() == 16
    assert x.identity().degree() == 16

    # Product in-place
    check_product_inplace(x)

    # T.images
    assert list(x.images()) == [
        1,
        2,
        3,
        0,
        6,
        5,
        4,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
    ]


def test_transf16():
    check_transf(Transf16)


def test_transf1():
    check_transf(Transf1)


def test_transf2():
    check_transf(Transf2)


def test_transf4():
    check_transf(Transf4)


def test_pperm16():
    check_pperm(PPerm16)


def test_pperm1():
    check_pperm(PPerm1)


def test_pperm2():
    check_pperm(PPerm2)


def test_pperm4():
    check_pperm(PPerm4)


def test_perm16():
    check_perm(Perm16)


def test_perm1():
    check_perm(Perm1)


def test_perm2():
    check_perm(Perm2)


def test_perm4():
    check_perm(Perm4)
