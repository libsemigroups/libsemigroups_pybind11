# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``ukkonen`` subpackage, that
contains helper functions for the :any:`Ukkonen` class.
"""

# TODO add ukkonen_ prefix to all functions

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
