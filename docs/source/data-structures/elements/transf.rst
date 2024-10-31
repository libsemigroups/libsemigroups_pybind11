.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Transformations
===============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for transformations in the :py:class:`Transf1`
class.

These transformations are optimised for the number of points in the image with
fewer points requiring less space per point.  

.. If ``libsemigroups`` has been
.. compiled with ``HPCombi`` enabled, then transformations with degree at most 16
.. use the SSE and AVX instruction sets for very fast manipulation.

There are numerous functions for computing things about :py:class:`Transf1`
objects detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

Contents
--------

.. autosummary::
   :nosignatures:

   ~Transf1
   Transf1.copy
   Transf1.images
   Transf1.increase_degree_by
   Transf1.one
   Transf1.product_inplace
   Transf1.rank
   Transf1.swap


Full API
--------

.. autoclass:: Transf1
   :class-doc-from: class
   :members:
   :inherited-members:
   :special-members: __init__
