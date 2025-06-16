..
    Copyright (c) 2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Kambites class
==================

.. autoclass:: Kambites
    :doc-only:

Contents
--------

.. autosummary::
    :signatures: short

    ~Kambites
    Kambites.add_generating_pair
    Kambites.contains
    Kambites.copy
    Kambites.currently_contains
    Kambites.generating_pairs
    Kambites.init
    Kambites.kind
    Kambites.number_of_classes
    Kambites.number_of_generating_pairs
    Kambites.presentation
    Kambites.reduce
    Kambites.reduce_no_run
    Kambites.small_overlap_class
    Kambites.ukkonen

Full API
--------

.. autoclass:: Kambites
    :class-doc-from: init
    :members:
    :exclude-members: 
        current_state, dead, finished, internal_generating_pairs,
        kill, last_report, report, report_every, report_prefix,
        report_why_we_stopped, reset_last_report, reset_start_time, run, run_for,
        run_until, running, running_for, running_until, start_time, started, state,
        stopped, stopped_by_predicate, success, timed_out
