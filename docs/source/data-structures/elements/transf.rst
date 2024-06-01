.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Transformations
===============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for transformations in the
:py:class:`StaticTransf16` class.

These transformations are optimised for the number of points in the image with
fewer points requiring less space per point.  If ``libsemigroups`` has been
compiled with ``HPCombi`` enabled, then transformations with degree at most 16
use the SSE and AVX instruction sets for very fast manipulation.

There are numerous functions for computing things about
:py:class:`StaticTransf16` objects detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

Contents
--------

.. autosummary::
   :nosignatures:

   ~StaticTransf16
   StaticTransf16.copy
   StaticTransf16.images
   StaticTransf16.increase_degree_by
   StaticTransf16.one
   StaticTransf16.product_inplace
   StaticTransf16.rank
   StaticTransf16.swap


Full API
--------

.. autoclass:: StaticTransf16
   :class-doc-from: class
   :members:
   :inherited-members:
   :special-members: __init__
