..
    Copyright (c) 2021-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Partial perms
=============

This page contains the documentation for functionality in
``libsemigroups_pybind11`` for partial permutations in the :py:class:`PPerm`
class.

These partial permutations are optimised for the number of points in the image
with fewer points requiring less space per point.

..
    TODO(0) use somewhere?

..
    If ``libsemigroups`` has been

..
    compiled with ``HPCombi`` enabled, then partial permutations with degree at most 16

..
    use the SSE and AVX instruction sets for very fast manipulation.

There are numerous functions for computing things about :py:class:`PPerm`
objects detailed below.

.. toctree::
    :maxdepth: 1

    transf-helpers

Contents
--------

.. autosummary::
    :signatures: short

    ~PPerm
    PPerm.copy
    PPerm.images
    PPerm.increase_degree_by
    PPerm.one
    PPerm.product_inplace
    PPerm.rank
    PPerm.swap

Full API
--------

.. autoclass:: PPerm
    :class-doc-from: init
    :members:
    :inherited-members:
    :exclude-members: init_cxx_obj, py_template_params_from_cxx_obj
