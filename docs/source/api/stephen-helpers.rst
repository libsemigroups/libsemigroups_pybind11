.. Copyright (c) 2022, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

   This file only exists because Breathe always displays all members when
   documenting a namespace, and this is nicer for now.

.. currentmodule:: _libsemigroups_pybind11

Stephen helpers
---------------

This page contains the documentation for various helper functions for
manipulating :py:class:`Stephen` objects. All such functions are contained in
``stephen`` module.

Contents
~~~~~~~~

.. list-table::
   :widths: 50 50
   :header-rows: 0

   * - :py:func:`accepts`
     - Check if a word is equivalent to :py:meth:`Stephen.word`.
   * - :py:func:`is_left_factor`
     - Check if a word is a left factor of :py:meth:`Stephen.word`.
   * - :py:meth:`left_factors`
     - Returns an iterator pointing at the first word (in short-lex order)
       that is a left factor of :py:meth:`Stephen.word`.
   * - :py:func:`number_of_left_factors`
     - Returns the number of left factors with length in given range.
   * - :py:func:`number_of_words_accepted`
     - Returns the number of words accepted with length in a given range.
   * - :py:func:`words_accepted`
     - Returns an iterator pointing at the first word equivalent to
       :py:meth:`Stephen.word` in short-lex order.

Full API
~~~~~~~~

.. .. autofunction:: accepts
.. .. autofunction:: is_left_factor
.. .. autofunction:: left_factors
.. .. autofunction:: number_of_left_factors
.. .. autofunction:: number_of_words_accepted
.. .. autofunction:: words_accepted
