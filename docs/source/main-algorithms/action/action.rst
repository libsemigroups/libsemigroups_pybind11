.. Copyright (c) 2024, James D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

The Action class
================

.. autoclass:: Action
   :doc-only:
   :class-doc-from: class

Contents
--------

.. autosummary::
    :signatures: short
    
    ~Action
    Action.add_generator
    Action.add_seed
    Action.cache_scc_multipliers
    Action.current_size
    Action.empty
    Action.init
    Action.multiplier_from_scc_root
    Action.multiplier_to_scc_root
    Action.position
    Action.reserve
    Action.root_of_scc
    Action.scc
    Action.size
    Action.word_graph

Full API
--------

.. autoclass:: Action
    :members:
    :class-doc-from: init
    :exclude-members: 
        current_state, dead, finished, kill,
        last_report, report, report_every, report_prefix, report_why_we_stopped,
        reset_last_report, reset_start_time, run, run_for, run_until, running,
        running_for, running_until, start_time, started, state, stopped,
        stopped_by_predicate, success, timed_out
