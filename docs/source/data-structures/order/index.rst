..
    Copyright (c) 2024 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Orders
======

This page contains the documentation for classes and several functions for
comparing words (i.e. lists of integers ``list[int]`` or strings ``str``) with
respect to certain reduction orderings.

Some of the orders are generalisations of others. In particular,
when the weight of every generator is the same:

  - :any:`len_wt_lex_cmp` is a generalisation of :any:`lenlex_cmp`;
  - :any:`rev_len_wt_lex_cmp` is a generalisation of :any:`rev_lenlex_cmp`;
  - :any:`wt_lenlex_cmp` is a generalisation of :any:`lenlex_cmp`; and
  - :any:`rev_wt_lenlex_cmp` is a generalisation of :any:`rev_lenlex_cmp`;
  - :any:`wt_lex_cmp` is a generalisation of :any:`lex_cmp`; and
  - :any:`rev_wt_lex_cmp` is a generalisation of :any:`rev_lex_cmp`.

Additionally:

  - :any:`wr_cmp` is a generalisation of :any:`lenlex_cmp` when all
    of the generators have the same level;
  - :any:`rev_wr_cmp` is a generalisation of :any:`rev_lenlex_cmp` when all
    of the generators have the same level;
  - :any:`wr_cmp` is a generalisation of :any:`rev_rpo_cmp` when all
    of the generators have a different level; and
  - :any:`rev_wr_cmp` is a generalisation of :any:`rpo_cmp` when all
    of the generators have a different level.

.. seealso::
    
    - :any:`Order`
    - :any:`Alphabet`

The classes and functions in |libsemigroups_pybind11| for comparing words
are described on the following pages:

.. toctree::
    :maxdepth: 1

    lex-cmp
    rev-lex-cmp
    lenlex-cmp
    rev-lenlex-cmp
    rpo-cmp
    rev-rpo-cmp
    wr-cmp
    rev-wr-cmp
    wt-lenlex-cmp
    rev-wt-lenlex-cmp
    wt-lex-cmp
    rev-wt-lex-cmp
    len-wt-lex-cmp
    rev-len-wt-lex-cmp
    functions
