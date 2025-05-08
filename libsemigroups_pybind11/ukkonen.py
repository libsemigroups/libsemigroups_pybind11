# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the ukkonen namespace from libsemigroups.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    ukkonen_add_word as add_word,
    add_words,
    ukkonen_dot as dot,
    is_piece,
    is_subword,
    is_suffix,
    length_maximal_piece_prefix,
    length_maximal_piece_suffix,
    maximal_piece_prefix,
    maximal_piece_suffix,
    number_of_distinct_subwords,
    number_of_pieces,
    pieces,
    traverse,
)
