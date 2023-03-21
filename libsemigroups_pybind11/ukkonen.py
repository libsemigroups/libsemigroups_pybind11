# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the ukkonen namespace from libsemigroups.
"""

import graphviz

from _libsemigroups_pybind11 import (
    Ukkonen,
    _dot,
    add_words_no_checks,
    add_words,
    is_piece_no_checks,
    is_piece,
    is_subword_no_checks,
    is_subword,
    is_suffix_no_checks,
    is_suffix,
    maximal_piece_prefix_no_checks,
    maximal_piece_prefix,
    maximal_piece_suffix_no_checks,
    maximal_piece_suffix,
    number_of_distinct_subwords,
    number_of_pieces_no_checks,
    number_of_pieces,
    pieces_no_checks,
    pieces,
)


def dot(u: Ukkonen) -> str:
    """
    Returns a ``graphviz.Digraph`` containing a `GraphViz
    <https://graphviz.org>`_ representation of a suffix tree.

    :param u:  the Ukkonen object
    :type u: Ukkonen

    :returns: A value of type ``graphviz.Digraph``.

    :raises RuntimeError:
      if ``u`` does not contain any words.
    :raises RuntimeError:
      if the number of words in ``u`` is greater than 24.
    """
    return graphviz.Digraph(body=[x + "\n" for x in _dot(u).split("\n")][1:-2])
