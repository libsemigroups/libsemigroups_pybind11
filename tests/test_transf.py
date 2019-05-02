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

from _libsemigroups_pybind11 import (
    Transf16,
    Transf1,
    Transf2,
    Transf4,
    PPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    Perm16,
    Perm1,
    Perm2,
    Perm4,
)


def check_identity_ops(self, T, x):
    # T.identity, operator==, and operator!=
    self.assertEqual(x.identity(), T.make(range(x.degree())))
    self.assertNotEqual(x.identity(), x)
    self.assertGreater(x, x.identity())
    self.assertLess(x.identity(), x)
    self.assertLessEqual(x.identity(), x)
    self.assertLessEqual(x, x)
    self.assertGreaterEqual(x.identity(), x.identity())
    self.assertGreaterEqual(x, x.identity())

    # T.make_identity (static)
    self.assertEqual(T.make_identity(x.degree()), x.identity())


def check_product_inplace(self, x):
    y = x.identity()
    z = x.identity()
    z.product_inplace(x, y)
    self.assertEqual(z, x)
    z.product_inplace(y, x)
    self.assertEqual(z, x)
    z.product_inplace(x, x)
    self.assertEqual(z, x * x)


def check_transf(self, T):

    # T.make
    with self.assertRaises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with self.assertRaises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T.make([1, 1, 2, 3] + list(range(4, 16)))
    self.assertEqual(x[0], 1)
    self.assertEqual(x[1], 1)
    self.assertEqual(x[2], 2)
    self.assertEqual(x[3], 3)

    # T.identity, T.make_identity, operator==, and operator!=
    check_identity_ops(self, T, x)

    # T.rank()
    self.assertEqual(x.rank(), 15)
    self.assertEqual(x.identity().rank(), 16)
    self.assertEqual(T.make([0] * 16).rank(), 1)

    # T.degree()
    self.assertEqual(x.degree(), 16)
    self.assertEqual(x.identity().degree(), 16)
    self.assertEqual(T.make([0] * 16).degree(), 16)

    # Product in-place
    check_product_inplace(self, x)

    # T.images
    self.assertEqual(
        list(x.images()), [1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
    )

    if T is not Transf16:
        x = T.make(
            [
                15,
                5,
                2,
                10,
                17,
                8,
                13,
                15,
                1,
                9,
                4,
                0,
                15,
                5,
                14,
                11,
                15,
                4,
                7,
                3,
            ]
        )
        self.assertEqual(x.degree(), 20)
        self.assertEqual(x.rank(), 15)
        self.assertEqual(x[5], 8)
        self.assertEqual(
            x * x * x,
            T.make(
                [
                    0,
                    1,
                    2,
                    17,
                    17,
                    5,
                    8,
                    0,
                    8,
                    9,
                    4,
                    11,
                    0,
                    1,
                    14,
                    15,
                    0,
                    4,
                    11,
                    4,
                ]
            ),
        )


def check_pperm(self, T):

    # T.make
    with self.assertRaises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with self.assertRaises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T.make([1, 2, 3], [4, 7, 6], 16)
    self.assertEqual(x[0], T.undef())
    self.assertEqual(x[1], 4)
    self.assertEqual(x[2], 7)
    self.assertEqual(x[3], 6)
    self.assertEqual(x[4], T.undef())

    # T.identity, T.make_identity, operator==, and operator!=
    check_identity_ops(self, T, x)

    self.assertEqual(x * x.right_one(), x)
    self.assertEqual(x.left_one() * x, x)
    self.assertEqual(x * x.inverse(), x.left_one())
    self.assertEqual(x.inverse() * x, x.right_one())
    # self.assertEqual(list(x.left_one().images()), x)

    # T.rank()
    self.assertEqual(x.rank(), 3)
    self.assertEqual(x.identity().rank(), 16)

    # T.degree()
    self.assertEqual(x.degree(), 16)
    self.assertEqual(x.identity().degree(), 16)

    # Product in-place
    check_product_inplace(self, x)

    # T.images
    self.assertEqual(
        list(x.images()),
        [T.undef(), 4, 7, 6] + [T.undef()] * 12,
    )


def check_perm(self, T):
    # T.make
    with self.assertRaises(RuntimeError):
        T.make([1, 1, 2, 16] + list(range(4, 16)))

    with self.assertRaises(TypeError):
        T.make([-1, 1, 2, 6] + list(range(4, 16)))

    # T.__get_item__
    x = T.make([1, 2, 3, 0, 6, 5, 4] + list(range(7, 16)))
    self.assertEqual(x[0], 1)
    self.assertEqual(x[1], 2)
    self.assertEqual(x[2], 3)
    self.assertEqual(x[3], 0)
    self.assertEqual(x[4], 6)

    # T.identity, T.make_identity, operator==, and operator!=
    check_identity_ops(self, T, x)

    self.assertEqual(x.inverse() * x, x.identity())
    self.assertEqual(x * x.inverse(), x.identity())

    # T.rank()
    self.assertEqual(x.rank(), 16)
    self.assertEqual(x.identity().rank(), 16)

    # T.degree()
    self.assertEqual(x.degree(), 16)
    self.assertEqual(x.identity().degree(), 16)

    # Product in-place
    check_product_inplace(self, x)

    # T.images
    self.assertEqual(
        list(x.images()), [1, 2, 3, 0, 6, 5, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15]
    )


class TestTransf(unittest.TestCase):
    def test_transf16(self):
        check_transf(self, Transf16)

    def test_transf1(self):
        check_transf(self, Transf1)

    def test_transf2(self):
        check_transf(self, Transf2)

    def test_transf4(self):
        check_transf(self, Transf4)


class TestPPerm(unittest.TestCase):
    def test_pperm16(self):
        check_pperm(self, PPerm16)

    def test_pperm1(self):
        check_pperm(self, PPerm1)

    def test_pperm2(self):
        check_pperm(self, PPerm2)

    def test_pperm4(self):
        check_pperm(self, PPerm4)


class TestPerm(unittest.TestCase):
    def test_perm16(self):
        check_perm(self, Perm16)

    def test_perm1(self):
        check_perm(self, Perm1)

    def test_perm2(self):
        check_perm(self, Perm2)

    def test_perm4(self):
        check_perm(self, Perm4)
