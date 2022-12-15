.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

BMat8
=====

Class for fast boolean matrices of dimension up to 8 x 8

The functions for these small matrices over the boolean semiring are more
optimised than the generic methods for boolean matrices. Note that all
``BMat8`` are represented internally as an 8 x 8 matrix; any entries not defined by
the user are taken to be 0. This does not affect the results of any
calculations.

``BMat8`` objects can be constructed using:

1. No parameters (uninitialized)
2. An integer (the binary representation of which corresponds to the matrix)
3. A list of ``n`` lists of truthy or falsy values, each sublist must have length ``n`` also.

See `the libsemigroups documentation for further details. <https://libsemigroups.readthedocs.io/en/latest/_generated/libsemigroups__bmat8.html>`_

.. autoclass:: BMat8
   :members:
