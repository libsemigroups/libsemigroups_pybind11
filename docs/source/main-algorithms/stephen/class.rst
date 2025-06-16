..
    Copyright (c) 2022-2025 R. Cirpons, J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Stephen class
=================

.. autoclass:: Stephen
    :doc-only:

Contents
--------

.. autosummary::
    :signatures: short

    ~Stephen
    Stephen.accept_state
    Stephen.copy
    Stephen.init
    Stephen.initial_state
    Stephen.is_word_set
    Stephen.presentation
    Stephen.set_word
    Stephen.word
    Stephen.word_graph

Full API
--------

.. autoclass:: Stephen
    :class-doc-from: init
    :members:
    :exclude-members: 
        current_state, dead, finished, internal_generating_pairs,
        kill, last_report, report, report_every, report_prefix,
        report_why_we_stopped, reset_last_report, reset_start_time, run, run_for,
        run_until, running, running_for, running_until, start_time, started, state,
        stopped, stopped_by_predicate, success, timed_out
