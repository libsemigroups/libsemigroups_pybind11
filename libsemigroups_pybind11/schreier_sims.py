# -*- coding: utf-8 -*-

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the schreier_sims namespace from libsemigroups.
"""

from functools import wraps
from typing import List, TypeVar as _TypeVar
from typing_extensions import Self

from _libsemigroups_pybind11 import (
    intersection,
    SchreierSimsPerm1 as _SchreierSimsPerm1,
    SchreierSimsPerm2 as _SchreierSimsPerm2,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    # Perm4 as _Perm4,
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


class SchreierSims(CxxWrapper):
    _py_to_cxx_type_dict = {
        (_Perm1,): _SchreierSimsPerm1,
        (_Perm2,): _SchreierSimsPerm2,
        # (_Perm4,): _SchreierSims,
    }

    ########################################################################
    # C++ Constructors
    ########################################################################

    def __init__(
        self: Self, *args
    ) -> None:  # pylint: disable=super-init-not-called
        if len(args) == 0:
            raise ValueError("expected at least 1 argument, found 0")
        if isinstance(args[0], list) and len(args) == 1:
            gens = args[0]
        else:
            gens = args
        cxx_obj_t = self._cxx_obj_type_from(
            samples=(to_cxx(gens[0]),),
        )
        self.Element = type(gens[0])

        self._cxx_obj = cxx_obj_t()
        for gen in gens:
            self._cxx_obj.add_generator(to_cxx(gen))

    ########################################################################
    # Methods returning an element
    ########################################################################

    @_returns_element
    def generator(self: Self, index: int) -> Element:
        return self._cxx_obj.generator(index)

    @_returns_element
    def inverse_transversal_element(self: Self, depth: int, pt: int) -> Element:
        return self._cxx_obj.inverse_transversal_element(depth, pt)

    @_returns_element
    def one(self: Self) -> Element:
        return self._cxx_obj.one()

    @_returns_element
    def strong_generator(self: Self, depth: int, index: int) -> Element:
        return self._cxx_obj.strong_generator(depth, index)

    @_returns_element
    def transversal_element(self: Self, depth: int, pt: int) -> Element:
        return self._cxx_obj.transversal_element(depth, pt)
