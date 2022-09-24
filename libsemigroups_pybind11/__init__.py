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
    PBR,
    POSITIVE_INFINITY,
    UNDEFINED,
    ActionDigraph,
    Bipartition,
    Congruence,
    FpSemigroup,
    KnuthBendix,
    ReportGuard,
    ToddCoxeter,
    add_cycle,
    congruence_kind,
    follow_path,
    is_acyclic,
    number_of_words,
    silo,
    sislo,
    topological_sort,
    tril,
    wilo,
    wislo,
    Sims1,
)

from .froidure_pin import FroidurePin
from .matrix import Matrix, MatrixKind, make_identity
from .presentation import Presentation, redundant_rule
from .tools import libsemigroups_version
from .transf import Transf
