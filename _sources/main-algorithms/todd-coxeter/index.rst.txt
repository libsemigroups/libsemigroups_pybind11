..
    Copyright (c) 2021-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Todd-Coxeter
============

This page contains documentation related to the implementation of the
Todd-Coxeter algorithm :cite:`Coleman2022aa` in libsemigroups_.

The purpose of this algorithm is to find the :any:`WordGraph` of the action of a
semigroup or monoid on the classes of a 1-sided (right), or 2-sided congruence;
see :cite:`Coleman2022aa` for more details.

Similar to other places in libsemigroups_ it is possible to create a
:any:`ToddCoxeter` object from a presentation consisting of strings or lists of
integers.


.. toctree::
    :maxdepth: 1

    class/index
    todd-coxeter.options
    helpers
    to-todd-coxeter
