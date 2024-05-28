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

from libsemigroups_pybind11 import one


def check_products(x):
    y = one(x)
    z = x * y
    assert z == x
    z = y * x
    assert z == x
    z = x * x
    assert z == x * x
    assert x * one(x) == x
    assert one(x) * x == x
