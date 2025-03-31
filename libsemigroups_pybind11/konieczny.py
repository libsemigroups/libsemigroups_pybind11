# -*- coding: utf-8 -*-

# Copyright (c) 2022-2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, duplicate-code
# pylint: disable=missing-class-docstring, missing-function-docstring
# pylint: disable=protected-access

"""
This package provides the user-facing python part of ``libsemigroups_pybind11`` for
:any:`KoniecznyBMat8`.
"""

from copy import copy
from functools import wraps
from typing import Self, TypeVar as _TypeVar, Iterator

from _libsemigroups_pybind11 import (
    BMat as _BMat,
    BMat8 as _BMat8,
    KoniecznyBMat as _KoniecznyBMat,
    KoniecznyBMat8 as _KoniecznyBMat8,
    KoniecznyPPerm1 as _KoniecznyPPerm1,
    KoniecznyPPerm2 as _KoniecznyPPerm2,
    KoniecznyPPerm4 as _KoniecznyPPerm4,
    KoniecznyTransf1 as _KoniecznyTransf1,
    KoniecznyTransf2 as _KoniecznyTransf2,
    KoniecznyTransf4 as _KoniecznyTransf4,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
)

from .detail.cxx_wrapper import (
    to_cxx,
    to_py,
    CxxWrapper,
)

Element = _TypeVar("Element")


########################################################################
# Decorators
########################################################################


def _returns_element(method):
    @wraps(method)
    def wrapper(self, *args):
        return to_py(self.Element, method(self, *args))

    return wrapper


def _returns_D_class(method):
    @wraps(method)
    def wrapper(self, *args):
        return Konieczny.DClass(self.Element, method(self, *args))

    return wrapper


class Konieczny(CxxWrapper):  # pylint: disable=missing-class-docstring
    _py_to_cxx_type_dict = {
        (_BMat,): _KoniecznyBMat,
        (_BMat8,): _KoniecznyBMat8,
        (_PPerm1,): _KoniecznyPPerm1,
        (_PPerm2,): _KoniecznyPPerm2,
        (_PPerm4,): _KoniecznyPPerm4,
        (_Transf1,): _KoniecznyTransf1,
        (_Transf2,): _KoniecznyTransf2,
        (_Transf4,): _KoniecznyTransf4,
    }

    _cxx_type_to_element_dict = {
        _KoniecznyBMat8: _BMat8,
        _KoniecznyBMat: _BMat,
        _KoniecznyPPerm1: _PPerm1,
        _KoniecznyPPerm2: _PPerm2,
        _KoniecznyPPerm4: _PPerm4,
        _KoniecznyTransf1: _Transf1,
        _KoniecznyTransf2: _Transf2,
        _KoniecznyTransf4: _Transf4,
    }

    ########################################################################
    # Konieczny nested classes
    ########################################################################

    class DClass(CxxWrapper):
        def __contains__(self: Self, x: Element) -> bool:
            return self.contains(x)

        def __init__(self: Self, element_type: type, cxx_obj) -> None:  # pylint: disable=super-init-not-called
            self._cxx_obj = cxx_obj
            self.Element = element_type

        @_returns_element
        def rep(self: Self) -> Element:
            return self._cxx_obj.rep()

    ########################################################################
    # Konieczny special methods
    ########################################################################

    # TODO(1) Add a keyword argument for element type to the __init__ function,
    # so that we know which Konieczny type to construct based on the element
    # and/or the underlying cxx type.
    def __init__(  # pylint: disable=super-init-not-called
        self: Self, *args
    ) -> None:
        if len(args) == 0:
            raise ValueError("expected at least 1 argument, found 0")
        # Check if we are constructing from an existing cxx Konieczny object
        if type(args[0]) in self._cxx_type_to_element_dict and len(args) == 1:
            self.Element = self._cxx_type_to_element_dict[type(args[0])]
            self._cxx_obj = copy(args[0])
            return

        if isinstance(args[0], list) and len(args) == 1:
            gens = args[0]
        else:
            gens = args
        cxx_obj_t = self._cxx_obj_type_from(
            samples=(to_cxx(gens[0]),),
        )
        self.Element = type(gens[0])
        self._cxx_obj = cxx_obj_t([to_cxx(x) for x in gens])

    def __contains__(self: Self, x: Element) -> bool:
        return self.contains(x)

    ########################################################################
    # Methods returning elements
    ########################################################################

    @_returns_element
    def generator(self: Self, i: int) -> Element:
        return self._cxx_obj.generator(i)

    def generators(self: Self) -> Iterator:
        return map(
            lambda x: to_py(self.Element, x),
            self._cxx_obj.generators(),
        )

    ########################################################################
    # Methods returning D-classes
    ########################################################################

    @_returns_D_class
    def D_class_of_element(self: Self, x: Element) -> DClass:
        return self._cxx_obj.D_class_of_element(x._cxx_obj)

    def current_D_classes(self: Self) -> Iterator:
        return map(
            lambda x: self.DClass(self.Element, x),
            self._cxx_obj.current_D_classes(),
        )

    def D_classes(self: Self) -> Iterator:
        return map(
            lambda x: self.DClass(self.Element, x),
            self._cxx_obj.D_classes(),
        )
