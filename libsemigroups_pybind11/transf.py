# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides a the user-facing python part of libsemigroups_pybind11
relating to transformations.
"""
from typing import List
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


def Transf(images: List[int]):
    if not isinstance(images, list):
        raise TypeError("the argument must be a list")
    if len(images) <= 16:
        images += range(len(images), 16)
        return Transf16.make(images)
    elif len(images) <= 2 ** 8:
        return Transf1.make(images)
    elif len(images) <= 2 ** 16:
        return Transf2.make(images)
    elif len(images) <= 2 ** 32:
        return Transf4.make(images)
    else:
        raise ValueError(
            "the argument (a list) is must have length at most %d, found %d"
            % (2 ** 32, len(images))
        )


def PPerm(images: List[int]):
    if not isinstance(images, list):
        raise TypeError("the argument must be a list")
    if len(images) <= 16:
        images += range(len(images), 16)
        return PPerm16.make(images)
    elif len(images) <= 2 ** 8:
        return PPerm1.make(images)
    elif len(images) <= 2 ** 16:
        return PPerm2.make(images)
    elif len(images) <= 2 ** 32:
        return PPerm4.make(images)
    else:
        raise ValueError(
            "the argument (a list) is must have length at most %d, found %d"
            % (2 ** 32, len(images))
        )


def Perm(images: List[int]):
    if not isinstance(images, list):
        raise TypeError("the argument must be a list")
    if len(images) <= 16:
        images += range(len(images), 16)
        return Perm16.make(images)
    elif len(images) <= 2 ** 8:
        return Perm1.make(images)
    elif len(images) <= 2 ** 16:
        return Perm2.make(images)
    elif len(images) <= 2 ** 32:
        return Perm4.make(images)
    else:
        raise ValueError(
            "the argument (a list) is must have length at most %d, found %d"
            % (2 ** 32, len(images))
        )
