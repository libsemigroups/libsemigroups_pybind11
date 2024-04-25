.. Copyright (c) 2023, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Strings
=======

``Strings`` is a class for generating strings in a given range and in a
particular order. 

The order and range of the words in a ``Strings`` instance can be set using the
member functions:

* :py:func:`Strings.order`
* :py:func:`Strings.alphabet`
* :py:func:`Strings.min`
* :py:func:`Strings.max`
* :py:func:`Strings.first`
* :py:func:`Strings.last`

.. seealso:: :py:class:`Words`

Example
-------

.. code-block:: python

  Strings strings;
  strings.order(Order::shortlex) # strings in shortlex order
         .alphabet("ab")         # on "ab" letters
         .min(1)                 # of length in the range from 1
         .max(5)                 # to 5

Contents
--------

.. autosummary::
   :nosignatures:

   ~Strings
   Strings.alphabet
   Strings.at_end
   Strings.count
   Strings.first
   Strings.last
   Strings.get
   Strings.init
   Strings.max
   Strings.min
   Strings.next
   Strings.order
   Strings.size_hint
   Strings.upper_bound

Full API
--------

.. autoclass:: Strings
   :members:
