# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for matrices.
"""

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    NEGATIVE_INFINITY,
    UNDEFINED,
)


def test_operators():
    """Test the comparison operators for constants in libsemigroups"""
    assert POSITIVE_INFINITY > NEGATIVE_INFINITY
    assert NEGATIVE_INFINITY < POSITIVE_INFINITY
    assert not NEGATIVE_INFINITY == POSITIVE_INFINITY
    assert NEGATIVE_INFINITY != POSITIVE_INFINITY
    assert not 6 == POSITIVE_INFINITY
    assert 6 != POSITIVE_INFINITY
    assert not POSITIVE_INFINITY == 6
    assert POSITIVE_INFINITY != 6
    assert not 6 == NEGATIVE_INFINITY
    assert 6 != NEGATIVE_INFINITY
    assert not NEGATIVE_INFINITY == 6
    assert NEGATIVE_INFINITY != 6
    assert not POSITIVE_INFINITY == NEGATIVE_INFINITY
    assert POSITIVE_INFINITY != NEGATIVE_INFINITY
    assert -6 > NEGATIVE_INFINITY
    assert 6 > NEGATIVE_INFINITY
    assert NEGATIVE_INFINITY < 6
    assert NEGATIVE_INFINITY < -6
    assert not -6 < NEGATIVE_INFINITY
    assert not 6 < NEGATIVE_INFINITY
    assert not NEGATIVE_INFINITY > 6
    assert not NEGATIVE_INFINITY > -6
    assert -6 < POSITIVE_INFINITY
    assert 6 < POSITIVE_INFINITY
    assert POSITIVE_INFINITY > 6
    assert POSITIVE_INFINITY > -6
    assert not -6 > POSITIVE_INFINITY
    assert not 6 > POSITIVE_INFINITY
    assert not POSITIVE_INFINITY < 6
    assert not POSITIVE_INFINITY < -6
    assert POSITIVE_INFINITY == 2147483646
    assert NEGATIVE_INFINITY == -2147483648
    assert 2147483646 == POSITIVE_INFINITY
    assert -2147483648 == NEGATIVE_INFINITY
    assert 18446744073709551615 == UNDEFINED
    assert UNDEFINED == 18446744073709551615
