# -*- coding: utf-8 -*-

# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-module-docstring, line-too-long

from typing import List, _GenericAlias
from _libsemigroups_pybind11 import (
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


def __to_congruence_word(*args):
    return _Congruence(_to_congruence_word(*args))


def __to_congruence_string(*args):
    return _Congruence(_to_congruence_string(*args))


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
        ...     _FroidurePin,
        ...     _KnuthBendix,
        ...     _Presentation,
        ...     presentation,
        ...     to,
        ... )

        >>> p = _Presentation([0, 1])
        >>> presentation.add_rule(p, [0, 1], [1, 0])
        >>> presentation.add_rule(p, [0, 0], [0])
        >>> presentation.add_rule(p, [1, 1], [1])
        >>> kb = _KnuthBendix(congruence_kind.twosided, p)

        >>> fp = to(kb, Return=_FroidurePin)
        >>> fp
        <partially enumerated _FroidurePin with 2 generators, 2 elements, Cayley graph ⌀ 1, & 0 rules>

        >>> fp.run()
        >>> fp
        <fully enumerated _FroidurePin with 2 generators, 3 elements, Cayley graph ⌀ 2, & 3 rules>

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
    cxx_args = [_to_cxx(arg) for arg in args]
    return_type_to_converter_function = {
        (_Congruence, str): __to_congruence_string,
        (_Congruence, List[int]): __to_congruence_word,
        _FroidurePin: lambda *x: _FroidurePin(_to_froidure_pin(*x)),
        _InversePresentation: lambda *x: _InversePresentation(
            _to_inverse_presentation(*x)
        ),
        (_InversePresentation, List[int]): lambda *x: _InversePresentation(
            _to_inverse_presentation_word(*x)
        ),
        (_InversePresentation, str): lambda *x: _InversePresentation(
            _to_inverse_presentation_string(*x)
        ),
        _KnuthBendix: lambda *x: _KnuthBendix(_to_knuth_bendix(*x)),
        (_KnuthBendix, "RewriteTrie"): lambda *x: _KnuthBendix(
            _to_knuth_bendix_RewriteTrie(*x)
        ),
        (_KnuthBendix, "RewriteFromLeft"): lambda *x: _KnuthBendix(
            _to_knuth_bendix_RewriteFromLeft(*x)
        ),
        (
            _KnuthBendix,
            List[int],
            "RewriteFromLeft",
        ): lambda *x: _KnuthBendix(_to_knuth_bendix_word_RewriteFromLeft(*x)),
        (
            _KnuthBendix,
            List[int],
            "RewriteTrie",
        ): lambda *x: _KnuthBendix(_to_knuth_bendix_word_RewriteTrie(*x)),
        (
            _KnuthBendix,
            str,
            "RewriteFromLeft",
        ): lambda *x: _KnuthBendix(_to_knuth_bendix_string_RewriteFromLeft(*x)),
        (_KnuthBendix, str, "RewriteTrie"): lambda *x: _KnuthBendix(
            _to_knuth_bendix_string_RewriteTrie(*x)
        ),
        _Presentation: lambda *x: _Presentation(_to_presentation(*x)),
        (_Presentation, str): lambda *x: _Presentation(
            _to_presentation_string(*x)
        ),
        (_Presentation, List[int]): lambda *x: _Presentation(
            _to_presentation_word(*x)
        ),
        _ToddCoxeter: lambda *x: _ToddCoxeter(_to_todd_coxeter(*x)),
        (_ToddCoxeter, str): lambda *x: _ToddCoxeter(
            _to_todd_coxeter_string(*x)
        ),
        (_ToddCoxeter, List[int]): lambda *x: _ToddCoxeter(
            _to_todd_coxeter_word(*x)
        ),
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
