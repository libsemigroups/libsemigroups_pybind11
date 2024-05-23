# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint:disable=no-name-in-module, unused-import

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the Action class from libsemigroups.
"""
from _libsemigroups_pybind11 import (
    RightActionPPerm16PPerm16 as _RightActionPPerm16PPerm16,
    LeftActionPPerm16PPerm16 as _LeftActionPPerm16PPerm16,
    RowActionBMat8 as _RowActionBMat8,
    ColActionBMat8 as _ColActionBMat8,
)

from _libsemigroups_pybind11 import BMat8, side, StaticPPerm16
from .adapters import ImageRightAction, ImageLeftAction


def Action(**kwargs):  # pylint: disable=invalid-name
    """
    Construct an :any:`Action` instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
        * *Func* -- the function defining the action
        * *Side* -- the side (or handedness) of the action
    """
    if len(kwargs) != 4:
        raise TypeError(f"expected 4 keyword arguments, found {len(kwargs)}")
    for kwarg in ("Element", "Point", "Func", "Side"):
        if kwarg not in kwargs:
            raise ValueError(
                f'unexpected keyword argument "{kwarg}", expected'
                + '"Element", "Point", "Func", and "Side"'
            )

    args = (kwargs["Element"], kwargs["Point"], kwargs["Func"], kwargs["Side"])

    lookup = {
        (BMat8, BMat8, ImageRightAction, side.right): _RowActionBMat8,
        (BMat8, BMat8, ImageLeftAction, side.left): _ColActionBMat8,
        (
            StaticPPerm16,
            StaticPPerm16,
            ImageRightAction,
            side.right,
        ): _RightActionPPerm16PPerm16,
        (
            StaticPPerm16,
            StaticPPerm16,
            ImageLeftAction,
            side.left,
        ): _LeftActionPPerm16PPerm16,
    }

    if args in lookup:
        return lookup[args]()

    raise ValueError(
        f"unexpected keyword argument combination {args}, expected one of {lookup.keys()}"
    )


def RightAction(
    Func=ImageRightAction, **kwargs
):  # pylint: disable=invalid-name
    """
    Construct a right :any:`Action` instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
        * *Func* -- the function defining the action (defaults to
          :any:`ImageRightAction`)
    """
    # TODO probably this will generate unhelpful error messages
    return Action(
        Func=Func,
        Element=kwargs["Element"],
        Point=kwargs["Point"],
        Side=side.right,
    )


def LeftAction(Func=ImageLeftAction, **kwargs):  # pylint: disable=invalid-name
    """
    Construct a left :any:`Action` instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
        * *Func* -- the function defining the action (defaults to
          :any:`ImageLeftAction`)

    """
    # TODO probably this will generate unhelpful error messages
    return Action(
        Func=Func,
        Element=kwargs["Element"],
        Point=kwargs["Point"],
        Side=side.left,
    )
