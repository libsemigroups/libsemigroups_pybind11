# -*- coding: utf-8 -*-

# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This page contains the documentation for various helper classes and functions
for manipulating :any:`Forest` objects. All such classes and functions are
contained in the submodule ``forest``.
"""

from typing_extensions import Self as _Self

from _libsemigroups_pybind11 import (  # pylint: disable= unused-import
    forest_depth as depth,
    forest_dot as dot,
    forest_is_root as is_root,
    forest_max_label as max_label,
    forest_path_from_root as path_from_root,
    forest_path_to_root as path_to_root,
    forest_PathsFromRoots as PathsFromRoots,
    forest_PathsToRoots as PathsToRoots,
)

# The following fools sphinx into thinking that PathsToRoots is not an
# aliases.
PathsFromRoots.__module__ = __name__
PathsFromRoots.__name__ = "PathsFromRoots"
PathsToRoots.__module__ = __name__
PathsToRoots.__name__ = "PathsToRoots"
