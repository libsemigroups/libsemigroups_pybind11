# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``bipartition`` subpackage, that
contains helper functions for the :any:`Bipartition` class.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    bipartition_underlying_partition as underlying_partition,
    bipartition_one as one,
    Bipartition,
)
