..
    Copyright (c) 2021-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The KnuthBendix class
=====================

.. autoclass:: KnuthBendix
    :doc-only:

Contents
--------

.. autosummary::
    :signatures: short

    ~KnuthBendix
    KnuthBendix.active_rules
    KnuthBendix.add_generating_pair
    KnuthBendix.check_confluence_interval
    KnuthBendix.confluent
    KnuthBendix.confluent_known
    KnuthBendix.contains
    KnuthBendix.copy
    KnuthBendix.currently_contains
    KnuthBendix.generating_pairs
    KnuthBendix.gilman_graph
    KnuthBendix.gilman_graph_node_labels
    KnuthBendix.init
    KnuthBendix.kind
    KnuthBendix.max_overlap
    KnuthBendix.max_pending_rules
    KnuthBendix.max_rules
    KnuthBendix.number_of_active_rules
    KnuthBendix.number_of_classes
    KnuthBendix.number_of_generating_pairs
    KnuthBendix.number_of_inactive_rules
    KnuthBendix.overlap_policy
    KnuthBendix.presentation
    KnuthBendix.reduce
    KnuthBendix.reduce_no_run
    KnuthBendix.total_rules

Full API
--------

.. autoclass:: KnuthBendix
    :class-doc-from: init
    :members:
    :exclude-members: 
        options, run_for, run_until, run, finished, stopped, current_state, dead,
        internal_generating_pairs, internal_presentation, kill, last_report, report, report_every,
        report_prefix, report_why_we_stopped, reset_last_report, reset_start_time,
        running, running_for, running_until, start_time, started, state,
        stopped_by_predicate, success, timed_out
