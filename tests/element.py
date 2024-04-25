# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for elements.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name


def check_products(x):
    y = x.identity()
    z = x.identity()
    z.product_inplace(x, y, 0)
    assert z == x
    z.product_inplace(y, x, 0)
    assert z == x
    z.product_inplace(x, x, 0)
    assert z == x * x
    assert x * x.identity() == x
    assert x.identity() * x == x
