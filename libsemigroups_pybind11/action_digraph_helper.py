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
    ActionDigraph,
    add_cycle,
    follow_path,
    is_acyclic,
    make,
    topological_sort,
)


def out_neighbors(d: ActionDigraph) -> List[List[int]]:
    """
    Returns the list of out-neighbor of the action digraph ``d``.

    :param d: the :py:class:`ActionDigraph`
    :type d: ActionDigraph

    :returns:
      A list ``l`` where ``l[i][j]`` equals
      :py:meth:`ActionDigraph.neighbor` with arguments ``i`` and ``j``.
    :rtype: List[List[int]]

    Example
    -------
    .. code-block:: python

       from libsemigroups_pybind11 import action_digraph_helper
       d = action_digraph_helper.make(5, [[1, 0], [2], [3, 4]])
       action_digraph_helper.out_neighbors(d)
       # returns [[1, 0],
       # [2, 18446744073709551615],
       # [3, 4],
       # [18446744073709551615, 18446744073709551615],
       # [18446744073709551615, 18446744073709551615]]
    """
    result = []
    for n in range(d.number_of_nodes()):
        result.append([d.neighbor(n, i) for i in range(d.out_degree())])
    return result


def dot(d: ActionDigraph) -> graphviz.Digraph:
    """
    Returns a :py:class:`graphviz.Digraph` of an :py:class:`ActionDigraph`.

    :param d: the :py:class:`ActionDigraph`
    :type d: ActionDigraph

    :returns:
      A graphviz representation of the input action digraph.
    :rtype:
      graphviz.Digraph

    Example
    -------
    .. code-block:: python

       from libsemigroups_pybind11 import action_digraph_helper
       d = action_digraph_helper.make(5, [[1, 0], [2], [3, 4]])
       action_digraph_helper.dot(d).view()
    """
    colors = [
        (238, 20, 135),
        (0, 221, 164),
        (86, 151, 209),
        (249, 185, 131),
        (150, 114, 196),
    ]
    # Add some tests too
    result = graphviz.Digraph()
    result.attr("node", shape="circle")
    for n in range(d.number_of_nodes()):
        result.node(str(n))
    for n in range(d.number_of_nodes()):
        for l in range(d.out_degree()):
            if d.neighbor(n, l) != 18446744073709551615:
                c = colors[l]
                if d.out_degree() < len(colors):
                    result.edge(
                        str(n),
                        str(d.neighbor(n, l)),
                        color=f"#{c[0]:x}{c[1]:x}{c[2]:x}",
                        label=str(l),
                    )
                else:
                    result.edge(
                        str(n),
                        str(d.neighbor(n, l)),
                        label=str(l),
                    )

    return result
