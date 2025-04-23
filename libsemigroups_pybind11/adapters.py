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

from typing import Any, Union, Iterator, TypeVar as _TypeVar, List

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (
    # TODO Transf
    # TODO other pperms
    BMat8 as _BMat8,
    ImageLeftActionBMat8BMat8 as _ImageLeftActionBMat8BMat8,
    ImageLeftActionPPerm1PPerm1 as _ImageLeftActionPPerm1PPerm1,
    ImageRightActionBMat8BMat8 as _ImageRightActionBMat8BMat8,
    ImageRightActionPPerm1List as _ImageRightActionPPerm1List,
    ImageRightActionPPerm1PPerm1 as _ImageRightActionPPerm1PPerm1,
    PPerm1 as _PPerm1,
)

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    to_cxx as _to_cxx,
    to_py_new as _to_py,
)

from .tools import ordinal
from .transf import PPerm as _PPerm


class _ImageAction(_CxxWrapper):
    """
    This is a protected base class for ImageRightAction and ImageLeftAction.
    """

    Element = _TypeVar("Element")
    Point = _TypeVar("Point")

    def __init__(self: _Self, *args, point=None, element=None) -> None:
        """
        TODO
        Note to self <point> and <element> are the types of the objects used by
        this function not examples of such objects.
        """
        super().__init__(
            *args,
            required_kwargs=("element", "point"),
            point=point,
            element=element,
        )
        if _to_cxx(self) is not None:
            return
        if len(args) != 0:
            raise ValueError(f"expected 0 positional arguments, but found {len(args)}")
        self.py_template_params = (
            type(_to_cxx(element)),
            type(_to_cxx(point)),
        )
        self.init_cxx_obj()

    def __call__(self: _Self, *args):
        return _to_py(_to_cxx(self)(*(_to_cxx(x) for x in args)))


class ImageRightAction(_ImageAction):
    """TODO update
    Construct a ImageRightAction instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
    """

    _py_template_params_to_cxx_type = {
        (_BMat8, _BMat8): _ImageRightActionBMat8BMat8,
        (_PPerm1, _PPerm1): _ImageRightActionPPerm1PPerm1,
        (_PPerm1, list): _ImageRightActionPPerm1List,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}


class ImageLeftAction(_ImageAction):
    """TODO update
    Construct a ImageLeftAction instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
    """

    _py_template_params_to_cxx_type = {
        (_BMat8, _BMat8): _ImageLeftActionBMat8BMat8,
        (_PPerm1, _PPerm1): _ImageLeftActionPPerm1PPerm1,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}
