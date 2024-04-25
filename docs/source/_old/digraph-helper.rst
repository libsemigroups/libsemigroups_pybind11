.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11.action_digraph_helper

Helper functions for ActionDigraph
==================================

This page contains the documentation for helper function for the class
:py:class:`ActionDigraph`.

.. .. autosummary::
..    :nosignatures:

..    add_cycle
..    dot
..    follow_path
..    is_acyclic
..    make
..    topological_sort

.. .. autofunction:: add_cycle
.. .. autofunction:: dot
.. .. autofunction:: follow_path
.. .. autofunction:: is_acyclic
.. py:function:: make(num_nodes: int, l: List[List[int]]) -> ActionDigraph

   Constructs a digraph from number of nodes and a list.

   This function constructs an ``ActionDigraph`` from its arguments
   whose out-degree is specified by the length of the first list in
   the 2nd parameter.

   :param num_nodes: the number of nodes in the digraph.
   :type num_nodes: int
   :param l: the out-neighbors of the digraph.
   :type l: List[List[int]]

   :returns: An ``ActionDigraph``.

   :raises RuntimeError:
     if :py:meth:`ActionDigraph.add_edge` raises when adding edges
     from ``l``.

   :Complexity:
     :math:`O(mn)` where :math:`m` is the length of `l` and :math:`n` is
     the parameter ``num_nodes``.

   .. TODO fix doctest

      .. >>> from libsemigroups_pybind11 import action_digraph_helper
      .. >>> # Construct an action digraph with 5 nodes and 10 edges (7
      .. >>> # specified)
      .. >>> action_digraph_helper.make(5, [[0, 0], [1, 1], [2], [3, 3]])
      .. <action digraph with 5 nodes, 7 edges, 2 out-degree>
.. .. autofunction:: topological_sort
