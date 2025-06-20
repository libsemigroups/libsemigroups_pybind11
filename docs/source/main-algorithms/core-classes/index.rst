..
    Copyright (c) 2025 Joseph Edwards

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

Core classes
============

Many of the classes in ``libsemigroups_pybind11`` implement algorithms, and
hence are runnable. During the running of these algorithms, it is often
desirable to report the state of the algorithm. Therefore, the classes
:any:`Runner` and :any:`Reporter` exist to provide common functions to many
classes that implement the main algorithms.

Documentation for these, and associated, classes can be found on the following
pages. 

.. toctree::
    :maxdepth: 1

    delta
    reporter
    report-guard
    runner
    runner.state
