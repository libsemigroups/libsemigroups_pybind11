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

from typing import Any

from typing_extensions import Self

from _libsemigroups_pybind11 import (
    ImageRightActionBMat8BMat8 as _ImageRightActionBMat8BMat8,
    ImageLeftActionBMat8BMat8 as _ImageLeftActionBMat8BMat8,
    ImageRightActionPPerm1PPerm1 as _ImageRightActionPPerm1PPerm1,
    ImageLeftActionPPerm1PPerm1 as _ImageLeftActionPPerm1PPerm1,
    ImageRightActionPPerm1List as _ImageRightActionPPerm1List,
    # TODO Transf
    # TODO other pperms
    BMat8 as _BMat8,
    PPerm1 as _PPerm1,
)

from .detail.cxx_wrapper import CxxWrapper, to_cxx, to_py

from .tools import ordinal
from .transf import PPerm


class _ImageAction(CxxWrapper):
    def __init__(self: Self, **kwargs):
        super().__init__(required_kwargs=("Element", "Point"), **kwargs)
        self.Element = kwargs["Element"]
        self.Point = kwargs["Point"]

    def _init_cxx_obj(self: Self, elt: Any, pt: Any) -> Any:
        cxx_obj_t = self._cxx_obj_type_from(samples=(elt, pt))
        if self._cxx_obj is None or not isinstance(self._cxx_obj, cxx_obj_t):
            self._cxx_obj = cxx_obj_t()
        return self._cxx_obj

    def __call__(  # pylint: disable=inconsistent-return-statements
        self: Self, *args
    ):
        # Point1, Point2, Element -> Point1 = Point2 ^ Element
        if 2 > len(args) or len(args) > 3:
            raise TypeError(f"expected 2 or 3 arguments, found {len(args)}")
        pt = args[-2]
        x = args[-1]
        if not isinstance(pt, self.Point):
            raise ValueError(
                f"the {ordinal(len(args) - 2)} argument (point) has incorrect type, "
                f"expected {self.Point} but found {type(pt)}"
            )
        if not isinstance(x, self.Element):
            raise ValueError(
                f"the {ordinal(len(args) - 1)} argument (element) has incorrect type, "
                f"expected {self.Element} but found {type(x)}"
            )
        if len(args) == 3:
            res = args[0]
            if not isinstance(res, self.Point):
                raise ValueError(
                    "the 1st argument (result) has incorrect type, "
                    f"expected {self.Point} but found {type(res)}"
                )
        if len(args) == 3 and self.Point is list:
            raise NotImplementedError("not yet implemented")

        self._init_cxx_obj(x, pt)
        return to_py(
            self.Element, self._cxx_obj(*(to_cxx(arg) for arg in args))
        )


class ImageRightAction(_ImageAction):
    # pylint: disable=too-few-public-methods, unused-private-member
    """
    Construct a ImageRightAction instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
    """

    _py_template_params_to_cxx_type = {
        (_BMat8, _BMat8): _ImageRightActionBMat8BMat8,
        (PPerm, PPerm): {
            (_PPerm1, _PPerm1): _ImageRightActionPPerm1PPerm1,
        },
        (PPerm, list): {
            (_PPerm1, list): _ImageRightActionPPerm1List,
        },
    }


class ImageLeftAction(_ImageAction):  # pylint: disable=invalid-name
    # pylint: disable=too-few-public-methods, unused-private-member
    """
    Construct a ImageLeftAction instance.

    :Keyword Arguments:
        * *Element* -- the type of the elements in the action
        * *Point* -- the type of the points acted on
    """

    _py_template_params_to_cxx_type = {
        (_BMat8, _BMat8): _ImageLeftActionBMat8BMat8,
        (PPerm, PPerm): {
            (_PPerm1, _PPerm1): _ImageLeftActionPPerm1PPerm1,
        },
    }
