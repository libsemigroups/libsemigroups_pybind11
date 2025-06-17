# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from bmat8.*pp in libsemigroups.
"""

# pylint: disable=missing-function-docstring

import pytest
from libsemigroups_pybind11 import BMat8, LibsemigroupsError


def test_bmat8_006():
    bm = BMat8(
        [
            [0, 1, 1, 1, 0, 1, 0, 1],
            [0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 1],
            [1, 1, 0, 1, 1, 1, 1, 1],
            [0, 0, 1, 0, 0, 1, 1, 1],
            [1, 1, 0, 0, 0, 0, 0, 1],
            [0, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 1, 1, 1, 0, 1, 0],
        ]
    )

    bm2 = BMat8(
        [
            [1, 1, 1, 1, 0, 1, 0, 1],
            [0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 1],
            [1, 1, 0, 1, 1, 1, 1, 1],
            [0, 0, 1, 0, 0, 1, 1, 1],
            [1, 1, 0, 0, 0, 0, 0, 1],
            [0, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 1, 1, 1, 0, 1, 0],
        ]
    )

    bm3 = BMat8(
        [
            [1, 0, 1, 1, 0, 1, 0, 1],
            [0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 1],
            [1, 1, 0, 1, 1, 1, 1, 1],
            [0, 0, 1, 0, 0, 1, 1, 1],
            [1, 1, 0, 0, 0, 0, 0, 1],
            [0, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 1, 1, 1, 0, 1, 0],
        ]
    )

    bm4 = BMat8(
        [
            [1, 0, 1, 1, 0, 1, 0, 1],
            [0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 1],
            [1, 1, 0, 1, 1, 1, 1, 1],
            [0, 0, 1, 0, 0, 1, 1, 1],
            [1, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 1, 1, 1, 0, 1, 0],
        ]
    )

    bm5 = BMat8(
        [
            [1, 0, 1, 1, 0, 1, 0, 1],
            [0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 1],
            [1, 1, 0, 1, 1, 1, 1, 1],
            [0, 0, 1, 0, 0, 1, 1, 1],
            [1, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 0, 0, 0, 0, 1, 1],
            [0, 1, 1, 1, 1, 0, 1, 1],
        ]
    )

    bm[0, 0] = True
    assert bm == bm2

    bm[0, 1] = False
    assert bm == bm3

    bm[5, 6] = True
    assert bm == bm4

    bm[7, 7] = True
    assert bm == bm5

    for i in range(8):
        for j in range(8):
            bm[i, j] = True

    ones = BMat8(0xFFFFFFFFFFFFFFFF)
    assert bm == ones

    for i in range(8):
        for j in range(8):
            bm[i, j] = False

    zeros = BMat8(0)
    assert bm == zeros
    with pytest.raises(LibsemigroupsError):
        zeros[0, 8] = True
    with pytest.raises(LibsemigroupsError):
        zeros[8, 0] = True
    with pytest.raises(LibsemigroupsError):
        zeros[8, 8] = True
