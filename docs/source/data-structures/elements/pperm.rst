.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Partial perms
=============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for partial permutations in the
:py:class:`StaticPPerm16` class.

These partial permutations are optimised for the number of points in the image with
fewer points requiring less space per point.  If ``libsemigroups`` has been
compiled with ``HPCombi`` enabled, then partial permutations with degree at most 16
use the SSE and AVX instruction sets for very fast manipulation.

There are numerous functions for computing things about
:py:class:`StaticPPerm16` objects detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

Contents
--------

.. autosummary::
   :nosignatures:

   ~StaticPPerm16
   StaticPPerm16.copy
   StaticPPerm16.images
   StaticPPerm16.increase_degree_by
   StaticPPerm16.one
   StaticPPerm16.product_inplace
   StaticPPerm16.rank
   StaticPPerm16.swap
   StaticPPerm16.undef


Full API
--------

.. autoclass:: StaticPPerm16
   :class-doc-from: class
   :members:
   :inherited-members:
   :special-members: __init__
