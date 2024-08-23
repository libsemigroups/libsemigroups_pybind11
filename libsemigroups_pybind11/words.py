# -*- coding: utf-8 -*-

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
#
# pylint: disable=redefined-builtin, no-name-in-module, missing-function-docstring, unused-import


"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the libsemigroups::words namespace from libsemigroups.
"""

from _libsemigroups_pybind11 import (
    human_readable_index,
    human_readable_letter,
    parse_relations,
    pow,
    prod,
)
