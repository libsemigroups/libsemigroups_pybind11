# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
# pylint:disable=no-name-in-module, unused-import
"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the KnuthBendix class from libsemigroups.
"""
from typing import Union as __Union
from _libsemigroups_pybind11 import (
    KnuthBendixRewriteFromLeft as __KnuthBendixRewriteFromLeft,
    KnuthBendixRewriteTrie as __KnuthBendixRewriteTrie,
    PresentationStrings as __PresentationStrings,
    PresentationWords as __PresentationWords,
    congruence_kind as __congruence_kind,
    by_overlap_length,
    normal_forms,
    non_trivial_classes,
    redundant_rule,
    is_reduced,
    # try_equal_to
)


__Presentation = __Union[__PresentationStrings, __PresentationWords]
__KnuthBendix = __Union[__KnuthBendixRewriteFromLeft, __KnuthBendixRewriteTrie]


def KnuthBendix(*args, rewriter="RewriteTrie"):  # pylint: disable=invalid-name
    """
    Construct a KnuthBendix instance of the type specified by its arguments
    """
    if not (len(args) == 1 or len(args) == 2):
        raise TypeError(
            f"KnuthBendix() takes either 1 or 2 positional arguments ({len(args)} given)"
        )

    if not isinstance(args[0], (__congruence_kind, __KnuthBendix)):
        raise TypeError(
            (
                f"the first positional argument of KnuthBendix() must either be a congruence_kind"
                f"or KnuthBendix instance ({type(args[0])} given)"
            )
        )

    if isinstance(args[0], __KnuthBendix) and len(args) != 1:
        raise TypeError(
            (
                f"when copying a KnuthBendix instance, KnuthBendix() must only have one positional"
                f"argument  ({len(args)} given)"
            )
        )

    if (
        isinstance(args[0], __KnuthBendixRewriteFromLeft)
        and rewriter != "RewriteFromLeft"
    ):
        raise TypeError(
            (
                f"when copying a RewriteFromLeft KnuthBendix instance, the rewriter kwarg must be"
                f"RewriteFromLeft ({rewriter} given)"
            )
        )

    if (
        isinstance(args[0], __KnuthBendixRewriteTrie)
        and rewriter != "RewriteTrie"
    ):
        raise TypeError(
            (
                f"when copying a RewriteTrie KnuthBendix instance, the rewriter kwarg must be"
                f"RewriteTrie ({rewriter} given)"
            )
        )

    if len(args) == 2 and not isinstance(args[1], __Presentation):
        raise TypeError(
            (
                f"when KnuthBendix() is called with two positional arguments, the second positional"
                f"argument must be a presentation ({type(args[1])} given)"
            )
        )

    match rewriter:
        case "RewriteFromLeft":
            result = __KnuthBendixRewriteFromLeft(*args)

        case "RewriteTrie":
            result = __KnuthBendixRewriteTrie(*args)
        case _:
            raise TypeError(
                (
                    f"KnuthBendix() expects the rewriter kwarg to be either RewriteTrie or"
                    f"RewriteFromLeft ({rewriter} given)"
                )
            )

    return result
