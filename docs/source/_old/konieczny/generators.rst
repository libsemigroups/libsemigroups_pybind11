.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Generators
----------

This page contains information about the methods of the :py:class:`Konieczny`
class that relate to generators.

.. py:method:: Konieczny.add_generator(self: Konieczny, x: Element) -> None

   Add a copy of an element to the generators.

   :param x: the generator to add.
   :type x: Element

   :return: None

.. py:method:: Konieczny.number_of_generators(self: Konieczny) -> int

   Returns the number of generators.

   :Parameters: None
   :return: An ``int``.

.. py:method:: Konieczny.generator(self: Konieczny, i: int) -> Element

   Returns the generator with specified index.

   :param i: the index of a generator.
   :type i: int

   :return: An element.

.. py:method:: Konieczny.generators(self: Konieczny) -> Iterator

   Returns an iterator pointing to the first generator.

   This  function does not trigger any enumeration; the iterator returned may
   be invalidated by any call to a method of the :py:obj:`Konieczny`
   class.

   :Parameters: None
   :return: An iterator.
