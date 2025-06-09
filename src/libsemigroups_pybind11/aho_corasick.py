# -*- coding: utf-8 -*-

# Copyright (c) 2024, Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains various helper functions for the class :any:`AhoCorasick`.
These functions could be functions of :any:`AhoCorasick` but they only use
public member functions of :any:`AhoCorasick`, and so they are declared
as free functions instead.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import
    aho_corasick_add_word as add_word,
    aho_corasick_rm_word as rm_word,
    aho_corasick_traverse_word as traverse_word,
    aho_corasick_dot as dot,
)
