.. Copyright (c) 2025 Joseph Edwards

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Converting to a THIS_TYPE
==============================

This page contains documentation relating to converting
``libsemigroups_pybind11`` objects into :any:`THIS_TYPE` instances using
the :any:`to` function.

.. seealso::

    :doc:`/data-structures/to-function` for an overview of possible conversions
    between ``libsemigroups_pybind11`` types.

Various uses
------------

Recall that the signature for the :any:`to` function is ``to(*args, Return)``.
In what follows, we explain how different values of *args* and *Return* may
be used to construct :any:`THIS_TYPE` objects.

Converting a :any:`THAT_TYPE` to a :any:`THIS_TYPE`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To construct a :any:`THIS_TYPE` from a :any:`THAT_TYPE`, specify all
of the following values for *args*:

    * **ARG1** (:any:`TYPE_1`) -- DESCRIPTION.

Additionally, specify one of the following for *Return*:

    * ``(TYPE, INFO)`` for constructing a :any:`THIS_TYPE`.

DESCRIPTION

.. doctest:: Python

    >>> pass
