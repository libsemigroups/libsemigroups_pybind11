# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""
This module contains some tests for number_of_words.
"""

from libsemigroups_pybind11 import number_of_words


def test_000():
    assert number_of_words(3, 1, 4) == 39
    assert number_of_words(3, 0, 10) == 29524
    assert number_of_words(2, 5, 6) == 32
    assert number_of_words(3, 0, 13) == 797161
    assert number_of_words(2, 4, 1) == 0
    assert number_of_words(2, 4, 4) == 0
    assert number_of_words(2, 4, 2) == 0
