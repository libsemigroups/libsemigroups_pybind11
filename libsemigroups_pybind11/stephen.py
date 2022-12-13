# -*- coding: utf-8 -*-

# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module, invalid-name, unused-import, fixme

"""
This package provides the user-facing python part of libsemigroups_pybind11 for
the stephen namespace from libsemigroups.
"""

from _libsemigroups_pybind11 import (accepts, is_left_factor, left_factors,
                                     number_of_left_factors,
                                     number_of_words_accepted, words_accepted)
