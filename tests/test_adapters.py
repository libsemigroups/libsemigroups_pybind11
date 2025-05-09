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

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name

import pytest

from libsemigroups_pybind11 import (
    ImageRightAction,
    ImageLeftAction,
    BMat8,
    bmat8,
    PPerm,
)


def test_adapters_017():
    A, B, C = bmat8.random(), BMat8(0), bmat8.one()
    right = ImageRightAction(point=A, element=A)
    # Point1, Point2, Element -> Point1 = Point2 ^ Element
    B = right(A, C)
    assert B == bmat8.row_space_basis(A)

    B = right(C, A)
    assert B == bmat8.row_space_basis(A)

    left = ImageLeftAction(point=A, element=A)
    B = left(A, C)
    assert B == bmat8.col_space_basis(A)

    B = left(C, A)
    assert B == bmat8.col_space_basis(A)

    B = right(A, A)
    C = left(bmat8.transpose(A), bmat8.transpose(A))
    assert B == bmat8.transpose(C)


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
