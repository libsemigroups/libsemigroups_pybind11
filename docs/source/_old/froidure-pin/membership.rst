.. Copyright (c) 2021-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Membership + comparison
=======================

This page contains information about the methods of the :py:class:`FroidurePin`
class related to testing membership.

.. py:method:: FroidurePin.contains(self: FroidurePin, x: Element) -> bool

   Test membership of an element.

   :param x: a possible element.
   :type x: Element

   :return: A ``bool``.

.. py:method:: FroidurePin.current_position(self: FroidurePin, w: List[int]) -> int

   Returns the position corresponding to a word.

   :Parameters: **w** (List[int]) - a word in the generators
   :Returns: An ``int`` or :py:obj:`UNDEFINED`.

.. py:method:: FroidurePin.current_position(self: FroidurePin, i: int) -> int
   :noindex:

   Returns the position in of the generator with specified index.

   :Parameters: **i** (int) - the index of the generator
   :Returns: An ``int``.

.. py:method:: FroidurePin.current_position(self: FroidurePin, x: Element) -> int
   :noindex:

   Find the position of an element with no enumeration.

   :Parameters: **x** (Transf1) - a possible element.
   :Returns: An ``int``.

.. py:method:: FroidurePin.position_to_sorted_position(self: FroidurePin, i: int) -> int

   Returns the sorted index of an element via its index.

   :param i: the index of the element
   :type i: int

   :return: An ``int``.

.. py:method:: FroidurePin.position(self: FroidurePin, x: Element) -> int

   Find the position of an element with enumeration if necessary.

   :param x: a possible element.
   :type x: Element

   :return: An ``int``.

.. py:method:: FroidurePin.sorted_position(self: FroidurePin, x: Element) -> int

   Returns the sorted index of an element.

   :param x:  a possible element.
   :type x: Element

   :return: An ``int``.

.. py:method:: FroidurePin.equal_to(self: FroidurePin, x: List[int], y: List[int]) -> bool

   Check equality of words in the generators.

   :param x: the first word for comparison
   :type x: List[int]
   :param y: the second word for comparison
   :type y: List[int]

   :return: A ``bool``.
