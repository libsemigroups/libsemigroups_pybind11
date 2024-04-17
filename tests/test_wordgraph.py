# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
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
        == 'digraph {\n\tnode [shape=box]\n\t0\n\t1\n\t2\n\t0 -> 0 [label=0 color="#cc6677"]\n\t0 -> 1 [label=1 color="#ddcc77"]\n\t1 -> 1 [label=0 color="#cc6677"]\n\t1 -> 0 [label=1 color="#ddcc77"]\n\t2 -> 2 [label=0 color="#cc6677"]\n\t2 -> 2 [label=1 color="#ddcc77"]\n}\n'  # pylint: disable=line-too-long
    )

    l = list(12 * [i] for i in range(4))

    d = word_graph.to_word_graph(4, l)
    assert (
        str(word_graph.dot(d))
        == 'digraph {\n\tnode [shape=box]\n\t0\n\t1\n\t2\n\t3\n\t0 -> 0 [label=0 color="#cc6677"]\n\t0 -> 0 [label=1 color="#ddcc77"]\n\t0 -> 0 [label=2 color="#117733"]\n\t0 -> 0 [label=3 color="#88ccee"]\n\t0 -> 0 [label=4 color="#44aa99"]\n\t0 -> 0 [label=5 color="#882255"]\n\t0 -> 0 [label=6 color="#44aa99"]\n\t0 -> 0 [label=7 color="#999933"]\n\t0 -> 0 [label=8 color="#332288"]\n\t0 -> 0 [label=9 color="#cc6677"]\n\t0 -> 0 [label=10 color="#ddcc77"]\n\t0 -> 0 [label=11 color="#117733"]\n\t1 -> 1 [label=0 color="#cc6677"]\n\t1 -> 1 [label=1 color="#ddcc77"]\n\t1 -> 1 [label=2 color="#117733"]\n\t1 -> 1 [label=3 color="#88ccee"]\n\t1 -> 1 [label=4 color="#44aa99"]\n\t1 -> 1 [label=5 color="#882255"]\n\t1 -> 1 [label=6 color="#44aa99"]\n\t1 -> 1 [label=7 color="#999933"]\n\t1 -> 1 [label=8 color="#332288"]\n\t1 -> 1 [label=9 color="#cc6677"]\n\t1 -> 1 [label=10 color="#ddcc77"]\n\t1 -> 1 [label=11 color="#117733"]\n\t2 -> 2 [label=0 color="#cc6677"]\n\t2 -> 2 [label=1 color="#ddcc77"]\n\t2 -> 2 [label=2 color="#117733"]\n\t2 -> 2 [label=3 color="#88ccee"]\n\t2 -> 2 [label=4 color="#44aa99"]\n\t2 -> 2 [label=5 color="#882255"]\n\t2 -> 2 [label=6 color="#44aa99"]\n\t2 -> 2 [label=7 color="#999933"]\n\t2 -> 2 [label=8 color="#332288"]\n\t2 -> 2 [label=9 color="#cc6677"]\n\t2 -> 2 [label=10 color="#ddcc77"]\n\t2 -> 2 [label=11 color="#117733"]\n\t3 -> 3 [label=0 color="#cc6677"]\n\t3 -> 3 [label=1 color="#ddcc77"]\n\t3 -> 3 [label=2 color="#117733"]\n\t3 -> 3 [label=3 color="#88ccee"]\n\t3 -> 3 [label=4 color="#44aa99"]\n\t3 -> 3 [label=5 color="#882255"]\n\t3 -> 3 [label=6 color="#44aa99"]\n\t3 -> 3 [label=7 color="#999933"]\n\t3 -> 3 [label=8 color="#332288"]\n\t3 -> 3 [label=9 color="#cc6677"]\n\t3 -> 3 [label=10 color="#ddcc77"]\n\t3 -> 3 [label=11 color="#117733"]\n}\n'  # pylint: disable=line-too-long
    )
