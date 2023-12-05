
.. Copyright (c) 2023, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Words
=====

``Words`` is a class for generating words (i.e. lists of ``int`` values) in a
given range and in a particular order. 

The order and range of the words in a ``Words`` instance can be set using the
member functions:

* :py:func:`Words.order`
* :py:func:`Words.number_of_letters`
* :py:func:`Words.min`
* :py:func:`Words.max`
* :py:func:`Words.first`
* :py:func:`Words.last`

.. seealso:: :py:class:`Strings`

Example
-------

.. code-block:: python

  Words words;
  words.order(Order::shortlex) # words in shortlex order
         .number_of_letters(2)   # on 2 letters
         .min(1)                 # of length in the range from 1
         .max(5)                 # to 5

Contents
--------

.. autosummary::
   :nosignatures:

   ~Words
   Words.at_end
   Words.count
   Words.first
   Words.get
   Words.init
   Words.last
   Words.max
   Words.min
   Words.next
   Words.number_of_letters
   Words.order
   Words.size_hint
   Words.upper_bound

Full API
--------

.. autoclass:: Words
   :members:
