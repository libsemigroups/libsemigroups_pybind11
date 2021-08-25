.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Transformations
===============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for transformations.

There are 4 classes for transformations in ``libsemigroups_pybind11``, which
are: ``Transf16``, ``Transf1``, ``Transf2``, and ``Transf4`` for
transformations on 16 and up to 256, 65536, and 4294967296 points,
respectively.  These transformations have different space requirements, and
some types may permit more efficient operations.  If ``libsemigroups`` has been
compiled with ``HPCombi`` enabled, then the SSE and AVX instruction sets are
used for very fast manipulation of the ``Transf16`` type.  If ``libsemigroups``
is not compiled with ``HPCombi`` enabled, then the ``Transf16`` type uses
memory allocated on the stack, and not the heap, and so may still provide
superior performance to a ``Transf1/2/4`` defined on the same points.

The methods of the types  ``Transf16``, ``Transf1``, ``Transf2``, and
``Transf4`` are all the same, and so we only include details for the
``Transf1`` class.

.. autosummary:: 
   :nosignatures:

   ~Transf1
   Transf1.__getitem__
   Transf1.__eq__
   Transf1.__lt__
   Transf1.__mul__
   Transf1.degree
   Transf1.identity
   Transf1.images
   Transf1.make
   Transf1.make_identity
   Transf1.product_inplace
   Transf1.rank

.. autoclass:: Transf1
   :members:
   :special-members: __getitem__, __eq__, __lt__, __mul__

