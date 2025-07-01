# -*- coding: utf-8 -*-

# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
Subpackage containing the :any:`to` function for converting
``libsemigroups_pybind11`` objects from one type to another.
"""

from typing import _GenericAlias

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    to_congruence_string as _to_congruence_string,
    to_congruence_word as _to_congruence_word,
    to_froidure_pin as _to_froidure_pin,
    to_inverse_presentation_string as _to_inverse_presentation_string,
    to_inverse_presentation_word as _to_inverse_presentation_word,
    to_inverse_presentation as _to_inverse_presentation,
    to_knuth_bendix_RewriteFromLeft as _to_knuth_bendix_RewriteFromLeft,
    to_knuth_bendix_RewriteTrie as _to_knuth_bendix_RewriteTrie,
    to_knuth_bendix_string_RewriteFromLeft as _to_knuth_bendix_string_RewriteFromLeft,
    to_knuth_bendix_string_RewriteTrie as _to_knuth_bendix_string_RewriteTrie,
    to_knuth_bendix_word_RewriteFromLeft as _to_knuth_bendix_word_RewriteFromLeft,
    to_knuth_bendix_word_RewriteTrie as _to_knuth_bendix_word_RewriteTrie,
    to_knuth_bendix as _to_knuth_bendix,
    to_presentation_string as _to_presentation_string,
    to_presentation_word as _to_presentation_word,
    to_presentation as _to_presentation,
    to_todd_coxeter_string as _to_todd_coxeter_string,
    to_todd_coxeter_word as _to_todd_coxeter_word,
    to_todd_coxeter as _to_todd_coxeter,
)

from .congruence import Congruence as _Congruence
from .froidure_pin import FroidurePin as _FroidurePin
from .knuth_bendix import KnuthBendix as _KnuthBendix
from .presentation import (
    Presentation as _Presentation,
    InversePresentation as _InversePresentation,
)
from .todd_coxeter import ToddCoxeter as _ToddCoxeter
from .detail.cxx_wrapper import to_cxx as _to_cxx


def _nice_name(type_list):
    """Convert an iterable of type-like things into a string"""
    single_element = False
    if not isinstance(type_list, tuple):
        single_element = True
        type_list = [type_list]
    out = []
    for t in type_list:
        if isinstance(t, str):
            out.append(t)
        elif isinstance(t, _GenericAlias):
            out.append(str(t))
        elif hasattr(t, "__name__"):
            out.append(t.__name__)
        else:
            out.append(str(t))
    if single_element:
        return out[0]
    return f"({', '.join(out)})"


_RETURN_TYPE_TO_CONVERTER_FUNCTION = {
    (_Congruence, str): _to_congruence_string,
    (_Congruence, list[int]): _to_congruence_word,
    (_FroidurePin,): _to_froidure_pin,
    (_InversePresentation,): _to_inverse_presentation,
    (_InversePresentation, list[int]): _to_inverse_presentation_word,
    (_InversePresentation, str): _to_inverse_presentation_string,
    (_KnuthBendix,): _to_knuth_bendix,
    (_KnuthBendix, "RewriteTrie"): _to_knuth_bendix_RewriteTrie,
    (_KnuthBendix, "RewriteFromLeft"): _to_knuth_bendix_RewriteFromLeft,
    (
        _KnuthBendix,
        list[int],
        "RewriteFromLeft",
    ): _to_knuth_bendix_word_RewriteFromLeft,
    (
        _KnuthBendix,
        list[int],
        "RewriteTrie",
    ): _to_knuth_bendix_word_RewriteTrie,
    (
        _KnuthBendix,
        str,
        "RewriteFromLeft",
    ): _to_knuth_bendix_string_RewriteFromLeft,
    (_KnuthBendix, str, "RewriteTrie"): _to_knuth_bendix_string_RewriteTrie,
    (_Presentation,): _to_presentation,
    (_Presentation, str): _to_presentation_string,
    (_Presentation, list[int]): _to_presentation_word,
    (_ToddCoxeter,): _to_todd_coxeter,
    (_ToddCoxeter, str): _to_todd_coxeter_string,
    (_ToddCoxeter, list[int]): _to_todd_coxeter_word,
}

_VALID_TYPES = (_nice_name(x) for x in _RETURN_TYPE_TO_CONVERTER_FUNCTION)
_VALID_TYPES_STRING = "\n    * " + "\n    * ".join(_VALID_TYPES) + "\n"


def to(*args, rtype):
    """Convert from one type of `libsemigroups` object to another

    This function converts the the arguments specified in *args* to object of
    type *rtype*.

    :param args: the objects to convert.
    :param rtype: the type of object to convert to.

    :returns: an object of type *rtype*.

    .. seealso::

        See the following pages for a detailed description of the various use
        cases of this function:

            * :doc:`/main-algorithms/congruence/to-cong`;
            * :doc:`/main-algorithms/froidure-pin/to-froidure-pin`;
            * :doc:`/data-structures/presentations/to-inverse-present`;
            * :doc:`/main-algorithms/knuth-bendix/to-knuth-bendix`;
            * :doc:`/data-structures/presentations/to-present`; and
            * :doc:`/main-algorithms/todd-coxeter/to-todd-coxeter`.


    .. doctest::

        >>> from libsemigroups_pybind11 import (
        ...     congruence_kind,
        ...     FroidurePin,
        ...     KnuthBendix,
        ...     Presentation,
        ...     presentation,
        ...     to,
        ... )

        >>> p = Presentation([0, 1])
        >>> presentation.add_rule(p, [0, 1], [1, 0])
        >>> presentation.add_rule(p, [0, 0], [0])
        >>> presentation.add_rule(p, [1, 1], [1])
        >>> kb = KnuthBendix(congruence_kind.twosided, p)
        >>> fp = to(kb, rtype=(FroidurePin,))
        >>> fp # doctest: +NORMALIZE_WHITESPACE
        <partially enumerated FroidurePin with 2 generators, 2 elements,
         Cayley graph ⌀ 1, & 0 rules>

        >>> fp.run()
        >>> fp
        <fully enumerated FroidurePin with 2 generators, 3 elements, Cayley graph ⌀ 2, & 3 rules>

    """
    cxx_args = [_to_cxx(arg) for arg in args]
    if rtype not in _RETURN_TYPE_TO_CONVERTER_FUNCTION:
        raise TypeError(
            "expected the first keyword argument to be one of:"
            f"{_VALID_TYPES_STRING}"
            f"but found: {_nice_name(rtype)}"
        )
    constructor = rtype[0]
    return constructor(_RETURN_TYPE_TO_CONVERTER_FUNCTION[rtype](*cxx_args))
