..
    Copyright (c) 2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Settings
========

This page contains information about the member functions of the
:any:`ToddCoxeter` that control various settings that influence the congruence
enumeration process.

There are a fairly large number of settings, they can profoundly alter the run
time of a congruence enumeration process, but it is hard to predict what
settings will work best for any particular input.

See also :any:`Runner` for further settings.

.. automethod:: ToddCoxeter.def_max

.. automethod:: ToddCoxeter.def_policy

.. automethod:: ToddCoxeter.def_version

.. automethod:: ToddCoxeter.f_defs

.. automethod:: ToddCoxeter.hlt_defs

.. automethod:: ToddCoxeter.large_collapse

.. automethod:: ToddCoxeter.lookahead_extent

.. automethod:: ToddCoxeter.lookahead_growth_factor

.. automethod:: ToddCoxeter.lookahead_growth_threshold

.. automethod:: ToddCoxeter.lookahead_min

.. automethod:: ToddCoxeter.lookahead_next

.. automethod:: ToddCoxeter.lookahead_stop_early_interval

.. automethod:: ToddCoxeter.lookahead_stop_early_ratio

.. automethod:: ToddCoxeter.lookahead_style

.. automethod:: ToddCoxeter.lower_bound

.. automethod:: ToddCoxeter.save

.. automethod:: ToddCoxeter.strategy

.. automethod:: ToddCoxeter.use_relations_in_extra
