# -*- coding: utf-8 -*-

# Copyright (c) 2025, James D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from forest.*pp in libsemigroups.
"""

import pytest

from libsemigroups_pybind11 import Forest, UNDEFINED, forest


@pytest.fixture(name="f")
def forest_fixture():
    return Forest([UNDEFINED, 0, 1, 2, 3], [UNDEFINED, 0, 0, 0, 0])


def test_forest_return_policy(f):
    assert f.add_nodes(2) is f
    assert f.init(2) is f
    assert f.labels() is not f.labels()
    assert f.parents() is not f.parents()
    assert f.labels() is not f.labels()
    assert f.set_parent_and_label(1, 0, 10) is f


def test_forest_depth(f):
    assert [forest.depth(f, n) for n in range(f.number_of_nodes())] == [
        0,
        1,
        2,
        3,
        4,
    ]


def test_forest_path_to_root(f):
    assert [forest.path_to_root(f, n) for n in range(f.number_of_nodes())] == [
        [],
        [0],
        [0] * 2,
        [0] * 3,
        [0] * 4,
    ]


def test_forest_path_from_root(f):
    assert [forest.path_from_root(f, n) for n in range(f.number_of_nodes())] == [
        [],
        [0],
        [0] * 2,
        [0] * 3,
        [0] * 4,
    ]


def test_forest_dot(f):
    assert (
        str(forest.dot(f))
        == """digraph Forest {
  rankdir="BT"
  0  [label="0: ε", shape="box"]
  1  [label="1: 0", shape="box"]
  2  [label="2: 00", shape="box"]
  3  [label="3: 000", shape="box"]
  4  [label="4: 0000", shape="box"]
  1 -> 0  [color="#00ff00"]
  2 -> 1  [color="#00ff00"]
  3 -> 2  [color="#00ff00"]
  4 -> 3  [color="#00ff00"]
}"""
    )
    assert (
        str(forest.dot(f, ["a"]))
        == """digraph Forest {
  rankdir="BT"
  0  [label="0: ε", shape="box"]
  1  [label="1: a", shape="box"]
  2  [label="2: aa", shape="box"]
  3  [label="3: aaa", shape="box"]
  4  [label="4: aaaa", shape="box"]
  1 -> 0  [color="#00ff00"]
  2 -> 1  [color="#00ff00"]
  3 -> 2  [color="#00ff00"]
  4 -> 3  [color="#00ff00"]
}"""
    )


def test_forest_is_root(f):
    assert [forest.is_root(f, n) for n in range(f.number_of_nodes())] == [
        True,
        False,
        False,
        False,
        False,
    ]


def test_forest_max_label(f):
    assert forest.max_label(f) == 0
    f = Forest()
    assert forest.max_label(f) == UNDEFINED
