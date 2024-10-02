# -*- coding: utf-8 -*-
# Copyright (c) 2023-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Ukkonen class and its helper namespace
ukkonen.
"""

# pylint: disable=fixme, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name


import pytest

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    UNDEFINED,
    Ukkonen,
    ukkonen,
)


def test_000_a():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])

    assert ukkonen.is_subword(t, [0, 0, 4, 0, 0, 0])
    assert ukkonen.is_subword(t, [0, 4])
    assert not ukkonen.is_subword(t, [4, 4])
    assert ukkonen.is_subword(t, [])
    assert ukkonen.is_subword(t, [0])
    assert ukkonen.is_subword(t, [0, 0])
    assert ukkonen.is_subword(t, [0, 0, 0])
    assert ukkonen.is_subword(t, [0, 0, 0])
    assert not ukkonen.is_subword(t, [0, 0, 0, 0])
    assert not ukkonen.is_subword(t, [1])
    assert ukkonen.number_of_distinct_subwords(t) == 16

    assert ukkonen.is_subword(t, [])
    assert ukkonen.is_subword(t, [0, 0, 4, 0, 0, 0])
    assert ukkonen.is_subword(t, [0, 0, 4, 0, 0])
    assert ukkonen.is_subword(t, [0, 0, 4, 0])
    assert ukkonen.is_subword(t, [0, 0, 4])
    assert ukkonen.is_subword(t, [0, 0])
    assert ukkonen.is_subword(t, [0])
    assert ukkonen.is_subword(t, [0, 4, 0, 0, 0])
    assert ukkonen.is_subword(t, [0, 4, 0, 0])
    assert ukkonen.is_subword(t, [0, 4, 0])
    assert ukkonen.is_subword(t, [0, 4])
    assert ukkonen.is_subword(t, [4, 0, 0, 0])
    assert ukkonen.is_subword(t, [4, 0, 0])
    assert ukkonen.is_subword(t, [4, 0])
    assert ukkonen.is_subword(t, [4])
    assert ukkonen.is_subword(t, [0, 0, 0])


def test_000_b():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])
    ukkonen.add_word(t, [0, 1, 2, 3])

    assert ukkonen.is_subword(t, [])
    assert ukkonen.is_subword(t, [0, 0, 4, 0, 0, 0])
    assert ukkonen.is_subword(t, [0, 0, 4, 0, 0])
    assert ukkonen.is_subword(t, [0, 0, 4, 0])
    assert ukkonen.is_subword(t, [0, 0, 4])
    assert ukkonen.is_subword(t, [0, 0])
    assert ukkonen.is_subword(t, [0])
    assert ukkonen.is_subword(t, [0, 4, 0, 0, 0])
    assert ukkonen.is_subword(t, [0, 4, 0, 0])
    assert ukkonen.is_subword(t, [0, 4, 0])
    assert ukkonen.is_subword(t, [0, 4])
    assert ukkonen.is_subword(t, [4, 0, 0, 0])
    assert ukkonen.is_subword(t, [4, 0, 0])
    assert ukkonen.is_subword(t, [4, 0])
    assert ukkonen.is_subword(t, [4])
    assert ukkonen.is_subword(t, [0, 0, 0])

    assert ukkonen.is_subword(t, [0, 1])
    assert ukkonen.is_subword(t, [0, 1, 2])
    assert ukkonen.is_subword(t, [0, 1, 2, 3])
    assert ukkonen.is_subword(t, [1])
    assert ukkonen.is_subword(t, [1, 2])
    assert ukkonen.is_subword(t, [1, 2, 3])
    assert ukkonen.is_subword(t, [2])
    assert ukkonen.is_subword(t, [2, 3])
    assert ukkonen.is_subword(t, [3])

    assert not ukkonen.is_subword(t, [3, 3])
    assert not ukkonen.is_subword(t, "ab")
    with pytest.raises(RuntimeError):
        ukkonen.is_subword(t, [UNDEFINED])


def test_000_c():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])
    ukkonen.add_word(t, [0, 1, 2, 3])
    assert ukkonen.number_of_distinct_subwords(t) == 25

    assert not ukkonen.is_suffix(t, [1, 2, 3, 5])
    assert ukkonen.is_suffix(t, [1, 2, 3])

    assert ukkonen.is_suffix(t, [])
    assert ukkonen.is_suffix(t, [0, 0, 4, 0, 0, 0])
    assert ukkonen.is_suffix(t, [0, 4, 0, 0, 0])
    assert ukkonen.is_suffix(t, [4, 0, 0, 0])
    assert ukkonen.is_suffix(t, [0, 0, 0])
    assert ukkonen.is_suffix(t, [0, 0])
    assert ukkonen.is_suffix(t, [0])
    assert ukkonen.is_suffix(t, [0, 1, 2, 3])
    assert ukkonen.is_suffix(t, [1, 2, 3])
    assert ukkonen.is_suffix(t, [2, 3])
    assert ukkonen.is_suffix(t, [3])

    assert not ukkonen.is_suffix(t, [3, 3])
    assert not ukkonen.is_suffix(t, "ab")

    assert len(ukkonen.maximal_piece_prefix(t, [0, 0, 4, 0, 0, 0])) == 2
    assert len(ukkonen.maximal_piece_prefix(t, [0, 1, 2, 3])) == 1


def test_002():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])
    ukkonen.add_word(t, [4, 5])

    assert ukkonen.number_of_distinct_subwords(t) == 18
    assert len(ukkonen.maximal_piece_prefix(t, [0, 0, 4, 0, 0, 0])) == 2
    assert len(ukkonen.maximal_piece_prefix(t, [4, 5])) == 1

    ukkonen.add_word(t, [0, 1, 2, 3])
    assert ukkonen.number_of_distinct_subwords(t) == 27
    assert len(ukkonen.maximal_piece_prefix(t, [0, 0, 4, 0, 0, 0])) == 2
    assert len(ukkonen.maximal_piece_prefix(t, [4, 5])) == 1
    assert len(ukkonen.maximal_piece_prefix(t, [0, 1, 2, 3])) == 1

    ukkonen.add_word(t, [0, 0, 4])
    assert ukkonen.number_of_distinct_subwords(t) == 27
    assert len(ukkonen.maximal_piece_prefix(t, [0, 0, 4, 0, 0, 0])) == 3
    assert len(ukkonen.maximal_piece_prefix(t, [0, 0, 4, 5, 6, 7, 8, 9])) == 3
    assert len(ukkonen.maximal_piece_prefix(t, [4, 5])) == 1
    assert len(ukkonen.maximal_piece_prefix(t, [0, 1, 2, 3])) == 1
    assert len(ukkonen.maximal_piece_prefix(t, [0, 0, 4])) == 3


def test_003_a():
    t = Ukkonen()
    assert t.number_of_distinct_words() == 0
    ukkonen.add_word(t, [0, 1, 2])
    assert t.number_of_distinct_words() == 1
    ukkonen.add_word(t, [1, 2, 4])
    assert t.number_of_distinct_words() == 2

    assert ukkonen.number_of_distinct_subwords(t) == 10

    assert ukkonen.is_subword(t, [])
    assert ukkonen.is_subword(t, [0])
    assert ukkonen.is_subword(t, [1])
    assert ukkonen.is_subword(t, [2])
    assert ukkonen.is_subword(t, [4])
    assert ukkonen.is_subword(t, [0, 1])
    assert ukkonen.is_subword(t, [1, 2])
    assert ukkonen.is_subword(t, [2, 4])
    assert ukkonen.is_subword(t, [0, 1, 2])
    assert ukkonen.is_subword(t, [1, 2, 4])
    with pytest.raises(TypeError):
        ukkonen.is_subword(t, [-1, 124])

    assert not ukkonen.is_subword(t, [1, 2, 3])
    assert not ukkonen.is_subword(t, [1, 2, 3, 4])
    assert not ukkonen.is_subword(t, [3])
    assert not ukkonen.is_subword(t, [1, 3])


def test_003_b():
    t = Ukkonen()
    assert t.number_of_distinct_words() == 0
    ukkonen.add_word(t, [0, 1, 2])
    assert t.number_of_distinct_words() == 1
    ukkonen.add_word(t, [1, 2, 4])
    assert t.number_of_distinct_words() == 2

    assert ukkonen.is_suffix(t, [])
    assert not ukkonen.is_suffix(t, [0])
    assert not ukkonen.is_suffix(t, [1])
    assert ukkonen.is_suffix(t, [2])
    assert ukkonen.is_suffix(t, [4])
    assert not ukkonen.is_suffix(t, [0, 1])
    assert ukkonen.is_suffix(t, [1, 2])
    assert ukkonen.is_suffix(t, [2, 4])
    assert ukkonen.is_suffix(t, [0, 1, 2])
    assert ukkonen.is_suffix(t, [1, 2, 4])
    with pytest.raises(TypeError):
        ukkonen.is_suffix(t, [-1, 124])

    assert ukkonen.maximal_piece_prefix(t, [0, 1, 2]) == []
    assert ukkonen.maximal_piece_suffix(t, [0, 1, 2]) == [1, 2]
    assert ukkonen.maximal_piece_suffix(t, [1, 2, 4]) == []

    assert ukkonen.maximal_piece_suffix(t, [0, 1, 2]) == [1, 2]
    assert ukkonen.maximal_piece_suffix(t, [1, 2, 4]) == []

    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == POSITIVE_INFINITY
    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == POSITIVE_INFINITY
    assert ukkonen.number_of_pieces(t, "abc") == POSITIVE_INFINITY
    assert ukkonen.pieces(t, [0, 1, 2]) == []
    assert ukkonen.maximal_piece_prefix(t, [1, 2, 4]) == [1, 2]
    assert ukkonen.number_of_pieces(t, [1, 2, 4]) == POSITIVE_INFINITY
    assert ukkonen.pieces(t, [1, 2, 4]) == []


def test_004():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 1, 2])
    ukkonen.add_word(t, [0])
    ukkonen.add_word(t, [1])
    ukkonen.add_word(t, [2])

    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == 3
    assert ukkonen.pieces(t, [0, 1, 2]) == [[0], [1], [2]]
    assert ukkonen.number_of_pieces(t, [0]) == 1
    assert ukkonen.pieces(t, [0]) == [[0]]
    assert ukkonen.number_of_pieces(t, [1]) == 1
    assert ukkonen.pieces(t, [1]) == [[1]]
    assert ukkonen.number_of_pieces(t, [2]) == 1
    assert ukkonen.pieces(t, [2]) == [[2]]

    ukkonen.add_word(t, [0, 1, 2, 8, 4, 5, 6, 7])
    ukkonen.add_word(t, [0, 1, 2])
    ukkonen.add_word(t, [8, 4, 5])
    ukkonen.add_word(t, [5, 6])
    ukkonen.add_word(t, [5, 6, 7])

    assert t.number_of_distinct_words() == 8
    assert t.number_of_words() == 9

    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == 1
    assert ukkonen.pieces(t, [0, 1, 2]) == [[0, 1, 2]]
    assert ukkonen.number_of_pieces(t, [0]) == 1
    assert ukkonen.number_of_pieces(t, [1]) == 1
    assert ukkonen.number_of_pieces(t, [2]) == 1

    assert ukkonen.number_of_pieces(t, [0, 1, 2, 8, 4, 5, 6, 7]) == 3
    assert ukkonen.pieces(t, [0, 1, 2, 8, 4, 5, 6, 7]) == [
        [0, 1, 2],
        [8, 4, 5],
        [6, 7],
    ]
    assert ukkonen.is_piece(t, [0, 1, 2])
    assert ukkonen.is_piece(t, [8, 4, 5])
    assert ukkonen.is_piece(t, [6, 7])
    assert ukkonen.number_of_pieces(t, [8, 4, 5]) == 1
    assert ukkonen.pieces(t, [8, 4, 5]) == [[8, 4, 5]]
    assert ukkonen.number_of_pieces(t, [5, 6]) == 1
    assert ukkonen.pieces(t, [5, 6]) == [[5, 6]]
    assert ukkonen.number_of_pieces(t, [5, 6, 7]) == 1

    assert ukkonen.pieces(t, [5, 6, 7]) == [[5, 6, 7]]
    assert ukkonen.number_of_pieces(t, [1, 2, 8, 4, 5]) == 2
    assert ukkonen.pieces(t, [1, 2, 8, 4, 5]) == [[1, 2], [8, 4, 5]]

    w = [0, 1, 2, 8, 4, 5, 6, 7, 0, 1, 2, 8, 4, 5, 6, 7]
    assert ukkonen.pieces(t, w) == [
        [0, 1, 2],
        [8, 4, 5],
        [6, 7],
        [0, 1, 2],
        [8, 4, 5],
        [6, 7],
    ]
