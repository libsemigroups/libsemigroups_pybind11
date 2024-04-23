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


from libsemigroups_pybind11 import ImageRightAction, ImageLeftAction, BMat8


def test_adapters_017():
    right = ImageRightAction(Point=BMat8, Element=BMat8)
    # Point1, Point2, Element -> Point1 = Point2 ^ Element
    A, B, C = BMat8.random(8), BMat8(0), BMat8.one(8)
    right(B, A, C)
    assert B == A.row_space_basis()

    right(B, C, A)
    assert B == A.row_space_basis()

    left = ImageLeftAction(Point=BMat8, Element=BMat8)
    left(B, A, C)
    assert B == A.col_space_basis()

    left(B, C, A)
    assert B == A.col_space_basis()

    right(B, A, A)
    left(C, A.transpose(), A.transpose())
    assert B == C.transpose()
