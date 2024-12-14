.. Copyright (c) 2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Constructors + initializers
===========================

This page documents the constructors and initialisers for the
:any:`ToddCoxeterWord` class.

Every constructor has a matching init function with the same signature that can
be used to re-initialize a :any:`ToddCoxeterWord` instance as if it had just been
constructed; but without necessarily releasing any previous allocated memory.

Constructors
------------

.. autoclass:: ToddCoxeterWord
   :class-doc-from: init

Initializers
------------

.. automethod:: ToddCoxeterWord.init

Copying
-------

.. automethod:: ToddCoxeterWord.copy
