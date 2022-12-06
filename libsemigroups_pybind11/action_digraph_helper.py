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


from _libsemigroups_pybind11 import (
    make,
    ActionDigraph,
    add_cycle,
    follow_path,
    is_acyclic,
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
