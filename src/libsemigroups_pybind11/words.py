# -*- coding: utf-8 -*-

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
#

"""
This page contains the documentation for the ``words`` subpackage, that
contains helper functions related to words.
"""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module,unused-import,redefined-builtin
    words_human_readable_index as human_readable_index,
    words_human_readable_letter as human_readable_letter,
    words_parse_relations as parse_relations,
    words_pow as pow,
    words_prod as prod,
)
