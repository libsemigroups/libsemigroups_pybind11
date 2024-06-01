# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, wrong-import-position, unused-import

"""
This package provides the user-facing python part of libsemigroups_pybind11
"""

import pkgconfig

DISCLAIMER = (
    "(You should not see this message unless you are installing libsemigroups_pybind11 from its "
    "sources. If you are not installing from the sources, please raise an issue at "
    "https://github.com/libsemigroups/libsemigroups_pybind11)"
)

assert pkgconfig.exists("libsemigroups")

try:
    from _libsemigroups_pybind11 import (
        NEGATIVE_INFINITY,
        POSITIVE_INFINITY,
        LIMIT_MAX,
        UNDEFINED,
        error_message_with_prefix,
        Forest,
        Gabow,
        Paths,
        ReportGuard,
        Strings,
        ToStrings,
        ToWord,
        WordGraph,
        Words,
        algorithm,
        congruence_kind,
        number_of_words,
        order,
        overlap,
        parse_relations,
        random_word,
        to_string,
        to_word,
        LibsemigroupsError,
        is_obviously_infinite,
        BMat8,
        side,
        AhoCorasick,
    )
except ModuleNotFoundError as e:
    raise ModuleNotFoundError(
        (
            f'{e.msg}, did you forget to run "pip install ." in the libsemigroups_pybind11 '
            f"directory? {DISCLAIMER}"
        )
    ) from e


from .action import Action, RightAction, LeftAction
from .adapters import ImageRightAction, ImageLeftAction
from .knuth_bendix import KnuthBendix
from .presentation import Presentation
from .transf import (
    PPerm,
    Transf,
    Perm,
    one,
    inverse,
    image,
    domain,
    right_one,
    left_one,
)
