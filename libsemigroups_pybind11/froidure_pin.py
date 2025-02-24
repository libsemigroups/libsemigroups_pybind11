# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, missing-function-docstring
# pylint: disable=unused-import, missing-module-docstring

# The module doc string is what appears at the top of the helper function
# doc page, and so is omitted.

from copy import copy
from functools import wraps
from typing import List, TypeVar as _TypeVar, Iterator, Union
from typing_extensions import Self

from _libsemigroups_pybind11 import (
    Transf1 as _Transf1,
    Transf2 as _Transf2,
    Transf4 as _Transf4,
    PPerm1 as _PPerm1,
    PPerm2 as _PPerm2,
    PPerm4 as _PPerm4,
    Perm1 as _Perm1,
    Perm2 as _Perm2,
    Perm4 as _Perm4,
    BMat as _BMat,
    IntMat as _IntMat,
    MaxPlusMat as _MaxPlusMat,
    MinPlusMat as _MinPlusMat,
    ProjMaxPlusMat as _ProjMaxPlusMat,
    MaxPlusTruncMat as _MaxPlusTruncMat,
    MinPlusTruncMat as _MinPlusTruncMat,
    NTPMat as _NTPMat,
    Bipartition as _Bipartition,
    PBR as _PBR,
    BMat8 as _BMat8,
    FroidurePinTransf1 as _FroidurePinTransf1,
    FroidurePinTransf2 as _FroidurePinTransf2,
    FroidurePinTransf4 as _FroidurePinTransf4,
    FroidurePinPPerm1 as _FroidurePinPPerm1,
    FroidurePinPPerm2 as _FroidurePinPPerm2,
    FroidurePinPPerm4 as _FroidurePinPPerm4,
    FroidurePinPerm1 as _FroidurePinPerm1,
    FroidurePinPerm2 as _FroidurePinPerm2,
    FroidurePinPerm4 as _FroidurePinPerm4,
    FroidurePinBipartition as _FroidurePinBipartition,
    FroidurePinPBR as _FroidurePinPBR,
    FroidurePinBMat8 as _FroidurePinBMat8,
    FroidurePinBMat as _FroidurePinBMat,
    FroidurePinIntMat as _FroidurePinIntMat,
    FroidurePinMaxPlusMat as _FroidurePinMaxPlusMat,
    FroidurePinMinPlusMat as _FroidurePinMinPlusMat,
    FroidurePinProjMaxPlusMat as _FroidurePinProjMaxPlusMat,
    FroidurePinMaxPlusTruncMat as _FroidurePinMaxPlusTruncMat,
    FroidurePinMinPlusTruncMat as _FroidurePinMinPlusTruncMat,
    FroidurePinNTPMat as _FroidurePinNTPMat,
    froidure_pin_current_position as _froidure_pin_current_position,
    froidure_pin_equal_to as _froidure_pin_equal_to,
    froidure_pin_factorisation as _froidure_pin_factorisation,
    froidure_pin_minimal_factorisation as _froidure_pin_minimal_factorisation,
    froidure_pin_position as _froidure_pin_position,
    froidure_pin_product_by_reduction as _froidure_pin_product_by_reduction,
    froidure_pin_to_element as _froidure_pin_to_element,
    froidure_pin_current_minimal_factorisation as _froidure_pin_current_minimal_factorisation,
    froidure_pin_rules as _froidure_pin_rules,
    froidure_pin_current_rules as _froidure_pin_current_rules,
    froidure_pin_normal_forms as _froidure_pin_normal_forms,
    froidure_pin_current_normal_forms as _froidure_pin_current_normal_forms,
    Undefined,
    FroidurePinBase,
)

from .detail.cxx_wrapper import (
    to_cxx,
    to_py,
    CxxWrapper,
)
from .detail.decorators import may_return_undefined, copydoc

Element = _TypeVar("Element")

########################################################################
# Decorators
########################################################################


def _returns_element(method):
    @wraps(method)
    def wrapper(self, *args):
        return to_py(self.Element, method(self, *args))

    return wrapper


class FroidurePin(CxxWrapper):  # pylint: disable=missing-class-docstring
    _py_to_cxx_type_dict = {
        (_Transf1,): _FroidurePinTransf1,
        (_Transf2,): _FroidurePinTransf2,
        (_Transf4,): _FroidurePinTransf4,
        (_PPerm1,): _FroidurePinPPerm1,
        (_PPerm2,): _FroidurePinPPerm2,
        (_PPerm4,): _FroidurePinPPerm4,
        (_Perm1,): _FroidurePinPerm1,
        (_Perm2,): _FroidurePinPerm2,
        (_Perm4,): _FroidurePinPerm4,
        (_Bipartition,): _FroidurePinBipartition,
        (_PBR,): _FroidurePinPBR,
        (_BMat8,): _FroidurePinBMat8,
        (_BMat,): _FroidurePinBMat,
        (_IntMat,): _FroidurePinIntMat,
        (_MaxPlusMat,): _FroidurePinMaxPlusMat,
        (_MinPlusMat,): _FroidurePinMinPlusMat,
        (_ProjMaxPlusMat,): _FroidurePinProjMaxPlusMat,
        (_MaxPlusTruncMat,): _FroidurePinMaxPlusTruncMat,
        (_MinPlusTruncMat,): _FroidurePinMinPlusTruncMat,
        (_NTPMat,): _FroidurePinNTPMat,
    }
    _cxx_type_to_element_dict = {
        _FroidurePinTransf1: _Transf1,
        _FroidurePinTransf2: _Transf2,
        _FroidurePinTransf4: _Transf4,
        _FroidurePinPPerm1: _PPerm1,
        _FroidurePinPPerm2: _PPerm2,
        _FroidurePinPPerm4: _PPerm4,
        _FroidurePinPerm1: _Perm1,
        _FroidurePinPerm2: _Perm2,
        _FroidurePinPerm4: _Perm4,
        _FroidurePinBipartition: _Bipartition,
        _FroidurePinPBR: _PBR,
        _FroidurePinBMat8: _BMat8,
        _FroidurePinBMat: _BMat,
        _FroidurePinIntMat: _IntMat,
        _FroidurePinMaxPlusMat: _MaxPlusMat,
        _FroidurePinMinPlusMat: _MinPlusMat,
        _FroidurePinProjMaxPlusMat: _ProjMaxPlusMat,
        _FroidurePinMaxPlusTruncMat: _MaxPlusTruncMat,
        _FroidurePinMinPlusTruncMat: _MinPlusTruncMat,
        _FroidurePinNTPMat: _NTPMat,
    }

    ########################################################################
    # C++ FroidurePin special methods
    ########################################################################

    # TODO(1): This __init__ is identical to the SchreierSims __init__. It would
    # probably be best to make an abstract base class from which all classes
    # that construct using a list of generators inherit.

    # TODO(0) Add a keyword argument for element type to the __init__ function,
    # so that we know which FroidurePin type to construct based on the element
    # and/or the underlying cxx type.
    def __init__(  # pylint: disable=super-init-not-called, duplicate-code
        self: Self, *args
    ) -> None:
        if len(args) == 0:
            raise ValueError("expected at least 1 argument, found 0")
        if isinstance(args[0], FroidurePinBase) and len(args) == 1:
            that = args[0]
            # self.Element = self._cxx_type_to_element_dict[type(that)]
            self._cxx_obj = copy(that)
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

    @_returns_element
    def __getitem__(self: Self, i: int) -> Element:
        return self._cxx_obj[i]

    def __iter__(self: Self) -> Iterator:
        return map(
            lambda x: to_py(self.Element, x),
            iter(self._cxx_obj),
        )

    ########################################################################
    # Iterators
    ########################################################################

    def current_elements(self: Self) -> Iterator:
        return map(
            lambda x: to_py(self.Element, x), self._cxx_obj.current_elements()
        )

    def idempotents(self: Self) -> Iterator:
        return map(
            lambda x: to_py(self.Element, x), self._cxx_obj.idempotents()
        )

    def sorted_elements(self: Self) -> Iterator:
        return map(
            lambda x: to_py(self.Element, x),
            self._cxx_obj.sorted_elements(),
        )

    ########################################################################
    # Methods returning an element
    ########################################################################

    @_returns_element
    def generator(self: Self, i: int) -> Element:
        return self._cxx_obj.generator(i)

    @_returns_element
    def sorted_at(self: Self, i: int) -> Element:
        return self._cxx_obj.sorted_at(i)

    @may_return_undefined
    def current_position(self: Self, x: Element) -> Element:
        return self._cxx_obj.current_position(to_cxx(x))


########################################################################
# Helpers -- from froidure-pin.cpp
########################################################################

# TODO: be good to get the notes about enumeration being triggered or not, in
# this doc


@may_return_undefined
def current_position(fp: FroidurePin, w: List[int]) -> Union[int, Undefined]:
    """
    Returns the position corresponding to a word.

    This function returns the position in *fp* corresponding to the the word
    *w* (in the generators). No enumeration is performed, and :any:`UNDEFINED`
    is returned if the position of the element corresponding to *w* cannot be
    determined.

    :param fp: the :any:`FroidurePinPBR` instance.
    :type fp: FroidurePinPBR

    :param w: a word in the generators.
    :type w: List[int]

    :returns: The current position of the element represented by a word.
    :rtype: int | Undefined

    :raises LibsemigroupsError:
        if *w* contains a value that is not strictly less than
        :any:`FroidurePinPBR.number_of_generators`.

    :complexity: :math:`O(n)` where :math:`n` is the length of the word *w*.
    """
    return _froidure_pin_current_position(to_cxx(fp), to_cxx(w))


def equal_to(fp: FroidurePin, x: List[int], y: List[int]) -> bool:
    """
    Check equality of words in the generators.

    This function returns ``True`` if the parameters *x* and *y* represent the same
    element of *fp* and ``False`` otherwise.

    :param fp: the :any:`FroidurePinPBR` instance.
    :type fp: FroidurePinPBR

    :param x: the first word for comparison.
    :type x: List[int]

    :param y: the second word for comparison.
    :type y: List[int]

    :returns: Whether or not the words *x* and *y* represent the same element.
    :rtype: bool

    :raises LibsemigroupsError:
        if *x* or *y* contains any value that is not strictly less than
        :any:`FroidurePinPBR.number_of_generators`.

    .. note::
        No enumeration of *fp* is triggered by calls to this function.
    """
    return _froidure_pin_equal_to(to_cxx(fp), x, y)


def factorisation(fp: FroidurePin, x: Union[Element, int]) -> List[int]:
    """
    Returns a word containing a factorisation (in the generators) of an
    element.

    This function returns a word in the generators that equals the given element
    *x*. The key difference between this function and :any:`minimal_factorisation`,
    is that the resulting factorisation may not be minimal.

    :param fp: the :any:`FroidurePinPBR` instance.
    :type fp: FroidurePinPBR

    :param x: a possible element, or index of element, to factorise.
    :type x: Element | int

    :returns: Returns a word in the generators which evaluates to *x*.
    :rtype: List[int]

    :raises LibsemigroupsError:
      if *x* is an ``Element`` and *x* does not belong to *fp*.

    :raises LibsemigroupsError:
      if *x* is an :any:`int` and *x* is greater than or equal to
      :any:`FroidurePinBase.size`.
    """
    return _froidure_pin_factorisation(to_cxx(fp), to_cxx(x))


def minimal_factorisation(fp: FroidurePin, x: Union[Element, int]) -> List[int]:
    """
    Returns a word containing a minimal factorisation (in the generators)
    of an element.

    This function returns the short-lex minimum word (if any) in the generators
    that evaluates to *x*.

    :param fp: the :any:`FroidurePinPBR` instance.
    :type fp: FroidurePinPBR

    :param x: a possible element, or index of element, to factorise.
    :type x: Element | int

    :returns: A word in the generators that evaluates to *x*.
    :rtype: List[int]

    :raises LibsemigroupsError:
      if *x* is an ``Element`` and *x* does not belong to *fp*.

    :raises LibsemigroupsError:
      if *x* is an :any:`int` and *x* is greater than or equal to
      :any:`FroidurePinBase.size`.
    """
    return _froidure_pin_minimal_factorisation(to_cxx(fp), to_cxx(x))


@may_return_undefined
def position(fp: FroidurePin, x: List[int]) -> Union[int, Undefined]:
    """
    Returns the position corresponding to a word.

    This function returns the position in *fp* corresponding to the word *w*. A
    full enumeration is triggered by calls to this function.

    :param fp: the :any:`FroidurePinPBR` instance.
    :type fp: FroidurePinPBR

    :param w: a word in the generators.
    :type w: List[int]

    :returns: The position of the element represented by a word.
    :rtype: int | Undefined

    :raises LibsemigroupsError:
        if *w* contains any values that are not strictly less than
        :any:`FroidurePinPBR.number_of_generators`.

    :complexity: :math:`O(n)` where :math:`n` is the length of the word *w*.
    """
    return _froidure_pin_position(to_cxx(fp), to_cxx(x))


def to_element(fp: FroidurePin, w: List[int]) -> Element:
    """
    Convert a word in the generators to an element.

    This function returns the element of *fp* obtained by evaluating *w*.

    :param fp: the :any:`FroidurePinPBR` instance.
    :type fp: FroidurePinPBR


    :param w: the word in the generators to evaluate.
    :type w: List[int]

    :returns: The element of *fp* corresponding to the word *w*.
    :rtype: Element

    :raises LibsemigroupsError:
      if *w* is not a valid word in the generators, i.e. if it contains a value
      greater than or equal to the number of generators.

    .. seealso::  :any:`current_position`.

    .. note::
        No enumeration of *fp* is triggered by calls to this function.
    """
    return to_py(fp.Element, _froidure_pin_to_element(to_cxx(fp), w))


########################################################################
# Helpers -- from froidure-pin-base.cpp
########################################################################


@copydoc(_froidure_pin_current_minimal_factorisation)
def current_minimal_factorisation(fpb: FroidurePinBase, x: int) -> List[int]:
    return _froidure_pin_current_minimal_factorisation(to_cxx(fpb), x)


@copydoc(_froidure_pin_current_rules)
def current_rules(fpb: FroidurePinBase) -> Iterator:
    return _froidure_pin_current_rules(to_cxx(fpb))


@copydoc(_froidure_pin_rules)
def rules(fpb: FroidurePinBase) -> Iterator:
    return _froidure_pin_rules(to_cxx(fpb))


@copydoc(_froidure_pin_current_normal_forms)
def current_normal_forms(fpb: FroidurePinBase) -> Iterator:
    return _froidure_pin_current_normal_forms(to_cxx(fpb))


@copydoc(_froidure_pin_normal_forms)
def normal_forms(fpb: FroidurePinBase) -> Iterator:
    return _froidure_pin_normal_forms(to_cxx(fpb))


@copydoc(_froidure_pin_product_by_reduction)
def product_by_reduction(fpb: FroidurePinBase, i: int, j: int) -> int:
    return _froidure_pin_product_by_reduction(to_cxx(fpb), i, j)
