# -*- coding: utf-8 -*-
# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Konieczny class.
"""

# pylint: disable=missing-function-docstring, invalid-name

from datetime import timedelta

import pytest
from libsemigroups_pybind11 import (
    Konieczny,
    PPerm,
    ReportGuard,
    Transf,
    Matrix,
    MatrixKind,
    BMat8,
)


def BMat(x):
    return Matrix(MatrixKind.Boolean, x)


###############################################################################
# Test functions begin
###############################################################################


def test_case_031():
    "transformations"
    ReportGuard(False)

    k = Konieczny(
        [
            Transf([1, 0, 2, 3, 4]),
            Transf([1, 2, 3, 4, 0]),
            Transf([0, 0, 2, 3, 4]),
        ]
    )
    assert k.size() == 3125

    val = sum(
        k.D_class_of_element(x.rep()).number_of_idempotents()
        for x in k.D_classes()
    )
    assert val == 196
    assert k.number_of_idempotents() == 196
    assert list(k.generators()) == [
        Transf([1, 0, 2, 3, 4]),
        Transf([1, 2, 3, 4, 0]),
        Transf([0, 0, 2, 3, 4]),
    ]


def test_case_032():
    "transformations - JDM favourite example"
    ReportGuard(False)
    S = Konieczny(
        [
            Transf([1, 7, 2, 6, 0, 4, 1, 5]),
            Transf([2, 4, 6, 1, 4, 5, 2, 7]),
            Transf([3, 0, 7, 2, 4, 6, 2, 4]),
            Transf([3, 2, 3, 4, 5, 3, 0, 1]),
            Transf([4, 3, 7, 7, 4, 5, 0, 4]),
            Transf([5, 6, 3, 0, 3, 0, 5, 1]),
            Transf([6, 0, 1, 1, 1, 6, 3, 4]),
            Transf([7, 7, 4, 0, 6, 4, 1, 7]),
        ]
    )
    assert S.size() == 597369
    val = sum(
        S.D_class_of_element(x.rep()).number_of_idempotents()
        for x in S.D_classes()
    )
    assert val == 8194
    assert S.number_of_idempotents() == 8194


def test_case_033():
    "transformations - large example"
    ReportGuard(False)
    gens = [
        Transf([2, 1, 0, 4, 2, 1, 1, 8, 0]),
        Transf([1, 7, 6, 2, 5, 1, 1, 4, 3]),
        Transf([1, 0, 7, 2, 1, 3, 1, 3, 7]),
        Transf([0, 3, 8, 1, 2, 8, 1, 7, 0]),
        Transf([0, 0, 0, 2, 7, 7, 5, 5, 3]),
    ]
    S = Konieczny(gens)

    for x in gens:
        assert S.contains(x)

    assert S.current_size() < 15000
    assert S.current_number_of_regular_elements() < 10000
    assert S.current_number_of_idempotents() < 500
    assert S.current_number_of_D_classes() < 2000
    assert S.current_number_of_L_classes() < 4000
    assert S.current_number_of_R_classes() < 6500

    assert S.size() == 232511
    assert S.current_number_of_D_classes() == 2122
    assert S.current_number_of_L_classes() == 8450
    assert S.current_number_of_R_classes() == 14706


def test_case_034():
    ReportGuard(False)
    S = Konieczny(
        [
            Transf([2, 1, 0, 4, 2, 1, 1, 8, 0]),
            Transf([1, 7, 6, 2, 5, 1, 1, 4, 3]),
            Transf([1, 0, 7, 2, 1, 3, 1, 3, 7]),
            Transf([0, 3, 8, 1, 2, 8, 1, 7, 0]),
            Transf([0, 0, 0, 2, 7, 7, 5, 5, 3]),
        ]
    )
    S.run_for(timedelta(milliseconds=50))
    assert S.size() == 232511


def test_case_035():
    "transformations - large example with run_until"
    ReportGuard(False)

    S = Konieczny(
        [
            Transf([2, 1, 0, 4, 2, 1, 1, 8, 0]),
            Transf([1, 7, 6, 2, 5, 1, 1, 4, 3]),
            Transf([1, 0, 7, 2, 1, 3, 1, 3, 7]),
            Transf([0, 3, 8, 1, 2, 8, 1, 7, 0]),
            Transf([0, 0, 0, 2, 7, 7, 5, 5, 3]),
        ]
    )
    S.run_until(lambda: S.current_number_of_D_classes() > 20)

    assert S.current_number_of_D_classes() > 20
    S.run()
    assert S.size() == 232511
    assert S.current_number_of_D_classes() == 2122


def test_case_038():
    "transformations: contains"
    ReportGuard(False)
    S = Konieczny(
        [
            Transf([1, 0, 2, 3, 4]),
            Transf([1, 2, 3, 4, 0]),
            Transf([0, 0, 2, 3, 4]),
        ]
    )
    assert S.contains(Transf([1, 0, 2, 3, 4]))
    assert S.contains(Transf([1, 2, 3, 4, 0]))
    assert S.contains(Transf([0, 0, 2, 3, 4]))
    assert not S.contains(Transf([1, 0, 2, 3, 4, 0]))
    assert not S.contains(Transf([1, 2, 3, 4, 0, 0]))
    assert not S.contains(Transf([0, 0, 2, 3, 4, 1]))

    with pytest.raises(RuntimeError):
        S.D_class_of_element(Transf([1, 0, 2, 3, 4, 0]))
    with pytest.raises(RuntimeError):
        S.D_class_of_element(Transf([1, 2, 3, 4, 0, 0]))
    with pytest.raises(RuntimeError):
        S.D_class_of_element(Transf([0, 0, 2, 3, 4, 0]))

    T = Konieczny([Transf([1, 0, 3, 4, 2]), Transf([0, 0, 2, 3, 4])])
    assert T.contains(Transf([1, 0, 2, 3, 4]))
    assert T.contains(Transf([0, 0, 2, 3, 4]))
    assert not T.contains(Transf([1, 2, 3, 4, 0]))
    assert not T.contains(Transf([1, 2, 3, 0, 4]))
    assert not T.contains(Transf([1, 2, 3, 4, 0, 5]))
    assert not T.contains(Transf([0, 2, 3, 4, 1]))

    with pytest.raises(RuntimeError):
        T.D_class_of_element(Transf([1, 2, 3, 4, 0]))
    with pytest.raises(RuntimeError):
        T.D_class_of_element(Transf([1, 2, 3, 4, 0, 5]))
    with pytest.raises(RuntimeError):
        T.D_class_of_element(Transf([0, 2, 3, 4, 1]))


def test_case_039():
    """transformations Hall monoid 5 (only the first couple of generators so that
    the test runs in short amount of time)"""

    ReportGuard(False)

    K = Konieczny(
        [
            Transf(
                [
                    0,
                    1,
                    2,
                    3,
                    4,
                    5,
                    6,
                    7,
                    16,
                    17,
                    18,
                    19,
                    20,
                    21,
                    22,
                    23,
                    8,
                    9,
                    10,
                    11,
                    12,
                    13,
                    14,
                    15,
                    24,
                    25,
                    26,
                    27,
                    28,
                    29,
                    30,
                ]
            ),
            Transf(
                [
                    0,
                    16,
                    8,
                    24,
                    4,
                    20,
                    12,
                    28,
                    2,
                    18,
                    10,
                    26,
                    6,
                    22,
                    14,
                    30,
                    0,
                    17,
                    8,
                    25,
                    4,
                    21,
                    12,
                    29,
                    2,
                    19,
                    10,
                    27,
                    6,
                    23,
                    14,
                ]
            ),
        ]
    )
    assert K.size() == 147


def test_case_040():
    ReportGuard(False)
    S = Konieczny(
        [
            Transf([2, 1, 0, 4, 2, 1, 1, 8, 0]),
            Transf([1, 7, 6, 2, 5, 1, 1, 4, 3]),
            Transf([1, 0, 7, 2, 1, 3, 1, 3, 7]),
            Transf([0, 3, 8, 1, 2, 8, 1, 7, 0]),
            Transf([0, 0, 0, 2, 7, 7, 5, 5, 3]),
        ]
    )
    S.run_until(lambda: S.current_number_of_regular_D_classes() > 2)

    assert S.current_number_of_regular_D_classes() < 5
    assert (
        S.current_number_of_D_classes() - S.number_of_regular_D_classes() < 2117
    )


def test_case_041():
    "current_number_D_classes"
    ReportGuard(False)
    S = Konieczny(
        [
            Transf([2, 1, 0, 4, 2, 1, 1, 8, 0]),
            Transf([1, 7, 6, 2, 5, 1, 1, 4, 3]),
            Transf([1, 0, 7, 2, 1, 3, 1, 3, 7]),
            Transf([0, 3, 8, 1, 2, 8, 1, 7, 0]),
            Transf([0, 0, 0, 2, 7, 7, 5, 5, 3]),
        ]
    )
    assert S.current_number_of_regular_D_classes() == 0
    assert S.current_number_of_D_classes() == 0
    S.run_until(lambda: S.current_number_of_regular_D_classes() > 2)
    S.run()
    assert S.current_number_of_regular_D_classes() == 5
    assert (
        S.current_number_of_D_classes() - S.number_of_regular_D_classes()
        == 2117
    )


def check_case_000(Mat):
    ReportGuard(False)
    gens = [
        Mat([[0, 1, 0, 1], [1, 0, 0, 0], [0, 1, 1, 1], [0, 1, 1, 0]]),
        Mat([[0, 1, 1, 1], [1, 1, 0, 0], [0, 0, 0, 0], [1, 1, 1, 1]]),
        Mat([[0, 1, 1, 0], [0, 1, 1, 0], [0, 1, 1, 1], [1, 1, 1, 1]]),
    ]
    S = Konieczny(gens)
    assert S.size() == 26


def test_case_000():
    check_case_000(BMat8)
    check_case_001(lambda x: Matrix(MatrixKind.Boolean, x))


def check_case_001(Mat):
    ReportGuard(False)
    gens = [
        Mat([[1, 0, 0, 0], [0, 0, 1, 0], [1, 0, 0, 1], [0, 1, 0, 0]]),
        Mat([[1, 0, 0, 1], [1, 0, 0, 1], [1, 1, 1, 1], [0, 1, 1, 0]]),
        Mat([[1, 0, 1, 0], [1, 0, 1, 1], [0, 0, 1, 1], [0, 1, 0, 1]]),
        Mat([[0, 0, 0, 0], [0, 1, 0, 1], [1, 1, 1, 0], [1, 0, 0, 1]]),
        Mat([[0, 0, 0, 1], [0, 0, 1, 0], [1, 0, 0, 1], [1, 1, 0, 0]]),
    ]

    S = Konieczny(gens)
    assert S.size() == 415


def test_case_001():
    check_case_001(BMat8)
    check_case_001(lambda x: Matrix(MatrixKind.Boolean, x))


def check_case_003(Mat):
    ReportGuard(False)
    gens = [
        Mat(
            [
                [0, 1, 1, 1, 0],
                [0, 0, 1, 0, 0],
                [1, 0, 0, 1, 0],
                [1, 1, 1, 0, 0],
                [0, 1, 1, 1, 1],
            ]
        ),
        Mat(
            [
                [0, 0, 0, 1, 0],
                [0, 0, 1, 0, 0],
                [1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0],
                [0, 1, 0, 1, 1],
            ]
        ),
        Mat(
            [
                [0, 0, 0, 1, 0],
                [1, 1, 0, 0, 0],
                [0, 0, 1, 1, 1],
                [1, 1, 0, 0, 1],
                [0, 0, 1, 1, 0],
            ]
        ),
        Mat(
            [
                [0, 1, 0, 0, 1],
                [0, 0, 1, 0, 1],
                [1, 0, 1, 0, 0],
                [0, 1, 1, 1, 0],
                [1, 0, 0, 0, 1],
            ]
        ),
    ]

    S = Konieczny(gens)
    assert S.size() == 513


def test_case_003():
    check_case_003(BMat8)
    check_case_003(lambda x: Matrix(MatrixKind.Boolean, x))


def test_case_028():
    "partial perm"
    ReportGuard(False)
    gens = [
        PPerm([0, 2, 3, 7], [1, 6, 7, 3], 9),
        PPerm([0, 1, 2, 3, 4, 7], [6, 5, 8, 0, 2, 1], 9),
        PPerm([0, 1, 2, 3, 4, 5, 6, 8], [1, 7, 2, 6, 0, 4, 8, 5], 9),
        PPerm([0, 1, 2, 3, 5, 6, 8], [2, 4, 6, 1, 5, 8, 7], 9),
        PPerm([0, 1, 2, 3, 5, 8], [7, 3, 6, 4, 2, 5], 9),
    ]
    S = Konieczny(gens)

    for x in gens:
        assert x in S
        assert x in S.D_class_of_element(x)
    assert PPerm([0, 1, 2, 3, 4, 5, 6, 7], [1, 7, 2, 6, 0, 4, 8, 5], 9) not in S
    assert not S.contains(
        PPerm([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 8], 9)
    )
    assert S.contains(PPerm([], [], 9))
    assert S.size() == 21033
    assert S.number_of_D_classes() == 3242


def test_case_029():
    "symmetric inverse monoid n = 8"
    ReportGuard(False)
    S = Konieczny(
        [
            PPerm([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], 8),
            PPerm([0, 1, 2, 3, 4, 5, 6, 7], [1, 2, 3, 4, 5, 6, 7, 0], 8),
            PPerm([0, 1, 2, 3, 4, 5, 6, 7], [1, 0, 2, 3, 4, 5, 6, 7], 8),
            PPerm([1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6], 8),
            PPerm([0, 1, 2, 3, 4, 5, 6], [1, 2, 3, 4, 5, 6, 7], 8),
        ]
    )
    assert S.size() == 1441729
    assert S.number_of_D_classes() == 9


def test_case_030():
    "exceptions"
    ReportGuard(False)

    gens = [
        PPerm([0, 2, 3, 7], [1, 6, 7, 3], 9),
        PPerm([0, 1, 2, 3, 4, 7], [6, 5, 8, 0, 2, 1], 9),
        PPerm([0, 1, 2, 3, 4, 5, 6, 8], [1, 7, 2, 6, 0, 4, 8, 5], 9),
        PPerm([0, 1, 2, 3, 5, 6, 8], [2, 4, 6, 1, 5, 8, 7], 9),
        PPerm([0, 1, 2, 3, 5, 8], [7, 3, 6, 4, 2, 5], 9),
    ]

    S = Konieczny(gens)
    S.add_generator(gens[0])
    S.add_generator(gens[1])
    assert S.size() == 21033
    with pytest.raises(RuntimeError):
        S.add_generator(gens[0])


def test_konieczny_return_policy():
    gens = [
        PPerm([0, 2, 3, 7], [1, 6, 7, 3], 9),
        PPerm([0, 1, 2, 3, 4, 7], [6, 5, 8, 0, 2, 1], 9),
        PPerm([0, 1, 2, 3, 4, 5, 6, 8], [1, 7, 2, 6, 0, 4, 8, 5], 9),
        PPerm([0, 1, 2, 3, 5, 6, 8], [2, 4, 6, 1, 5, 8, 7], 9),
        PPerm([0, 1, 2, 3, 5, 8], [7, 3, 6, 4, 2, 5], 9),
    ]

    S = Konieczny(gens)
    assert S.copy() is not S
    assert S.add_generator(gens[0]) is S
    assert S.add_generators(gens) is S
    assert S.D_class_of_element(gens[0]) is S.D_class_of_element(gens[0])
    assert S.generator(0) is S.generator(0)
    assert (
        S.D_class_of_element(gens[0]).rep()
        is S.D_class_of_element(gens[0]).rep()
    )
