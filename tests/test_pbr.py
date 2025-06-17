# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for PBRs.
"""

# pylint: disable= missing-function-docstring, comparison-with-itself

from copy import copy
import pytest
from libsemigroups_pybind11 import PBR, pbr


def test_ops():
    x = PBR([[0, 1, 2]] * 6)
    n = x.degree()

    assert x <= x
    assert x >= x
    assert x == x
    assert pbr.one(n) == PBR([[3], [4], [5], [0], [1], [2]])
    assert pbr.one(n) != x
    assert x < pbr.one(n)
    assert pbr.one(n) > x
    assert pbr.one(n) >= x
    assert not pbr.one(n) < x
    assert not pbr.one(n) <= x
    assert pbr.one(x.degree()) == pbr.one(x)


def test_constructors():
    # make
    with pytest.raises(RuntimeError):
        PBR([[1, 1, 2, 16]] * 2)
    with pytest.raises(TypeError):
        PBR([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        PBR([-1, 1, 2, 6] + list(range(4, 16)))

    # __get_item__
    x = PBR([[0, 1, 2]] * 6)
    assert x[0] == [0, 1, 2]
    assert x[1] == x[0]
    assert x[2] == x[0]
    assert x[3] == x[0]

    y = PBR.copy(x)
    z = copy(x)
    assert x is not y
    assert x is not z
    assert y is not z


def test_product():
    left = [[-3, -1], [-3, -2, -1, 1, 2, 3], [-3, -2, -1, 1, 3]]
    right = [[-3, -1, 1, 2, 3], [-3, 1, 3], [-3, -2, -1, 2, 3]]
    x = PBR(left, right)
    y = PBR(
        [[-3, -2, -1, 1], [-3, -2, 3], [-3, 2, 3]],
        [[-3, -2, -1, 3], [-3, -2, -1, 3], [-2, 2, 3]],
    )
    z = PBR(3)

    xx = PBR(
        [
            [3, 5],
            [0, 1, 2, 3, 4, 5],
            [0, 2, 3, 4, 5],
            [0, 1, 2, 3, 5],
            [0, 2, 5],
            [1, 2, 3, 4, 5],
        ]
    )
    yy = PBR(
        [
            [0, 3, 4, 5],
            [2, 4, 5],
            [1, 2, 5],
            [2, 3, 4, 5],
            [2, 3, 4, 5],
            [1, 2, 4],
        ]
    )

    assert x == xx
    assert y == yy
    assert x.degree() == 3

    z.product_inplace(x, y)
    t = x * y

    expected = PBR(
        [
            [0, 1, 2, 3, 4, 5],
            [0, 1, 2, 3, 4, 5],
            [0, 1, 2, 3, 4, 5],
            [0, 1, 2, 3, 4, 5],
            [0, 1, 2, 3, 4, 5],
            [0, 1, 2, 3, 4, 5],
        ]
    )
    assert z == expected
    assert t == expected


def test_rank_degree():
    x = PBR([[0, 1, 2]] * 6)

    # PBR.degree()
    assert x.degree() == 3
    assert pbr.one(x).degree() == 3
    assert PBR([[0, 1, 2]] * 16).degree() == 8


def test_pbr_return_policy():
    x = PBR([[0, 1, 2]] * 6)

    assert x.copy() is not x
