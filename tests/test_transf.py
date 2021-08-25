# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for
"""

import unittest

from libsemigroups_pybind11 import Transf16, Transf1, Transf2, Transf4


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

    # T.identity, operator==, and operator!=
    self.assertEqual(x.identity(), T.make(range(16)))
    self.assertNotEqual(x.identity(), x)
    self.assertGreater(x, x.identity())
    self.assertLess(x.identity(), x)
    self.assertLessEqual(x.identity(), x)
    self.assertLessEqual(x, x)
    self.assertGreaterEqual(x.identity(), x.identity())
    self.assertGreaterEqual(x, x.identity())

    # T.make_identity (static)
    self.assertEqual(T.make_identity(16), x.identity())

    # T.rank()
    self.assertEqual(x.rank(), 15)
    self.assertEqual(x.identity().rank(), 16)
    self.assertEqual(T.make([0] * 16).rank(), 1)

    # T.degree()
    self.assertEqual(x.degree(), 16)
    self.assertEqual(x.identity().degree(), 16)
    self.assertEqual(T.make([0] * 16).degree(), 16)

    # Product in-place
    y = x.identity()
    z = x.identity()
    z.product_inplace(x, y)
    self.assertEqual(z, x)
    self.assertEqual(x, T.make([1, 1, 2, 3] + list(range(4, 16))))
    self.assertEqual(y, x.identity())
    z.product_inplace(x, x)
    self.assertEqual(z, x * x)
    self.assertEqual(x, T.make([1, 1, 2, 3] + list(range(4, 16))))
    self.assertEqual(y, x.identity())

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


class TestTransf(unittest.TestCase):
    def test_transf16(self):
        check_transf(self, Transf16)

    def test_transf1(self):
        check_transf(self, Transf1)

    def test_transf2(self):
        check_transf(self, Transf2)

    def test_transf4(self):
        check_transf(self, Transf4)
