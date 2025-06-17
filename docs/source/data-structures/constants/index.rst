..
    Copyright (c) 2024 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Constants
=========

This page describes some of the constants used in ``libsemigroups_pybind11``.

Constant types
--------------

.. autoclass:: LimitMax

.. autoclass:: NegativeInfinity

.. autoclass:: PositiveInfinity

.. autoclass:: Undefined

Constant values
---------------

.. py:data:: LIMIT_MAX

    This value represents the maximum value that certain function parameters
    can have. This value has type :any:`LimitMax`.

    :any:`LIMIT_MAX` is comparable via ``==``, ``!=``, ``<``, ``>``
    with any integral value, and is comparable to any other constant via ``==``
    and ``!=``, but not by ``<`` and ``>``.

.. py:data:: NEGATIVE_INFINITY
   :value: -∞

    This value represents :math:`-\infty` and has type :any:`NegativeInfinity`.

    :any:`NEGATIVE_INFINITY` is comparable via ``==``, ``!=``, ``<``, ``>``
    with any integer and with :any:`POSITIVE_INFINITY`, and is comparable to
    any other constant via ``==`` and ``!=``.

.. py:data:: POSITIVE_INFINITY
   :value: +∞

    This value represents :math:`+\infty` and has type :any:`PositiveInfinity`.

    :any:`POSITIVE_INFINITY` is comparable via ``==``, ``!=``, ``<``, ``>``
    with any integral and with :any:`NEGATIVE_INFINITY`, and is comparable to
    any other constant via ``==`` and ``!=``, but not by ``<`` and ``>``.

.. py:data:: UNDEFINED

    This value is used to indicate that a value is undefined and is of type
    :any:`Undefined`. 

    This value is comparable with other constants via ``==`` and ``!=``, and to
    itself by ``<`` and ``>`` but is not comparable to any other types using
    ``<`` or ``>``.
