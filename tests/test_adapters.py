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
    right = ImageRightAction(Point=BMat8, Element=BMat8)
    # Point1, Point2, Element -> Point1 = Point2 ^ Element
    A, B, C = bmat8.random(), BMat8(0), bmat8.one()
    right(B, A, C)
    assert B == bmat8.row_space_basis(A)

    right(B, C, A)
    assert B == bmat8.row_space_basis(A)

    left = ImageLeftAction(Point=BMat8, Element=BMat8)
    left(B, A, C)
    assert B == bmat8.col_space_basis(A)

    left(B, C, A)
    assert B == bmat8.col_space_basis(A)

    right(B, A, A)
    left(C, bmat8.transpose(A), bmat8.transpose(A))
    assert B == bmat8.transpose(C)


def test_image_right_action_pperm_pperm():
    func = ImageRightAction(Point=PPerm, Element=PPerm)
    x = PPerm([0, 1, 2, 3, 5, 6, 9], [9, 7, 3, 5, 4, 2, 1], 10)
    pt = PPerm([0, 1, 2, 3, 5, 6], [0, 1, 2, 3, 5, 6], 10)
    assert func(pt, x) == PPerm([2, 4, 5, 3, 7, 9], [2, 4, 5, 3, 7, 9], 10)

    func = ImageLeftAction(Point=PPerm, Element=PPerm)
    assert func(pt, x) == PPerm([2, 3, 6, 9], [2, 3, 6, 9], 10)


def test_image_right_action_exceptions():
    func = ImageRightAction(Point=PPerm, Element=PPerm)
    with pytest.raises(ValueError):
        func(BMat8(0), BMat8(0))
    with pytest.raises(ValueError):
        func(PPerm([], [], 10), PPerm([], [], 10), BMat8(0))
    with pytest.raises(ValueError):
        func(PPerm([], [], 10), BMat8(0), PPerm([], [], 10))
    with pytest.raises(ValueError):
        func(BMat8(0), PPerm([], [], 10), PPerm([], [], 10))
    with pytest.raises(TypeError):
        func(1, 2, 3, 4)
    with pytest.raises(TypeError):
        func(1)
    func = ImageRightAction(Point=list, Element=PPerm)
    with pytest.raises(NotImplementedError):
        func([1, 2], [1, 2], PPerm([], [], 10))
