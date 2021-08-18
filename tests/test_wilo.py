# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for wilo.
"""

import unittest
from libsemigroups_pybind11 import wilo, number_of_words


class TestWilo(unittest.TestCase):
    def test_001(self):
        u = [0, 0, 0, 0]
        v = [1, 1, 1, 1]
        self.assertEqual(list(wilo(2, 1, v, u)), [])
        self.assertEqual(list(wilo(2, 1, u, u)), [])
        w1 = list(wilo(2, 2, [], [1, 1]))
        self.assertEqual(len(w1), 3)
        w2 = [list(x) for x in w1]
        self.assertEqual(w2, [[], [0], [1]])
        w3 = list(wilo(2, 1, [], [1, 1]))
        self.assertEqual(len(w3), 1)
        self.assertEqual(list(w3[0]), [])
        w4 = list(wilo(2, 1, [], [0]))
        self.assertEqual(len(w4), 1)
        self.assertEqual(list(w4[0]), [])

    def test_002(self):
        w = list(wilo(2, 4, [0], [1, 1, 1, 1]))
        u = [list(x) for x in w]
        self.assertEqual(
            u,
            [
                [0],
                [0, 0],
                [0, 0, 0],
                [0, 0, 1],
                [0, 1],
                [0, 1, 0],
                [0, 1, 1],
                [1],
                [1, 0],
                [1, 0, 0],
                [1, 0, 1],
                [1, 1],
                [1, 1, 0],
                [1, 1, 1],
            ],
        )
        self.assertEqual(len(u), 14)

    def test_003(self):
        first = [0]
        last = [2, 2, 2, 2]
        w = list(wilo(3, 4, first, last))
        self.assertEqual(len(w), 39)
        self.assertEqual(len(w), number_of_words(3, 1, 4))
        u = [list(x) for x in w]
        self.assertEqual(
            u,
            [
                [0],
                [0, 0],
                [0, 0, 0],
                [0, 0, 1],
                [0, 0, 2],
                [0, 1],
                [0, 1, 0],
                [0, 1, 1],
                [0, 1, 2],
                [0, 2],
                [0, 2, 0],
                [0, 2, 1],
                [0, 2, 2],
                [1],
                [1, 0],
                [1, 0, 0],
                [1, 0, 1],
                [1, 0, 2],
                [1, 1],
                [1, 1, 0],
                [1, 1, 1],
                [1, 1, 2],
                [1, 2],
                [1, 2, 0],
                [1, 2, 1],
                [1, 2, 2],
                [2],
                [2, 0],
                [2, 0, 0],
                [2, 0, 1],
                [2, 0, 2],
                [2, 1],
                [2, 1, 0],
                [2, 1, 1],
                [2, 1, 2],
                [2, 2],
                [2, 2, 0],
                [2, 2, 1],
                [2, 2, 2],
            ],
        )

    def test_004(self):
        first = []
        last = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
        w = list(wilo(3, 10, first, last))
        self.assertEqual(len(w), 29524)
        self.assertEqual(len(w), number_of_words(3, 0, 10))

    def test_005(self):
        first = []
        last = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
        w = list(wilo(3, 13, first, last))
        self.assertEqual(len(w), 797161)
        self.assertEqual(len(w), number_of_words(3, 0, 13))

    def test_006(self):
        first = []
        last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        w = list(wilo(1, 10, first, last))
        self.assertEqual(len(w), 10)
        u = [list(x) for x in w]
        self.assertEqual(
            u,
            [
                [],
                [0],
                [0, 0],
                [0, 0, 0],
                [0, 0, 0, 0],
                [0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0],
            ],
        )
        w = list(wilo(0, 0, first, last))
        self.assertEqual(w, [])
        first = [0, 0]
        last = [0, 0, 0, 0]
        w = list(wilo(1, 4, first, last))
        self.assertEqual(len(w), 2)
        self.assertEqual(list(w[0]), [0, 0])
        self.assertEqual(list(w[1]), [0, 0, 0])
        w = list(wilo(1, 1, first, last))
        self.assertEqual(w, [])
        w = list(wilo(1, 5, [0], [1]))
        self.assertEqual(len(w), 4)
        u = [list(x) for x in w]
        self.assertEqual(u, [[0], [0, 0], [0, 0, 0], [0, 0, 0, 0]])

    def test_007(self):
        first = [0, 1, 2, 3]
        last = [4, 4, 4, 4, 4]
        w = list(wilo(4, 5, first, last))
        self.assertEqual(len(w), 303)

    def test_008(self):
        first = [0, 1]
        last = [1, 1, 1]
        w = list(wilo(2, 3, first, last))
        self.assertEqual(len(w), 4)
        u = [list(x) for x in w]
        self.assertEqual(u, [[0, 1], [1], [1, 0], [1, 1]])
        w = list(wilo(2, 1, first, last))
        self.assertEqual(w, [])
