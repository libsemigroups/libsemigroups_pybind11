# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for bipartitions.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

from itertools import chain

import pytest
from element import check_products
from libsemigroups_pybind11 import Bipartition


def test_ops():
    x = Bipartition.make([0, 1, 2, 3, 0, 2])
    # T.identity, operator==, and operator!=
    assert x.identity() == Bipartition.make(
        list(chain(range(x.degree()), range(x.degree())))
    )
    assert x.identity() != x
    # self.assertGreater(x, x.identity())
    assert x.identity() < x
    # self.assertLessEqual(x.identity(), x)
    # self.assertLessEqual(x, x)
    # self.assertGreaterEqual(x.identity(), x.identity())
    # self.assertGreaterEqual(x, x.identity())

    # T.make_identity (static)
    assert Bipartition.make_identity(x.degree()) == x.identity()


def test_products():
    x = Bipartition.make([0, 1, 2, 3, 0, 2])
    check_products(x)


def test_constructors():
    T = Bipartition
    # T.make
    with pytest.raises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = Bipartition.make([0, 1, 2, 3, 0, 2])
    assert x[0] == 0
    assert x[1] == 1
    assert x[2] == 2
    assert x[3] == 3

    y = Bipartition(x)
    assert not y is x


def test_rank_degree():
    T = Bipartition
    x = Bipartition.make([0, 1, 2, 3, 0, 2])
    # T.rank()
    assert x.rank() == 2
    assert x.identity().rank() == 3
    assert T.make([0] * 16).rank() == 1

    # T.degree()
    assert x.degree() == 3
    assert x.identity().degree() == 3
    assert T.make([0] * 16).degree() == 8


def test_iterators():
    x = Bipartition.make([0, 1, 2, 3, 0, 2])

    assert list(x.left_blocks()) == [0, 1, 2]
    assert list(x.right_blocks()) == [3, 0, 2]

    assert x.lookup() == [True, False, True]
