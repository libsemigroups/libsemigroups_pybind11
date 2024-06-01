.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Permutations
============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for permutations in the
:py:class:`StaticPerm16` class.

These permutations are optimised for the number of points in the image with
fewer points requiring less space per point.  If ``libsemigroups`` has been
compiled with ``HPCombi`` enabled, then permutations with degree at most 16
use the SSE and AVX instruction sets for very fast manipulation.

There are numerous functions for computing things about
:py:class:`StaticPerm16` objects detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

Contents
--------

.. autosummary::
   :nosignatures:

   ~StaticPerm16
   StaticPerm16.copy
   StaticPerm16.images
   StaticPerm16.increase_degree_by
   StaticPerm16.one
   StaticPerm16.product_inplace
   StaticPerm16.rank
   StaticPerm16.swap


Full API
--------

.. autoclass:: StaticPerm16
   :class-doc-from: class
   :members:
   :inherited-members:
   :special-members: __init__
