# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for number_of_words.
"""

import unittest
from libsemigroups_pybind11 import number_of_words


class TestNumberOfWords(unittest.TestCase):
    def test_000(self):
        self.assertEqual(number_of_words(3, 1, 4), 39)
        self.assertEqual(number_of_words(3, 0, 10), 29524)
        self.assertEqual(number_of_words(2, 5, 6), 32)
        self.assertEqual(number_of_words(3, 0, 13), 797161)
        self.assertEqual(number_of_words(2, 4, 1), 0)
        self.assertEqual(number_of_words(2, 4, 4), 0)
        self.assertEqual(number_of_words(2, 4, 2), 0)
