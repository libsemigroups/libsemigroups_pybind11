..
    Copyright (c) 2021-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

HPCombi
=======

This page describes the functionality from `HPCombi`_ available in
``libsemigroups_pybind11``. This functionality is only available if
:any:`LIBSEMIGROUPS_HPCOMBI_ENABLED` is ``True``. `HPCombi`_ provides high
performance (SIMD accelerated) partial transformations, transformations,
permutations, and partial permutations on up to ``16`` points. The classes
implementing these element types belong to the ``hpcombi`` subpackage of
``libsemigroups_pybind11``, and can be used with the ``libsemigroups_pybind11``
classes:

* :any:`FroidurePin`
* :any:`Konieczny`
* :any:`SchreierSims`

Variables
~~~~~~~~~

.. py:attribute:: LIBSEMIGROUPS_HPCOMBI_ENABLED
   :type: bool

   This variable indicates whether or not the version of ``libsemigroups``
   being used by ``libsemigroups_pybind11`` was compiled with `HPCombi`_
   enabled.

Classes
~~~~~~~

.. toctree::
    :maxdepth: 1

    perm16
    pperm16
    ptransf16
    transf16
    vect16


.. _HPCombi: https://libsemigroups.github.io/HPCombi/
