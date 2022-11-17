# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for Congruence from libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name
from datetime import timedelta
from libsemigroups_pybind11 import (
    Congruence,
    FpSemigroup,
    ReportGuard,
    congruence_kind,
)


def test_018():
    ReportGuard(False)
    twosided = congruence_kind.twosided
    S = FpSemigroup()
    S.set_alphabet(3)
    S.add_rule([0, 0], [0, 0])
    S.add_rule([0, 1], [1, 0])
    S.add_rule([0, 2], [2, 0])
    S.add_rule([0, 0], [0])
    S.add_rule([0, 2], [0])
    S.add_rule([2, 0], [0])
    S.add_rule([1, 0], [0, 1])
    S.add_rule([1, 1], [1, 1])
    S.add_rule([1, 2], [2, 1])
    S.add_rule([1, 1, 1], [1])
    S.add_rule([1, 2], [1])
    S.add_rule([2, 1], [1])

    assert S.is_obviously_infinite()

    cong = Congruence(twosided, S)
    cong.add_pair([0], [1])
    assert not cong.finished()
    assert cong.number_of_non_trivial_classes() == 1
    assert cong.finished()
    assert cong.non_trivial_classes(0) == [[0], [1], [0, 1], [1, 1], [0, 1, 1]]
    # The next line does nothing except check that it's possible to call
    # `run_for` with a timedelta
    cong.run_for(timedelta(seconds=1))
