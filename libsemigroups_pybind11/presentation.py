# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the Presentation class from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    PresentationStrings as __PresentationStrings,
    PresentationWords as __PresentationWords,
    InversePresentationStrings as __InversePresentationStrings,
    InversePresentationWords as __InversePresentationWords,
    add_identity_rules,
    add_inverse_rules,
    add_rule,
    add_rules,
    add_zero_rules,
    are_rules_sorted,
    change_alphabet,
    contains_rule,
    first_unused_letter,
    greedy_reduce_length,
    greedy_reduce_length_and_number_of_gens,
    human_readable_letter,
    is_strongly_compressible,
    length,
    longest_rule,
    longest_rule_length,
    longest_subword_reducing_length,
    make_semigroup,
    normalize_alphabet,
    reduce_complements,
    reduce_to_2_generators,
    remove_duplicate_rules,
    remove_redundant_generators,
    remove_trivial_rules,
    replace_subword,
    replace_word_with_new_generator,
    replace_word,
    reverse,
    shortest_rule_length,
    shortest_rule,
    sort_each_rule,
    sort_rules,
    strongly_compress,
    validate_semigroup_inverses,
)


def Presentation(arg):
    """
    Construct a Presentation instance of the type specified by its argument.
    """
    if isinstance(arg, str):
        result = __PresentationStrings()
        result.alphabet(arg)
    elif isinstance(arg, list) and all(isinstance(x, int) for x in arg):
        result = __PresentationWords()
        result.alphabet(arg)
    elif isinstance(arg, (__PresentationStrings, __PresentationWords)):
        raise TypeError(
            "expected the argument to be a string or a list of ints; received a "
            "presentation. If you are trying to copy a presentation, consider "
            "using copy.copy"
        )
    else:
        raise TypeError(
            "expected the argument to be a string or a list of ints"
        )
    return result


def InversePresentation(arg):
    """
    InversePresentation(arg: PresentationStrings | Word) -> InversePresentationStrings
    Construct an :any:`InversePresentationStrings` instance of the type
    specified by its argument.

    If *arg* is a :any:`PresentationStrings`, then the an
    :any:`InversePresentationStrings` is constructed with the same alphabet and
    rules as that of *arg*, and empty inverses.

    If *arg* is a :ref:`Word<pseudo_word_type_inv_class>`, then an
    :any:`InversePresentationStrings` is constructed with the alphabet equal to
    *arg*, empty rules, and empty inverses.

    :param arg: the argument from which the :any:`InversePresentationStrings`
        should be constructed
    :type arg: PresentationStrings | Word
    """
    if type(arg) is __PresentationStrings:
        result = __InversePresentationStrings(arg)
    elif type(arg) is __PresentationWords:
        result = __InversePresentationWords(arg)
    elif isinstance(arg, str):
        result = __InversePresentationStrings()
        result.alphabet(arg)
    elif isinstance(arg, list) and all(isinstance(x, int) for x in arg):
        result = __InversePresentationWords()
        result.alphabet(arg)
    elif isinstance(
        arg, (__InversePresentationStrings, __InversePresentationWords)
    ):
        raise TypeError(
            "expected the argument to be a presentation, a string or a list of "
            "ints; received an InversePresentation. If you are trying to copy "
            "an InversePresentation, consider using copy.copy"
        )
    else:
        raise TypeError(
            "expected the argument to be a Presentation, string, or list of "
            "ints"
        )
    return result
