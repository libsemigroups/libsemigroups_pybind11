# -*- coding: utf-8 -*-

# Copyright (c) 2023-2024 M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for Gabow and related functionality in
libsemigroups_pybind11.
"""

# pylint: disable=missing-function-docstring

import pytest

from libsemigroups_pybind11 import (
    Gabow,
    WordGraph,
    word_graph,
)


@pytest.fixture(name="wg")
def word_graph_fixture():
    w = WordGraph(17, 31)
    for i in range(17):
        for j in range(31):
            w.target(i, j, (7 * i + 23 * j) % 17)
    return w


def test_001():
    w = WordGraph(17, 31)
    for i in range(17):
        for j in range(31):
            w.target(i, j, (7 * i + 23 * j) % 17)

    assert w.number_of_edges() == 31 * 17
    assert w.number_of_nodes() == 17

    g = Gabow(w)
    assert g.number_of_components() == 1
    assert g.id(5) == 0
    assert g.component_of(2) == [
        13,
        14,
        2,
        10,
        16,
        12,
        9,
        11,
        4,
        3,
        15,
        7,
        1,
        5,
        8,
        6,
        0,
    ]
    assert g.components()[0] == g.component_of(2)


def test_002():
    w = WordGraph()
    w.add_to_out_degree(1)
    word_graph.add_cycle(w, 33)
    g = Gabow(w)
    for i in range(33):
        assert g.id(i) == 0


def test_003():
    w = WordGraph(0)
    for j in range(1, 100):
        w.add_nodes(j)
    g = Gabow(w)
    for j in range(1, 100):
        for i in range(j * (j + 1) // 2):
            assert g.id(i) == i


def test_004():
    j = 33
    w = WordGraph()
    w.add_to_out_degree(1)
    for k in range(10):
        w.add_nodes(j)
        for i in range(k * j, (k + 1) * j - 1):
            w.target(i, 0, i + 1)
        w.target((k + 1) * j - 1, 0, k * j)
    g = Gabow(w)
    for i in range(10 * j):
        assert g.id(i) == i // j

    assert g.root_of(5) == 32

    forest = g.spanning_forest()
    assert forest.parent(0) == 32
    assert forest.parent(5) == 4
    forest.parents()
    assert len(forest.parents()) == 330

    revforest = g.reverse_spanning_forest()
    assert revforest.parent(0) == 1
    assert len(revforest.parents()) == 330

    assert w == g.word_graph()
    assert w is g.word_graph()

    assert list(g.roots()) == [32, 65, 98, 131, 164, 197, 230, 263, 296, 329]


def test_gabow_return_policy(wg):
    g = Gabow(wg)
    assert g.component(0) is not g.component(0)
    assert g.component_of(0) is not g.component_of(0)
    assert g.components() is not g.components()

    assert g.init(wg) is g
    assert not g.has_components()
    assert not g.has_components()
    assert g.reverse_spanning_forest() is g.reverse_spanning_forest()
    assert g.spanning_forest() is g.spanning_forest()
    assert g.word_graph() is g.word_graph()
