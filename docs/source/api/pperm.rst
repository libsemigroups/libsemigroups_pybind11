.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Partial Perms
=============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for transformations.

There are 4 classes for transformations in ``libsemigroups_pybind11``, which
are: ``PPerm16``, ``PPerm1``, ``PPerm2``, and ``PPerm4`` for
transformations on 16 and up to 256, 65536, and 4294967296 points,
respectively.  These transformations have different space requirements, and
some types may permit more efficient operations.  If ``libsemigroups`` has been
compiled with ``HPCombi`` enabled, then the SSE and AVX instruction sets are
used for very fast manipulation of the ``PPerm16`` type.  If ``libsemigroups``
is not compiled with ``HPCombi`` enabled, then the ``PPerm16`` type uses
memory allocated on the stack, and not the heap, and so may still provide
superior performance to a ``PPerm1/2/4`` defined on the same points.

The methods of the types  ``PPerm16``, ``PPerm1``, ``PPerm2``, and
``PPerm4`` are all the same, and so we only include details for the
``PPerm1`` class.

.. autosummary:: 
   :nosignatures:

   ~PPerm1
   PPerm1.__getitem__
   PPerm1.__eq__
   PPerm1.__lt__
   PPerm1.__mul__
   PPerm1.degree
   PPerm1.identity
   PPerm1.inverse
   PPerm1.images
   PPerm1.left_one
   PPerm1.right_one
   PPerm1.make
   PPerm1.make_identity
   PPerm1.product_inplace
   PPerm1.rank

.. autoclass:: PPerm1 
   :members:
   :special-members: __getitem__, __eq__, __lt__, __mul__
