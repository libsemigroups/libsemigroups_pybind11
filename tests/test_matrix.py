# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for transformations, partial perms, and
permutations.
"""

import unittest

from libsemigroups_pybind11 import BMat


def check_constructors(self, T):
    # number rows/cols
    x = T(10, 10)
    self.assertEqual(x.number_of_rows(), 10)
    self.assertEqual(x.number_of_cols(), 10)

    # copy
    y = T(x)
    self.assertFalse(y is x)
    self.assertFalse(x is y)
    self.assertEqual(y.number_of_rows(), 10)
    self.assertEqual(y.number_of_cols(), 10)

    # from list of lists
    x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
    self.assertEqual(x.number_of_rows(), 3)
    self.assertEqual(x.number_of_cols(), 3)
    self.assertEqual(x[0, 0], 0)
    self.assertEqual(x[0, 1], 1)
    self.assertEqual(x[0, 2], 1)
    self.assertEqual(x[1, 0], 1)
    self.assertEqual(x[1, 1], 0)
    self.assertEqual(x[1, 2], 1)
    self.assertEqual(x[2, 0], 1)
    self.assertEqual(x[2, 1], 1)
    self.assertEqual(x[2, 2], 1)

    # T.make_identity (static)
    self.assertEqual(T.make_identity(3), T([[1, 0, 0], [0, 1, 0], [0, 0, 1]]))


def check_comparison_ops(self, T):
    x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
    y = T.make_identity(3)
    self.assertLess(x, y)
    self.assertEqual(x, x)
    self.assertEqual(y, y)
    self.assertGreater(y, x)
    self.assertNotEqual(y, x)
    self.assertNotEqual(x, y)


def check_add_ops(self, T):
    x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
    y = T([[0, 0, 0], [0, 1, 0], [1, 1, 1]])
    x += y
    self.assertEqual(x, T([[0, 1, 1], [1, 1, 1], [1, 1, 1]]))
    self.assertEqual(x + y, T([[0, 1, 1], [1, 1, 1], [1, 1, 1]]))


def check_mul_ops(self, T):
    x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
    y = T([[0, 0, 0], [0, 1, 0], [1, 1, 1]])
    z = x * y
    x *= y
    self.assertEqual(z, x)


def check_rows(self, T):
    x = T([[0, 1, 1], [1, 0, 1], [1, 1, 1]])
    self.assertEqual(x.row(0), T([[0, 1, 1]]))
    self.assertEqual(x.row(1), T([[1, 0, 1]]))
    self.assertEqual(x.row(2), T([[1, 1, 1]]))
    self.assertEqual(x.rows(), [T([[0, 1, 1]]), T([[1, 0, 1]]), T([[1, 1, 1]])])


class TestBMat(unittest.TestCase):
    def test_constructors(self):
        check_constructors(self, BMat)

    def test_comparison_ops(self):
        check_comparison_ops(self, BMat)

    def test_add_ops(self):
        check_add_ops(self, BMat)

    def test_mul_ops(self):
        check_mul_ops(self, BMat)

    def test_rows(self):
        check_rows(self, BMat)
