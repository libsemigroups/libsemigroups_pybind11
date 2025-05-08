..
    Copyright (c) 2024 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Exceptions
==========

This page describes the custom error type used in ``libsemigroups_pybind11``,
namely :any:`LibsemigroupsError`. Other built-in exceptions, such as
:any:`ValueError` and :any:`TypeError`, may also be raised in this project. See
the `Python documentation <https://docs.python.org/3/library/exceptions.html>`_
for further details.

.. autoexception:: LibsemigroupsError
    :show-inheritance:

    This is the main type of exception raised by the custom data-structures and
    algorithms of ``libsemigroups_pybind11``. It is raised when the underlying
    C++ code from ``libsemigroups`` raises a ``LibsemigroupsException``.

    .. doctest:: python

        >>> from libsemigroups_pybind11 import FroidurePin, Perm
        >>> gens = [Perm([3, 0, 1, 2]), Perm([1, 2, 0, 3]), Perm([2, 1, 0, 3])]
        >>> S = FroidurePin(gens[0])
        >>> S.add_generators(gens[1:])
        <partially enumerated FroidurePin with 3 generators, 3 elements, Cayley graph ⌀ 1, & 0 rules>

        >>> S.generator(3) # Bad: there are only three generators
        Traceback (most recent call last):
            ...
        _libsemigroups_pybind11.LibsemigroupsError: generator index out of bounds, expected value in [0, 3), got 3

    .. note::

        If you believe your code is incorrectly throwing a
        :any:`LibsemigroupsError`, or isn't throwing a :any:`LibsemigroupsError`
        but you think it should, please let us known by opening an issue on the
        `issue tracker
        <https://github.com/libsemigroups/libsemigroups_pybind11/issues>`_.
