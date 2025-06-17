# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring, invalid-name

"""
This module contains some tests for transformations, partial perms, and
permutations.
"""

import copy
import pytest


from libsemigroups_pybind11.transf import (
    right_one,
    left_one,
    inverse,
    Transf,
    Perm,
    PPerm,
    one,
    image,
    domain,
)

from libsemigroups_pybind11 import UNDEFINED


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

    # T.degree
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
    assert x[0] == UNDEFINED
    assert x[1] == 4
    assert x[2] == 7
    assert x[3] == 6
    assert x[4] == UNDEFINED

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

    # T.degree
    assert x.degree() == 16
    assert one(x).degree() == 16

    # Product in-place
    check_product_inplace(x)

    # T.images
    assert list(x.images()) == [UNDEFINED, 4, 7, 6] + [UNDEFINED] * 12


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

    # T.degree
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


def check_increase_degree_by(T):
    x = T([0])
    assert x.degree() == 1
    x.increase_degree_by(2)
    assert x.degree() == 3
    x.increase_degree_by(15)
    assert x.degree() == 18
    x.increase_degree_by(15)
    assert x.degree() == 33
    x.increase_degree_by(255)
    assert x.degree() == 288
    x.increase_degree_by(2**16)
    assert x.degree() == 288 + 2**16
    with pytest.raises(ValueError):
        x.increase_degree_by(2**32)


def test_transf():
    check_transf(Transf)


def test_pperm():
    check_pperm(PPerm)


def test_perm():
    check_perm(Perm)


def test_exceptions():
    with pytest.raises(ValueError):
        Transf(range(2**32 + 1))
    with pytest.raises(TypeError):
        PPerm(1, 2, 3, 4)
    with pytest.raises(TypeError):
        PPerm(1, 2)


def test_operators():
    # different degrees
    assert Transf([0]) != Transf([0, 1])
    assert Transf([0]) < Transf([0, 1])


def test_copy():
    x = Transf([0, 1])
    y = x.copy()
    assert x is not y
    assert x == y
    z = copy.copy(x)
    assert x is not z
    assert x == z


def test_images():
    x = Transf(range(17))
    assert list(x.images()) == list(range(17))


def test_transf_increase_degree_by():
    check_increase_degree_by(Transf)


def test_pperm_increase_degree_by():
    check_increase_degree_by(PPerm)


def test_perm_increase_degree_by():
    check_increase_degree_by(Perm)


def test_swap():
    x = Transf([0])
    y = Transf([0, 1])

    x.swap(y)
    assert x == Transf([0, 1])
    assert y == Transf([0])


def test_repr():
    x = Transf([0])
    assert repr(x) == "Transf([0])"
    x.increase_degree_by(32)
    assert repr(x) == "<transformation of degree 33 and rank 33>"
    x = PPerm([0])
    assert repr(x) == "PPerm([0], [0], 1)"
    x.increase_degree_by(32)
    assert repr(x) == "<partial perm of degree 33 and rank 33>"
    x = Perm([0])
    assert repr(x) == "Perm([0])"
    x.increase_degree_by(32)
    assert repr(x) == "<permutation of degree 33>"


def test_image():
    x = Transf([0])
    assert image(x) == [0]
    x = PPerm([0, 1], [2, 1], 3)
    assert image(x) == [1, 2]
    x = Perm([0, 1])
    assert image(x) == [0, 1]


def test_domain():
    x = Transf([0])
    assert domain(x) == [0]
    x = PPerm([0, 1], [2, 1], 3)
    assert domain(x) == [0, 1]
    x = Perm([0, 1])
    assert domain(x) == [0, 1]


def test_corner_cases():
    # Here we test that an exception is thrown when UNDEFINED is one of the
    # values given in the argument
    assert PPerm([255], [255], 256).rank() == 1
    assert PPerm([], [], 0) * PPerm([], [], 0) == PPerm([], [], 0)
    assert PPerm([], [], 10) * PPerm([], [], 10) == PPerm([], [], 10)
    assert PPerm([], [], 256) * PPerm([], [], 256) == PPerm([], [], 256)


def test_transf_return_policy():
    for TestType in (Transf, PPerm, Perm):
        x = TestType([0])
        assert x.copy() is not x
        assert x.images() is not x.images()
        assert x.increase_degree_by(2) is x
