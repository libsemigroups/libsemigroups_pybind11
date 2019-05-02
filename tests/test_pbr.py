# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for PBRs.
"""

import unittest

from element import check_products
from libsemigroups_pybind11 import PBR


class TestPBR(unittest.TestCase):
    def test_ops(self):
        x = PBR.make([[0, 1, 2]] * 6)
        # T.identity, operator==, and operator!=
        self.assertEqual(
            x.identity(),
            PBR.make([[3], [4], [5], [0], [1], [2]]),
        )
        self.assertNotEqual(x.identity(), x)
        # self.assertGreater(x, x.identity())
        self.assertLess(x, x.identity())
        # self.assertLessEqual(x.identity(), x)
        # self.assertLessEqual(x, x)
        # self.assertGreaterEqual(x.identity(), x.identity())
        # self.assertGreaterEqual(x, x.identity())

        # T.make_identity (static)
        self.assertEqual(PBR.make_identity(x.degree()), x.identity())

    def test_products(self):
        x = PBR.make([[0, 1, 2]] * 6)
        check_products(self, x)

    def test_constructors(self):
        T = PBR
        # T.make
        with self.assertRaises(RuntimeError):
            T.make([[1, 1, 2, 16]] * 2)
        with self.assertRaises(TypeError):
            T.make([1, 1, 2, 16] + list(range(4, 16)))

        with self.assertRaises(TypeError):
            T.make([-1, 1, 2, 6] + list(range(4, 16)))

        # T.__get_item__
        x = PBR.make([[0, 1, 2]] * 6)
        self.assertEqual(x[0], [0, 1, 2])
        self.assertEqual(x[1], x[0])
        self.assertEqual(x[2], x[0])
        self.assertEqual(x[3], x[0])

        y = PBR(x)
        self.assertFalse(y is x)

    def test_rank_degree(self):
        x = PBR.make([[0, 1, 2]] * 6)

        # T.degree()
        self.assertEqual(x.degree(), 3)
        self.assertEqual(x.identity().degree(), 3)
        self.assertEqual(PBR.make([[0, 1, 2]] * 16).degree(), 8)
