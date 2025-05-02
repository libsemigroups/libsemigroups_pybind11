# -*- coding: utf-8 -*-

# Copyright (c) 2025 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# The module doc string is what appears at the top of the helper function
# doc page, and so is omitted.

"""Subpackage for importing is_obviously_infinite, and wrapping it."""

from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    is_obviously_infinite as _is_obviously_infinite,
)

from .detail.cxx_wrapper import wrap_cxx_free_fn as _wrap_cxx_free_fn


is_obviously_infinite = _wrap_cxx_free_fn(_is_obviously_infinite)
