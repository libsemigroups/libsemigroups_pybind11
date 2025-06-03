# -*- coding: utf-8 -*-

# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.


"""
This page contains the documentation for various helper functions for
manipulating :any:`WordGraph` objects. All such functions
are contained in the subpackage ``word_graph``.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    word_graph_add_cycle as add_cycle,
    word_graph_adjacency_matrix as adjacency_matrix,
    word_graph_dot as dot,
    word_graph_equal_to as equal_to,
    word_graph_follow_path as follow_path,
    word_graph_is_acyclic as is_acyclic,
    word_graph_is_compatible as is_compatible,
    word_graph_is_complete as is_complete,
    word_graph_is_connected as is_connected,
    word_graph_is_reachable as is_reachable,
    word_graph_is_strictly_cyclic as is_strictly_cyclic,
    word_graph_last_node_on_path as last_node_on_path,
    word_graph_nodes_reachable_from as nodes_reachable_from,
    word_graph_number_of_nodes_reachable_from as number_of_nodes_reachable_from,
    word_graph_random_acyclic as random_acyclic,
    word_graph_spanning_tree as spanning_tree,
    word_graph_standardize as standardize,
    word_graph_topological_sort as topological_sort,
)
