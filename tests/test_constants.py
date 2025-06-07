# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=comparison-with-itself
"""
This module contains some tests for matrices.
"""

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    NEGATIVE_INFINITY,
    UNDEFINED,
    LIMIT_MAX,
)


def test_UNDEFINED():  # pylint: disable=invalid-name
    """
    Test the properties of the UNDEFINED constant
    """
    assert UNDEFINED == UNDEFINED
    assert not UNDEFINED == POSITIVE_INFINITY
    assert not UNDEFINED == NEGATIVE_INFINITY
    assert not UNDEFINED == LIMIT_MAX
    assert not UNDEFINED == 0
    assert 18446744073709551615 != UNDEFINED
    assert UNDEFINED != 18446744073709551615

    assert not UNDEFINED != UNDEFINED
    assert UNDEFINED != POSITIVE_INFINITY
    assert UNDEFINED != NEGATIVE_INFINITY
    assert UNDEFINED != LIMIT_MAX
    assert UNDEFINED != 0
    assert 0 != UNDEFINED

    assert not UNDEFINED > UNDEFINED
    assert not UNDEFINED < UNDEFINED

    d = {UNDEFINED: 0}
    assert UNDEFINED in d


def test_POSITIVE_INFINITY():  # pylint: disable=invalid-name
    """
    Test the properties of the POSITIVE_INFINITY constant
    """
    assert POSITIVE_INFINITY == POSITIVE_INFINITY
    assert POSITIVE_INFINITY != 2147483646
    assert 2147483646 != POSITIVE_INFINITY
    assert not POSITIVE_INFINITY == UNDEFINED
    assert not POSITIVE_INFINITY == NEGATIVE_INFINITY
    assert not POSITIVE_INFINITY == LIMIT_MAX

    assert not POSITIVE_INFINITY == 0
    assert not 0 == POSITIVE_INFINITY

    # operator!=
    assert not POSITIVE_INFINITY != POSITIVE_INFINITY
    assert POSITIVE_INFINITY != UNDEFINED
    assert POSITIVE_INFINITY != NEGATIVE_INFINITY
    assert POSITIVE_INFINITY != LIMIT_MAX

    assert not POSITIVE_INFINITY == 0
    assert 0 != POSITIVE_INFINITY

    # operator<
    assert not POSITIVE_INFINITY < 0
    assert not POSITIVE_INFINITY < POSITIVE_INFINITY
    assert 0 < POSITIVE_INFINITY
    assert 1000 < POSITIVE_INFINITY
    assert NEGATIVE_INFINITY < POSITIVE_INFINITY

    # operator>
    assert not POSITIVE_INFINITY > POSITIVE_INFINITY
    assert POSITIVE_INFINITY > 0
    assert not 0 > POSITIVE_INFINITY
    assert POSITIVE_INFINITY > 100
    assert POSITIVE_INFINITY > NEGATIVE_INFINITY

    d = {POSITIVE_INFINITY: 0}
    assert POSITIVE_INFINITY in d


def test_NEGATIVE_INFINITY():  # pylint: disable=invalid-name
    """
    Test the properties of the NEGATIVE_INFINITY constant
    """
    assert NEGATIVE_INFINITY == NEGATIVE_INFINITY
    assert NEGATIVE_INFINITY != -2147483648
    assert -2147483648 != NEGATIVE_INFINITY
    assert not NEGATIVE_INFINITY == UNDEFINED
    assert not NEGATIVE_INFINITY == POSITIVE_INFINITY
    assert not NEGATIVE_INFINITY == LIMIT_MAX

    assert not NEGATIVE_INFINITY == 0
    assert not 0 == NEGATIVE_INFINITY

    # operator!=
    assert not NEGATIVE_INFINITY != NEGATIVE_INFINITY
    assert NEGATIVE_INFINITY != UNDEFINED
    assert NEGATIVE_INFINITY != POSITIVE_INFINITY
    assert NEGATIVE_INFINITY != LIMIT_MAX

    assert NEGATIVE_INFINITY != 0
    assert 0 != NEGATIVE_INFINITY

    # operator<
    assert NEGATIVE_INFINITY < 0
    assert not NEGATIVE_INFINITY < NEGATIVE_INFINITY
    assert not 0 < NEGATIVE_INFINITY
    assert not 1000 < NEGATIVE_INFINITY
    assert not POSITIVE_INFINITY < NEGATIVE_INFINITY

    # operator>
    assert not NEGATIVE_INFINITY > NEGATIVE_INFINITY
    assert not NEGATIVE_INFINITY > 0
    assert 0 > NEGATIVE_INFINITY
    assert not NEGATIVE_INFINITY > 100
    assert not NEGATIVE_INFINITY > POSITIVE_INFINITY

    d = {NEGATIVE_INFINITY: 0}
    assert NEGATIVE_INFINITY in d


def test_LIMIT_MAX():  # pylint: disable=invalid-name
    """
    Test the properties of the LIMIT_MAX constant
    """
    assert LIMIT_MAX == LIMIT_MAX
    assert not LIMIT_MAX == UNDEFINED
    assert not LIMIT_MAX == POSITIVE_INFINITY
    assert not LIMIT_MAX == NEGATIVE_INFINITY

    assert not LIMIT_MAX == -1
    assert not -1 == LIMIT_MAX

    # operator!=
    assert not LIMIT_MAX != LIMIT_MAX
    assert LIMIT_MAX != UNDEFINED
    assert LIMIT_MAX != POSITIVE_INFINITY
    assert LIMIT_MAX != NEGATIVE_INFINITY

    assert LIMIT_MAX != -1
    assert -1 != LIMIT_MAX

    # operator>
    assert not LIMIT_MAX > LIMIT_MAX
    assert not LIMIT_MAX < LIMIT_MAX

    d = {LIMIT_MAX: 0}
    assert LIMIT_MAX in d
