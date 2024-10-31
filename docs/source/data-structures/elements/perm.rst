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
``libsemigroups_pybind11`` for permutations in the :py:class:`Perm1` class.

These permutations are optimised for the number of points in the image with
fewer points requiring less space per point.  

.. TODO(0) If ``libsemigroups`` has been compiled with ``HPCombi`` enabled, then
   permutations with degree at most 16 use the SSE and AVX instruction sets for
   very fast manipulation.

There are numerous functions for computing things about
:py:class:`Perm1` objects detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

Contents
--------

.. autosummary::
   :nosignatures:

   ~Perm1
   Perm1.copy
   Perm1.images
   Perm1.increase_degree_by
   Perm1.one
   Perm1.product_inplace
   Perm1.rank
   Perm1.swap


Full API
--------

.. autoclass:: Perm1
   :class-doc-from: class
   :members:
   :inherited-members:
   :special-members: __init__
