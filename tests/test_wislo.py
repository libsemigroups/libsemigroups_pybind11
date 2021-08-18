# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for wislo.
"""

import unittest
from libsemigroups_pybind11 import wislo, number_of_words


class WisloTests(unittest.TestCase):
    def test_000(self):
        first = [0]
        last = [0, 0, 0, 0]
        w = list(wislo(2, first, last))
        self.assertEqual(len(w), 14)
        u = [list(x) for x in w]
        self.assertEqual(
            u,
            [
                [0],
                [1],
                [0, 0],
                [0, 1],
                [1, 0],
                [1, 1],
                [0, 0, 0],
                [0, 0, 1],
                [0, 1, 0],
                [0, 1, 1],
                [1, 0, 0],
                [1, 0, 1],
                [1, 1, 0],
                [1, 1, 1],
            ],
        )

    def test_001(self):
        first = [0, 0, 0, 0]
        last = [0, 0, 0, 0, 0]
        w1 = list(wislo(2, last, first))
        self.assertEqual(w1, [])
        w2 = list(wislo(2, last, last))
        self.assertEqual(w2, [])
        w3 = list(wislo(2, last, [0, 0, 0, 0, 0, 0]))
        self.assertEqual(len(w3), 32)
        u = [list(x) for x in w3]
        self.assertEqual(
            u,
            [
                [0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1],
                [0, 0, 0, 1, 0],
                [0, 0, 0, 1, 1],
                [0, 0, 1, 0, 0],
                [0, 0, 1, 0, 1],
                [0, 0, 1, 1, 0],
                [0, 0, 1, 1, 1],
                [0, 1, 0, 0, 0],
                [0, 1, 0, 0, 1],
                [0, 1, 0, 1, 0],
                [0, 1, 0, 1, 1],
                [0, 1, 1, 0, 0],
                [0, 1, 1, 0, 1],
                [0, 1, 1, 1, 0],
                [0, 1, 1, 1, 1],
                [1, 0, 0, 0, 0],
                [1, 0, 0, 0, 1],
                [1, 0, 0, 1, 0],
                [1, 0, 0, 1, 1],
                [1, 0, 1, 0, 0],
                [1, 0, 1, 0, 1],
                [1, 0, 1, 1, 0],
                [1, 0, 1, 1, 1],
                [1, 1, 0, 0, 0],
                [1, 1, 0, 0, 1],
                [1, 1, 0, 1, 0],
                [1, 1, 0, 1, 1],
                [1, 1, 1, 0, 0],
                [1, 1, 1, 0, 1],
                [1, 1, 1, 1, 0],
                [1, 1, 1, 1, 1],
            ],
        )

    def test_002(self):
        first = []
        last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        w = list(wislo(3, first, last))
        self.assertEqual(len(w), 29524)
        self.assertEqual(len(w), number_of_words(3, 0, 10))

    def test_003(self):
        first = []
        last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        w = list(wislo(3, first, last))
        self.assertEqual(len(w), 797161)
        self.assertEqual(len(w), number_of_words(3, 0, 13))
