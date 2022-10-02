# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for PBRs.
"""

# pylint: disable= missing-function-docstring, invalid-name

import pytest

from element import check_products
from libsemigroups_pybind11 import PBR


def test_ops():
    x = PBR.make([[0, 1, 2]] * 6)
    # T.identity, operator==, and operator!=
    assert x.identity() == PBR.make([[3], [4], [5], [0], [1], [2]])
    assert x.identity() != x
    # self.assertGreater(x, x.identity())
    assert x < x.identity()
    # self.assertLessEqual(x.identity(), x)
    # self.assertLessEqual(x, x)
    # self.assertGreaterEqual(x.identity(), x.identity())
    # self.assertGreaterEqual(x, x.identity())

    # T.make_identity (static)
    assert PBR.make_identity(x.degree()) == x.identity()


def test_products():
    x = PBR.make([[0, 1, 2]] * 6)
    check_products(x)


def test_constructors():
    T = PBR
    # T.make
    with pytest.raises(RuntimeError):
        T.make([[1, 1, 2, 16]] * 2)
    with pytest.raises(TypeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = PBR.make([[0, 1, 2]] * 6)
    assert x[0] == [0, 1, 2]
    assert x[1] == x[0]
    assert x[2] == x[0]
    assert x[3] == x[0]

    y = PBR(x)
    assert not y is x


def test_rank_degree():
    x = PBR.make([[0, 1, 2]] * 6)

    # T.degree()
    assert x.degree() == 3
    assert x.identity().degree() == 3
    assert PBR.make([[0, 1, 2]] * 16).degree() == 8
