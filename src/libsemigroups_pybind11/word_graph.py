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
    add_cycle,
    adjacency_matrix,
    word_graph_dot as dot,
    equal_to,
    follow_path,
    is_acyclic,
    is_compatible,
    is_complete,
    is_connected,
    is_reachable,
    is_strictly_cyclic,
    last_node_on_path,
    nodes_reachable_from,
    number_of_nodes_reachable_from,
    random_acyclic,
    spanning_tree,
    standardize,
    topological_sort,
)
