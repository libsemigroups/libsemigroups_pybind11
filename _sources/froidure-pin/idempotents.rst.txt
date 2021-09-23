.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Idempotents
===========

This page contains information about the methods of the :py:class:`FroidurePin`
class related to idempotents.

.. py:method:: FroidurePin.is_idempotent(self: FroidurePin, i: int) -> bool

   Check if an element is an idempotent via its index.

   :param i: the index of the element
   :type i: int

   :return: A ``bool``.
                
.. py:method:: FroidurePin.idempotents(self: FroidurePin) -> Iterator

   Returns an iterator pointing at the first idempotent.

   :return: An iterator.

.. py:method:: FroidurePin.number_of_idempotents(self: FroidurePin) -> int

   Returns the number of idempotents.

   :Parameters: None
   :return: An ``int``.
