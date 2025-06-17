# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from adapters.*pp in libsemigroups.
"""

# pylint: disable=missing-function-docstring

import pytest

from libsemigroups_pybind11 import (
    ImageRightAction,
    ImageLeftAction,
    BMat8,
    bmat8,
    PPerm,
)


def test_adapters_017():
    a, b, c = bmat8.random(), BMat8(0), bmat8.one()
    right = ImageRightAction(point=a, element=a)
    # Point1, Point2, Element -> Point1 = Point2 ^ Element
    b = right(a, c)
    assert b == bmat8.row_space_basis(a)

    b = right(c, a)
    assert b == bmat8.row_space_basis(a)

    left = ImageLeftAction(point=a, element=a)
    b = left(a, c)
    assert b == bmat8.col_space_basis(a)

    b = left(c, a)
    assert b == bmat8.col_space_basis(a)

    b = right(a, a)
    c = left(bmat8.transpose(a), bmat8.transpose(a))
    assert b == bmat8.transpose(c)


def test_image_right_action_pperm_pperm():
    x = PPerm([0, 1, 2, 3, 5, 6, 9], [9, 7, 3, 5, 4, 2, 1], 10)
    pt = PPerm([0, 1, 2, 3, 5, 6], [0, 1, 2, 3, 5, 6], 10)
    func = ImageRightAction(point=pt, element=x)
    assert func(pt, x) == PPerm([2, 4, 5, 3, 7, 9], [2, 4, 5, 3, 7, 9], 10)

    func = ImageLeftAction(point=pt, element=x)
    assert func(pt, x) == PPerm([2, 3, 6, 9], [2, 3, 6, 9], 10)


def test_image_right_action_exceptions():
    x = PPerm([], [], 10)
    func = ImageRightAction(point=x, element=x)
    assert func(PPerm([], [], 11), PPerm([], [], 11)) == PPerm([], [], 11)
    with pytest.raises(TypeError):
        func(BMat8(0), BMat8(0))
    with pytest.raises(TypeError):
        func(PPerm([], [], 10), PPerm([], [], 10), BMat8(0))
    with pytest.raises(TypeError):
        func(PPerm([], [], 10), BMat8(0), PPerm([], [], 10))
    with pytest.raises(TypeError):
        func(BMat8(0), PPerm([], [], 10), PPerm([], [], 10))
    with pytest.raises(TypeError):
        func(1, 2, 3, 4)
    with pytest.raises(TypeError):
        func(1)
    func = ImageRightAction(point=[], element=x)
    with pytest.raises(TypeError):
        func([1, 2], [1, 2], PPerm([], [], 10))
