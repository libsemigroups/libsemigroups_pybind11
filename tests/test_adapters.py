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


from libsemigroups_pybind11 import (
    ImageRightAction,
    ImageLeftAction,
    BMat8,
    bmat8,
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
