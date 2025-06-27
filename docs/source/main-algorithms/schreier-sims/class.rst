..
    Copyright (c) 2023-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The SchreierSims class
======================

.. autoclass:: SchreierSims
    :doc-only:

Contents
--------

.. autosummary::
    :signatures: short

    ~SchreierSims
    SchreierSims.add_base_point
    SchreierSims.add_generator
    SchreierSims.base
    SchreierSims.base_size
    SchreierSims.contains
    SchreierSims.copy
    SchreierSims.current_size
    SchreierSims.currently_contains
    SchreierSims.empty
    SchreierSims.finished
    SchreierSims.generator
    SchreierSims.init
    SchreierSims.inverse_transversal_element
    SchreierSims.number_of_generators
    SchreierSims.number_of_strong_generators
    SchreierSims.one
    SchreierSims.orbit_lookup
    SchreierSims.run
    SchreierSims.sift
    SchreierSims.sift_inplace
    SchreierSims.size
    SchreierSims.strong_generator
    SchreierSims.transversal_element

Full API
--------

.. autoclass:: SchreierSims
    :class-doc-from: init
    :members:
    :exclude-members: 
        current_state, dead, internal_generating_pairs,
        kill, last_report, report, report_every, report_prefix,
        report_why_we_stopped, reset_last_report, reset_start_time, run_for,
        run_until, running, running_for, running_until, start_time, started, state,
        stopped, stopped_by_predicate, success, timed_out
