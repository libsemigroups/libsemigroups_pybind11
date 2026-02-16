..
    Copyright (c) 2024-2026 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Modifiers
=========

This page contains documentation of the member functions of :any:`ToddCoxeter`
that can be used to modify the state of a :any:`ToddCoxeter` instance. In other
words, for modifying the :any:`WordGraph` that is the output of the algorithm in
a way that preserves it up to isomorphism.

Contents
--------

.. autosummary::
   :signatures: short

   ToddCoxeter.perform_lookahead
   ToddCoxeter.perform_lookahead_for
   ToddCoxeter.perform_lookahead_until
   ToddCoxeter.perform_lookbehind
   ToddCoxeter.perform_lookbehind_no_checks
   ToddCoxeter.perform_lookbehind_for
   ToddCoxeter.perform_lookbehind_for_no_checks
   ToddCoxeter.perform_lookbehind_until
   ToddCoxeter.perform_lookbehind_until_no_checks
   ToddCoxeter.shrink_to_fit
   ToddCoxeter.standardize

Full API
--------

.. automethod:: ToddCoxeter.perform_lookahead

.. automethod:: ToddCoxeter.perform_lookahead_for

.. automethod:: ToddCoxeter.perform_lookahead_until

.. automethod:: ToddCoxeter.perform_lookbehind

.. automethod:: ToddCoxeter.perform_lookbehind_no_checks

.. automethod:: ToddCoxeter.perform_lookbehind_for

.. automethod:: ToddCoxeter.perform_lookbehind_for_no_checks

.. automethod:: ToddCoxeter.perform_lookbehind_until

.. automethod:: ToddCoxeter.perform_lookbehind_until_no_checks

.. automethod:: ToddCoxeter.shrink_to_fit

.. automethod:: ToddCoxeter.standardize
