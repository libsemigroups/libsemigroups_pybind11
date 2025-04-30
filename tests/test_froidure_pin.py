# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name, redefined-outer-name

# Copyright (c) 2021-2024 J. D. Mitchell
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

from libsemigroups_pybind11 import (
    FroidurePin,
    ReportGuard,
    PBR,
    Bipartition,
    Matrix,
    MatrixKind,
    Transf,
    PPerm,
    Perm,
    BMat8,
    froidure_pin,
    LibsemigroupsError,
    UNDEFINED,
)


def check_constructors(coll):
    ReportGuard(False)
    # default constructor
    S = FroidurePin([coll[0]])
    S.add_generators(coll[1:])

    # copy constructor
    U = S.copy()
    assert S is not U
    assert S.number_of_generators() == U.number_of_generators()
    assert S.current_size() == U.current_size()


def check_generators(coll):
    ReportGuard(False)
    S = FroidurePin([coll[0]])
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
    assert S.batch_size() == 8192
    S.batch_size(S.batch_size())
    S.reserve(100)


def check_mem_compare(S):
    ReportGuard(False)

    with pytest.raises(RuntimeError):
        froidure_pin.current_position(
            S, [0, 0, 0, 0, 0, 0, 0, S.number_of_generators(), 1]
        )
    with pytest.raises(RuntimeError):
        S.position_of_generator(S.number_of_generators())

    S.run()
    assert all(S.contains(x) for x in S)

    assert [S.position(x) for x in S] == list(range(S.size()))
    #  not implemented
    # self.assertEqual(
    #     [S.position(froidure_pin.factorisation(S, x)) for x in S], list(range(S.size()))
    # )
    assert [
        froidure_pin.current_position(S, froidure_pin.factorisation(S, x))
        for x in S
    ] == list(range(S.size()))

    assert [S.current_position(x) for x in S] == list(range(S.size()))
    assert [
        S.position_of_generator(i) for i in range(S.number_of_generators())
    ] == list(range(S.number_of_generators()))

    for x in S:
        assert S.sorted_position(x) == S.to_sorted_position(S.position(x))

    for x in S.idempotents():
        w = froidure_pin.factorisation(S, x)
        assert froidure_pin.equal_to(S, w + w, w)


def check_accessors(S):
    ReportGuard(False)
    # current_size
    assert S.current_size() == S.number_of_generators()
    S.run()
    # size
    assert S.current_size() == S.size()

    # at, sorted_at
    for i in range(S.size()):
        assert S[i] == S.sorted_at(S.to_sorted_position(i))

    # __iter__, sorted
    assert sorted(list(S)) == list(S.sorted_elements())


def check_attributes(S):
    ReportGuard(False)

    S.contains_one()
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
    assert g.number_of_nodes() == S.size()
    assert g.out_degree() == S.number_of_generators()
    g = S.left_cayley_graph()
    assert g.number_of_nodes() == S.size()
    assert g.out_degree() == S.number_of_generators()


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
    # (minimal_)factorisation + to_element
    for i, x in enumerate(S):
        assert froidure_pin.to_element(S, froidure_pin.factorisation(S, x)) == x
        assert (
            froidure_pin.to_element(S, froidure_pin.minimal_factorisation(S, i))
            == x
        )

    # rules, number_of_rules
    assert len(list(froidure_pin.rules(S))) == S.number_of_rules()

    for lhs, rhs in froidure_pin.rules(S):
        assert froidure_pin.current_position(
            S, lhs
        ) == froidure_pin.current_position(S, rhs)
        assert (
            froidure_pin.factorisation(S, froidure_pin.current_position(S, rhs))
            == rhs
        )

    # product_by_reduction + fast_product
    try:
        for i in range(S.size()):
            for j in range(S.size()):
                assert froidure_pin.product_by_reduction(S, i, j) == S.position(
                    S[i] * S[j]
                )
                assert S.fast_product(i, j) == S.position(S[i] * S[j])
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
    S = FroidurePin(T([1, 7, 2, 6, 0, 4, 1, 5]))

    S.add_generator(T([2, 4, 6, 1, 4, 5, 2, 7]))
    S.add_generator(T([3, 0, 7, 2, 4, 6, 2, 4]))
    S.add_generator(T([3, 2, 3, 4, 5, 3, 0, 1]))
    S.add_generator(T([4, 3, 7, 7, 4, 5, 0, 4]))
    S.add_generator(T([5, 6, 3, 0, 3, 0, 5, 1]))
    S.add_generator(T([6, 0, 1, 1, 1, 6, 3, 4]))
    S.add_generator(T([7, 7, 4, 0, 6, 4, 1, 7]))
    S.reserve(600000)

    assert S.number_of_generators() == 8
    assert S.current_size() == 8
    assert S.current_number_of_rules() == 0
    S.enumerate(100)
    assert S.current_size() == 8200
    assert S.current_number_of_rules() == 1049

    assert S.degree() == 8


def check_froidure_pin_transf2(T):
    add = list(range(3, 32))
    gens = [
        T([1, 0, 2] + add),
        T([1, 2, 0] + add),
    ]
    S = FroidurePin(gens)
    S.run()
    assert list(S) == [
        T([1, 0, 2] + add),
        T([1, 2, 0] + add),
        T([0, 1, 2] + add),
        T([2, 1, 0] + add),
        T([0, 2, 1] + add),
        T([2, 0, 1] + add),
    ]

    assert list(S.sorted_elements()) == [
        T([0, 1, 2] + add),
        T([0, 2, 1] + add),
        T([1, 0, 2] + add),
        T([1, 2, 0] + add),
        T([2, 0, 1] + add),
        T([2, 1, 0] + add),
    ]


@pytest.fixture
def checks_for_generators():
    return (check_constructors,)


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
    gens = [
        Transf([1, 0, 2]),
        Transf([1, 2, 0]),
        Transf([0, 0, 1]),
    ]
    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))

    check_froidure_pin_transf1(Transf)
    check_froidure_pin_transf2(Transf)


def test_runner_transf():
    S = FroidurePin(Transf([1, 7, 2, 6, 0, 4, 1, 5]))
    S.add_generator(Transf([2, 4, 6, 1, 4, 5, 2, 7]))
    S.add_generator(Transf([3, 0, 7, 2, 4, 6, 2, 4]))
    S.add_generator(Transf([3, 2, 3, 4, 5, 3, 0, 1]))
    S.add_generator(Transf([4, 3, 7, 7, 4, 5, 0, 4]))
    S.add_generator(Transf([5, 6, 3, 0, 3, 0, 5, 1]))
    S.add_generator(Transf([6, 0, 1, 1, 1, 6, 3, 4]))
    S.add_generator(Transf([7, 7, 4, 0, 6, 4, 1, 7]))
    check_runner(S, timedelta(microseconds=1000))


def test_froidure_pin_pperm(checks_for_froidure_pin, checks_for_generators):
    gens = [
        PPerm([0, 1, 2], [1, 0, 2], 16),
        PPerm([0, 1, 2], [1, 2, 0], 16),
        PPerm([0, 1], [0, 1], 16),
    ]
    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_runner_pperm():
    S = FroidurePin(PPerm(list(range(9)), [1, 0] + list(range(2, 9)), 16))
    S.add_generator(PPerm(list(range(9)), list(range(1, 9)) + [0], 16))
    S.add_generator(PPerm([0, 1], [0, 1], 16))
    check_runner(S, timedelta(microseconds=1000))


def test_froidure_pin_perm(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [
        Perm([1, 0] + list(range(2, 4))),
        Perm(list(range(1, 4)) + [0]),
    ]
    assert FroidurePin(gens).size() == 24

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_runner_perm():
    gens = [
        Perm([1, 0] + list(range(2, 9))),
        Perm(list(range(1, 9)) + [0]),
    ]
    S = FroidurePin(gens)
    check_runner(S, timedelta(microseconds=1000))


def test_froidure_pin_bipart(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    T = Bipartition
    gens = [
        T([0, 1, 1, 0]),
        T([0, 1, 2, 1]),
        T([0, 0, 0, 0]),
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
        T([[], [0]]),
        T([[0, 1], [0]]),
        T([[1], []]),
        T([[1], [0, 1]]),
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
    gens = [Matrix(MatrixKind.Integer, [[0, -3], [-2, -10]])]
    S = FroidurePin(gens)
    # This example is probably infinite really, here we are using 64 bit
    # integers
    assert S.size() == 128

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_max_plus(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    gens = [Matrix(MatrixKind.MaxPlus, [[0, -3], [-2, -10]])]
    assert FroidurePin(gens).size() == 2

    for check in checks_for_generators:
        check(gens)

    for check in checks_for_froidure_pin:
        check(FroidurePin(gens))


def test_froidure_pin_min_plus(checks_for_froidure_pin, checks_for_generators):
    ReportGuard(False)
    x = Matrix(MatrixKind.MinPlus, 2, 2)
    gens = [Matrix(MatrixKind.MinPlus, [[1, 0], [0, x.scalar_zero()]])]
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
    gens = [Matrix(MatrixKind.ProjMaxPlus, [[1, 0], [0, x.scalar_zero()]])]
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


def test_froidure_pin_method_wrap():
    with pytest.raises(LibsemigroupsError):
        FroidurePin(Perm([1, 0, 2, 3, 4, 5, 6]), Perm([1, 2, 3, 4, 5, 0]))

    with pytest.raises(TypeError):
        FroidurePin(Perm([1, 0, 2, 3, 4, 5, 6]), BMat8(0))

    S = FroidurePin(Perm([1, 0, 2, 3, 4, 5, 6]), Perm([1, 2, 3, 4, 5, 6, 0]))
    with pytest.raises(LibsemigroupsError):
        S.add_generator(Perm([0, 1]))
    with pytest.raises(TypeError):
        S.add_generator(BMat8(0))
    S.add_generator(Perm([0, 1, 2, 3, 4, 5, 6]))
    assert S.number_of_generators() == 3

    S.add_generators([])
    with pytest.raises(TypeError):
        S.add_generators(Perm([0, 1]))
    with pytest.raises(LibsemigroupsError):
        S.add_generators([Perm([0, 1])])
    with pytest.raises(TypeError):
        S.add_generators([BMat8(0)])

    S.init()
    with pytest.raises(LibsemigroupsError):
        S.add_generators(
            [Perm([0, 1, 2, 3, 4, 5]), Perm([0, 1, 2, 3, 4, 5, 6])]
        )

    S = FroidurePin(Perm([1, 0, 2, 3, 4, 5, 6]), Perm([1, 2, 3, 4, 5, 6, 0]))

    with pytest.raises(TypeError):
        S.closure(Perm([0, 1]))
    with pytest.raises(LibsemigroupsError):
        S.closure([Perm([0, 1])])
    with pytest.raises(TypeError):
        S.closure([BMat8(0)])

    S.init()
    assert S.number_of_generators() == 0
    S.add_generator(Perm([1, 0]))
    assert S.degree() == 2

    S.init()
    assert S.number_of_generators() == 0
    S.add_generators([Perm([1, 0, 2])])
    assert S.degree() == 3

    # TODO more


def test_froidure_pin_return_undefined_1():
    S = FroidurePin(Perm([1, 0, 2, 3, 4, 5, 6]))
    assert S.current_position(Perm([1, 0, 2])) == UNDEFINED


# def test_froidure_pin_tce(checks_for_froidure_pin):
#     ReportGuard(False)
#     tc = ToddCoxeter(congruence_kind.twosided)
#     tc.set_number_of_generators(2)
#     tc.add_pair([0, 0, 0, 0], [0])
#     tc.add_pair([1, 1, 1, 1], [1])
#     tc.add_pair([0, 1], [1, 0])
#
#     assert tc.number_of_classes() == 15
#
#     for check in checks_for_froidure_pin:
#         check(FroidurePin(tc.quotient_froidure_pin()))
#
#
# def test_froidure_pin_kbe(checks_for_froidure_pin):
#     ReportGuard(False)
#     kb = KnuthBendix()
#     kb.set_alphabet(2)
#     kb.add_rule([0, 0, 0, 0], [0])
#     kb.add_rule([1, 1, 1, 1], [1])
#     kb.add_rule([0, 1], [1, 0])
#
#     assert kb.size() == 15
#
#     for check in checks_for_froidure_pin:
#         check(FroidurePin(kb.froidure_pin()))
