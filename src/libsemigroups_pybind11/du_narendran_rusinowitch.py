# Copyright (c) 2026 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Subpackage for importing and wrapping :any:`du_narendran_rusinowitch`."""

from _libsemigroups_pybind11 import du_narendran_rusinowitch as _du_narendran_rusinowitch

from .detail.cxx_wrapper import wrap_cxx_free_fn as _wrap_cxx_free_fn

du_narendran_rusinowitch = _wrap_cxx_free_fn(_du_narendran_rusinowitch)

__all__ = ["du_narendran_rusinowitch"]
