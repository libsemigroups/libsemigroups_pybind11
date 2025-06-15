..
    Copyright (c) 2021-2024, J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Matrix class
================

.. autoclass:: Matrix
    :doc-only:
    :class-doc-from: class

MatrixKind
----------

.. TODO move MatrixKind to the "enums" page when it exists

.. autoclass:: MatrixKind
    :show-inheritance:

Contents
--------

.. autosummary::
    :signatures: short

    Matrix.__init__
    Matrix.copy
    Matrix.degree
    Matrix.number_of_cols
    Matrix.number_of_rows
    Matrix.one
    Matrix.product_inplace
    Matrix.row
    Matrix.rows
    Matrix.scalar_one
    Matrix.scalar_zero
    Matrix.swap
    Matrix.transpose

Full API
--------

.. autoclass:: Matrix
    :class-doc-from: init
    :members:
    :inherited-members:
    :exclude-members: py_template_params_from_cxx_obj, init_cxx_obj
