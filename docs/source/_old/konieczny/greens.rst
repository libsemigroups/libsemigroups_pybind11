.. Copyright (c) 2022-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Green's relations
=================

This page contains information about the methods of the :py:class:`Konieczny`
class related to Green's relations.


.. py:method:: Konieczny.is_regular_element(self: Konieczny, x: Element) -> bool

   Test regularity of an element.

   :param x: a possible element.
   :type x: Element

   :return: A ``bool``.

.. py:method:: Konieczny.D_class_of_element(self: Konieczny, x: Element) -> DClass

   Returns the :math:`\mathscr{D}`-class containing an element.

   :Raises: **RuntimeError** if ``x`` is not an element of ``self``.

   :note:
     This function computes as many frames for :math:`\mathscr{D}`-classes as
     necessary (this is all of them in the case that ``x`` is not an element of
     ``self``).

   :param x:  a possible element.
   :type x: Element

   :return: A :py:obj:`DClass`.

.. py:method:: Konieczny.D_classes(self: Konieczny) -> Iterator

   Returns an iterator pointing to the first :math:`\mathscr{D}`-class.

   :note:
     This  function does not trigger any enumeration; the iterator returned may
     be invalidated by any call to a method of the :py:obj:`Konieczny`
     class.

   :Parameters: None

   :return: An iterator.

.. py:method:: Konieczny.regular_D_classes(self: Konieczny) -> Iterator

   Returns an iterator pointing to the first regular :math:`\mathscr{D}`-class.

   :note:
     This  function does not trigger any enumeration; the iterator returned may
     be invalidated by any call to a method of the :py:obj:`Konieczny`
     class.

   :Parameters: None

   :return: An iterator.

.. py:method:: Konieczny.number_of_D_classes(self: Konieczny) -> int

   Returns the number of :math:`\mathscr{D}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.number_of_R_classes(self: Konieczny) -> int

   Returns the number of :math:`\mathscr{R}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.number_of_L_classes(self: Konieczny) -> int

   Returns the number of :math:`\mathscr{L}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.number_of_H_classes(self: Konieczny) -> int

   Returns the number of :math:`\mathscr{L}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_D_classes(self: Konieczny) -> int

   Returns the current number of :math:`\mathscr{D}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_R_classes(self: Konieczny) -> int

   Returns the current number of :math:`\mathscr{R}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_L_classes(self: Konieczny) -> int

   Returns the current number of :math:`\mathscr{L}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_H_classes(self: Konieczny) -> int

   Returns the current number of :math:`\mathscr{L}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.number_of_regular_D_classes(self: Konieczny) -> int

   Returns the number of regular :math:`\mathscr{D}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.number_of_regular_R_classes(self: Konieczny) -> int

   Returns the number of regular :math:`\mathscr{R}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.number_of_regular_L_classes(self: Konieczny) -> int

   Returns the number of regular :math:`\mathscr{L}`-classes.

   :note:
     This function triggers a full enumeration of the frames of every
     :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_regular_D_classes(self: Konieczny) -> int

   Returns the current number of regular :math:`\mathscr{D}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_regular_R_classes(self: Konieczny) -> int

   Returns the current number of regular :math:`\mathscr{R}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.current_number_of_regular_L_classes(self: Konieczny) -> int

   Returns the current number of regular :math:`\mathscr{L}`-classes.

   :note:
     This function triggers no enumeration.

   :Parameters: None

   :return: An integer.
