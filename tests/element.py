# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for elements.
"""


def check_products(self, x):
    y = x.identity()
    z = x.identity()
    z.product_inplace(x, y, 0)
    self.assertEqual(z, x)
    z.product_inplace(y, x, 0)
    self.assertEqual(z, x)
    z.product_inplace(x, x, 0)
    self.assertEqual(z, x * x)
    self.assertEqual(x * x.identity(), x)
    self.assertEqual(x.identity() * x, x)
