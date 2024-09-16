# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for PBRs.
"""

# pylint: disable= missing-function-docstring, invalid-name

from copy import copy
import pytest
from libsemigroups_pybind11 import PBR, pbr


def test_ops():
    x = PBR([[0, 1, 2]] * 6)
    n = x.degree()
    # pbr.one, operator==, and operator!=
    assert pbr.one(n) == PBR([[3], [4], [5], [0], [1], [2]])
    assert pbr.one(n) != x
    # operator <
    assert x < pbr.one(n)
    assert pbr.one(n) > x

    assert pbr.one(x.degree()) == pbr.one(x)


def test_constructors():
    T = PBR
    # T.make
    with pytest.raises(RuntimeError):
        T([[1, 1, 2, 16]] * 2)
    with pytest.raises(TypeError):
        T([1, 1, 2, 16] + list(range(4, 16)))

    with pytest.raises(TypeError):
        T([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = PBR([[0, 1, 2]] * 6)
    assert x[0] == [0, 1, 2]
    assert x[1] == x[0]
    assert x[2] == x[0]
    assert x[3] == x[0]

    y = PBR.copy(x)
    z = copy(x)
    assert not x is y
    assert not x is z
    assert not y is z


def test_rank_degree():
    x = PBR([[0, 1, 2]] * 6)

    # PBR.degree()
    assert x.degree() == 3
    assert pbr.one(x).degree() == 3
    assert PBR([[0, 1, 2]] * 16).degree() == 8
