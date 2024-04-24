.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Attributes
==========

This page contains information about attributes of the :py:class:`Konieczny`
class.

.. py:method:: Konieczny.degree(self: Konieczny) -> int

   Returns the degree of any and all elements.

   :Parameters: None
   :Returns: An ``int``.

.. py:method:: Konieczny.size(self: Konieczny) -> int

   Returns the size.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None
   :Returns: An ``int``.

.. py:method:: Konieczny.current_size(self: Konieczny) -> int

   Returns the current size.

   :note:
     This function triggers no enumeration.

   :Parameters: None
   :return: An ``int``.
