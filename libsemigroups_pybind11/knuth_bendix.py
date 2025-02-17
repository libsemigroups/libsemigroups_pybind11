# -*- coding: utf-8

# Copyright (c) 2024, J. D. Mitchell, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, unused-import, protected-access,
# pylint: disable=missing-function-docstring, line-too-long, duplicate-code

"""
This page contains the documentation for various helper functions for
manipulating :any:`KnuthBendixStringRewriteTrie` objects. All such functions
are contained in the submodule ``knuth_bendix``.
"""

from typing import List, Iterator, Union

from _libsemigroups_pybind11 import (
    KnuthBendixStringRewriteFromLeft as _KnuthBendixStringRewriteFromLeft,
    KnuthBendixStringRewriteTrie as _KnuthBendixStringRewriteTrie,
    KnuthBendixWordRewriteFromLeft as _KnuthBendixWordRewriteFromLeft,
    KnuthBendixWordRewriteTrie as _KnuthBendixWordRewriteTrie,
    PresentationStrings as _PresentationStrings,
    PresentationWords as _PresentationWords,
    congruence_kind as _congruence_kind,
    knuth_bendix_by_overlap_length as by_overlap_length,
    knuth_bendix_is_reduced as is_reduced,
    knuth_bendix_non_trivial_classes as non_trivial_classes,
    knuth_bendix_normal_forms as normal_forms,
    knuth_bendix_partition as partition,
    knuth_bendix_redundant_rule as redundant_rule,
)

from .detail.decorators import (
    may_return_positive_infinity as _may_return_positive_infinity,
)

from .detail import cong_intf

_Presentation = (_PresentationStrings, _PresentationWords)

for _KnuthBendix in (
    _KnuthBendixStringRewriteFromLeft,
    _KnuthBendixStringRewriteTrie,
    _KnuthBendixWordRewriteFromLeft,
    _KnuthBendixWordRewriteTrie,
):
    _KnuthBendix.number_of_classes = _may_return_positive_infinity(
        _KnuthBendix._number_of_classes
    )


def KnuthBendix(*args, Rewriter="RewriteTrie", **kwargs):  # pylint: disable=invalid-name
    """
    Construct a KnuthBendix instance of the type specified by its arguments.

    Options for calling this function are:
    1. KnuthBendix(Word = str | List[int], Rewriter = "RewriteTrie" | "RewriteFromLeft")
    2. KnuthBendix(Word = str | List[int], Rewriter = "RewriteTrie" | "RewriteFromLeft")
    3  KnuthBendix(congruence_kind, Presentation, Rewriter = "RewriteTrie" | "RewriteFromLeft")
    """

    # len(args) != 0 => not (1 or 2) and len(kwargs) != 0 => not 3.
    msg2 = f"""expected either:
    1) 2 positional arguments of types congruence_kind and Presentation; or
    2) 0 positional arguments and the keyword argument "Word";
(and possibly the keyword argument "Rewriter"). Found {len(args)} positional arguments
and keyword arguments {list(kwargs.keys())}!"""

    cong_intf.raise_if_bad_args(*args, msg2=msg2, **kwargs)

    if len(args) == 0:
        Word = kwargs["Word"]  # pylint: disable=invalid-name
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

    if Rewriter not in ("RewriteFromLeft", "RewriteTrie"):
        raise TypeError(
            f'expected the keyword argument "Rewriter" to be "RewriteFromLeft" or "RewriteTrie", but found "{Rewriter}"'
        )

    cpp_type = {
        (List[int], "RewriteTrie"): _KnuthBendixWordRewriteTrie,
        (str, "RewriteTrie"): _KnuthBendixStringRewriteTrie,
        (List[int], "RewriteFromLeft"): _KnuthBendixWordRewriteFromLeft,
        (str, "RewriteFromLeft"): _KnuthBendixStringRewriteFromLeft,
    }

    return cpp_type[(Word, Rewriter)](*args)


KnuthBendix.options = _KnuthBendixStringRewriteTrie.options
