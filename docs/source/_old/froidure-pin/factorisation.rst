.. Copyright (c) 2021-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Factorisation, products, and relations
======================================

This page contains information about the functionality of the
:py:class:`FroidurePin` class related to factorisation, products, and
relations.

.. py:method:: FroidurePin.current_length(self: FroidurePin, pos: int) -> int

   Returns the length of the short-lex least word equal to the element in
   position ``pos`` (if any). No enumeration is performed.

   :param pos: the position
   :type pos: int

   :return: An ``int``.

.. py:method:: FroidurePin.current_max_word_length(self: FroidurePin) -> int

   Returns the maximum length of a word in the generators so far computed.

   :Parameters: None
   :return: An ``int``.

.. py:method:: FroidurePin.current_number_of_rules(self: FroidurePin) -> int

   Returns the number of relations that have been found so far.

   :Parameters: None
   :return: An ``int``.

.. py:method:: FroidurePin.factorisation(self: FroidurePin, pos: int) -> List[int]

   Returns a word representing an element given by index.

   :Parameters: **pos** (int) - the index of the element whose
                factorisation is sought

   :Returns: A ``List[int]``.

.. py:method:: FroidurePin.factorisation(self: FroidurePin, x: Element) -> List[int]
   :noindex:

   Factorise an element as a word in the generators.

   :Parameters: **x** (Element) - the possible element to factorise.

   :return: A ``List[int]``.

.. py:method:: FroidurePin.length(self: FroidurePin, pos: int) -> int

   Returns the length of the short-lex least word equal to the element in
   position ``pos`` (if any). Enumeration is triggered.

   :param pos: the position
   :type pos: int

   :return: An ``int``.

.. py:method:: FroidurePin.minimal_factorisation(self: FroidurePin, pos: int) -> List[int]

   Returns a short-lex least word representing an element given by index.

   :Parameters: **pos** (int) - the index of the element whose factorisation is
                                sought
   :return: A ``List[int]``.

.. py:method:: FroidurePin.number_of_rules(self: FroidurePin) -> int

   Returns the total number of relations in the presentation.

   :Parameters: None
   :return: An ``int``.

.. py:method:: FroidurePin.product_by_reduction(self: FroidurePin, i: int, j: int) -> int

   Compute a product using the Cayley graph.

   :param i: the first index of an element
   :type i: int
   :param j: the second index of an element
   :type j: int

   :return: An ``int``.

.. py:method:: FroidurePin.fast_product(self: FroidurePin, i: int, j: int) -> int

   Multiply elements via their indices.

   :param i: the index of the first element to multiply
   :type i: int
   :param j: the index of the second element to multiply
   :type j: int
    :return: An ``int``.

.. py:method:: FroidurePin.rules(self: FroidurePin) -> Iterator

   Returns an iterator pointing to the first rule (if any).

   :Parameters: None
   :return: An iterator.

.. py:method:: FroidurePin.word_to_element(self: FroidurePin, w: List[int]) -> Element

   Convert a word in the generators to an element.

   :param w: the word in the generators to evaluate.
   :type w: List[int]

   :return: A copy of the element represented by the word ``w``.
