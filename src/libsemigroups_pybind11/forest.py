# -*- coding: utf-8 -*-

# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This page contains the documentation for various helper functions for
manipulating :any:`Forest` objects. All such functions are contained in
the submodule ``forest``.
"""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    forest_depth as depth,
    forest_dot as dot,
    forest_is_root as is_root,
    forest_path_to_root as path_to_root,
    forest_path_from_root as path_from_root,
    forest_max_label as max_label,
)
