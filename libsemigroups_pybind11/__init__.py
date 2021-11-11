# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module

"""
This package provides a the user-facing python part of libsemigroups_pybind11
"""

from _libsemigroups_pybind11 import (
    ToddCoxeter,
    KnuthBendix,
    Bipartition,
    PBR,
    Congruence,
    FpSemigroup,
    ActionDigraph,
    congruence_kind,
    ReportGuard,
    add_cycle,
    is_acyclic,
    topological_sort,
    UNDEFINED,
    POSITIVE_INFINITY,
    silo,
    sislo,
    wilo,
    wislo,
    number_of_words,
    tril,
    follow_path,
)

from .transf import Transf
from .matrix import Matrix, MatrixKind, make_identity
from .froidure_pin import FroidurePin
from .tools import libsemigroups_version
