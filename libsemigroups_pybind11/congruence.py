# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the libsemigroups::congruence namespace from libsemigroups.
"""

from typing import List, Iterator, Union

from .kambites import Kambites as _Kambites
from .knuth_bendix import KnuthBendix as _KnuthBendix
from .todd_coxeter import ToddCoxeter as _ToddCoxeter

from _libsemigroups_pybind11 import (
    CongruenceString as _CongruenceString,
    CongruenceWord as _CongruenceWord,
    PresentationStrings as _PresentationStrings,
    PresentationWords as _PresentationWords,
    congruence_non_trivial_classes as non_trivial_classes,
    congruence_partition as partition,
)


from .detail.decorators import (
    may_return_positive_infinity as _may_return_positive_infinity,
)

from .detail import cong_intf


def _get(self, t):
    """
    Returns the *t* instance used to compute the congruence (if any).

    This function returns (a copy of) the object of type *t* used to compute
    the congruence (if any). If there is no such object, then an exception is
    raised.

    :param t:
        The type of object being sought (must be :any:`KambitesMultiStringView`,
        :any:`KnuthBendixStringRewriteTrie`, or :any:`ToddCoxeterWord`).
    :type t: type

    :returns: A copy of the instance of type *t*.

    :raises LibsemigroupsError:
        if there is no object of type *t* within *self*.
    """
    if t is _KnuthBendix:
        return self._get_knuth_bendix()
    if t is _ToddCoxeter:
        return self._get_todd_coxeter()
    if t is _Kambites:
        return self._get_kambites()


def _has(self, t) -> bool:
    """
    Returns whether or not there is an instance of type *t* within the
    congruence.

    This function returns ``True`` if *self* contains an instance of type *t*,
    and ``False`` if not.

    :param t:
        The type of object being sought (must be :any:`KambitesMultiStringView`,
        :any:`KnuthBendixStringRewriteTrie`, or :any:`ToddCoxeterWord`).
    :type t: type

    :returns: Whether or not there is an object of type *t* in *self*.
    """
    if t is _KnuthBendix:
        return self._has_knuth_bendix()
    if t is _ToddCoxeter:
        return self._has_todd_coxeter()
    if t is _Kambites:
        return self._has_kambites()


for _Congruence in (_CongruenceWord, _CongruenceString):
    _Congruence.get = _get
    _Congruence.has = _has

    _Congruence.number_of_classes = _may_return_positive_infinity(
        _Congruence._number_of_classes
    )

    # TODO(0) move to conf.py
    _Congruence.number_of_classes.__doc__ = "\n".join(
        _Congruence._number_of_classes.__doc__.split("\n")[1:]
    )


def Congruence(*args, **kwargs):
    cong_intf.raise_if_bad_args(*args, **kwargs)

    if len(args) == 0:
        Word = kwargs["Word"]
    else:
        assert len(args) == 2
        if isinstance(args[1], _PresentationStrings):
            Word = str
        elif isinstance(args[1], _PresentationWords):
            Word = List[int]
        else:
            raise TypeError(
                f"expected the 2nd positional argument to be Presentation but found {type(args[1])}"
            )

    cpp_type = {
        List[int]: _CongruenceWord,
        str: _CongruenceString,
    }
    return cpp_type[Word](*args)
