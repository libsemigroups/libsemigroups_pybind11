.. Copyright (c) 2021-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Prefixes and suffixes
=====================

This page contains information about the methods of the :py:class:`FroidurePin`
class related to prefixes and suffixes.

.. py:method:: FroidurePin.final_letter(self: FroidurePin, pos: int) -> int

   Returns the last letter of the element with specified index.

   :param pos: the position
   :type pos: int

   :return: An ``int``.

.. py:method:: FroidurePin.first_letter(self: FroidurePin, pos: int) -> int

   Returns the first letter of the element with specified index.

   :param pos: the position
   :type pos: int

   :return: An ``int``.

.. py:method:: FroidurePin.prefix(self: FroidurePin, pos: int) -> int

   Returns the index of the longest proper prefix.

   :param pos: the index
   :type pos: int

   :return: An ``int``.

.. py:method:: FroidurePin.suffix(self: FroidurePin, pos: int) -> int

   Returns the position of the longest proper suffix.

   :param pos: the position
   :type pos: int

   :return: An ``int``.
