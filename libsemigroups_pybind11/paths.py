# -*- coding: utf-8 -*-

# Copyright (c) 2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, missing-function-docstring, protected-access

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the libsemigroups::paths namespace from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    Paths,
)

from .detail.decorators import may_return_positive_infinity


Paths.count = may_return_positive_infinity(Paths.count)
Paths.max = may_return_positive_infinity(Paths.max)
