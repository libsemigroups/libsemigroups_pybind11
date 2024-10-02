.. Copyright (c) 2023-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Ukkonen class
=================

.. autoclass:: Ukkonen
   :doc-only:
   :class-doc-from: class

Contents
--------

.. autosummary::
    :nosignatures:

    ~Ukkonen.State
    Ukkonen.State.pos
    Ukkonen.State.v
    ~Ukkonen.Node
    Ukkonen.Node.child
    Ukkonen.Node.children
    Ukkonen.Node.is_leaf
    Ukkonen.Node.is_real_suffix
    Ukkonen.Node.is_root
    Ukkonen.Node.l
    Ukkonen.Node.length
    Ukkonen.Node.link
    Ukkonen.Node.parent
    Ukkonen.Node.r
    ~Ukkonen
    Ukkonen.distance_from_root
    Ukkonen.index
    Ukkonen.init
    Ukkonen.is_suffix
    Ukkonen.length_of_distinct_words
    Ukkonen.length_of_words
    Ukkonen.max_word_length
    Ukkonen.multiplicity
    Ukkonen.nodes
    Ukkonen.number_of_distinct_words
    Ukkonen.number_of_words
    Ukkonen.throw_if_not_unique_letters
    Ukkonen.unique_letter
    Ukkonen.word_index

Full API
--------

.. autoclass:: libsemigroups_pybind11::Ukkonen.State
   :members:

.. autoclass:: libsemigroups_pybind11::Ukkonen.Node
   :members:

.. autoclass:: Ukkonen
   :members:
   :class-doc-from: init
   :exclude-members: State, Node

