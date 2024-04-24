.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Methods
=======

This page contains information about the methods of the
:py:class:`Konieczny.DClass` class.

.. py:method:: Konieczny.DClass.contains(self: Konieczny.DClass, x: Element) -> bool

   Test membership of an element in a :math:`\mathscr{D}`-class.

   :param x: a possible element.
   :type x: Element

   :return: A ``bool``.

.. py:method:: Konieczny.DClass.is_regular_D_class(self: Konieczny.DClass) -> bool

   Test regularity of a :math:`\mathscr{D}`-class.

   :Parameters: None

   :return: A ``bool``.

.. py:method:: Konieczny.DClass.number_of_R_classes(self: Konieczny.DClass) -> int

   Returns the number of :math:`\mathscr{R}`-classes.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.DClass.number_of_L_classes(self: Konieczny.DClass) -> int

   Returns the number of :math:`\mathscr{L}`-classes.

   :Parameters: None

   :return: An integer.

.. py:method:: Konieczny.DClass.number_of_idempotents(self: Konieczny.DClass) -> int

   Returns the number of idempotents.

   :Parameters: None
   :return: An ``int``.

.. py:method:: Konieczny.DClass.rep(self: Konieczny.DClass) -> Element

   Returns a representative of the :math:`\mathscr{D}`-class.

   :Parameters: None
   :return: An ``Element``.

.. py:method:: Konieczny.DClass.size(self: Konieczny.DClass) -> int

   Returns the size of a :math:`\mathscr{D}`-class.

   :Parameters: None
   :Returns: An ``int``.

.. py:method:: Konieczny.DClass.size_H_class(self: Konieczny.DClass) -> int

   Returns the size of the :math:`\mathscr{H}`-classes.

   :Parameters: None
   :Returns: An ``int``.
