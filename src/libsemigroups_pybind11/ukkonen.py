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

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    ukkonen_add_word as add_word,
    ukkonen_add_words as add_words,
    ukkonen_dot as dot,
    ukkonen_is_piece as is_piece,
    ukkonen_is_subword as is_subword,
    ukkonen_is_suffix as is_suffix,
    ukkonen_length_maximal_piece_prefix as length_maximal_piece_prefix,
    ukkonen_length_maximal_piece_suffix as length_maximal_piece_suffix,
    ukkonen_maximal_piece_prefix as maximal_piece_prefix,
    ukkonen_maximal_piece_suffix as maximal_piece_suffix,
    ukkonen_number_of_distinct_subwords as number_of_distinct_subwords,
    ukkonen_number_of_pieces as number_of_pieces,
    ukkonen_pieces as pieces,
    ukkonen_traverse as traverse,
)
