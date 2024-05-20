# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint:disable=no-name-in-module, unused-import

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
various adapters from libsemigroups.
"""
from _libsemigroups_pybind11 import (
    ImageRightActionBMat8BMat8 as _ImageRightActionBMat8BMat8,
    ImageLeftActionBMat8BMat8 as _ImageLeftActionBMat8BMat8,
    ImageLeftActionPPerm16PPerm16 as _ImageLeftActionPPerm16PPerm16,
    ImageRightActionPPerm16PPerm16 as _ImageRightActionPPerm16PPerm16,
)
from _libsemigroups_pybind11 import BMat8, StaticPPerm16


def ImageRightAction(**kwargs):  # pylint: disable=invalid-name
    """
    Construct a ImageRightAction instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
    """
    if len(kwargs) != 2:
        raise TypeError(f"expected 2 keyword arguments, found {len(kwargs)}")
    for kwarg in ("Element", "Point"):
        if kwarg not in kwargs:
            raise ValueError(
                f'unexpected keyword argument "{kwarg}", expected'
                + '"Element" and "Point"'
            )

    lookup = {
        (BMat8, BMat8): _ImageRightActionBMat8BMat8,
        (StaticPPerm16, StaticPPerm16): _ImageRightActionPPerm16PPerm16,
    }
    args = (kwargs["Element"], kwargs["Point"])

    if args in lookup:
        return lookup[args]()

    raise ValueError(
        f"unexpected keyword argument combination {args}, expected one of {lookup.keys()}"
    )


def ImageLeftAction(**kwargs):  # pylint: disable=invalid-name
    """
    Construct a ImageLeftAction instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
    """
    if len(kwargs) != 2:
        raise TypeError(f"expected 2 keyword arguments, found {len(kwargs)}")
    for kwarg in ("Element", "Point"):
        if kwarg not in kwargs:
            raise ValueError(
                f'unexpected keyword argument "{kwarg}", expected'
                + '"Element" and "Point"'
            )

    lookup = {
        (BMat8, BMat8): _ImageLeftActionBMat8BMat8,
        (StaticPPerm16, StaticPPerm16): _ImageLeftActionPPerm16PPerm16,
    }

    args = (kwargs["Element"], kwargs["Point"])

    if args in lookup:
        return lookup[args]()

    raise ValueError(
        f"unexpected keyword argument combination {args}, expected one of {lookup.keys()}"
    )
