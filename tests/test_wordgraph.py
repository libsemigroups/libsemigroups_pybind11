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

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name,
# pylint: disable=duplicate-code, too-many-lines

from libsemigroups_pybind11 import (
    WordGraph,
    word_graph,
)


def test_000():
    w = WordGraph()
    assert w.number_of_nodes() == 0
    assert w.number_of_edges() == 0
    w.add_nodes(10)
    w.add_to_out_degree(1)
    w.set_target(2, 0, 3)
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
            w.set_target(i, 0, i + 1)
        w.set_target((k + 1) * j - 1, 0, k * j)
    assert w.out_degree() == 1
    assert next(w.edges_iterator(2)) == 3
    w.reverse_nodes_iterator()
    assert next(w.nodes_iterator()) == 0
    assert w.next_target(0, 0) == (1, 0)
    assert w.next_target_no_checks(1, 0) == (2, 0)
    w.reserve(400, 400)
    assert w.number_of_nodes() == 330
    assert w.number_of_edges() == 659
    assert w.out_degree() == 1


def test_random():
    w = WordGraph.random(5, 5)
    assert w.out_degree() == 5
    assert w.number_of_nodes() == 5
    assert w.number_of_edges() == 25

    w = WordGraph.random_acyclic(5, 6, 7)
    assert w.number_of_nodes() == 5
    assert w.out_degree() == 6
    assert w.number_of_edges() == 7


def test_dot():
    l = [[0, 1], [1, 0], [2, 2]]

    d = word_graph.to_word_graph(3, l)
    assert (
        str(word_graph.dot(d))
        == 'digraph WordGraph {\n\n  0  [shape="box"]\n  1  [shape="box"]\n  2'
        '  [shape="box"]\n  0 -> 0  [color="#00ff00"]\n  0 -> 1'
        '  [color="#ff00ff"]\n  1 -> 1  [color="#00ff00"]\n  1 -> 0'
        '  [color="#ff00ff"]\n  2 -> 2  [color="#00ff00"]\n  2 -> 2'
        '  [color="#ff00ff"]\n}'
    )

    l = list(12 * [i] for i in range(4))

    d = word_graph.to_word_graph(4, l)
    assert (
        str(word_graph.dot(d))
        == 'digraph WordGraph {\n\n  0  [shape="box"]\n  1  [shape="box"]\n  2  [shape="box"]\n  3  [shape="box"]\n  0 -> 0  [color="#00ff00"]\n  0 -> 0  [color="#ff00ff"]\n  0 -> 0  [color="#007fff"]\n  0 -> 0  [color="#ff7f00"]\n  0 -> 0  [color="#7fbf7f"]\n  0 -> 0  [color="#4604ac"]\n  0 -> 0  [color="#de0328"]\n  0 -> 0  [color="#19801d"]\n  0 -> 0  [color="#d881f5"]\n  0 -> 0  [color="#00ffff"]\n  0 -> 0  [color="#ffff00"]\n  0 -> 0  [color="#00ff7f"]\n  1 -> 1  [color="#00ff00"]\n  1 -> 1  [color="#ff00ff"]\n  1 -> 1  [color="#007fff"]\n  1 -> 1  [color="#ff7f00"]\n  1 -> 1  [color="#7fbf7f"]\n  1 -> 1  [color="#4604ac"]\n  1 -> 1  [color="#de0328"]\n  1 -> 1  [color="#19801d"]\n  1 -> 1  [color="#d881f5"]\n  1 -> 1  [color="#00ffff"]\n  1 -> 1  [color="#ffff00"]\n  1 -> 1  [color="#00ff7f"]\n  2 -> 2  [color="#00ff00"]\n  2 -> 2  [color="#ff00ff"]\n  2 -> 2  [color="#007fff"]\n  2 -> 2  [color="#ff7f00"]\n  2 -> 2  [color="#7fbf7f"]\n  2 -> 2  [color="#4604ac"]\n  2 -> 2  [color="#de0328"]\n  2 -> 2  [color="#19801d"]\n  2 -> 2  [color="#d881f5"]\n  2 -> 2  [color="#00ffff"]\n  2 -> 2  [color="#ffff00"]\n  2 -> 2  [color="#00ff7f"]\n  3 -> 3  [color="#00ff00"]\n  3 -> 3  [color="#ff00ff"]\n  3 -> 3  [color="#007fff"]\n  3 -> 3  [color="#ff7f00"]\n  3 -> 3  [color="#7fbf7f"]\n  3 -> 3  [color="#4604ac"]\n  3 -> 3  [color="#de0328"]\n  3 -> 3  [color="#19801d"]\n  3 -> 3  [color="#d881f5"]\n  3 -> 3  [color="#00ffff"]\n  3 -> 3  [color="#ffff00"]\n  3 -> 3  [color="#00ff7f"]\n}'  # pylint: disable=line-too-long
    )
