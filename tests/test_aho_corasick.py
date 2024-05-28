# -*- coding: utf-8 -*-

# Copyright (c) 2024, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from aho-corasick.*pp in libsemigroups.
"""
import copy, pytest
from libsemigroups_pybind11 import (
    AhoCorasick,
    aho_corasick,
    LibsemigroupsError,
    UNDEFINED,
)


def basic_ac():
    ac = AhoCorasick()
    aho_corasick.add_word(ac, [0, 1, 0, 1])
    aho_corasick.add_word(ac, [0, 1, 1, 0])
    aho_corasick.add_word(ac, [0, 1, 1, 0, 1])
    aho_corasick.add_word(ac, [0, 1, 1, 0, 0])
    aho_corasick.rm_word(ac, [0, 1, 1, 0, 1])

    return ac


def test_add_rm_word():
    ac = AhoCorasick()
    assert ac.number_of_nodes() == 1

    aho_corasick.add_word(ac, [0, 1, 0, 1])
    with pytest.raises(LibsemigroupsError):
        # Can't add the same word twice
        aho_corasick.add_word(ac, [0, 1, 0, 1])

    aho_corasick.add_word(ac, [0, 1, 1, 0])
    aho_corasick.add_word(ac, [0, 1, 1, 0, 1])
    aho_corasick.add_word(ac, [0, 1, 1, 0, 0])
    aho_corasick.rm_word(ac, [0, 1, 1, 0, 1])
    with pytest.raises(LibsemigroupsError):
        # Can't remove a word not in the trie
        aho_corasick.rm_word(ac, [0, 1, 1, 0, 1])
    with pytest.raises(LibsemigroupsError):
        # Can't remove a non-terminal node
        aho_corasick.rm_word(ac, [0, 1])


def test_copy_constructor():
    ac = basic_ac()
    ac2 = copy.copy(ac)
    assert ac != ac2
    test_child(ac2)
    test_number_of_nodes(ac2)
    test_signature(ac2)
    test_traverse(ac2)
    test_validate_active_node_index(ac2)
    test_validate_node_index(ac2)


def test_child(ac=None):
    if ac is None:
        ac = basic_ac()

    assert ac.child(2, 0) == 3
    assert ac.child(2, 1) == 5
    assert ac.child(2, 2) == UNDEFINED
    assert ac.child(6, 0) == 8
    assert ac.child(6, 1) == UNDEFINED
    with pytest.raises(LibsemigroupsError):
        # Can't get the child of a node that doesn't exist
        ac.child(9, 0)
    with pytest.raises(LibsemigroupsError):
        # Can't get the child of an inactive node
        ac.child(7, 0)


def test_number_of_nodes(ac=None):
    if ac is None:
        ac = basic_ac()

    assert ac.number_of_nodes() == 8


def test_signature(ac=None):
    if ac is None:
        ac = basic_ac()

    assert ac.signature(8) == [0, 1, 1, 0, 0]
    with pytest.raises(LibsemigroupsError):
        # Can't get the signature of a node that doesn't exist
        ac.signature(9)
    with pytest.raises(LibsemigroupsError):
        # Can't get the signature of an inactive node
        ac.signature(7)


def test_traverse(ac=None):
    if ac is None:
        ac = basic_ac()

    assert ac.traverse(0, 0) == 1
    assert ac.traverse(0, 1) == 0
    assert ac.traverse(2, 0) == 3
    assert ac.traverse(2, 1) == 5
    assert ac.traverse(3, 0) == 1
    assert ac.traverse(3, 1) == 4
    with pytest.raises(LibsemigroupsError):
        # Can't traverse from inactive node
        ac.traverse(7, 0)
    with pytest.raises(LibsemigroupsError):
        # Can't traverse from a node that doesn't exist
        ac.traverse(9, 0)

    assert aho_corasick.traverse_word(ac, 1, [1, 1, 0, 1]) == 2
    assert aho_corasick.traverse_word(ac, 8, [0, 0, 0, 0, 0, 1, 0, 1]) == 4
    assert aho_corasick.traverse_word(ac, [0, 0, 0, 0, 0, 1, 0, 1]) == 4
    assert aho_corasick.traverse_word(ac, [0, 1, 0, 1, 1, 0, 1]) == 2
    with pytest.raises(LibsemigroupsError):
        # Can't traverse from inactive node
        aho_corasick.traverse_word(ac, 7, [0, 1, 0])
    with pytest.raises(LibsemigroupsError):
        # Can't traverse from a node that doesn't exist
        aho_corasick.traverse_word(ac, 9, [0, 1, 0])


def test_validate_active_node_index(ac=None):
    if ac is None:
        ac = basic_ac()

    ac.validate_active_node_index(8)
    with pytest.raises(LibsemigroupsError):
        ac.validate_active_node_index(7)
    with pytest.raises(LibsemigroupsError):
        ac.validate_active_node_index(9)


def test_validate_node_index(ac=None):
    if ac is None:
        ac = basic_ac()

    # 7 is a node, but note an active one
    ac.validate_node_index(7)
    ac.validate_node_index(8)
    with pytest.raises(LibsemigroupsError):
        ac.validate_node_index(9)
