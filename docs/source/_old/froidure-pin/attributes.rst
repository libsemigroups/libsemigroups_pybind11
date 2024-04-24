.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Attributes
==========

This page contains information about attributes of the :py:class:`FroidurePin`
class.

.. py:method:: FroidurePin.is_monoid(self: FroidurePin) -> bool

   Check if the semigroup is a monoid.

   :Parameters: None
   :return:
     ``True`` if the semigroup contains the identity of its element
     type, and ``False`` if not.

.. py:method:: FroidurePin.is_finite(self: FroidurePin) -> tril

   Check finiteness.

   :return: A value of type :py:obj:``tril``.

.. py:method:: FroidurePin.degree(self: FroidurePin) -> int

   Returns the degree of any and all elements.

   :Parameters: None
   :Returns: An ``int``.
