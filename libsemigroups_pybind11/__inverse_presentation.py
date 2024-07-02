# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unidiomatic-typecheck

"""
This package provides the constructor for the InversePresentation class from 
libsemigroups.
"""

from _libsemigroups_pybind11 import (
    InversePresentationStrings as __InversePresentationStrings,
    InversePresentationWords as __InversePresentationWords,
    PresentationStrings as __PresentationStrings,
    PresentationWords as __PresentationWords,
)


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
