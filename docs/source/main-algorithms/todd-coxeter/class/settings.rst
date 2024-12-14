.. Copyright (c) 2024 J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: _libsemigroups_pybind11

Settings
========

This page contains information about the member functions of the
:any:`ToddCoxeterWord` that control various settings that influence the congruence
enumeration process.

There are a fairly large number of settings, they can profoundly alter the run
time of a congruence enumeration process, but it is hard to predict what
settings will work best for any particular input.

See also :any:`Runner` for further settings.

.. automethod:: ToddCoxeterWord.def_max
.. automethod:: ToddCoxeterWord.def_policy
.. automethod:: ToddCoxeterWord.def_version
.. automethod:: ToddCoxeterWord.f_defs
.. automethod:: ToddCoxeterWord.hlt_defs
.. automethod:: ToddCoxeterWord.large_collapse
.. automethod:: ToddCoxeterWord.lookahead_extent
.. automethod:: ToddCoxeterWord.lookahead_growth_factor
.. automethod:: ToddCoxeterWord.lookahead_growth_threshold
.. automethod:: ToddCoxeterWord.lookahead_min
.. automethod:: ToddCoxeterWord.lookahead_next
.. automethod:: ToddCoxeterWord.lookahead_stop_early_interval
.. automethod:: ToddCoxeterWord.lookahead_stop_early_ratio
.. automethod:: ToddCoxeterWord.lookahead_style
.. automethod:: ToddCoxeterWord.lower_bound
.. automethod:: ToddCoxeterWord.save
.. automethod:: ToddCoxeterWord.strategy
.. automethod:: ToddCoxeterWord.use_relations_in_extra

