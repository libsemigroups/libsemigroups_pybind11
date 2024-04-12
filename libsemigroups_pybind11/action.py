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

from _libsemigroups_pybind11 import RowActionBMat8 as _RowActionBMat8
from _libsemigroups_pybind11 import ColActionBMat8 as _ColActionBMat8
from _libsemigroups_pybind11 import BMat8, side
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

    if (
        kwargs["Element"] == BMat8
        and kwargs["Point"] == BMat8
        and kwargs["Func"] == ImageRightAction
        and kwargs["Side"] == side.right
    ):
        return _RowActionBMat8()
    if (
        kwargs["Element"] == BMat8
        and kwargs["Point"] == BMat8
        and kwargs["Func"] == ImageLeftAction
        and kwargs["Side"] == side.left
    ):
        return _ColActionBMat8()
    raise ValueError("unexpected keyword argument combination")


def RightAction(
    Func=ImageRightAction, **kwargs
):  # pylint: disable=invalid-name
    """
    Construct a right :any:`Action` instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
        * *Func* -- the function defining the action (defaults to :any:`ImageRightAction`)

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
        * *Func* -- the function defining the action (defaults to :any:`ImageLeftAction`)

    """
    # TODO probably this will generate unhelpful error messages
    return Action(
        Func=Func,
        Element=kwargs["Element"],
        Point=kwargs["Point"],
        Side=side.left,
    )
