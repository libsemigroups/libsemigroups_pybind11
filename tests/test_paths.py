# Copyright (c) 2024, M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for Paths and related functionality in
libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name,
# pylint: disable=duplicate-code, too-many-lines

import sys
import pytest

from libsemigroups_pybind11 import (
    LibsemigroupsError,
    Order,
    Paths,
    WordGraph,
    ToString,
    ReversiblePaths,
    word_graph,
    POSITIVE_INFINITY,
)


def test_001():
    w = WordGraph(0)
    n = 100
    w.add_nodes(n)
    w.add_to_out_degree(2)
    for i in range(n - 1):
        w.set_target(i, i % 2, i + 1)

    p = Paths(w)

    with pytest.raises(LibsemigroupsError):
        p.count()

    assert p.order() == Order.shortlex
    p.order(Order.lex)
    assert p.order() == Order.lex

    p.source(0)
    assert p.count() == 100
    assert p.get() == []
    p.next()
    assert p.get() == [0]
    p.next()
    assert p.get() == [0, 1]
    assert not p.at_end()

    p.source(50)
    assert p.count() == 50
    assert p.min() == 0

    p.source(10)
    p.target(20)
    assert p.source() == 10
    assert p.target() == 20
    assert p.count() == 1
    p.max(5)
    assert p.count() == 0
    p.max(11)
    assert p.count() == 1
    p.min(11)
    assert p.count() == 0


def test_ToString():
    w = WordGraph(0)
    n = 100
    w.add_nodes(n)
    w.add_to_out_degree(2)
    for i in range(n - 1):
        w.set_target(i, i % 2, i + 1)

    p = Paths(w)
    p.source(95)
    assert list(p) == [[], [1], [1, 0], [1, 0, 1], [1, 0, 1, 0]]
    p = p | ToString("ab")
    assert list(p) == ["", "b", "ba", "bab", "baba"]

    p = Paths(w)
    with pytest.raises(LibsemigroupsError):
        len(p)
    p.source(50)
    assert len(p) == 50
    p = p.source(95) | ToString("ba")
    assert len(p) == 5

    assert list(p) == ["", "a", "ab", "aba", "abab"]


def test_001_reversed():
    w = WordGraph(0)
    n = 100
    w.add_nodes(n)
    w.add_to_out_degree(2)
    for i in range(n - 1):
        w.set_target(i, i % 2, i + 1)

    p = ReversiblePaths(w)
    p.reverse(True)

    with pytest.raises(LibsemigroupsError):
        p.count()

    assert p.order() == Order.shortlex
    p.order(Order.lex)
    assert p.order() == Order.lex

    p.source(0)
    assert p.count() == 100
    assert p.get() == []
    p.next()
    assert p.get() == [0]
    p.next()
    assert p.get() == [1, 0]
    assert not p.at_end()

    p.source(50)
    assert p.count() == 50
    assert p.min() == 0

    p.source(10)
    p.target(20)
    assert p.source() == 10
    assert p.target() == 20
    assert p.count() == 1
    p.max(5)
    assert p.count() == 0
    p.max(11)
    assert p.count() == 1
    p.min(11)
    assert p.count() == 0


def test_ToString_reversed():
    w = WordGraph(0)
    n = 100
    w.add_nodes(n)
    w.add_to_out_degree(2)
    for i in range(n - 1):
        w.set_target(i, i % 2, i + 1)

    p = ReversiblePaths(w)
    p.reverse(True).source(95)
    assert p.reverse()
    assert p.source() == 95
    assert list(p) == [[], [1], [0, 1], [1, 0, 1], [0, 1, 0, 1]]
    p = p | ToString("ab")
    assert list(p) == ["", "b", "ab", "bab", "abab"]

    p = ReversiblePaths(w)
    with pytest.raises(LibsemigroupsError):
        len(p)
    p.reverse(True).source(50)
    assert len(p) == 50
    p = p.source(95) | ToString("ba")
    assert len(p) == 5

    assert list(p) == ["", "a", "ba", "aba", "baba"]


def test_paths_bug():
    wg = word_graph.to_word_graph(4, [[0, 1], [1, 0], [2, 2]])
    p = Paths(wg)
    p.source(0).target(1)
    assert p.source() == 0
    assert p.target() == 1
    assert p.count() == POSITIVE_INFINITY


def test_paths_bug2():
    wg = word_graph.to_word_graph(4, [[0, 1], [1, 0], [2, 2]])
    p = Paths(wg)
    assert p.max() is POSITIVE_INFINITY

    p.max(10)
    assert p.max() == 10
    p.max(POSITIVE_INFINITY)
    assert p.max() is POSITIVE_INFINITY

    with pytest.raises(LibsemigroupsError):
        len(p)
    p.source(0)
    assert len(p) == sys.maxsize
    assert p.count() is POSITIVE_INFINITY
