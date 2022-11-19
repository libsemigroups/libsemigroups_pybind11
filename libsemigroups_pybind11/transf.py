# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name
# pylint: disable=bad-option-value, consider-using-f-string, duplicate-code

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
    """
    Construct the minimum space occupying _libsemigroups_pybind11
    transformation from a list of images.
    """
    if not isinstance(images, list):
        raise TypeError("the argument must be a list")
    if len(images) <= 16:
        images += range(len(images), 16)
        return Transf16.make(images)
    if len(images) <= 2 ** 8:
        return Transf1.make(images)
    if len(images) <= 2 ** 16:
        return Transf2.make(images)
    if len(images) <= 2 ** 32:
        return Transf4.make(images)
    raise ValueError(
        "the argument (a list) is must have length at most %d, found %d"
        % (2 ** 32, len(images))
    )


def PPerm(dom: List[int], ran: List[int], deg: int):
    """
    Construct the minimum space occupying _libsemigroups_pybind11
    partial perm from a list of images.
    """
    if not isinstance(dom, list):
        raise TypeError("the 1st argument must be a list")
    if not isinstance(ran, list):
        raise TypeError("the 2nd argument must be a list")
    if not isinstance(deg, int):
        raise TypeError("the 3rd argument must be an int")
    if deg <= 16:
        return PPerm16.make(dom, ran, 16)
    if deg <= 2 ** 8:
        return PPerm1.make(dom, ran, deg)
    if deg <= 2 ** 16:
        return PPerm2.make(dom, ran, deg)
    if deg <= 2 ** 32:
        return PPerm4.make(dom, ran, deg)
    raise ValueError(
        "the argument (a list) is must have length at most %d, found %d"
        % (2 ** 32, deg)
    )


def Perm(images: List[int]):
    """
    Construct the minimum space occupying _libsemigroups_pybind11
    perm from a list of images.
    """
    if not isinstance(images, list):
        raise TypeError("the argument must be a list")
    if len(images) <= 16:
        images += range(len(images), 16)
        return Perm16.make(images)
    if len(images) <= 2 ** 8:
        return Perm1.make(images)
    if len(images) <= 2 ** 16:
        return Perm2.make(images)
    if len(images) <= 2 ** 32:
        return Perm4.make(images)
    raise ValueError(
        "the argument (a list) is must have length at most %d, found %d"
        % (2 ** 32, len(images))
    )
