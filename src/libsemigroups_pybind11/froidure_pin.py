# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# The module doc string is what appears at the top of the helper function
# doc page, and so is omitted.

"""
This page contains the documentation for various helper functions for
manipulating :any:`FroidurePin` objects. All such functions
are contained in the submodule ``libsemigroups_pybind11.froidure_pin``.
"""

from collections.abc import Iterator as _Iterator
from typing import TypeVar as _TypeVar
from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    BMat as _BMat,
    BMat8 as _BMat8,
    Bipartition as _Bipartition,
    FroidurePinBMat as _FroidurePinBMat,
    FroidurePinBMat8 as _FroidurePinBMat8,
    FroidurePinBipartition as _FroidurePinBipartition,
    FroidurePinIntMat as _FroidurePinIntMat,
    FroidurePinMaxPlusMat as _FroidurePinMaxPlusMat,
    FroidurePinMaxPlusTruncMat as _FroidurePinMaxPlusTruncMat,
    FroidurePinMinPlusMat as _FroidurePinMinPlusMat,
    FroidurePinMinPlusTruncMat as _FroidurePinMinPlusTruncMat,
    FroidurePinNTPMat as _FroidurePinNTPMat,
    FroidurePinPBR as _FroidurePinPBR,
    FroidurePinPPerm1 as _FroidurePinPPerm1,
    FroidurePinPPerm2 as _FroidurePinPPerm2,
    FroidurePinPPerm4 as _FroidurePinPPerm4,
    FroidurePinPerm1 as _FroidurePinPerm1,
    FroidurePinPerm2 as _FroidurePinPerm2,
    FroidurePinPerm4 as _FroidurePinPerm4,
    FroidurePinProjMaxPlusMat as _FroidurePinProjMaxPlusMat,
    FroidurePinTransf1 as _FroidurePinTransf1,
    FroidurePinTransf2 as _FroidurePinTransf2,
    FroidurePinTransf4 as _FroidurePinTransf4,
    FroidurePinKBEStringRewriteFromLeft as _FroidurePinKBEStringRewriteFromLeft,
    FroidurePinKBEStringRewriteTrie as _FroidurePinKBEStringRewriteTrie,
    FroidurePinKBEWordRewriteFromLeft as _FroidurePinKBEWordRewriteFromLeft,
    FroidurePinKBEWordRewriteTrie as _FroidurePinKBEWordRewriteTrie,
    FroidurePinKEMultiViewString as _FroidurePinKEMultiViewString,
    FroidurePinKEString as _FroidurePinKEString,
    FroidurePinKEWord as _FroidurePinKEWord,
    FroidurePinTCE as _FroidurePinTCE,
    IntMat as _IntMat,
    KBEStringTrie as _KBEStringTrie,
    KBEStringFromLeft as _KBEStringFromLeft,
    KBEWordTrie as _KBEWordTrie,
    KBEWordFromLeft as _KBEWordFromLeft,
    MaxPlusMat as _MaxPlusMat,
    MaxPlusTruncMat as _MaxPlusTruncMat,
    MinPlusMat as _MinPlusMat,
    MinPlusTruncMat as _MinPlusTruncMat,
    NTPMat as _NTPMat,
    PBR as _PBR,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    Perm4 as _Perm4,
    ProjMaxPlusMat as _ProjMaxPlusMat,
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
    froidure_pin_current_minimal_factorisation as _froidure_pin_current_minimal_factorisation,
    froidure_pin_current_normal_forms as _froidure_pin_current_normal_forms,
    froidure_pin_current_position as _froidure_pin_current_position,
    froidure_pin_current_rules as _froidure_pin_current_rules,
    froidure_pin_equal_to as _froidure_pin_equal_to,
    froidure_pin_factorisation as _froidure_pin_factorisation,
    froidure_pin_minimal_factorisation as _froidure_pin_minimal_factorisation,
    froidure_pin_normal_forms as _froidure_pin_normal_forms,
    froidure_pin_position as _froidure_pin_position,
    froidure_pin_product_by_reduction as _froidure_pin_product_by_reduction,
    froidure_pin_rules as _froidure_pin_rules,
    froidure_pin_to_element as _froidure_pin_to_element,
)

from .detail.cxx_wrapper import (
    CxxWrapper as _CxxWrapper,
    copy_cxx_mem_fns as _copy_cxx_mem_fns,
    register_cxx_wrapped_type as _register_cxx_wrapped_type,
    to_cxx as _to_cxx,
    to_py as _to_py,
    wrap_cxx_free_fn as _wrap_cxx_free_fn,
)

from .detail.decorators import copydoc as _copydoc


########################################################################
# The FroidurePin python class
########################################################################


class FroidurePin(_CxxWrapper):  # pylint: disable=missing-class-docstring
    Element = _TypeVar("Element")

    __doc__ = _FroidurePinPBR.__doc__

    _py_template_params_to_cxx_type = {
        (_BMat,): _FroidurePinBMat,
        (_BMat8,): _FroidurePinBMat8,
        (_Bipartition,): _FroidurePinBipartition,
        (_IntMat,): _FroidurePinIntMat,
        (_MaxPlusMat,): _FroidurePinMaxPlusMat,
        (_MaxPlusTruncMat,): _FroidurePinMaxPlusTruncMat,
        (_MinPlusMat,): _FroidurePinMinPlusMat,
        (_MinPlusTruncMat,): _FroidurePinMinPlusTruncMat,
        (_NTPMat,): _FroidurePinNTPMat,
        (_PBR,): _FroidurePinPBR,
        (_PPerm1,): _FroidurePinPPerm1,
        (_PPerm2,): _FroidurePinPPerm2,
        (_PPerm4,): _FroidurePinPPerm4,
        (_Perm1,): _FroidurePinPerm1,
        (_Perm2,): _FroidurePinPerm2,
        (_Perm4,): _FroidurePinPerm4,
        (_ProjMaxPlusMat,): _FroidurePinProjMaxPlusMat,
        (_Transf1,): _FroidurePinTransf1,
        (_Transf2,): _FroidurePinTransf2,
        (_Transf4,): _FroidurePinTransf4,
        (_KBEStringTrie,): _FroidurePinKBEStringRewriteTrie,
        (_KBEStringFromLeft,): _FroidurePinKBEStringRewriteFromLeft,
        (_KBEWordTrie,): _FroidurePinKBEWordRewriteTrie,
        (_KBEWordFromLeft,): _FroidurePinKBEWordRewriteFromLeft,
    }

    _cxx_type_to_py_template_params = dict(
        zip(
            _py_template_params_to_cxx_type.values(),
            _py_template_params_to_cxx_type.keys(),
        )
    )

    _all_wrapped_cxx_types = {*_py_template_params_to_cxx_type.values()} | {
        _FroidurePinKEMultiViewString,
        _FroidurePinKEString,
        _FroidurePinKEWord,
        _FroidurePinTCE,
    }

    ########################################################################
    # Protected methods
    ########################################################################

    def _raise_if_element_not_implemented(self: _Self):
        if type(_to_cxx(self)) not in self._cxx_type_to_py_template_params:
            raise NotImplementedError(
                "It is not yet possible to call functions that return an element "
                "on FroidurePin objects with this type of element."
            )

    ########################################################################
    # Special methods
    ########################################################################

    # TODO(1) Add a keyword argument for element type to the __init__ function,
    # so that we know which FroidurePin type to construct based on the element
    # and/or the underlying cxx type.
    @_copydoc(_FroidurePinPBR.__init__)
    def __init__(self: _Self, *args) -> None:
        super().__init__(*args)
        if _to_cxx(self) is not None:
            return
        if len(args) == 0:
            raise TypeError("expected at least 1 argument, found 0")

        if isinstance(args[0], list) and len(args) == 1:
            gens = args[0]
        else:
            gens = list(args)
        gens = [_to_cxx(x) for x in gens]
        self.py_template_params = (type(gens[0]),)
        self.init_cxx_obj(gens)

    def __getitem__(self: _Self, i: int) -> Element:
        self._raise_if_element_not_implemented()
        return _to_py(_to_cxx(self)[i])

    def __iter__(self: _Self) -> _Iterator[Element]:
        self._raise_if_element_not_implemented()
        return map(_to_py, iter(_to_cxx(self)))

    # TODO(1) __contains__?

    ########################################################################
    # Iterators
    ########################################################################

    @_copydoc(_FroidurePinPBR.current_elements)
    def current_elements(  # pylint: disable=missing-function-docstring
        self: _Self,
    ) -> _Iterator[Element]:
        self._raise_if_element_not_implemented()
        return map(
            _to_py,
            _to_cxx(self).current_elements(),
        )

    @_copydoc(_FroidurePinPBR.idempotents)
    def idempotents(  # pylint: disable=missing-function-docstring
        self: _Self,
    ) -> _Iterator[Element]:
        self._raise_if_element_not_implemented()
        return map(
            _to_py,
            _to_cxx(self).idempotents(),
        )

    @_copydoc(_FroidurePinPBR.sorted_elements)
    def sorted_elements(  # pylint: disable=missing-function-docstring
        self: _Self,
    ) -> _Iterator[Element]:
        self._raise_if_element_not_implemented()
        return map(
            _to_py,
            _to_cxx(self).sorted_elements(),
        )


_copy_cxx_mem_fns(_FroidurePinBMat, FroidurePin)

for (
    _fp_type
) in (
    FroidurePin._py_template_params_to_cxx_type.values()  # pylint:disable=protected-access
):
    _register_cxx_wrapped_type(_fp_type, FroidurePin)


_register_cxx_wrapped_type(_FroidurePinKEMultiViewString, FroidurePin)
_register_cxx_wrapped_type(_FroidurePinKEString, FroidurePin)
_register_cxx_wrapped_type(_FroidurePinKEWord, FroidurePin)
_register_cxx_wrapped_type(_FroidurePinTCE, FroidurePin)

########################################################################
# Helpers -- from froidure-pin.cpp
########################################################################

# TODO(1) be good to get the notes about enumeration being triggered or not, in
# this doc

current_minimal_factorisation = _wrap_cxx_free_fn(
    _froidure_pin_current_minimal_factorisation
)
current_normal_forms = _wrap_cxx_free_fn(_froidure_pin_current_normal_forms)
current_position = _wrap_cxx_free_fn(_froidure_pin_current_position)
current_rules = _wrap_cxx_free_fn(_froidure_pin_current_rules)
equal_to = _wrap_cxx_free_fn(_froidure_pin_equal_to)
factorisation = _wrap_cxx_free_fn(_froidure_pin_factorisation)
minimal_factorisation = _wrap_cxx_free_fn(_froidure_pin_minimal_factorisation)
normal_forms = _wrap_cxx_free_fn(_froidure_pin_normal_forms)
position = _wrap_cxx_free_fn(_froidure_pin_position)
product_by_reduction = _wrap_cxx_free_fn(_froidure_pin_product_by_reduction)
rules = _wrap_cxx_free_fn(_froidure_pin_rules)
to_element = _wrap_cxx_free_fn(_froidure_pin_to_element)
