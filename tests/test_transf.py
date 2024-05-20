# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
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

from libsemigroups_pybind11 import (
    one,
    right_one,
    left_one,
    inverse,
    StaticTransf16,
    Transf1,
    Transf2,
    Transf4,
    StaticPPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    StaticPerm16,
    Perm1,
    Perm2,
    Perm4,
)


def check_one_ops(T, x):
    # T.one, operator==, and operator!=
    assert one(x) == T(range(x.degree()))
    assert one(x) != x
    assert x > one(x)
    assert one(x) < x
    assert one(x) <= x
    assert x <= x  # pylint: disable=comparison-with-itself
    assert one(x) <= one(x)
    assert x >= one(x)

    # T.one (static)
    assert T.one(x.degree()) == one(x)


def check_product_inplace(x):
    y = one(x)
    z = one(x)
    z.product_inplace(x, y)
    assert z == x
    z.product_inplace(y, x)
    assert z == x
    z.product_inplace(x, x)
    assert z == x * x


def check_transf(T):
    # T.make
    with pytest.raises(RuntimeError):
        T([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T([1, 1, 2, 3] + list(range(4, 16)))
    assert x[0] == 1
    assert x[1] == 1
    assert x[2] == 2
    assert x[3] == 3

    # T.one, T.one, operator==, and operator!=
    check_one_ops(T, x)

    # T.rank()
    assert x.rank() == 15
    assert one(x).rank() == 16
    assert T([0] * 16).rank() == 1

    # T.degree()
    assert x.degree() == 16
    assert one(x).degree() == 16
    assert T([0] * 16).degree() == 16

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

    if T is not StaticTransf16:
        x = T(
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
        assert x * x * x == T(
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
        T([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T([1, 2, 3], [4, 7, 6], 16)
    assert x[0] == T.undef()
    assert x[1] == 4
    assert x[2] == 7
    assert x[3] == 6
    assert x[4] == T.undef()

    # T.one, T.one, operator==, and operator!=
    check_one_ops(T, x)

    assert x * right_one(x) == x
    assert left_one(x) * x == x
    assert x * inverse(x) == left_one(x)
    assert inverse(x) * x == right_one(x)
    # assert list(left_one(x).images()) ==  x

    # T.rank()
    assert x.rank() == 3
    assert one(x).rank() == 16

    # T.degree()
    assert x.degree() == 16
    assert one(x).degree() == 16

    # Product in-place
    check_product_inplace(x)

    # T.images
    assert list(x.images()) == [T.undef(), 4, 7, 6] + [T.undef()] * 12


def check_perm(T):
    # T.make
    with pytest.raises(RuntimeError):
        T([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T([1, 2, 3, 0, 6, 5, 4] + list(range(7, 16)))
    assert x[0] == 1
    assert x[1] == 2
    assert x[2] == 3
    assert x[3] == 0
    assert x[4] == 6

    # T.one, T.one, operator==, and operator!=
    check_one_ops(T, x)

    assert inverse(x) * x == one(x)
    assert x * inverse(x) == one(x)

    # T.rank()
    assert x.rank() == 16
    assert one(x).rank() == 16

    # T.degree()
    assert x.degree() == 16
    assert one(x).degree() == 16

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
    check_transf(StaticTransf16)


def test_transf1():
    check_transf(Transf1)


def test_transf2():
    check_transf(Transf2)


def test_transf4():
    check_transf(Transf4)


def test_pperm16():
    check_pperm(StaticPPerm16)


def test_pperm1():
    check_pperm(PPerm1)


def test_pperm2():
    check_pperm(PPerm2)


def test_pperm4():
    check_pperm(PPerm4)


def test_perm16():
    check_perm(StaticPerm16)


def test_perm1():
    check_perm(Perm1)


def test_perm2():
    check_perm(Perm2)


def test_perm4():
    check_perm(Perm4)
