# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Word-ordering comparison functions."""

from _libsemigroups_pybind11 import wr_cmp as _wr_cmp

from .detail.cxx_wrapper import wrap_cxx_free_fn as _wrap_cxx_free_fn

wr_cmp = _wrap_cxx_free_fn(_wr_cmp)

__all__ = ["wr_cmp"]
