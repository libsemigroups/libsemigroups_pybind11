.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Idempotents
===========

This page contains information about the methods of the :py:class:`Konieczny`
class related to idempotents.

.. py:method:: Konieczny.number_of_idempotents(self: Konieczny) -> int

   Returns the number of idempotents.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None
   :return: An ``int``.

.. py:method:: Konieczny.current_number_of_idempotents(self: Konieczny) -> int

   Returns the current number of idempotents.

   :note:
     This function triggers no enumeration.

   :Parameters: None
   :return: An ``int``.
