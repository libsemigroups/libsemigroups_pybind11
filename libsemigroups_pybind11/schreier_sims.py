# -*- coding: utf-8 -*-

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme
# pylint: disable=missing-function-docstring

"""
This package provides the user-facing python part of ``libsemigroups_pybind11`` for
the ``schreier_sims`` namespace from ``libsemigroups``.
"""

from functools import wraps
from typing import TypeVar as _TypeVar, List
from typing_extensions import Self

from _libsemigroups_pybind11 import (
    # TODO rename intersection -> schreier_sims_intersection
    intersection as _schreier_sims_intersection,
    SchreierSimsPerm1 as _SchreierSimsPerm1,
    SchreierSimsPerm2 as _SchreierSimsPerm2,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    # Perm4 as _Perm4,
)

from .detail.cxx_wrapper import (
    to_cxx as _to_cxx,
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)

from .detail.decorators import copydoc as _copydoc

Element = _TypeVar("Element")

########################################################################
# ScheierSims python class
########################################################################


class SchreierSims(_CxxWrapper):  # pylint: disable=missing-class-docstring
    __doc__ = _SchreierSimsPerm1.__doc__

    _py_template_params_to_cxx_type = {
        (_Perm1,): _SchreierSimsPerm1,
        (_Perm2,): _SchreierSimsPerm2,
        # (_Perm4,): _SchreierSims,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()}

    ########################################################################
    # C++ Constructor
    ########################################################################

    # TODO(1): This __init__ is identical to the FroidurePin __init__. It would
    # probably be best to make an abstract base class from which all classes
    # that construct using a list of generators inherit.
    @_copydoc(_SchreierSimsPerm1.__init__)
    def __init__(self: Self, gens: List[Element]) -> None:
        super().__init__(gens)

        if self._cxx_obj is not None:
            return

        if not isinstance(gens, list):
            raise TypeError(
                f"expected the 1st argument to be a list, found {type(gens)}"
            )
        elif len(gens) == 0:
            raise TypeError(
                f"expected the 1st argument to be a list of length > 0, found {len(gens)}"
            )

        self.Element = type(gens[0])
        # Default construct the correct type of cxx SchreierSims object
        self._cxx_obj = self._py_template_params_to_cxx_type[
            (type(_to_cxx(gens[0])),)
        ]()
        for gen in gens:
            self.add_generator(gen)


_copy_cxx_mem_fns(_SchreierSimsPerm1, SchreierSims)

_register_cxx_wrapped_type(_SchreierSimsPerm1, SchreierSims)
_register_cxx_wrapped_type(_SchreierSimsPerm2, SchreierSims)

########################################################################
# Helpers -- from schreier-sims.cpp
########################################################################


intersection = _wrap_cxx_free_fn(_schreier_sims_intersection)
