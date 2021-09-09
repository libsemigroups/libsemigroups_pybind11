# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for matrices.
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
    t = T.make_identity(3)
    t.product_inplace(x, y)
    self.assertEqual(z, t)


def check_transpose(self, T):
    x = T([[0, 1, 1], [0, 0, 1], [0, 0, 0]])
    y = T(x)
    x.transpose()
    self.assertEqual(x, T([[0, 0, 0], [1, 0, 0], [1, 1, 0]]))
    x.transpose()
    self.assertEqual(x, y)


def check_swap(self, T):
    x = T([[0, 1, 1], [0, 0, 1], [0, 0, 0]])
    y = T(x)
    x.transpose()
    x.swap(y)
    self.assertEqual(x, T([[0, 1, 1], [0, 0, 1], [0, 0, 0]]))
    self.assertEqual(y, T([[0, 0, 0], [1, 0, 0], [1, 1, 0]]))
    x.transpose()
    self.assertEqual(x, y)


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

    def test_transpose(self):
        check_transpose(self, BMat)

    def test_swap(self):
        check_swap(self, BMat)

    def test_repr(self):
        self.assertEqual(str(BMat([[0, 1], [1, 0]])), "BMat([[0, 1], [1, 0]])")
