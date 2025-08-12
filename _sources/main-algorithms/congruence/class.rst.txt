..
    Copyright (c) 2025 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Congruence class
====================

.. autoclass:: Congruence
    :doc-only:

Contents
--------

.. autosummary::
    :signatures: short

    ~Congruence
    Congruence.add_generating_pair
    Congruence.contains
    Congruence.copy
    Congruence.currently_contains
    Congruence.generating_pairs
    Congruence.get
    Congruence.has
    Congruence.init
    Congruence.kind
    Congruence.max_threads
    Congruence.number_of_classes
    Congruence.number_of_generating_pairs
    Congruence.number_of_runners
    Congruence.presentation
    Congruence.reduce
    Congruence.reduce_no_run

Full API
--------

.. autoclass:: Congruence
    :class-doc-from: init
    :members:
    :exclude-members: 
        current_state, dead, finished, internal_generating_pairs,
        kill, last_report, report, report_every, report_prefix,
        report_why_we_stopped, reset_last_report, reset_start_time, run, run_for,
        run_until, running, running_for, running_until, start_time, started, state,
        stopped, stopped_by_predicate, success, timed_out
