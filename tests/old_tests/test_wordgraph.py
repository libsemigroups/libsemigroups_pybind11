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

import pytest
from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    UNDEFINED,
    WordGraph,
    word_graph,
)


def binary_tree(number_of_levels):
    wg = WordGraph()
    wg.add_nodes(2**number_of_levels - 1)
    wg.add_to_out_degree(2)
    wg.add_edge(0, 1, 0)
    wg.add_edge(0, 2, 1)

    for i in range(2, number_of_levels + 1):
        counter = 2 ** (i - 1) - 1
        for j in range(2 ** (i - 2) - 1, 2 ** (i - 1) - 1):
            wg.add_edge(j, counter, 0)
            counter += 1
            wg.add_edge(j, counter, 1)
            counter += 1
    return wg


def test_000():
    g = WordGraph()
    assert g.number_of_nodes() == 0
    assert g.number_of_edges() == 0


def test_001():
    g = WordGraph(42)
    assert g.number_of_nodes() == 42
    assert g.number_of_edges() == 0


def test_out_neighbors():
    l = [[0, 1], [1, 0], [2, 2]]

    d = word_graph.to_word_graph(3, l)
    assert word_graph.out_neighbors(d) == l
    assert d == word_graph.to_word_graph(3, word_graph.out_neighbors(d))

    d = word_graph.to_word_graph(4, l)
    ll = [[0, 1], [1, 0], [2, 2], [18446744073709551615, 18446744073709551615]]
    assert word_graph.out_neighbors(d) == ll
    assert word_graph.to_word_graph(4, word_graph.out_neighbors(d)) == d


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
