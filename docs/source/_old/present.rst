.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Presentation
============

   This page describes the functionality in ``Presentation``.

   The objects returned by ``Presentation`` can be used to construction
   presentations for semigroups or monoids and is intended to be used as the
   input to other algorithms. The idea is to provide a shallow wrapper around a
   list of words represented by lists of integers or strings. We refer to this
   list of words as the **rules** of the presentation. The objects returned by ``Presentation``
   also provide some methods for checking that the rules really defines a
   presentation, (i.e. it's consistent with its alphabet). Some related
   functionality is available in ``libsemigroups_pybind11.presentation``.

   While :py:class:`Presentation` is not a class, the objects returned by
   :py:class:`Presentation` have identical methods, and so we document
   :py:class:`Presentation` as if it was a class.

Contents
--------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:class:`Presentation()`
     - Construct a presentation instance.

   * - :py:attr:`Presentation.rules`
     - The rules of the presentation.

   * - :py:meth:`Presentation.alphabet()`
     - Overloaded function.

   * - :py:meth:`Presentation.alphabet_from_rules()`
     - Set the alphabet to be the letters in the rules.

   * - :py:meth:`Presentation.contains_empty_word()`
     - Overloaded function.

   * - :py:meth:`Presentation.index()`
     - Get the index of a letter in the alphabet.

   * - :py:meth:`Presentation.letter()`
     - Get a letter in the alphabet by index.

   * - :py:meth:`Presentation.validate()`
     - Check if the alphabet and rules are valid, and raise an exception if not.

   * - :py:meth:`Presentation.validate_alphabet()`
     - Check if the alphabet is valid, and raise an exception if not.

   * - :py:meth:`Presentation.validate_letter()`
     - Check if a letter belongs to the alphabet or not, and raise an exception if not.

   * - :py:meth:`Presentation.validate_rules()`
     - Check if every rule consists of letters belonging to the alphabet, and raise an exception if not.

Full API
--------

.. py:class:: Presentation(alph: Union[str, List[int]])

   Construct a presentation instance with alphabet ``alph``. Note that
   ``alph`` can be the empty string or empty list.

.. py:attribute:: Presentation.rules

   Data member containing the rules of the presentation.

.. py:method:: Presentation.alphabet(self: Presentation) -> Union[str, List[int]]

   Return the alphabet of the presentation.

   :parameters: None

   :return: The alphabet of the presentation ``self``.

.. py:method:: Presentation.alphabet(self: Presentation, alph: Union[str, List[int]]) -> Presentation
   :noindex:

   Set the alphabet of the presentation to ``alph``. The argument ``alph`` must match the
   type of the argument with which the presentation ``self`` was initialised.

   :param alph: the alphabet.
   :type alph: str or List[int]

   :return: The presentation instance ``self``.

.. py:method:: Presentation.alphabet(self: Presentation, size: int) -> Presentation
   :noindex:

   Set the alphabet by specifying its size ``size``.

   :param size: the size to which the alphabet is set.
   :type size: int

   :return: The presentation instance ``self``.

.. py:method:: Presentation.alphabet_from_rules(self: Presentation) -> Presentation

   Set the alphabet to be the letters in its rules (see :py:attr:`Presentation.rules`).

   :parameters: None

   :return: The presentation instance ``self``.

.. py:method:: Presentation.contains_empty_word(self: Presentation) -> bool

   Check if the presentation is allowed to contain the empty word; that is,
   whether the presentation is a *semigroup* presentation or a *monoid* presentation.

   :parameters: None
   :return: A bool.

.. py:method:: Presentation.contains_empty_word(self: Presentation, val: bool) -> Presentation
   :noindex:

   Specify whether the presentation should (not) contain the empty word; that is, whether
   the presentation is a *semigroup* presentation or a monoid presentation.

   In order for a presentation which contains the empty word in any of its rules to be
   valid (see :py:meth:`validate_rules`), this value must be set to ``True``.


   :param val: whether or not the presentation should contain the empty word.
   :type val: bool

   :return: The presentation instance ``self``.

.. py:method:: Presentation.index(self: Presentation, val: int) -> int

   Get the index of a letter in the alphabet.

   :param val: the letter.
   :type val: int or str

   :returns: The index of ``val`` in the alphabet of the presentation ``self``.

.. py:method:: Presentation.letter(self: Presentation, i: int) -> Union[int, str]

   Get a letter in the alphabet by index.

   :param i: the index.
   :type i: int or str

   :return: The letter in the alphabet of the presentation ``self`` with index ``i``.

.. py:method:: Presentation.validate(self: Presentation) -> None

   Check if the alphabet and rules are valid, and raise an exception if not.

   :parameters: None

   :return: None

   :raises RuntimeError:

.. py:method:: Presentation.validate_alphabet(self: Presentation) -> None

   Check if the alphabet is valid, and raise an exception if not.

   :parameters: None

   :return: None

   :raises RuntimeError:

.. py:method:: Presentation.validate_letter(self: Presentation, c: Union[int, str]) -> None

   Check if a letter belongs to the alphabet or not, and raise an exception if not.

   :param c: the letter to check.
   :type c: int or str

   :return: None

   :raises RuntimeError:

.. py:method:: Presentation.validate_rules(self: Presentation) -> None

   Check if every rule consists of letters belonging to the alphabet,
   and raise an exception if not.

   If the presentation is a *semigroup* presentation
   rather than a *monoid* presentation (see :py:meth:`contains_empty_word`), an exception
   is raised if any rule contains the empty word.

   :parameters: None

   :returns: None

   :raises RuntimeError:
