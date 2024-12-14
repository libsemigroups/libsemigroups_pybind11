# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=unused-import, no-name-in-module, invalid-name
# pylint: disable=protected-access, duplicate-code

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the libsemigroups::kambites namespace from libsemigroups.
"""

from typing import List

from _libsemigroups_pybind11 import (
    KambitesMultiStringView as _KambitesMultiStringView,
    KambitesWord as _KambitesWord,
    PresentationStrings as _PresentationStrings,
    PresentationWords as _PresentationWords,
    congruence_kind as _congruence_kind,
    kambites_non_trivial_classes as non_trivial_classes,
    kambites_normal_forms as normal_forms,
    kambites_partition as partition,
)

from .detail.decorators import (
    may_return_positive_infinity as _may_return_positive_infinity,
)

from .detail import cong_intf


def Kambites(*args, **kwargs):  # pylint: disable=invalid-name
    """
    Function pretending to be a class, just dispatches to the corresponding
    constructor of the appropriate type based on the arguments.
    """
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
                f"expected the 2nd argument to be Presentation, but found {type(args[1])}"
            )

    cpp_type = {List[int]: _KambitesWord, str: _KambitesMultiStringView}
    return cpp_type[Word](*args)


for _Kambites in [_KambitesWord, _KambitesMultiStringView]:
    _Kambites.number_of_classes = _may_return_positive_infinity(
        _Kambites._number_of_classes  # pylint: disable=protected-access
    )
    # TODO(0) move this to conf.py
    _Kambites.number_of_classes.__doc__ = "\n".join(
        _Kambites._number_of_classes.__doc__.split("\n")[1:]
    )
    _Kambites.small_overlap_class = _may_return_positive_infinity(
        _Kambites._small_overlap_class  # pylint: disable=protected-access
    )
    # TODO(0) move this to conf.py
    _Kambites.small_overlap_class.__doc__ = "\n".join(
        _Kambites._small_overlap_class.__doc__.split("\n")[1:]
    )
