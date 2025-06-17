# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for WordGraph and related functionality in
libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring

import copy
import pytest

from _libsemigroups_pybind11 import LIBSEMIGROUPS_EIGEN_ENABLED

from libsemigroups_pybind11 import (
    WordGraph,
    word_graph,
    LibsemigroupsError,
    UNDEFINED,
    Forest,
    Order,
    Meeter,
    Joiner,
    Matrix,
    MatrixKind,
)

if LIBSEMIGROUPS_EIGEN_ENABLED:
    import numpy as np


@pytest.fixture(name="word_graphs")
def word_graph_fixture():
    wg1 = WordGraph(0, 1)
    word_graph.add_cycle(wg1, 5)

    wg2 = WordGraph(0, 1)
    word_graph.add_cycle(wg2, 5)
    word_graph.add_cycle(wg2, 5)

    return wg1, wg2


def test_000():
    w = WordGraph()
    assert w.number_of_nodes() == 0
    assert w.number_of_edges() == 0
    w.add_nodes(10)
    w.add_to_out_degree(1)
    w.target(2, 0, 3)
    assert w.number_of_edges() == 1
    assert w.number_of_nodes() == 10

    g = WordGraph(42)
    assert g.number_of_nodes() == 42
    assert g.number_of_edges() == 0


def test_002():
    j = 33
    w = WordGraph()
    w.add_to_out_degree(1)
    for k in range(10):
        w.add_nodes(j)
        for i in range(k * j, (k + 1) * j - 1):
            w.target(i, 0, i + 1)
        w.target((k + 1) * j - 1, 0, k * j)
    assert w.out_degree() == 1
    assert next(w.targets(2)) == 3
    assert list(w.targets(2)) == [3]
    with pytest.raises(LibsemigroupsError):
        w.targets(9000)
    assert next(w.nodes()) == 0
    assert w.next_label_and_target(0, 0) == (0, 1)
    w.reserve(400, 400)
    assert w.number_of_nodes() == 330
    assert w.number_of_edges() == 659
    assert w.out_degree() == 1
    assert list(w.nodes()) == list(range(330))
    assert w.number_of_edges() == 659


def test_adjacency_matrix(word_graphs):
    wg1, _ = word_graphs
    assert wg1.number_of_edges() == 5
    assert wg1.number_of_nodes() == 5

    if LIBSEMIGROUPS_EIGEN_ENABLED:
        assert np.array_equal(
            word_graph.adjacency_matrix(wg1),
            np.array(
                [
                    [0.0, 1.0, 0.0, 0.0, 0.0],
                    [0.0, 0.0, 1.0, 0.0, 0.0],
                    [0.0, 0.0, 0.0, 1.0, 0.0],
                    [0.0, 0.0, 0.0, 0.0, 1.0],
                    [1.0, 0.0, 0.0, 0.0, 0.0],
                ]
            ),
        )
    else:
        assert word_graph.adjacency_matrix(wg1) == Matrix(
            MatrixKind.Integer,
            [
                [0, 1, 0, 0, 0],
                [0, 0, 1, 0, 0],
                [0, 0, 0, 1, 0],
                [0, 0, 0, 0, 1],
                [1, 0, 0, 0, 0],
            ],
        )


def test_equal_to(word_graphs):
    wg1, wg2 = word_graphs
    assert wg1 != wg2
    assert word_graph.equal_to(wg1, wg2, 0, 5)

    with pytest.raises(LibsemigroupsError):
        word_graph.equal_to(wg1, wg2, 1, 6)


def test_follow_path(word_graphs):
    wg1, _ = word_graphs
    assert word_graph.follow_path(wg1, 0, [0] * 6) == 1
    assert word_graph.follow_path(wg1, 0, []) == 0

    with pytest.raises(LibsemigroupsError):
        word_graph.follow_path(wg1, 0, [0, 1])

    with pytest.raises(LibsemigroupsError):
        word_graph.follow_path(wg1, 6, [0])


def test_is_acyclic(word_graphs):
    wg1, wg2 = word_graphs
    assert not word_graph.is_acyclic(wg1)
    wg1.remove_target(0, 0)
    assert word_graph.is_acyclic(wg1)
    wg2.remove_target(0, 0)
    assert not word_graph.is_acyclic(wg2)
    assert word_graph.is_acyclic(wg2, 0)
    assert word_graph.is_acyclic(wg2, 1)

    wg1.init(1, 1)
    word_graph.add_cycle(wg1, 4)
    wg1.target(0, 0, 1)
    assert not word_graph.is_acyclic(wg1, 0, 3)


def test_is_compatible(word_graphs):
    wg1, _ = word_graphs
    assert word_graph.is_compatible(wg1, 0, 1, [0] * 5, [0] * 10)
    assert not word_graph.is_compatible(wg1, 0, 1, [0] * 5, [0] * 9)


def test_is_complete(word_graphs):
    wg1, _ = word_graphs

    assert word_graph.is_complete(wg1)
    wg1.remove_target(0, 0)
    assert not word_graph.is_complete(wg1)

    assert wg1.number_of_nodes() == 5
    assert word_graph.is_complete(wg1, 1, 5)

    with pytest.raises(LibsemigroupsError):
        word_graph.is_complete(wg1, 1, 10)


def test_is_connected(word_graphs):
    wg1, _ = word_graphs
    wg1.remove_target(0, 0)
    wg1.remove_target(4, 0)
    assert not word_graph.is_connected(wg1)
    wg1.target(0, 0, 1)
    assert word_graph.is_connected(wg1)


def test_is_reachable(word_graphs):
    wg1, _ = word_graphs

    assert word_graph.is_reachable(wg1, 0, 1)
    assert word_graph.is_reachable(wg1, 0, 4)
    with pytest.raises(LibsemigroupsError):
        word_graph.is_reachable(wg1, 10, 0)
    with pytest.raises(LibsemigroupsError):
        word_graph.is_reachable(wg1, 0, 10)


def test_is_strictly_cyclic(word_graphs):
    wg1, _ = word_graphs

    assert word_graph.is_strictly_cyclic(wg1)
    wg1.remove_target(0, 0)
    assert word_graph.is_strictly_cyclic(wg1)
    wg1.remove_target(4, 0)
    assert not word_graph.is_strictly_cyclic(wg1)


def test_last_node_on_path(word_graphs):
    wg1, _ = word_graphs
    assert word_graph.last_node_on_path(wg1, 0, []) == (0, 0)
    assert word_graph.last_node_on_path(wg1, 0, [0]) == (1, 1)
    assert word_graph.last_node_on_path(wg1, 0, [0] * 7) == (2, 7)


def test_nodes_reachable_from(word_graphs):
    wg1, _ = word_graphs
    wg1.remove_target(4, 0)

    assert word_graph.nodes_reachable_from(wg1, 0) == {0, 1, 2, 3, 4}
    assert word_graph.nodes_reachable_from(wg1, 1) == {1, 2, 3, 4}
    assert word_graph.nodes_reachable_from(wg1, 3) == {3, 4}

    with pytest.raises(LibsemigroupsError):
        word_graph.nodes_reachable_from(wg1, 10)


def test_number_of_nodes_reachable_from(word_graphs):
    wg1, _ = word_graphs
    wg1.remove_target(4, 0)
    assert word_graph.number_of_nodes_reachable_from(wg1, 0) == 5
    assert word_graph.number_of_nodes_reachable_from(wg1, 1) == 4
    assert word_graph.number_of_nodes_reachable_from(wg1, 3) == 2

    with pytest.raises(LibsemigroupsError):
        word_graph.number_of_nodes_reachable_from(wg1, 10)


def test_spanning_tree(word_graphs):
    wg1, _ = word_graphs

    assert word_graph.spanning_tree(wg1, 0) == Forest(
        [UNDEFINED, 0, 1, 2, 3], [UNDEFINED, 0, 0, 0, 0]
    )

    f = Forest(0)
    word_graph.spanning_tree(wg1, 0, f)
    assert word_graph.spanning_tree(wg1, 0) == f


def test_standardize(word_graphs):
    wg1, _ = word_graphs
    f = Forest(0)
    assert not word_graph.standardize(wg1, f, Order.shortlex)
    assert f == word_graph.spanning_tree(wg1, 0)
    changed, f = word_graph.standardize(wg1, Order.lex)
    assert not changed
    assert f == word_graph.spanning_tree(wg1, 0)


def test_topological_sort(word_graphs):
    wg1, _ = word_graphs
    assert word_graph.topological_sort(wg1) == []
    wg1.remove_target(4, 0)
    assert word_graph.topological_sort(wg1) == [4, 3, 2, 1, 0]
    assert word_graph.topological_sort(wg1, 1) == [4, 3, 2, 1]


def test_meeter(word_graphs):
    wg1, _ = word_graphs
    wg1.remove_target(4, 0)

    meet = Meeter()
    assert wg1 == WordGraph(5, [[1], [2], [3], [4], [UNDEFINED]])
    assert meet(wg1, wg1) == wg1

    wg2 = WordGraph(2, [[1, 0], [1, 0]])
    wg3 = WordGraph(2, [[1, 1], [1, 1]])

    assert meet(wg3, wg2) == WordGraph(3, [[1, 2], [1, 2], [1, 2]])
    meet(wg1, wg3, wg2)
    assert wg1 == WordGraph(3, [[1, 2], [1, 2], [1, 2]])
    assert meet.is_subrelation(wg1, wg3)
    assert meet.is_subrelation(wg1, wg2)


def test_joiner(word_graphs):
    wg1, _ = word_graphs
    wg1.remove_target(0, 0)
    assert wg1 == WordGraph(5, [[UNDEFINED], [2], [3], [4], [0]])

    join = Joiner()
    assert join(wg1, wg1) == WordGraph(1, [[UNDEFINED]])

    wg3 = WordGraph(2, [[1, 1], [1, 1]])
    wg2 = WordGraph(2, [[1, 0], [1, 0]])

    assert join(wg3, wg2) == WordGraph(1, [[0, 0]])
    join(wg1, wg3, wg2)
    assert wg1 == WordGraph(1, [[0, 0]])
    assert join.is_subrelation(wg3, wg1)
    assert join.is_subrelation(wg2, wg1)


def test_str(word_graphs):
    wg1, wg2 = word_graphs
    assert str(wg1) == "WordGraph(5, [[1], [2], [3], [4], [0]])"
    assert (
        str(wg2)
        == "WordGraph(10, [[1], [2], [3], [4], [0], [6], [7], [8], [9], [5]])"
    )


def test_copy(word_graphs):
    wg1, wg2 = word_graphs
    assert wg1.copy() == wg1
    assert wg1.copy() is not wg1
    assert wg2.copy() == wg2
    assert wg2.copy() is not wg2

    assert copy.copy(wg1) == wg1
    assert copy.copy(wg1) is not wg1
    assert copy.copy(wg2) == wg2
    assert copy.copy(wg2) is not wg2


def test_random():
    w = WordGraph.random(5, 5)
    assert w.out_degree() == 5
    assert w.number_of_nodes() == 5
    assert w.number_of_edges() == 25

    w = word_graph.random_acyclic(5, 6)
    assert w.number_of_nodes() == 5
    assert w.out_degree() == 6
    assert word_graph.is_acyclic(w)
    assert word_graph.is_connected(w)

    d = WordGraph(3, [[0, 1], [1, 0], [2, 2]])
    assert (
        str(word_graph.dot(d))
        == 'digraph WordGraph {\n\n  0  [shape="box"]\n  1  [shape="box"]\n  2'
        '  [shape="box"]\n  0 -> 0  [color="#00ff00"]\n  0 -> 1'
        '  [color="#ff00ff"]\n  1 -> 1  [color="#00ff00"]\n  1 -> 0'
        '  [color="#ff00ff"]\n  2 -> 2  [color="#00ff00"]\n  2 -> 2'
        '  [color="#ff00ff"]\n}'
    )

    d = WordGraph(4, list(12 * [i] for i in range(4)))
    assert str(word_graph.dot(d)).startswith(
        'digraph WordGraph {\n\n  0  [shape="box"]\n  1  [shape="box"]\n'
    )


def test_word_graph_return_policy():
    wg = WordGraph.random(5, 5)
    assert wg.copy() is not wg
    assert wg.add_nodes(2) is wg
    assert wg.add_to_out_degree(2) is wg
    with pytest.raises(LibsemigroupsError):
        wg.disjoint_union_inplace(wg)

    assert wg.disjoint_union_inplace(wg.copy()) is wg
    assert wg.induced_subgraph(0, 7) is wg
    assert wg.init(5, 5) is wg
    assert wg.remove_all_targets() is wg
    assert wg.remove_label(0) is wg
    assert wg.remove_target(0, 1) is wg
    assert wg.reserve(5, 5) is wg
    assert wg.swap_targets(3, 2, 0) is wg


def test_meeter_return_policy():
    meet = Meeter()
    assert meet.copy() is not meet


def test_joiner_return_policy():
    join = Joiner()
    assert join.copy() is not join
