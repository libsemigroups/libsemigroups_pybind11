.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Konieczny
=========

``libsemigroups_pybind11`` contains a preliminary implementation of Konieczny's
generalisation of the Lallement-McFadden algorithm for computing the structure
of finite semigroups that act on sets; see the documentation below for further
details. It is preliminary in the sense that the interface exposed by the
relevant classes is minimal, and that certain optimisations remain to be
performed.

The implementation of the Konieczny and Lallement-McFadden algorithm can be
used "interactively", in the sense that they can be run for a particular amount
of time, or until some condition is met; for further details see, for example,
:py:meth:`Konieczny.run_for` and :py:meth:`Konieczny.run_until`.

The classes implementing the Konieczny and Lallement-McFadden algorithm are:

.. toctree::
   :maxdepth: 1

   konieczny/index.rst
   konieczny/dclass/index.rst
