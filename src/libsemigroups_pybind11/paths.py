# -*- coding: utf-8 -*-

# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for the ``paths`` subpackage, that
contains helper functions for the :any:`Paths` class.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    paths_algorithm as algorithm,
    paths_number_of_paths_algorithm as number_of_paths_algorithm,
    paths_number_of_paths as number_of_paths,
)

# The following fools sphinx into thinking that "algorithm" is not an
# alias.
algorithm.__module__ = __name__
algorithm.__name__ = "algorithm"
