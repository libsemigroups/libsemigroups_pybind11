# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name, redefined-outer-name

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

import pytest
from copy import copy

from libsemigroups_pybind11 import (
    Perm,
    LibsemigroupsError,
    SchreierSims,
    ReportGuard,
)


def check_constructors(gens):
    ReportGuard(False)
    # default constructor
    with pytest.raises(ValueError):
        SchreierSims()

    S1 = SchreierSims(gens)

    # copy constructor
    S2 = copy(S1)

    assert S1 is not S2
    assert S1.number_of_generators() == S2.number_of_generators()
    assert S1.current_size() == S2.current_size()
    assert S1.finished() == S2.finished()


def check_generators(gens):
    ReportGuard(False)
    S = SchreierSims(gens)
    for i, gen in enumerate(gens):
        assert S.generator(i) == gen

    with pytest.raises(LibsemigroupsError):
        S.generator(len(gens))

    U = SchreierSims(gens[0])
    for x in gens[1:]:
        U.add_generator(x)
    assert S.number_of_generators() == U.number_of_generators()
    assert S.size() == U.size()


@pytest.fixture
def checks_for_SchreierSims():
    return (
        # check_constructors,
        check_generators,
    )


# def test_SchreierSims_001(checks_for_generators):
#     gens = [
#         Perm([1, 0, 2, 3, 4] + list(range(5, 255))),
#         Perm([1, 2, 3, 4, 0] + list(range(5, 255))),
#     ]
#     for check in checks_for_generators:
#         check(gens)

# def test_SchreierSims_002(checks_for_generators):
#     gens = [
#         Perm([1, 0, 2, 3, 4] + list(range(5, 511))),
#         Perm([1, 2, 3, 4, 0] + list(range(5, 511))),
#     ]
#     for check in checks_for_generators:
#         check(gens)


def test_SchreierSims_002(checks_for_SchreierSims):
    gens = [
        Perm([1, 0, 2, 3, 4] + list(range(5, 255))),
        Perm([1, 2, 3, 4, 0] + list(range(5, 255))),
    ]
    for check in checks_for_SchreierSims:
        check(gens)
