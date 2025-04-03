# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module provides some tools for libsemigroups_pybind11.
"""


def ordinal(n: int):
    """
    Returns 1st from 1, 2nd from 2 etc.
    From https://stackoverflow.com/questions/9647202/
    """
    if 11 <= (n % 100) <= 13:
        suffix = "th"
    else:
        suffix = ["th", "st", "nd", "rd", "th"][min(n % 10, 4)]
    return str(n) + suffix
