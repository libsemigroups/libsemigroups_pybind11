.. Copyright (c) 2024 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Constants
=========

This page describes some of the constants used in ``libsemigroups_pybind11``.

.. autoclass:: Undefined

    .. autodata:: UNDEFINED

        This variable is used to indicate that a value is undefined. :any:`UNDEFINED`
        is comparable with any integral value (signed or unsigned) or constant via
        ``==`` and ``!=`` but not via ``<`` or ``>``.

.. autoclass:: PositiveInfinity

    .. autodata:: POSITIVE_INFINITY

        This variable of type :class:`_libsemigroups_pybind11.PositiveInfinity` represents :math:`\infty`. :any:`POSITIVE_INFINITY` is
        comparable via ``==``, ``!=``, ``<``, ``>`` with any integral value (signed or
        unsigned) and with  :any:`NEGATIVE_INFINITY`, and is comparable to any other
        constant via ``==`` and ``!=``, but not by ``<`` and ``>``.

.. autoclass:: LimitMax

    .. autodata:: LIMIT_MAX

        This variable represents the maximum value that certain function
        parameters can have. :any:`LIMIT_MAX` is comparable via ``==``, ``!=``, ``<``, ``>``
        with any integral value (signed or unsigned), and is comparable to any
        other constant via ``==`` and ``!=``, but not by ``<`` and ``>``.

.. autoclass:: NegativeInfinity

    .. autodata:: NEGATIVE_INFINITY

        This variable represents :math:`-\infty`. :any:`NEGATIVE_INFINITY` is
        comparable via ``==``, ``!=``, ``<``, ``>`` with any signed integral value and
        with :any:`POSITIVE_INFINITY`, and is comparable to any other constant via
        ``==`` and ``!=``.

  