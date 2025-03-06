# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the stephen namespace from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    PresentationWords as _PresentationWords,
    InversePresentationWords as _InversePresentationWords,
    StephenPresentationWords as _StephenPresentationWords,
    StephenInversePresentationWords as _StephenInversePresentationWords,
    accepts,
    is_left_factor,
    left_factors,
    number_of_left_factors,
    number_of_words_accepted,
    words_accepted,
)

from libsemigroups_pybind11.presentation import Presentation


# TODO(2): Make this work with string presentations once it works
def Stephen(*args):  # pylint: disable=invalid-name
    """Construct a Stephen instance of the type specified by its arguments.

    Options for calling this function are:
    1  Stephen(presentation: PresentationWords)
    2  Stephen(presentation: InversePresentationWords)
    3  Stephen(presentation: StephenPresentationWords)
    4  Stephen(presentation: StephenInversePresentationWords)

    In cases 1 and 2 a new Stephen object is constructed with the given
    presentation. In cases 3 and 4 the Stephen object is constructed by copying
    an existing Stephen object. In cases 1 and 3 a StephenPresentationWords
    object is returned. In cases 2 and 4 a StephenInversePresentationWords
    object is returned.
    """

    if len(args) != 1:
        raise ValueError(f"expected 1 argument, but got {len(args)}")

    presentation_or_stephen = args[0]
    # Order important here due to inheritance
    if isinstance(
        presentation_or_stephen, _InversePresentationWords
    ) or isinstance(presentation_or_stephen, _StephenInversePresentationWords):
        PresentationType = _InversePresentationWords
    elif (
        isinstance(presentation_or_stephen, _PresentationWords)
    ) or isinstance(presentation_or_stephen, _StephenPresentationWords):
        PresentationType = _PresentationWords
    else:
        raise TypeError(
            f"expected first argument to have type in {{PresentationWords, "
            f"InversePresentationWords, StephenPresentationWords, "
            f"StephenInversePresentationWords}}, but found {type(presentation_or_stephen)}"
        )

    cpp_type = {
        _PresentationWords: _StephenPresentationWords,
        _InversePresentationWords: _StephenInversePresentationWords,
    }

    return cpp_type[PresentationType](presentation_or_stephen)
