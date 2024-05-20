.. Copyright (c) 2021-2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Transformations
===============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for transformations.

There are 4 types of transformations implemented in ``libsemigroups_pybind11``:

* ``StaticTransf16`` with fixed degree of 16;
* ``Transf1`` with any degree between ``0`` and ``2 ** 8`` where 1 byte
  integers are used to store the image points;
* ``Transf2`` with any degree between ``0`` and ``2 ** 16`` where 2 byte
  integers are used to store the image points;
* ``Transf4`` with any degree between ``0`` and ``2 ** 32`` where 4 byte
  integers are used to store the image points.

These types of transformations are optimised for the number of points in the
image with fewer points requiring less space per point.  If ``libsemigroups``
has been compiled with ``HPCombi`` enabled, then the objects returned by
:py:class:`StaticTransf16` use the SSE and AVX instruction sets for very fast
manipulation.

.. currentmodule:: libsemigroups_pybind11

The helper function :py:func:`Transf` can be used to automatically produce a
transformation of any of the above types depending on the input. 

While :py:func:`Transf` is not a class, the objects it returns all have identical
methods, and so we document the 4 types above as if they were a single class
named ``Transf``.

.. autofunction:: Transf

.. currentmodule:: _libsemigroups_pybind11

Full API
--------

.. autoclass:: StaticTransf16
   :class-doc-from: both
   :members:
   :inherited-members:
