# -*- coding: utf-8 -*-

# Copyright (c) 2024, James D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
# pylint: disable=missing-function-docstring

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from dot.*pp in libsemigroups.
"""

import copy
import pytest

from libsemigroups_pybind11 import (
    Dot,
    LibsemigroupsError,
    word_graph,
    WordGraph,
)


def test_edge():
    wg = WordGraph(3, [[0, 1], [1, 0], [2, 2]])
    d = word_graph.dot(wg)
    edges = d.edges()
    assert len(edges) == 6
    assert edges[0].head == "0"
    assert edges[0].tail == "0"
    assert edges[0].attrs == {"color": "#00ff00"}
    edges[0].add_attr("style", "dashed")
    assert edges[0].attrs == {"color": "#00ff00", "style": "dashed"}
    edges[0].attrs["color"] = "blue"  # FIXME this should throw
    assert edges[0].attrs == {"color": "#00ff00", "style": "dashed"}


def test_node():
    wg = WordGraph(3, [[0, 1], [1, 0], [2, 2]])
    d = word_graph.dot(wg)
    nodes = d.nodes()
    assert len(nodes) == 3
    assert nodes[0].name == "0"
    assert nodes[0].attrs == {"shape": "box"}
    nodes[0].add_attr("shape", "circle")
    assert nodes[0].attrs == {"shape": "circle"}
    nodes[0].attrs["color"] = "blue"  # FIXME this should throw
    assert nodes[0].attrs == {"shape": "circle"}


def test_dot_copy():
    wg = WordGraph(3, [[0, 1], [1, 0], [2, 2]])
    d = word_graph.dot(wg)
    assert copy.copy(d) is not d


def test_dot_attrs():
    wg = WordGraph(3, [[0, 1], [1, 0], [2, 2]])
    d = word_graph.dot(wg)
    d.add_attr("node [shape=circle]")
    assert d.attrs() == {"node [shape=circle]": ""}
    d.add_attr("splines", "line")
    assert d.attrs() == {"node [shape=circle]": "", "splines": "line"}
    del d.attrs()["splines"]  # TODO(1) should raise
    assert d.attrs() == {"node [shape=circle]": "", "splines": "line"}


def test_dot_add_node():
    d = Dot()
    d.add_node("cat")
    with pytest.raises(LibsemigroupsError):
        d.add_node("cat")
    n = d.add_node("dog")
    assert len(d.nodes()) == 2
    assert d.to_string() == "digraph {\n\n  cat\n  dog\n}"
    n.add_attr("shape", "box")
    assert d.to_string() == 'digraph {\n\n  cat\n  dog  [shape="box"]\n}'


def test_dot_add_edge():
    d = Dot()
    d.add_node("cat")
    d.add_node("dog")
    assert len(d.edges()) == 0
    e = d.add_edge("cat", "dog")
    assert len(d.edges()) == 1
    assert e.attrs == {}
    e.add_attr("color", "#00FF00")
    assert e.attrs == {"color": "#00FF00"}


def test_dot_add_subgraph():
    d = Dot()
    d.name("pets")
    assert d.name() == "pets"
    d.add_node("cat")
    d.add_node("dog")
    d.add_edge("cat", "dog")

    assert len(d.subgraphs()) == 0
    d.add_subgraph(d)
    assert len(d.subgraphs()) == 1
    assert d.subgraphs()[0].kind() == Dot.Kind.subgraph
    assert (
        d.to_string()
        == 'digraph pets {\n\nsubgraph cluster_pets {\n  label="pets"\n  '
        'cluster_pets_cat  [label="cat"]\n  cluster_pets_dog  '
        '[label="dog"]\n  cluster_pets_cat -> cluster_pets_dog  '
        '[constraint="false"]\n}\n  cat\n  dog\n  cat -> dog\n}'
    )


def test_dot_colors():
    d = Dot()
    assert d.colors == [
        "#00ff00",
        "#ff00ff",
        "#007fff",
        "#ff7f00",
        "#7fbf7f",
        "#4604ac",
        "#de0328",
        "#19801d",
        "#d881f5",
        "#00ffff",
        "#ffff00",
        "#00ff7f",
        "#ad5867",
        "#85f610",
        "#84e9f5",
        "#f5c778",
        "#207090",
        "#764ef3",
        "#7b4c00",
        "#0000ff",
        "#b80c9a",
        "#601045",
        "#29b7c0",
        "#839f12",
    ]


def test_dot_kind():
    d = Dot()
    d.kind(Dot.Kind.digraph)
    assert d.kind() == Dot.Kind.digraph
    d.kind(Dot.Kind.graph)
    assert d.kind() == Dot.Kind.graph


def test_dot_return_policy():
    d = Dot()
    assert d.colors is not d.colors
    with pytest.raises(AttributeError):
        d.colors = [""]
    assert d.add_node("Sir Lancelot") is d.node("Sir Lancelot")
    assert d.add_edge("Sir Lancelot", "Sir Lancelot") is d.edge(
        "Sir Lancelot", "Sir Lancelot"
    )
    assert d.add_subgraph(d) is d
    assert d.edges() is not d.edges()
    assert d.subgraphs() is not d.subgraphs()
    assert d.attrs() is not d.attrs()
    assert d.nodes() is not d.nodes()
    with pytest.raises(LibsemigroupsError):
        assert d.node("Sir Lcanelot")
    assert d.node("Sir Lancelot") is d.node("Sir Lancelot")
    assert d.edge("Sir Lancelot", "Sir Lancelot") is d.edge(
        "Sir Lancelot", "Sir Lancelot"
    )
    assert d.add_attr("shape", "box") is d
    assert d.add_attr("cannot think") is d
