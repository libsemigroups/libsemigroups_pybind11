# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name, redefined-outer-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for FroidurePin
"""

from datetime import timedelta
import pytest
from runner import check_runner

from _libsemigroups_pybind11 import (
    Transf16,
    Transf1,
    Transf2,
    Transf4,
    PPerm16,
    PPerm1,
    PPerm2,
    PPerm4,
    Perm16,
    Perm1,
    Perm2,
    Perm4,
    BMat8,
    IntMat,
    MaxPlusMat,
    MinPlusMat,
)

from libsemigroups_pybind11 import (
    FroidurePin,
    ReportGuard,
    ToddCoxeter,
    KnuthBendix,
    PBR,
    Bipartition,
    congruence_kind,
    Matrix,
    MatrixKind,
)


def check_constructors(coll):
    ReportGuard(False)
    # default constructor
    S = FroidurePin(coll[0])
    S.add_generators(coll[1:])

    # copy constructor
    U = FroidurePin(S)
    assert S is not U
    assert S.number_of_generators() == U.number_of_generators()
    assert S.current_size() == U.current_size()


def check_generators(coll):
    ReportGuard(False)
    S = FroidurePin(coll[0])
    S.add_generators(coll[1:])
    for i, x in enumerate(coll):
        assert S.generator(i) == x

    with pytest.raises(RuntimeError):
        S.generator(len(coll))

    U = FroidurePin(coll[0])
    for x in coll[1:]:
        U.add_generator(x)
    assert S.number_of_generators() == U.number_of_generators()
    assert S.size() == U.size()

    S.closure(coll)
    assert S.number_of_generators() == len(coll)

    U = S.copy_closure(coll)
    assert U.number_of_generators() == S.number_of_generators()
    assert U.size() == S.size()
    assert U is not S

    U = S.copy_add_generators(coll)
    assert U.number_of_generators() == 2 * S.number_of_generators()
    assert U.size() == S.size()
    assert U is not S


def check_settings(S):
    ReportGuard(False)
    assert not S.immutable()
    S.immutable(False)
    assert S.batch_size() == 8192
    S.batch_size(S.batch_size())
    assert S.concurrency_threshold() == 823543
    S.concurrency_threshold(S.concurrency_threshold())
    S.max_threads(2)
    assert S.max_threads() == 2
    S.reserve(100)


def check_mem_compare(S):
    ReportGuard(False)

    with pytest.raises(RuntimeError):
        S.current_position([0, 0, 0, 0, 0, 0, 0, S.number_of_generators(), 1])
    with pytest.raises(RuntimeError):
        S.current_position(S.number_of_generators())

    S.run()
    assert all(S.contains(x) for x in S)

    assert [S.position(x) for x in S] == list(range(S.size()))
    #  not implemented
    # self.assertEqual(
    #     [S.position(S.factorisation(x)) for x in S], list(range(S.size()))
    # )
    assert [S.current_position(S.factorisation(x)) for x in S] == list(
        range(S.size())
    )

    assert [S.current_position(x) for x in S] == list(range(S.size()))
    assert [
        S.current_position(x) for x in range(S.number_of_generators())
    ] == list(range(S.number_of_generators()))

    for x in S:
        assert S.sorted_position(x) == S.position_to_sorted_position(
            S.position(x)
        )

    for x in S.idempotents():
        w = S.factorisation(x)
        assert S.equal_to(w + w, w)


def check_accessors(S):
    ReportGuard(False)
    # current_size
    assert S.current_size() == S.number_of_generators()
    S.run()
    # size
    assert S.current_size() == S.size()

    # at, sorted_at
    for i in range(S.size()):
        assert S.at(i) == S.sorted_at(S.position_to_sorted_position(i))

    # __iter__, sorted
    assert sorted(list(S)) == list(S.sorted())


def check_attributes(S):
    ReportGuard(False)

    S.is_monoid()
    assert S.is_finite()
    S.degree()  # just check it doesn't throw


def check_idempotents(S):
    ReportGuard(False)

    try:
        assert all((x * x == x for x in S.idempotents()))
    except TypeError:  # no multiplication provides
        pass

    assert all(
        S.fast_product(S.position(x), S.position(x)) == S.position(x)
        for x in S.idempotents()
    )

    assert (
        sum(1 for x in range(S.size()) if S.is_idempotent(x))
        == S.number_of_idempotents()
    )


def check_cayley_graphs(S):
    ReportGuard(False)

    g = S.right_cayley_graph()
    assert len(g) == S.size()
    assert len(g[0]) == S.number_of_generators()
    g = S.left_cayley_graph()
    assert len(g) == S.size()
    assert len(g[0]) == S.number_of_generators()


def check_factor_prod_rels(S):
    ReportGuard(False)

    # current_length
    for i in range(S.number_of_generators()):
        assert S.current_length(i) == 1

    with pytest.raises(RuntimeError):
        S.current_length(S.number_of_generators())

    # current_max_word_length
    assert S.current_max_word_length() == 1
    assert S.current_number_of_rules() == 0

    S.run()
    # (minimal_)factorisation + word_to_element
    for i, x in enumerate(S):
        assert S.word_to_element(S.factorisation(x)) == x
        assert S.word_to_element(S.minimal_factorisation(i)) == x

    # rules, number_of_rules
    assert len(list(S.rules())) == S.number_of_rules()

    for l, r in S.rules():
        assert S.current_position(l) == S.current_position(r)
        assert S.factorisation(S.current_position(r)) == r

    # product_by_reduction + fast_product
    try:
        for i in range(S.size()):
            for j in range(S.size()):
                assert S.product_by_reduction(i, j) == S.position(
                    S.at(i) * S.at(j)
                )
                assert S.fast_product(i, j) == S.position(S.at(i) * S.at(j))
    except TypeError:  # no product defined
        pass


def check_prefix_suffix(S):
    ReportGuard(False)
    S.run()

    for i in range(S.number_of_generators(), S.size()):
        assert S.fast_product(S.prefix(i), S.final_letter(i)) == i
        assert S.fast_product(S.first_letter(i), S.suffix(i)) == i


def check_froidure_pin_transf1(T):
    ReportGuard(False)
    if T is Transf16:
        add = list(range(8, 16))
    else:
        add = []

    S = FroidurePin(T.make([1, 7, 2, 6, 0, 4, 1, 5] + add))

    S.add_generator(T.make([2, 4, 6, 1, 4, 5, 2, 7] + add))
    S.add_generator(T.make([3, 0, 7, 2, 4, 6, 2, 4] + add))
    S.add_generator(T.make([3, 2, 3, 4, 5, 3, 0, 1] + add))
    S.add_generator(T.make([4, 3, 7, 7, 4, 5, 0, 4] + add))
    S.add_generator(T.make([5, 6, 3, 0, 3, 0, 5, 1] + add))
    S.add_generator(T.make([6, 0, 1, 1, 1, 6, 3, 4] + add))
    S.add_generator(T.make([7, 7, 4, 0, 6, 4, 1, 7] + add))
    S.reserve(600000)

    assert S.number_of_generators() == 8
    assert S.current_size() == 8
    assert S.current_number_of_rules() == 0
    S.enumerate(100)
    assert S.current_size() == 8200
    assert S.current_number_of_rules() == 1049

    if T == Transf16:
        assert S.degree() == 16
    else:
        assert S.degree() == 8


def check_froidure_pin_transf2(T):
    add = list(range(3, 16)) if T is Transf16 else []
    gens = [
        T.make([1, 0, 2] + add),
        T.make([1, 2, 0] + add),
    ]
    S = FroidurePin(gens)
    S.run()
    assert list(S) == [
        T.make([1, 0, 2] + add),
        T.make([1, 2, 0] + add),
        T.make([0, 1, 2] + add),
        T.make([2, 1, 0] + add),
        T.make([0, 2, 1] + add),
        T.make([2, 0, 1] + add),
    ]

    assert list(S.sorted()) == [
        T.make([0, 1, 2] + add),
        T.make([0, 2, 1] + add),
        T.make([1, 0, 2] + add),
        T.make([1, 2, 0] + add),
        T.make([2, 0, 1] + add),
        T.make([2, 1, 0] + add),
    ]


@pytest.fixture
def checks_for_generators():
    return (check_constructors, check_generators)


@pytest.fixture
def checks_for_froidure_pin():
    return (
        check_settings,
        check_mem_compare,
        check_accessors,
        check_attributes,
        check_idempotents,
        check_cayley_graphs,
        check_factor_prod_rels,
        check_prefix_suffix,
    )


def test_froidure_pin_transf(checks_for_froidure_pin, checks_for_generators):
    for T in (Transf16, Transf1, Transf2, Transf4):
        add = list(range(3, 16)) if T is Transf16 else []
        gens = [
            T.make([1, 0, 2] + add),
            T.make([1, 2, 0] + add),
            T.make([0, 0, 1] + add),
        ]
        for check in checks_for_generators:
            check(gens)

        for check in checks_for_froidure_pin:
            check(FroidurePin(gens))

        check_froidure_pin_transf1(T)
        check_froidure_pin_transf2(T)


def test_runner_transf():
    for T in (Transf16, Transf1, Transf2, Transf4):
        add = list(range(8, 16)) if T is Transf16 else []
        S = FroidurePin(T.make([1, 7, 2, 6, 0, 4, 1, 5] + add))

        S.add_generator(T.make([2, 4, 6, 1, 4, 5, 2, 7] + add))
        S.add_generator(T.make([3, 0, 7, 2, 4, 6, 2, 4] + add))
        S.add_generator(T.make([3, 2, 3, 4, 5, 3, 0, 1] + add))
        S.add_generator(T.make([4, 3, 7, 7, 4, 5, 0, 4] + add))
        S.add_generator(T.make([5, 6, 3, 0, 3, 0, 5, 1] + add))
        S.add_generator(T.make([6, 0, 1, 1, 1, 6, 3, 4] + add))
        S.add_generator(T.make([7, 7, 4, 0, 6, 4, 1, 7] + add))
        check_runner(S, timedelta(microseconds=1000))


def test_froidure_pin_pperm(checks_for_froidure_pin, checks_for_generators):
    for T in (PPerm16, PPerm1, PPerm2, PPerm4):
        gens = [
            T.make([0, 1, 2], [1, 0, 2], 16),
            T.make([0, 1, 2], [1, 2, 0], 16),
            T.make([0, 1], [0, 1], 16),
        ]
        for check in checks_for_generators:
            check(gens)

        for check in checks_for_froidure_pin:
            check(FroidurePin(gens))


def test_runner_pperm():
    for T in (PPerm16, PPerm1, PPerm2, PPerm4):
        S = FroidurePin(T.make(list(range(9)), [1, 0] + list(range(2, 9)), 16))
        S.add_generator(T.make(list(range(9)), list(range(1, 9)) + [0], 16))
        S.add_generator(T.make([0, 1], [0, 1], 16))
        check_runner(S, timedelta(microseconds=1000))


def test_froidure_pin_perm(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    for T in (Perm16, Perm1, Perm2, Perm4):
        add = list(range(4, 16)) if T is Perm16 else []
        gens = [
            T.make([1, 0] + list(range(2, 4)) + add),
            T.make(list(range(1, 4)) + [0] + add),
        ]
        assert FroidurePin(gens).size() == 24

        for check in checks_for_generators:
            check(gens)

        for check in checks_for_froidure_pin:
            check(FroidurePin(gens))


def test_runner_perm():
    for T in (Perm16, Perm1, Perm2, Perm4):
        add = list(range(9, 16)) if T is Perm16 else []
        gens = [
            T.make([1, 0] + list(range(2, 9)) + add),
            T.make(list(range(1, 9)) + [0] + add),
        ]
        S = FroidurePin(gens)
        check_runner(S, timedelta(microseconds=1000))


def test_froidure_pin_tce(checks_for_froidure_pin):
    ReportGuard(False)
    tc = ToddCoxeter(congruence_kind.twosided)
    tc.set_number_of_generators(2)
    tc.add_pair([0, 0, 0, 0], [0])
    tc.add_pair([1, 1, 1, 1], [1])
    tc.add_pair([0, 1], [1, 0])

    assert tc.number_of_classes() == 15

    for check in checks_for_froidure_pin:
        check(FroidurePin(tc.quotient_froidure_pin()))


def test_froidure_pin_kbe(checks_for_froidure_pin):
    ReportGuard(False)
    kb = KnuthBendix()
    kb.set_alphabet(2)
    kb.add_rule([0, 0, 0, 0], [0])
    kb.add_rule([1, 1, 1, 1], [1])
    kb.add_rule([0, 1], [1, 0])

    assert kb.size() == 15

    for check in checks_for_froidure_pin:
        check(FroidurePin(kb.froidure_pin()))


def test_froidure_pin_bipart(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    T = Bipartition
    gens = [
        T.make([0, 1, 1, 0]),
        T.make([0, 1, 2, 1]),
        T.make([0, 0, 0, 0]),
    ]
    assert FroidurePin(gens).size() == 15

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_pbr(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    T = PBR
    gens = [
        T.make([[], [0]]),
        T.make([[0, 1], [0]]),
        T.make([[1], []]),
        T.make([[1], [0, 1]]),
    ]
    assert FroidurePin(gens).size() == 15

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_bmat(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [
        Matrix(MatrixKind.Boolean, [[0, 1], [1, 0]]),
        Matrix(MatrixKind.Boolean, [[1, 0], [1, 1]]),
        Matrix(MatrixKind.Boolean, [[1, 0], [0, 0]]),
    ]
    assert FroidurePin(gens).size() == 16

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_bmat8(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [
        BMat8([[0, 1], [1, 0]]),
        BMat8([[1, 0], [1, 1]]),
        BMat8([[1, 0], [0, 0]]),
    ]
    assert FroidurePin(gens).size() == 16

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_int_mat(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [IntMat([[0, -3], [-2, -10]])]
    assert FroidurePin(gens).size() == 64

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_max_plus(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [MaxPlusMat([[0, -3], [-2, -10]])]
    assert FroidurePin(gens).size() == 2

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_min_plus(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    x = MinPlusMat(2, 2)
    gens = [MinPlusMat([[1, 0], [0, x.zero()]])]
    assert FroidurePin(gens).size() == 3

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_proj_max_plus(
    checks_for_froidure_pin, checks_for_generators
):
    ReportGuard(False)
    x = Matrix(MatrixKind.ProjMaxPlus, 2, 2)
    gens = [Matrix(MatrixKind.ProjMaxPlus, [[1, 0], [0, x.zero()]])]
    assert FroidurePin(gens).size() == 2

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_max_plus_trunc(
    checks_for_froidure_pin, checks_for_generators
):
    ReportGuard(False)
    gens = [Matrix(MatrixKind.MaxPlusTrunc, 11, [[1, 0], [0, 1]])]
    assert FroidurePin(gens).size() == 12

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_min_plus_trunc(
    checks_for_froidure_pin, checks_for_generators
):
    ReportGuard(False)
    gens = [Matrix(MatrixKind.MinPlusTrunc, 11, [[1, 0], [0, 1]])]
    assert FroidurePin(gens).size() == 2

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_ntp(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [Matrix(MatrixKind.NTP, 5, 7, [[1, 1], [1, 1]])]
    assert FroidurePin(gens).size() == 6

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))
