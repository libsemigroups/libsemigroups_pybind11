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

# pylint: disable=missing-function-docstring

import pytest

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    LibsemigroupsError,
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
    with pytest.raises(TypeError):
        ukkonen.is_subword(t, [UNDEFINED])


def test_000_c():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])
    ukkonen.add_word(t, [0, 1, 2, 3])
    assert ukkonen.number_of_distinct_subwords(t) == 25

    assert not ukkonen.is_suffix(t, [1, 2, 3, 5])

    st, w = ukkonen.traverse(t, [1, 2, 3, 5])
    assert not (len(w) == 4 and t.is_suffix(st))
    st, w = ukkonen.traverse(t, [5, 5])
    with pytest.raises(LibsemigroupsError):
        t.is_suffix(st)

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
    assert ukkonen.length_maximal_piece_prefix(t, [0, 0, 4, 0, 0, 0]) == 2
    assert ukkonen.length_maximal_piece_prefix(t, [0, 1, 2, 3]) == 1


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

    assert t.index([0, 1, 2]) == 0
    assert t.index([1, 2, 4]) == 1
    assert t.index([0, 1, 3]) == UNDEFINED
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

    assert ukkonen.length_maximal_piece_prefix(t, [0, 1, 2]) == 0
    assert ukkonen.length_maximal_piece_suffix(t, [0, 1, 2]) == 2
    assert ukkonen.length_maximal_piece_suffix(t, [1, 2, 4]) == 0

    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == POSITIVE_INFINITY
    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == POSITIVE_INFINITY
    assert ukkonen.number_of_pieces(t, "abc") == POSITIVE_INFINITY
    assert ukkonen.pieces(t, [0, 1, 2]) == []
    assert ukkonen.maximal_piece_prefix(t, [1, 2, 4]) == [1, 2]
    assert ukkonen.number_of_pieces(t, [1, 2, 4]) == POSITIVE_INFINITY
    assert ukkonen.pieces(t, [1, 2, 4]) == []


def test_004():
    t = Ukkonen()
    ukkonen.add_words(t, [[0, 1, 2], [0], [1], [2]])

    assert ukkonen.number_of_pieces(t, [0, 1, 2]) == 3
    assert ukkonen.pieces(t, [0, 1, 2]) == [[0], [1], [2]]
    assert ukkonen.number_of_pieces(t, [0]) == 1
    assert ukkonen.pieces(t, [0]) == [[0]]
    assert ukkonen.number_of_pieces(t, [1]) == 1
    assert ukkonen.pieces(t, [1]) == [[1]]
    assert ukkonen.number_of_pieces(t, [2]) == 1
    assert ukkonen.pieces(t, [2]) == [[2]]

    ukkonen.add_words(
        t, [[0, 1, 2, 8, 4, 5, 6, 7], [0, 1, 2], [8, 4, 5], [5, 6], [5, 6, 7]]
    )

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


def test_005():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])

    s = ukkonen.traverse(t, [])
    assert s[0].v == 0
    assert s[0].pos == 0
    assert s[1] == []

    s = ukkonen.traverse(t, [4])
    assert s[0].v == 4
    assert s[0].pos == 1
    assert s[1] == [4]

    s = ukkonen.traverse(t, [4, 0])
    assert s[0].v == 4
    assert s[0].pos == 2
    assert s[1] == [4, 0]

    s = ukkonen.traverse(t, [4, 0, 0, 0])
    assert s[0].v == 4
    assert s[0].pos == 4

    s = ukkonen.traverse(t, [0])
    assert s[0].v == 2
    assert s[0].pos == 1

    s = ukkonen.traverse(t, [0, 4])
    assert s[0].v == 3
    assert s[0].pos == 1

    s = ukkonen.traverse(t, [0, 4, 0, 0, 0])
    assert s[0].v == 3
    assert s[0].pos == 4
    assert s[0] == Ukkonen.State(3, 4)

    s = ukkonen.traverse(t, [0, 0, 2])
    assert t.distance_from_root(t.nodes()[s[0].v])


def test_006():
    t = Ukkonen()
    ukkonen.add_word(t, [0, 0, 4, 0, 0, 0])

    st = Ukkonen.State()
    ukkonen.traverse(t, st, [])
    assert st.v == 0
    assert st.pos == 0

    ukkonen.traverse(t, st, [4])
    assert st.v == 4
    assert st.pos == 1


def test_007():
    t = Ukkonen()
    ukkonen.add_words(t, [[0, 1, 2], [0], [1], [2]])

    nodes = t.nodes()
    root = nodes[0]
    assert root.is_root()
    assert not root.is_leaf()
    assert len(root.children) == 7
    assert root.l == 0
    assert root.r == 0
    assert root.parent == UNDEFINED
    assert root.child(0) == 5
    assert nodes[nodes[5].parent] == root
    assert root.length() == 0


def test_008():
    t = Ukkonen()
    ukkonen.add_words(
        t,
        [
            [0, 1, 2],
            [0, 1, 2],
            [0, 1, 2],
            [0],
            [1],
            [2],
            [4, 2, 1, 2, 3, 4],
            [4, 2, 1, 2],
            [2, 3, 4],
            [1, 2, 3, 4],
            [4, 2, 3, 4],
        ],
    )
    assert t.length_of_distinct_words() == 27
    assert t.length_of_words() == 33
    assert t.max_word_length() == 6
    assert t.multiplicity(0) == 3
    assert t.multiplicity(1) == 1
    assert t.number_of_distinct_words() == 9
    assert t.number_of_words() == 11
    assert len(t.nodes()) == 48
    t.init()
    assert t.length_of_distinct_words() == 0
    assert t.length_of_words() == 0
    assert t.max_word_length() == 0
    with pytest.raises(LibsemigroupsError):
        t.multiplicity(0)
    assert t.number_of_distinct_words() == 0
    assert t.number_of_words() == 0
    assert len(t.nodes()) == 1


def test_ukkonen_return_policy():
    kknn = Ukkonen()
    ukkonen.add_words(
        kknn,
        [
            [0, 1, 2],
            [0, 1, 2],
            [0, 1, 2],
            [0],
            [1],
            [2],
            [4, 2, 1, 2, 3, 4],
            [4, 2, 1, 2],
            [2, 3, 4],
            [1, 2, 3, 4],
            [4, 2, 3, 4],
        ],
    )
    assert kknn.copy() is not kknn
    assert kknn.init() is kknn

    st, _ = ukkonen.traverse(kknn, [1, 2, 3, 5])
    assert kknn.is_suffix(st) == UNDEFINED
