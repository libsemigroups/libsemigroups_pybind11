# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
various adapters from libsemigroups.
"""

from typing import TypeVar as _TypeVar
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    # TODO(1) Transf
    # TODO(1) other pperms
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
    to_py as _to_py,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
)

from .detail.decorators import copydoc as _copydoc


########################################################################
# The ImageAction protected class
########################################################################


class _ImageAction(_CxxWrapper):
    """
    This is a protected base class for ImageRightAction and ImageLeftAction.
    See the documentation for more details.
    """

    Element = _TypeVar("Element")
    Point = _TypeVar("Point")

    def __init__(
        self: _Self, *args, point: Point = None, element: Element = None
    ) -> None:
        """
        Construct from sample element and sample point.

        :Keyword Arguments:
          * **element** (*Element*) -- a sample element.
          * **point** (*Point*) -- a sample point.

        :raises KeyError:
            if the action defined by the arguments is not defined.
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
            raise TypeError(
                f"expected 0 positional arguments, but found {len(args)}"
            )
        self.py_template_params = (
            type(_to_cxx(element)),
            type(_to_cxx(point)),
        )
        self.init_cxx_obj()

    def __call__(self: _Self, *args):
        return _to_py(_to_cxx(self)(*(_to_cxx(x) for x in args)))


########################################################################
# The ImageRightAction class
########################################################################


class ImageRightAction(_ImageAction):  # pylint: disable=missing-class-docstring
    Element = _TypeVar("Element")
    Point = _TypeVar("Point")

    __doc__ = _ImageRightActionPPerm1PPerm1.__doc__

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

    @_copydoc(_ImageRightActionPPerm1PPerm1.__call__)
    def __call__(self: _Self, pt: Point, x: Element) -> Point:
        return _to_py(_to_cxx(self)(_to_cxx(pt), _to_cxx(x)))


########################################################################
# Copy mem fns from sample C++ type and register types
########################################################################

_copy_cxx_mem_fns(_ImageRightActionPPerm1PPerm1, ImageRightAction)

for (
    _type
) in (
    ImageRightAction._py_template_params_to_cxx_type.values()  # pylint:disable=protected-access
):
    _register_cxx_wrapped_type(_type, ImageRightAction)


########################################################################
# The ImageLeftAction class
########################################################################


class ImageLeftAction(_ImageAction):  # pylint: disable=missing-class-docstring
    Element = _TypeVar("Element")
    Point = _TypeVar("Point")

    __doc__ = _ImageLeftActionPPerm1PPerm1.__doc__

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

    @_copydoc(_ImageLeftActionPPerm1PPerm1.__call__)
    def __call__(self: _Self, pt: Point, x: Element) -> Point:
        return _to_py(_to_cxx(self)(_to_cxx(pt), _to_cxx(x)))
