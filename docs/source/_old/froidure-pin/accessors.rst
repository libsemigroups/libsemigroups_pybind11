.. Copyright (c) 2021-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Accessors
=========

This page contains information about the methods of the
:py:class:`FroidurePin` class related to accessing elements.

.. py:method:: FroidurePin.__iter__(self: FroidurePin) -> Iterator

   Returns an iterator pointing to the first element.

   The order of iteration is the same as the order that elements were
   enumerated.

   :parameters: None
   :return: Iterator

.. py:method:: FroidurePin.at(self: FroidurePin, i: int) -> Element

   Access element specified by index with bound checks.

   :param i: the index of the element to access.
   :type i: int

   :return: The element with index ``i`` (if any).

.. py:method:: FroidurePin.current_size(self: FroidurePin) -> int

   Returns the number of elements so far enumerated.

   :Parameters: None
   :return: An ``int``.

.. py:method:: FroidurePin.size(self: FroidurePin) -> int

   Returns the size.

   :Parameters: None
   :return: An ``int``.

.. py:method:: FroidurePin.sorted_at(self: FroidurePin, i: int) -> Element

   Access element specified by sorted index with bound checks.

   :param i: the sorted index of the element to access.
   :type i: int

   :return: The element with index ``i`` (if any).

.. py:method:: FroidurePin.sorted(self: FroidurePin) -> Iterator

   Returns an iterator pointing to the first element (sorted).

   :Parameters: None
   :return: An iterator..
