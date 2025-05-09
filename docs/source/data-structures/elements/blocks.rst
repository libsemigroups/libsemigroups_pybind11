..
    Copyright (c) 2021-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Blocks
======

This page contains an overview of the functionality in
``libsemigroups_pybind11`` for blocks. See also :any:`Bipartition` for more
details and context.

There are a number of functions for computing things about :py:class:`Blocks`
objects detailed below.

.. toctree::
    :maxdepth: 1

    blocks-helpers

Contents
--------

.. autosummary::
    :signatures: short

    ~Blocks
    Blocks.copy
    Blocks.degree
    Blocks.is_transverse_block
    Blocks.iterator
    Blocks.lookup
    Blocks.number_of_blocks
    Blocks.rank

Full API
--------

.. autoclass:: Blocks
    :class-doc-from: class
    :members:
    :inherited-members:
    :special-members: __init__
