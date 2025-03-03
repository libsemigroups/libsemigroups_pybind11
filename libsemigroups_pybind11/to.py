# -*- coding: utf-8 -*-

# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-module-docstring, line-too-long

from typing import List, _GenericAlias
from _libsemigroups_pybind11 import (
    to_congruence_string,
    to_congruence_word,
    to_froidure_pin,
    to_inverse_presentation_string,
    to_inverse_presentation_word,
    to_inverse_presentation,
    to_knuth_bendix_RewriteFromLeft,
    to_knuth_bendix_RewriteTrie,
    to_knuth_bendix_string_RewriteFromLeft,
    to_knuth_bendix_string_RewriteTrie,
    to_knuth_bendix_word_RewriteFromLeft,
    to_knuth_bendix_word_RewriteTrie,
    to_knuth_bendix,
    to_presentation_string,
    to_presentation_word,
    to_presentation,
    to_todd_coxeter_string,
    to_todd_coxeter_word,
    to_todd_coxeter,
)

from .congruence import Congruence
from .froidure_pin import FroidurePin
from .knuth_bendix import KnuthBendix
from .presentation import Presentation, InversePresentation
from .todd_coxeter import ToddCoxeter
from .detail.cxx_wrapper import to_cxx


def _nice_name(type_list):
    """Convert an itterable of type-like things into a string"""
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


def to(*args, Return):
    """Convert from one type of `libsemigroups` object to another

    This function converts the the arguments specified in *args* to object of
    type *Return*.

    :param args: the objects to convert.
    :param Return: the type of object to convert to.

    :returns: an object of type *Return*.

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

        >>> fp = to(kb, Return=FroidurePin)
        >>> fp
        <partially enumerated FroidurePin with 2 generators, 2 elements, Cayley graph ⌀ 1, & 0 rules>

        >>> fp.run()
        >>> fp
        <fully enumerated FroidurePin with 2 generators, 3 elements, Cayley graph ⌀ 2, & 3 rules>

    .. seealso::

        See the following pages for a detailed description of the various use
        cases of this function:

            * :doc:`/main-algorithms/congruence/to-cong`;
            * :doc:`/main-algorithms/froidure-pin/to-froidure-pin`;
            * :doc:`/data-structures/presentations/to-inverse-present`;
            * :doc:`/main-algorithms/knuth-bendix/to-knuth-bendix`;
            * :doc:`/data-structures/presentations/to-present`; and
            * :doc:`/main-algorithms/todd-coxeter/to-todd-coxeter`.

    """
    cxx_args = [to_cxx(arg) for arg in args]
    return_type_to_converter_function = {
        (Congruence, str): to_congruence_string,
        (Congruence, List[int]): to_congruence_word,
        FroidurePin: lambda *x: FroidurePin(to_froidure_pin(*x)),
        InversePresentation: to_inverse_presentation,
        (InversePresentation, List[int]): to_inverse_presentation_word,
        (InversePresentation, str): to_inverse_presentation_string,
        KnuthBendix: to_knuth_bendix,
        (KnuthBendix, "RewriteTrie"): to_knuth_bendix_RewriteTrie,
        (KnuthBendix, "RewriteFromLeft"): to_knuth_bendix_RewriteFromLeft,
        (
            KnuthBendix,
            List[int],
            "RewriteFromLeft",
        ): to_knuth_bendix_word_RewriteFromLeft,
        (
            KnuthBendix,
            List[int],
            "RewriteTrie",
        ): to_knuth_bendix_word_RewriteTrie,
        (
            KnuthBendix,
            str,
            "RewriteFromLeft",
        ): to_knuth_bendix_string_RewriteFromLeft,
        (KnuthBendix, str, "RewriteTrie"): to_knuth_bendix_string_RewriteTrie,
        Presentation: to_presentation,
        (Presentation, str): to_presentation_string,
        (Presentation, List[int]): to_presentation_word,
        ToddCoxeter: to_todd_coxeter,
        (ToddCoxeter, str): to_todd_coxeter_string,
        (ToddCoxeter, List[int]): to_todd_coxeter_word,
    }

    if Return not in return_type_to_converter_function:
        valid_types = (_nice_name(x) for x in return_type_to_converter_function)
        valid_types_string = "\n    * " + "\n    * ".join(valid_types) + "\n"
        raise TypeError(
            "expected the first keyword argument to be one of:"
            f"{valid_types_string}"
            f"but found: {_nice_name(Return)}"
        )

    return return_type_to_converter_function[Return](*cxx_args)
