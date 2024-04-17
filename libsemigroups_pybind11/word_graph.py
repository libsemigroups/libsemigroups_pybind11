# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the action_digraph_helper namespace from libsemigroups.
"""

from typing import List

import graphviz
from _libsemigroups_pybind11 import (
    WordGraph,
    add_cycle,
    follow_path,
    is_acyclic,
    to_word_graph,
    topological_sort,
)


def dot(d: WordGraph, node_labels=None) -> graphviz.Digraph:
    """
    Returns a :py:class:`graphviz.Digraph` of an :py:class:`WordGraph`.

    :param d: the :py:class:`WordGraph`
    :type d: WordGraph

    :returns:
      A graphviz representation of the input action digraph.
    :rtype:
      graphviz.Digraph

    .. doctest::

       >>> from libsemigroups_pybind11 import word_graph
       >>> d = word_graph.make(5, [[1, 0], [2], [3, 4]])
       >>> word_graph.dot(d).view()  # doctest: +SKIP

    """
    # the below is the muted, qualatative colour scheme from https://personal.sron.nl/~pault/
    color_scheme = [
        (204, 102, 119),  # rose
        (221, 204, 119),  # sand
        (17, 119, 51),  # green
        (136, 204, 238),  # cyan
        (68, 170, 153),  # teal
        (136, 34, 85),  # wine
        (68, 170, 153),  # teal
        (153, 153, 51),  # olive
        (51, 34, 136),  # indigo
    ]
    # the same set of colours is used cyclically
    k = int(d.out_degree() / len(color_scheme)) + 1
    colors = k * color_scheme
    # Add some tests too
    result = graphviz.Digraph()
    result.attr("node", shape="box")
    for n in range(d.number_of_nodes()):
        if node_labels is None:
            result.node(str(n))
        else:
            result.node(str(n), node_labels[n])
    for n in range(d.number_of_nodes()):
        for l in range(d.out_degree()):
            if d.target(n, l) != 18446744073709551615:
                c = colors[l]
                if d.out_degree() < len(colors):
                    result.edge(
                        str(n),
                        str(d.target(n, l)),
                        color=f"#{c[0]:x}{c[1]:x}{c[2]:x}",
                        label=str(l),
                    )
                else:
                    result.edge(
                        str(n),
                        str(d.target(n, l)),
                        label=str(l),
                    )

    return result
