# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module provides some tools for building libsemigroups_pybind11.
"""


def minimum_libsemigroups_version():
    """
    Returns the minimum required version of libsemigroups required to build
    libsemigroups_pybind11.
    """
    return "3.1.3"
