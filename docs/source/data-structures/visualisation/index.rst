..
    Copyright (c) 2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

Visualisation
=============

This page contains links to the classes and functions in
``libsemigroups_pybind11`` for visualising instances of some of the other
objects implemented in ``libsemigroups_pybind11``.

The class :any:`Dot` facilitates the creation and rendering of graph
descriptions in the DOT_ language of Graphviz_ graph drawing software.

.. toctree::
    :maxdepth: 1

    dot
    dot.edge
    dot.kind
    dot.node

Functions for creating Dot objects
----------------------------------

The function ``dot`` can be used to produce initial :any:`Dot` objects for
various of the data structures and algorithms in libsemigroups:

.. currentmodule:: libsemigroups_pybind11

- :any:`aho_corasick.dot`
- :any:`stephen.dot`
- :any:`ukkonen.dot`
- :any:`word_graph.dot`
