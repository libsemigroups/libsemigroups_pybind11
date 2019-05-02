# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for bipartitions.
"""

import unittest
from itertools import chain

from element import check_products
from libsemigroups_pybind11 import Bipartition


class TestBipart(unittest.TestCase):
    def test_ops(self):
        x = Bipartition.make([0, 1, 2, 3, 0, 2])
        # T.identity, operator==, and operator!=
        self.assertEqual(
            x.identity(),
            Bipartition.make(list(chain(range(x.degree()), range(x.degree())))),
        )
        self.assertNotEqual(x.identity(), x)
        # self.assertGreater(x, x.identity())
        self.assertLess(x.identity(), x)
        # self.assertLessEqual(x.identity(), x)
        # self.assertLessEqual(x, x)
        # self.assertGreaterEqual(x.identity(), x.identity())
        # self.assertGreaterEqual(x, x.identity())

        # T.make_identity (static)
        self.assertEqual(Bipartition.make_identity(x.degree()), x.identity())

    def test_products(self):
        x = Bipartition.make([0, 1, 2, 3, 0, 2])
        check_products(self, x)

    def test_constructors(self):
        T = Bipartition
        # T.make
        with self.assertRaises(RuntimeError):
            T.make([1, 1, 2, 16] + list(range(4, 16)))

        with self.assertRaises(TypeError):
            T.make([-1, 1, 2, 6] + list(range(4, 16)))

        # T.__get_item__
        x = Bipartition.make([0, 1, 2, 3, 0, 2])
        self.assertEqual(x[0], 0)
        self.assertEqual(x[1], 1)
        self.assertEqual(x[2], 2)
        self.assertEqual(x[3], 3)

        y = Bipartition(x)
        self.assertFalse(y is x)

    def test_rank_degree(self):
        T = Bipartition
        x = Bipartition.make([0, 1, 2, 3, 0, 2])
        # T.rank()
        self.assertEqual(x.rank(), 2)
        self.assertEqual(x.identity().rank(), 3)
        self.assertEqual(T.make([0] * 16).rank(), 1)

        # T.degree()
        self.assertEqual(x.degree(), 3)
        self.assertEqual(x.identity().degree(), 3)
        self.assertEqual(T.make([0] * 16).degree(), 8)

    def test_iterators(self):
        x = Bipartition.make([0, 1, 2, 3, 0, 2])

        self.assertEqual(
            list(x.left_blocks()),
            [0, 1, 2],
        )
        self.assertEqual(
            list(x.right_blocks()),
            [3, 0, 2],
        )

        self.assertEqual(
            x.lookup(),
            [True, False, True],
        )
