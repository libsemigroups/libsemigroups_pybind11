.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.


Presentation helpers
====================

This page contains the documentation for various helper functions for
manipulating ``Presentation`` objects. All such functions are contained in the
submodule ``libsemigroups_pybind11.presentation``.

Contents
--------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:func:`add_identity_rules`
     - Add rules for an identity element.

   * - :py:func:`add_inverse_rules`
     - Add rules for inverses.

   * - :py:func:`add_rule_and_check`
     - Add a rule to the presentation by reference and check.

   * - :py:func:`add_rule`
     - Add a rule to the presentation.

   * - :py:func:`add_rules`
     - Add a rule to the presentation from another presentation.

   * - :py:func:`add_zero_rules`
     - Add rules for a zero element.

   * - :py:func:`are_rules_sorted`
     - Check if the rules :math:`u_1 = v_1, \ldots, u_n = v_n` satisfy :math:`u_1
       v_1 < \cdots < u_n v_n`, where :math:`<` is the shortlex order.

   * - :py:func:`change_alphabet`
     - Change or re-order the alphabet.

   * - :py:func:`character`
     - Return a possible character by index.

   * - :py:func:`length`
     - Return the sum of the lengths of the rules.

   * - :py:func:`letter`
     - Return the sum of the lengths of the rules.

   * - :py:func:`longest_common_subword`
     - Return the longest common subword of the rules.

   * - :py:func:`make`
     - Make a presentation from another type of presentation or a
       ``FroidurePin`` instance.

   * - :py:func:`normalize_alphabet`
     - Modify the presentation so that the alphabet is :math:`\{0, \ldots, n -
       1\}` (or equivalent), and rewrites the rules to use this alphabet.

   * - :py:func:`reduce_complements`
     - If there are rules :math:`u = v` and :math:`v = w` where :math:`\lvert w
       \rvert < \lvert v \rvert`, then replace :math:`u = v` with :math:`u =
       w`.

   * - :py:func:`redundant_rule`
     - Returns the index of the left hand side of a redundant rule.

   * - :py:func:`remove_duplicate_rules`
     - Remove duplicate rules.

   * - :py:func:`remove_redundant_generators`
     - Remove any trivially redundant generators.

   * - :py:func:`remove_trivial_rules`
     - Remove rules consisting of identical words.

   * - :py:func:`replace_subword`
     - Replace non-overlapping instances of a subword.

   * - :py:func:`replace_word`
     - Replace instances of a word occupying either side of a rule.

   * - :py:func:`reverse`
     - Reverse every rule.

   * - :py:func:`sort_each_rule`
     - Sort each rule :math:`u = v` so that the left hand side is shortlex
       greater than the right hand side.

   * - :py:func:`sort_rules`
     - Sort the rules :math:`u_1 = v_1, \ldots, u_n = v_n` so that :math:`u_1
       v_1 < \cdots < u_n v_n`, where :math:`<` is the shortlex order.

Full API
--------

.. py:function:: add_identity_rules(p: Presentation, e: Union[str, int]) -> None

   Add rules for an identity element.

   Adds rules of the form :math:`a e = e a = a` for every letter :math:`a` in
   the alphabet of ``p``, where :math:`e` is the second parameter.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param e: the identity element
   :type e: str or int

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("abc")
      presentation.add_identity_rules(p, "c")
      p.rules  # ['ac', 'a', 'ca', 'a', 'bc', 'b', 'cb', 'b', 'cc', 'c']


.. py:function:: add_inverse_rules(p: Presentation, vals: Union[str, List[int], e: Union[str, int]) -> None

   Add rules for inverses.

   The letter ``a`` with index ``i`` in ``vals`` is the inverse of the letter
   in the alphabet of ``p`` with index ``i``. The rules added are :math:`a_i
   b_i = e`, where the alphabet is :math:`\{a_i, \ldots, a_n\}`; the parameter
   ``vals`` is :math:`\{b_1, \ldots, b_n\}`; and :math:`e` is the 3rd
   parameter.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param vals: the inverses
   :type vals: str or List[int]
   :param e: the identity element
   :type e: str or int

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("abc")
      presentation.add_inverse_rules(p, "bac", "c")
      p.rules  # ['ab', 'c', 'ba', 'c']


.. py:function:: add_rule_and_check(p: Presentation, lhop: Union[str, List[int]], rhop: Union[str, List[int]]) -> None

   Add a rule to the presentation, and check that it is valid.

   Adds the rule with left hand side ``lhop`` and right hand side ``rhop`` to the rules,
   after checking that ``lhop`` and ``rhop`` consist entirely of letters in the
   alphabet of ``p`` (see :py:meth:`Presentation.validate_rules`).

   :param p: the presentation
   :type p: Presentation
   :param lhop: the left hand side of the rule
   :type lhop: str or List[int]
   :param rhop: the right hand side of the rule
   :type rhop: str or List[int]

   :returns: None


.. py:function:: add_rule(p: Presentation, lhop: Union[str, List[int]], rhop: Union[str, List[int]]) -> None

   Add a rule to the presentation.

   Adds the rule with left hand side ``lhop`` and right hand side ``rhop``
   to the rules.

   :param p: the presentation
   :type p: Presentation
   :param lhop: the left hand side of the rule
   :type lhop: str or List[int]
   :param rhop: the right hand side of the rule
   :type rhop: str or List[int]

   :returns: None

   :warning:
     No checks that the arguments describe words over the alphabet of the
     presentation are performed.

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("ab")
      p.rules  # []
      presentation.add_rule(p, "ab", "baa")
      p.rules  # ['ab', 'baa']
      presentation.add_rule(p, "aaa", "a")
      p.rules  # ['ab', 'baa', 'aaa', 'a']


.. py:function:: add_rules(p: Presentation, q: Presentation) -> None

   Add all the rules from one presentation to another presentation.

   Adds all the rules of the second argument to the first argument, which is modified in-place.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param q: the presentation with the rules to add
   :type q: Presentation

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("ab")
      presentation.add_rule(p, "ab", "baa")
      presentation.add_rule(p, "aaa", "a")
      p.rules  # ['ab', 'baa', 'aaa', 'a']
      q = Presentation("ab")
      q.add_rule("bbbb", "b")
      q.rules  # ['bbbb', 'b']
      presentation.add_rules(p, q)
      p.rules  # ['ab', 'baa', 'aaa', 'a', 'bbbb', 'b']
      q.rules  # ['bbbb', 'b']


.. py:function:: add_zero_rules(p: Presentation, z: Union[str, int]) -> None

   Add rules for a zero element.

   Adds rules of the form :math:`a z = z a = z` for every letter :math:`a` in
   the alphabet of ``p``, where :math:`z` is the second parameter.

   :param p: the presentation to add rules to
   :type p: Presentation
   :param z: the zero element
   :type z: str or int

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("abc")
      presentation.add_zero_rules(p, "c")
      p.rules  # ['ac', 'c', 'ca', 'c', 'bc', 'c', 'cb', 'c', 'cc', 'c']


.. py:function:: are_rules_sorted(p: Presentation) -> None

   Check if the rules :math:`u_1 = v_1, \ldots, u_n = v_n` satisfy :math:`u_1
   v_1 < \cdots < u_n v_n`, where :math:`<` is the shortlex order.

   :param p: the presentation to check
   :type p: Presentation

   :returns: ``True`` if the rules are sorted, and ``False`` if not.


.. py:function:: change_alphabet(p: Presentation) -> None

   Change or re-order the alphabet.

   This function replaces ``p.alphabet()`` with ``new_alphabet``, where
   possible, and re-writes the rules in the presentation using the new
   alphabet.


   :param p: the presentation
   :type p: Presentation
   :param new_alphabet: the replacement alphabet
   :type new_alphabet: Union[str, List[int]]

   :returns: None.

   :raises RuntimeError:
     if the size of ``p.alphabet()`` and ``new_alphabet`` do not agree.


.. py:function:: character(i: int) -> str

   Return a possible character by index.

   This function returns the ``i``-th letter in the alphabet consisting of all
   possible characters. This function exists so that visible ASCII characters
   occur before invisible ones, so that when manipulating presentations over
   strings the human readable characters are used before non-readable
   ones.

   :param i: the index
   :type i: int
   :returns: A ``str``.

   :raises RuntimeError:
     if ``i`` exceeds the number of letters in supported by ``str``.

   .. seealso:: :py:func:`letter`


.. py:function:: length(p: Presentation) -> int

   Return the sum of the lengths of the rules.

   :param p: the presentation
   :type p: Presentation

   :returns: An ``int``.


.. py:function:: letter(p: Presentation, i: int) -> int

    Return a possible letter by index.

    This function returns the ``i``-th letter in the alphabet consisting of
    all possible letters of type Presentation.letter_type. This function exists
    so that visible ASCII characters occur before invisible ones, so that when
    manipulating presentations over strings the human readable characters are
    used before non-readable ones.

    :param p: a presentation
    :type p: Presentation
    :param i: the index
    :type i: int

    :returns: A ``str``.

    :raises RuntimeError:
      if ``i`` exceeds the number of letters in supported by ``str``.

    .. seealso:: :py:func:`character`


.. py:function:: longest_common_subword(p: Presentation) -> Union[str, List[int]]

   Return the longest common subword of the rules.

   If it is possible to find a subword :math:`w` of the rules :math:`u_1 = v_1,
   \ldots, u_n = v_n` such that the introduction of a new generator :math:`z`
   and the relation :math:`z = w` reduces the length (see :py:func:`length`) of
   the presentation, then this function returns the word :math:`w`. If no such
   word can be found, a word of length :math:`0` is returned.

   :param p: the presentation
   :type p: Presentation

   :returns: str or List[int]


.. py:function:: make(p: Presentation) -> Presentation

   Converts a presentation over strings to one over lists of integers or vice
   versa.

   :param p: the presentation
   :type p: Presentation

   :returns: A ``Presentation``.


.. py:function:: make(S: FroidurePin) -> Presentation
   :noindex:

   Returns a presentation defining a semigroup isomorphic to that represented
   by a ``FroidurePin`` instance.

   :param S: the ``FroidurePin`` instance.
   :type S: FroidurePin

   :returns: A ``Presentation``.


.. py:function:: normalize_alphabet(p: Presentation) -> None

   Modify the presentation so that the alphabet is :math:`\{0, \ldots, n - 1\}`
   (or equivalent), and rewrites the rules to use this alphabet.

   If the alphabet is already normalized, then no changes are made to the
   presentation.

   :param p: the presentation
   :type p: Presentation

   :returns: None


.. py:function:: reduce_complements(p: Presentation) -> None

   If there are rules :math:`u = v` and :math:`v = w` where :math:`\lvert w
   \rvert < \lvert v \rvert`, then replace :math:`u = v` with :math:`u = w`.

   Attempts to reduce the length of the words by finding the equivalence
   relation on the relation words generated by the pairs of identical relation
   words. If :math:`\{u_1, u_2, \ldots, u_n\}` are distinct words in an
   equivalence class and :math:`u_1` is the shortlex minimum word in the class,
   then the relation words are replaced by :math:`u_1 = u_2, u_1 = u_3, \ldots,
   u_1 = u_n`.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("a")
      presentation.add_rule(p, "aaaaa", "aaa")
      presentation.add_rule(p, "aaa", "a")
      p.rules  # ['aaaaa', 'aaa', 'aaa', 'a']
      presentation.reduce_complements(p)
      p.rules  # ['a', 'aaa', 'a', 'aaaaa']


.. py:function:: redundant_rule(p: Presentation, t: datetime.timedelta) -> int

   Return the index of the the left hand side of a redundant rule.

   Starting with the last rule in the presentation, this function attempts to
   run the Knuth-Bendix algorithm on the rules of the presentation except for
   the given omitted rule. For every such omitted rule, Knuth-Bendix is run for
   the length of time indicated by the second parameter ``t`` and then it is
   checked if the omitted rule can be shown to be redundant (rewriting both
   sides of the omitted rule using the other rules using the output of the, not
   necessarily finished, Knuth-Bendix algorithm).

   If the omitted rule can be shown to be redundant in this way, then the index
   of its left hand side is returned.

   If no rule can be shown to be redundant in this way, then ``len(p.rules)``
   is returned.

   :warning:
     The progress of the Knuth-Bendix algorithm may differ between different
     calls to this function even if the parameters are identical. As such this
     is non-deterministic, and may produce different results with the same
     input.

   :param p: the presentation.
   :type p: Presentation
   :param t: time to run KnuthBendix for every omitted rule
   :type t: datetime.timedelta

   :return: The index of a redundant rule (if any).

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      from datetime import timedelta
      p = Presentation("ab")
      presentation.add_rule(p, "ab", "ba")
      presentation.add_rule(p, "bab", "abb")
      t = timedelta(seconds = 1)
      p.rules  # ['ab', 'ba', 'bab', 'abb']
      presentation.redundant_rule(p, t)  # 2


.. py:function:: remove_duplicate_rules(p: Presentation) -> None

   Remove duplicate rules.

   Removes all but one instance of any duplicate rules (if any). Note that
   rules of the form :math:`u = v` and :math:`v = u` (if any) are considered
   duplicates. Also note that the rules may be reordered by this function even
   if there are no duplicate rules.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation("ab")
      presentation.add_rule(p, "ab", "baa")
      presentation.add_rule(p, "baa", "ab")
      p.rules  # ['ab', 'baa', 'baa', 'ab']
      presentation.remove_duplicate_rules(p)
      p.rules  # ['ab', 'baa']


.. py:function:: remove_redundant_generators(p: Presentation) -> None

   Remove any trivially redundant generators.

   If one side of any of the rules in the presentation ``p`` is a letter
   ``a`` and the other side of the rule does not contain ``a``, then this
   function replaces every occurrence of ``a`` in every rule by the other
   side of the rule. This substitution is performed for every such
   rule in the presentation; and the trivial rules (with both sides being
   identical) are removed. If both sides of a rule are letters, then the
   greater letter is replaced by the lesser one.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   :raises RuntimeError: if ``len(p.rules)`` is odd.


.. py:function:: remove_trivial_rules(p: Presentation) -> None

   Remove rules consisting of identical words.

   Removes all instance of rules (if any) where the left hand side and the
   right hand side are identical.

   :param p: the presentation
   :type p: Presentation

   :returns: None

   :raises RuntimeError: if ``len(p.rules)`` is odd.


.. py:function:: replace_subword(p: Presentation, existing: Union[str, List[int]], replacement: Union[str, List[int]]) -> None
   :noindex:

   Replace non-overlapping instances of a subword by another word.

   If ``existing`` and ``replacement`` are words, then this function replaces
   every non-overlapping instance of ``existing`` in every rule by
   ``replacement``. The presentation ``p`` is changed in-place.

   :param p: the presentation
   :type p: Presentation
   :param existing: the word to be replaced
   :type existing: str or List[int]
   :param replacement: the replacement word.
   :type replacement: str or List[int]

   :returns: None

   :raises RuntimeError: if ``existing`` is empty.


.. py:function:: replace_subword(p: Presentation, w: Union[str, List[int]]) -> None

   Replace non-overlapping instances of a subword.

   A new generator :math:`z` is added to the presentation, along with the rule
   :math:`w = z`. Each (if any) non-overlapping instance (from left to right)
   of the word :math:`w` in every rule of the presentation is replaced with
   :math:`z`.

   :param p: the presentation
   :type p: Presentation
   :param w: the word to be replaced by a new generator
   :type w: str or List[int]

   :returns: None

   **Example**::

      from libsemigroups_pybind11 import presentation, Presentation
      p = Presentation([0, 1])
      presentation.add_rule(p, [1, 0, 0, 1, 0], [0, 1, 0, 0, 1])
      p.rules  # [[1, 0, 0, 1, 0], [0, 1, 0, 0, 1]]
      presentation.replace_subword(p, [0, 0, 1])
      p.rules  # [[1, 2, 0], [0, 1, 2], [2], [0, 0, 1]]


.. py:function:: replace_word(p: Presentation, existing: Union[str, List[int]], replacement: Union[str, List[int]]) -> None

   Replace instances of a word occupying either side of a rule.

   If ``existing`` and ``replacement`` are words, then this function replaces
   every instance of ``existing`` in every rule of the form
   ``existing`` :math:`= w` or :math:`w =` ``existing``, with the word
   ``replacement``. The presentation ``p`` is changed in-place.

   :param p: the presentation
   :type p: Presentation
   :param existing: the word to be replaced
   :type existing: str or List[int]
   :param replacement: the replacement word
   :type replacement: str or List[int]

   :returns: None

.. py:function:: reverse(p: Presentation) -> None

   Reverse every rule.

   :param p: the presentation
   :type p: Presentation

   :returns: None

.. py:function:: sort_each_rule(p: Presentation) -> None

   Sort each rule :math:`u = v` so that the left hand side is shortlex greater
   than the right hand side.

   :param p: the presentation
   :type p: Presentation

   :returns: None

.. py:function:: sort_rules(p: Presentation) -> None

   Sort the rules :math:`u_1 = v_1, \ldots, u_n = v_n` so that
   :math:`u_1 < \cdots < u_n`, where :math:`<` is the shortlex order.

   :param p: the presentation
   :type p: Presentation

   :returns: None
