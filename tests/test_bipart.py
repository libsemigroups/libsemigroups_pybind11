# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for bipartitions.
"""

# pylint: disable=missing-function-docstring, invalid-name

from itertools import chain

import pytest


from libsemigroups_pybind11.bipartition import one
from libsemigroups_pybind11 import (
    LibsemigroupsError,
    Bipartition,
    bipartition,
    blocks,
)


def check_products(x):
    y = one(x)
    z = x * y
    assert z == x
    z = y * x
    assert z == x
    z = x * x
    assert z == x * x
    assert x * one(x) == x
    assert one(x) * x == x


def test_ops():
    x = Bipartition([0, 1, 2, 3, 0, 2])
    # T.identity, operator==, and operator!=
    assert bipartition.one(x) == Bipartition(
        list(chain(range(x.degree()), range(x.degree())))
    )
    assert bipartition.one(x) != x
    # self.assertGreater(x, bipartition.one(x))
    assert bipartition.one(x) < x
    # self.assertLessEqual(one(x), x)
    # self.assertLessEqual(x, x)
    # self.assertGreaterEqual(one(x), bipartition.one(x))
    # self.assertGreaterEqual(x, bipartition.one(x))

    # T.make_identity (static)
    assert Bipartition.one(x.degree()) == bipartition.one(x)


def test_products():
    x = Bipartition([0, 1, 2, 3, 0, 2])
    check_products(x)


def test_constructors():
    T = Bipartition
    # T.make
    with pytest.raises(LibsemigroupsError):
        T([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = Bipartition([0, 1, 2, 3, 0, 2])
    assert x[0] == 0
    assert x[1] == 1
    assert x[2] == 2
    assert x[3] == 3

    y = x.copy()
    assert y is not x


def test_rank_degree():
    T = Bipartition
    x = Bipartition([0, 1, 2, 3, 0, 2])
    # T.rank()
    assert x.rank() == 2
    assert bipartition.one(x).rank() == 3
    assert T([0] * 16).rank() == 1

    # T.degree()
    assert x.degree() == 3
    assert bipartition.one(x).degree() == 3
    assert T([0] * 16).degree() == 8


def test_iterators():
    x = Bipartition([0, 1, 2, 3, 0, 2])

    assert list(x.left_blocks()) == [0, 1, 2]
    assert list(x.right_blocks()) == [0, 1, 2]

    assert x.lookup() == [True, False, True]


def test_helpers():
    x = Bipartition([[1, 2], [-1, -2]])
    assert bipartition.underlying_partition(x) == [[1, 2], [-1, -2]]
    assert blocks.underlying_partition(x.left_blocks()) == [[1, 2]]
    assert blocks.underlying_partition(x.right_blocks()) == [[1, 2]]


def test_bipartition_return_policy():
    x = Bipartition([[1, 2], [-1, -2]])
    assert x.right_blocks() is not x.right_blocks()
    assert x.right_blocks() == x.right_blocks()
    assert x.left_blocks() is not x.left_blocks()
    assert x.left_blocks() == x.left_blocks()
