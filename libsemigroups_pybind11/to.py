# -*- coding: utf-8 -*-

# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-module-docstring

from typing import List
from _libsemigroups_pybind11 import (
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
from .froidure_pin import FroidurePin
from .knuth_bendix import KnuthBendix
from .todd_coxeter import ToddCoxeter
from .presentation import Presentation, InversePresentation
from .detail.cxx_wrapper import to_cxx


def to(*args, Return):
    """Convert from one type of `libsemigroups` object to another

    This function converts the the arguments specified in *args* to object of
    type *Return*.

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

    """
    cxx_args = [to_cxx(arg) for arg in args]
    if Return is FroidurePin:
        return FroidurePin(to_froidure_pin(*cxx_args))
    elif Return is ToddCoxeter:
        return to_todd_coxeter(*cxx_args)
    elif Return == (ToddCoxeter, str):
        return to_todd_coxeter_string(*cxx_args)
    elif Return == (ToddCoxeter, List[int]):
        return to_todd_coxeter_word(*cxx_args)
    elif Return is Presentation:
        return to_presentation(*cxx_args)
    elif Return == (Presentation, str):
        return to_presentation_string(*cxx_args)
    elif Return == (Presentation, List[int]):
        return to_presentation_word(*cxx_args)
    elif Return is InversePresentation:
        return to_inverse_presentation(*cxx_args)
    elif Return == (InversePresentation, str):
        return to_inverse_presentation_string(*cxx_args)
    elif Return == (InversePresentation, List[int]):
        return to_inverse_presentation_word(*cxx_args)
    elif Return == KnuthBendix:
        return to_knuth_bendix(*cxx_args)
    elif Return == (KnuthBendix, str, "RewriteFromLeft"):
        return to_knuth_bendix_string_RewriteFromLeft(*cxx_args)
    elif Return == (KnuthBendix, str, "RewriteTrie"):
        return to_knuth_bendix_string_RewriteTrie(*cxx_args)
    elif Return == (KnuthBendix, List[int], "RewriteFromLeft"):
        return to_knuth_bendix_word_RewriteFromLeft(*cxx_args)
    elif Return == (KnuthBendix, List[int], "RewriteTrie"):
        return to_knuth_bendix_word_RewriteTrie(*cxx_args)
    elif Return == (KnuthBendix, "RewriteTrie"):
        return to_knuth_bendix_RewriteTrie(*cxx_args)
    elif Return == (KnuthBendix, "RewriteFromLeft"):
        return to_knuth_bendix_RewriteFromLeft(*cxx_args)

    raise TypeError(
        """expected the 2nd positional argument to be one of:
            * FroidurePin
            * ToddCoxeter
            * (ToddCoxeter, str)
            * (ToddCoxeter, List[int])
            * Presentation
            * (Presentation, str)
            * (Presentation, List[int])
            * InversePresentation
            * (InversePresentation, str)
            * (InversePresentation, List[int])
            * KnuthBendix
            * (KnuthBendix, "RewriteFromLeft")
            * (KnuthBendix, "RewriteTrie")
            * (KnuthBendix, str, "RewriteFromLeft")
            * (KnuthBendix, str, "RewriteTrie")
            * (KnuthBendix, List[int], "RewriteFromLeft")
            * (KnuthBendix, List[int], "RewriteTrie")
        """
        f"but found {Return}"
    )
