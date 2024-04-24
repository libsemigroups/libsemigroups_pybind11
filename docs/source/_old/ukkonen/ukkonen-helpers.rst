.. Copyright (c) 2023, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Ukkonen helper functions
========================

This page contains the documentation for various helper functions for
manipulating ``Ukkonen`` objects. All such functions are contained in the
submodule ``libsemigroups_pybind11.ukkonen``.

Contents
--------

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:func:`add_words_no_checks`
     - Add all words in a ``list`` to a :py:class:`Ukkonen` object.
   * - :py:func:`add_words`
     - Add all words in a ``list`` to a :py:class:`Ukkonen` object.
   * - :py:func:`dot`
     - Returns a string containing a `GraphViz <https://graphviz.org>`_
       representation of a suffix tree.
   * - :py:func:`is_piece_no_checks`
     - Check if a word is a piece.
   * - :py:func:`is_piece`
     - Check if a word is a piece.
   * - :py:func:`is_subword_no_checks`
     - Check if a word is a subword of any word in a suffix tree.
   * - :py:func:`is_subword`
     - Check if a word is a subword of any word in a suffix tree.
   * - :py:func:`is_suffix_no_checks`
     - Check if a word is a suffix of any word in a suffix tree.
   * - :py:func:`is_suffix`
     - Check if a word is a suffix of any word in a suffix tree.
   * - :py:func:`maximal_piece_prefix_no_checks`
     - Find the maximal piece prefix of a word.
   * - :py:func:`maximal_piece_prefix`
     - Find the maximal piece prefix of a word.
   * - :py:func:`maximal_piece_suffix_no_checks`
     - Find the maximal piece suffix of a word.
   * - :py:func:`maximal_piece_suffix`
     - Find the maximal piece suffix of a word.
   * - :py:func:`number_of_distinct_subwords`
     - Returns the number of distinct subwords of the words in a suffix tree.
   * - :py:func:`number_of_pieces_no_checks`
     - Find the number of pieces in a decomposition of a word (if any).
   * - :py:func:`number_of_pieces`
     - Find the number of pieces in a decomposition of a word (if any).
   * - :py:func:`pieces_no_checks`
     - Find the pieces in a decomposition of a word (if any).
   * - :py:func:`pieces`
     - Find the pieces in a decomposition of a word (if any).

Full API
--------

.. .. autofunction:: add_words_no_checks
.. .. autofunction:: add_words
.. .. autofunction:: libsemigroups_pybind11.ukkonen.dot
.. .. autofunction:: is_piece_no_checks
.. .. autofunction:: is_piece
.. .. autofunction:: is_subword_no_checks
.. .. autofunction:: is_subword
.. .. autofunction:: is_suffix_no_checks
.. .. autofunction:: is_suffix
.. .. autofunction:: maximal_piece_prefix_no_checks
.. .. autofunction:: maximal_piece_prefix
.. .. autofunction:: maximal_piece_suffix_no_checks
.. .. autofunction:: maximal_piece_suffix
.. .. autofunction:: number_of_distinct_subwords
.. .. autofunction:: number_of_pieces_no_checks
.. .. autofunction:: number_of_pieces
.. .. autofunction:: pieces_no_checks
.. .. autofunction:: pieces