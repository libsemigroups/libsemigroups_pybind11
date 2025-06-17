# -*- coding: utf-8 -*-

# Copyright (c) 2024 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This file contains test for SchreierSims"""

# pylint: disable=missing-function-docstring, invalid-name

# TODO(0):
# * test number_of_strong_generators
# * test strong_generator

from copy import copy
import pytest

from libsemigroups_pybind11 import (
    Perm,
    LibsemigroupsError,
    SchreierSims,
    ReportGuard,
)

from libsemigroups_pybind11.schreier_sims import intersection


def check_constructors(gens):
    ReportGuard(False)
    # default constructor
    with pytest.raises(TypeError):
        SchreierSims()

    S1 = SchreierSims(gens)

    # copy
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

    assert S.number_of_generators() == len(gens)

    U = SchreierSims([gens[0]])
    for x in gens[1:]:
        U.add_generator(x)
    assert S.number_of_generators() == U.number_of_generators()
    assert S.size() == U.size()


def check_empty(gens):
    ReportGuard(False)
    S = SchreierSims(gens)
    assert not S.empty()
    S.init()
    assert S.empty()


def check_finished(gens):
    ReportGuard(False)
    S = SchreierSims(gens)
    assert not S.finished()
    S.run()
    assert S.finished()


def check_one(n):
    S = SchreierSims([Perm(range(n))])
    assert S.contains(S.one())
    assert S.one() == Perm(range(n))


def check_elements(n):
    ReportGuard(False)
    S = SchreierSims([Perm(range(n))])

    S.add_base_point(0)
    S.add_base_point(1)
    S.add_base_point(2)
    S.add_generator(
        Perm(
            [
                0,
                2,
                59,
                57,
                16,
                18,
                43,
                41,
                36,
                38,
                31,
                29,
                52,
                54,
                15,
                13,
                8,
                10,
                51,
                49,
                24,
                26,
                35,
                33,
                44,
                46,
                23,
                21,
                60,
                62,
                7,
                5,
                32,
                34,
                27,
                25,
                48,
                50,
                11,
                9,
                4,
                6,
                63,
                61,
                20,
                22,
                47,
                45,
                40,
                42,
                19,
                17,
                56,
                58,
                3,
                1,
                12,
                14,
                55,
                53,
                28,
                30,
                39,
                37,
            ]
            + list((range(64, n)))
        )
    )
    S.add_generator(
        Perm(
            [
                0,
                40,
                51,
                27,
                1,
                41,
                50,
                26,
                2,
                42,
                49,
                25,
                3,
                43,
                48,
                24,
                4,
                44,
                55,
                31,
                5,
                45,
                54,
                30,
                6,
                46,
                53,
                29,
                7,
                47,
                52,
                28,
                16,
                56,
                35,
                11,
                17,
                57,
                34,
                10,
                18,
                58,
                33,
                9,
                19,
                59,
                32,
                8,
                20,
                60,
                39,
                15,
                21,
                61,
                38,
                14,
                22,
                62,
                37,
                13,
                23,
                63,
                36,
                12,
            ]
            + list(range(64, n))
        )
    )
    S.add_generator(
        Perm(
            [
                1,
                0,
                3,
                2,
                5,
                4,
                7,
                6,
                9,
                8,
                11,
                10,
                13,
                12,
                15,
                14,
                17,
                16,
                19,
                18,
                21,
                20,
                23,
                22,
                25,
                24,
                27,
                26,
                29,
                28,
                31,
                30,
                33,
                32,
                35,
                34,
                37,
                36,
                39,
                38,
                41,
                40,
                43,
                42,
                45,
                44,
                47,
                46,
                49,
                48,
                51,
                50,
                53,
                52,
                55,
                54,
                57,
                56,
                59,
                58,
                61,
                60,
                63,
                62,
            ]
            + list(range(64, n))
        )
    )
    S.run()

    with pytest.raises(LibsemigroupsError):
        S.transversal_element(3, 0)
    with pytest.raises(LibsemigroupsError):
        S.transversal_element(4, 0)
    with pytest.raises(LibsemigroupsError):
        S.transversal_element(0, 64)
    with pytest.raises(LibsemigroupsError):
        S.transversal_element(0, 65)
    with pytest.raises(LibsemigroupsError):
        S.inverse_transversal_element(3, 0)
    with pytest.raises(LibsemigroupsError):
        S.inverse_transversal_element(4, 0)
    with pytest.raises(LibsemigroupsError):
        S.inverse_transversal_element(0, 64)
    with pytest.raises(LibsemigroupsError):
        S.inverse_transversal_element(0, 65)

    for i in range(3):
        for j in range(64):
            if S.orbit_lookup(i, j):
                assert S.transversal_element(i, j)[S.base(i)] == j
                assert S.inverse_transversal_element(i, j)[j] == S.base(i)
            else:
                with pytest.raises(LibsemigroupsError):
                    S.transversal_element(i, j)
                with pytest.raises(LibsemigroupsError):
                    S.inverse_transversal_element(i, j)


def check_sift(gens):
    ReportGuard(False)
    S = SchreierSims(gens)
    S.run()
    for i, gen in enumerate(gens):
        assert S.generator(i) == gen
        assert list(S.sift(gen)) == list(S.one())


def check_sift_inplace(gens):
    ReportGuard(False)
    S = SchreierSims(gens)
    one = S.one()
    S.run()
    for gen in gens:
        S.sift_inplace(gen)
    assert all(gen == one for gen in gens)


def check_intersection(n):
    ReportGuard(False)
    gens_S = [
        Perm([1, 3, 7, 5, 2, 0, 4, 6] + list(range(8, n))),
        Perm([2, 4, 3, 6, 5, 7, 0, 1] + list(range(8, n))),
        Perm([3, 5, 6, 0, 7, 1, 2, 4] + list(range(8, n))),
    ]
    gens_T = [
        Perm([1, 0, 7, 5, 6, 3, 4, 2] + list(range(8, n))),
        Perm([2, 4, 3, 6, 5, 7, 0, 1] + list(range(8, n))),
        Perm([3, 5, 6, 0, 7, 1, 2, 4] + list(range(8, n))),
    ]
    gens_U = [Perm(list(range(n)))]

    S = SchreierSims(gens_S)
    T = SchreierSims(gens_T)
    U = SchreierSims(gens_U)
    intersection(U, S, T)
    assert U.size() == 4
    assert U.contains(Perm([2, 4, 3, 6, 5, 7, 0, 1] + list(range(8, n))))


def check_SchreierSims_001(n):
    ReportGuard(False)
    S = SchreierSims([Perm(range(n))])
    S.init()
    assert S.size() == 1
    S.add_generator(
        Perm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0]
            + list(range(17, n))
        )
    )
    S.add_generator(
        Perm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 14]
            + list(range(17, n))
        )
    )

    assert not S.currently_contains(
        Perm(
            [1, 0, 3, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
            + list(range(17, n))
        )
    )
    assert S.current_size() == 17
    assert S.size() == 177843714048000
    assert S.base(0) == 0
    assert S.contains(
        Perm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0]
            + list(range(17, n))
        )
    )

    assert not S.contains(
        Perm(
            [1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
            + list(range(17, n))
        )
    )
    assert S.contains(
        Perm(
            [1, 0, 3, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
            + list(range(17, n))
        )
    )

    S.init()
    with pytest.raises(LibsemigroupsError):
        S.base(0)
    with pytest.raises(LibsemigroupsError if n != 255 else TypeError):
        S.add_base_point(n + 1)
    S.add_base_point(14)
    S.add_base_point(15)
    S.add_generator(
        Perm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0]
            + list(range(17, n))
        )
    )
    S.add_base_point(1)
    S.add_base_point(3)
    S.add_generator(
        Perm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 14]
            + list(range(17, n))
        )
    )
    assert S.base_size() == 4
    assert S.size() == 177843714048000
    assert S.base(0) == 14
    assert S.base(1) == 15
    assert S.base(2) == 1
    assert S.base(3) == 3
    assert S.base_size() == 15

    with pytest.raises(LibsemigroupsError):
        S.add_base_point(1)
    with pytest.raises(LibsemigroupsError):
        S.base(15)

    assert S.contains(
        Perm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0]
            + list(range(17, n))
        )
    )
    assert not S.contains(
        Perm(
            [1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
            + list(range(17, n))
        )
    )
    assert S.contains(
        Perm(
            [1, 0, 3, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
            + list(range(17, n))
        )
    )
    with pytest.raises(LibsemigroupsError):
        S.add_base_point(1)

    S.init()
    S.add_generator(
        Perm(
            [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0]
            + list(range(17, n))
        )
    )
    assert S.size() == 17
    S.add_generator(
        Perm(
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 14]
            + list(range(17, n))
        )
    )
    assert S.size() == 177843714048000


@pytest.fixture(name="checks_with_generators")
def fixture_checks_with_generators():
    return (
        check_constructors,
        check_generators,
        check_empty,
        check_finished,
        check_sift,
        check_sift_inplace,
    )


@pytest.fixture(name="checks_with_int")
def fixture_checks_with_int():
    return (
        check_SchreierSims_001,
        check_one,
        check_elements,
        check_intersection,
    )


def test_SchreierSims_001(checks_with_generators):
    gens = [
        Perm([1, 0, 2, 3, 4] + list(range(5, 255))),
        Perm([1, 2, 3, 4, 0] + list(range(5, 255))),
    ]
    for check in checks_with_generators:
        check(gens)


def test_SchreierSims_002(checks_with_generators):
    gens = [
        Perm([0, 2, 4, 6, 7, 3, 8, 1, 5] + list(range(9, 255))),
        Perm([0, 3, 5, 4, 8, 7, 2, 6, 1] + list(range(9, 255))),
    ]
    for check in checks_with_generators:
        check(gens)


def test_SchreierSims_003(checks_with_generators):
    gens = [
        Perm([1, 0, 2, 3, 4] + list(range(5, 511))),
        Perm([1, 2, 3, 4, 0] + list(range(5, 511))),
    ]
    for check in checks_with_generators:
        check(gens)


def test_SchreierSims_004(checks_with_generators):
    gens = [
        Perm([0, 2, 4, 6, 7, 3, 8, 1, 5] + list(range(9, 511))),
        Perm([0, 3, 5, 4, 8, 7, 2, 6, 1] + list(range(9, 511))),
    ]
    for check in checks_with_generators:
        check(gens)


def test_SchreierSims_Perm1(checks_with_int):
    for check in checks_with_int:
        check(255)


def test_SchreierSims_Perm2(checks_with_int):
    for check in checks_with_int:
        check(511)


def test_SchreierSims_return_policy():
    gens = [
        Perm([0, 2, 4, 6, 7, 3, 8, 1, 5] + list(range(9, 511))),
        Perm([0, 3, 5, 4, 8, 7, 2, 6, 1] + list(range(9, 511))),
    ]
    S = SchreierSims(gens)
    assert S.copy() is not S
    assert S.add_base_point(0) is S
    assert S.generator(0) is S.generator(0)
    assert S.inverse_transversal_element(0, 1) is S.inverse_transversal_element(
        0, 1
    )
    assert S.transversal_element(0, 1) is S.transversal_element(0, 1)
    assert S.strong_generator(0, 1) is S.strong_generator(0, 1)
    assert S.one() is S.one()
    assert S.sift(gens[0]) is not gens[0]
    S.sift_inplace(gens[0])
    assert gens[0] == S.one()

    assert S.init() is S
