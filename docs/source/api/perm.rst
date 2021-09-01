.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Partial Perms
=============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for transformations.

There are 4 classes for transformations in ``libsemigroups_pybind11``, which
are: ``Perm16``, ``Perm1``, ``Perm2``, and ``Perm4`` for
transformations on 16 and up to 256, 65536, and 4294967296 points,
respectively.  These transformations have different space requirements, and
some types may permit more efficient operations.  If ``libsemigroups`` has been
compiled with ``HPCombi`` enabled, then the SSE and AVX instruction sets are
used for very fast manipulation of the ``Perm16`` type.  If ``libsemigroups``
is not compiled with ``HPCombi`` enabled, then the ``Perm16`` type uses
memory allocated on the stack, and not the heap, and so may still provide
superior performance to a ``Perm1/2/4`` defined on the same points.

The methods of the types  ``Perm16``, ``Perm1``, ``Perm2``, and
``Perm4`` are all the same, and so we only include details for the
``Perm1`` class.

.. autosummary:: 
   :nosignatures:

   ~Perm1
   Perm1.__getitem__
   Perm1.__eq__
   Perm1.__lt__
   Perm1.__mul__
   Perm1.degree
   Perm1.identity
   Perm1.inverse
   Perm1.images
   Perm1.make
   Perm1.make_identity
   Perm1.product_inplace
   Perm1.rank

.. autoclass:: Perm1 
   :members:
   :special-members: __getitem__, __eq__, __lt__, __mul__

