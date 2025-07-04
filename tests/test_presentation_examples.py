# -*- coding: utf-8 -*-
# Copyright (c) 2022-2024 M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for fpsemi-examples.
"""

# pylint: disable=missing-function-docstring, invalid-name

from math import factorial
import pytest

from libsemigroups_pybind11 import (
    ToddCoxeter,
    congruence_kind,
    LibsemigroupsError,
    ReportGuard,
)

from libsemigroups_pybind11.presentation import examples


def check_symmetric_group(min_n, max_n, symmetric_group_implementation):
    ReportGuard(False)
    with pytest.raises(RuntimeError):
        symmetric_group_implementation(min_n - 1)
    for i in range(min_n, max_n):
        p = symmetric_group_implementation(i)
        p.throw_if_bad_alphabet_or_rules()
        tc = ToddCoxeter(congruence_kind.twosided, p)
        assert tc.number_of_classes() == factorial(i)


def check_full_transformation_monoid(ns, ftm_implementation):
    ReportGuard(False)
    with pytest.raises(RuntimeError):
        ftm_implementation(ns[0] - 1)
    for n in ns:
        p = ftm_implementation(n)
        p.throw_if_bad_alphabet_or_rules()
        tc = ToddCoxeter(congruence_kind.twosided, p)
        assert tc.number_of_classes() == n**n


def check_symmetric_inverse_monoid(sim_implementation):
    ReportGuard(False)
    n = 5
    p = sim_implementation(n)
    p.throw_if_bad_alphabet_or_rules()

    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 1546


def test_semigroup_status():
    assert not examples.fibonacci_semigroup_CRRT94(5, 2).contains_empty_word()
    assert not examples.monogenic_semigroup(2, 6).contains_empty_word()
    assert not examples.rectangular_band_ACOR00(5, 5).contains_empty_word()


def test_monoid_status():
    ReportGuard(False)

    assert examples.alternating_group_Moo97(5).contains_empty_word()
    assert examples.brauer_monoid_KM07(5).contains_empty_word()
    assert examples.chinese_monoid_CEKNH01(5).contains_empty_word()
    assert examples.cyclic_inverse_monoid_Fer22_a(5).contains_empty_word()
    assert examples.cyclic_inverse_monoid_Fer22_b(5).contains_empty_word()
    assert examples.dual_symmetric_inverse_monoid_EEF07(5).contains_empty_word()
    assert examples.full_transformation_monoid_Aiz58(5).contains_empty_word()
    assert examples.full_transformation_monoid_II74(5).contains_empty_word()
    assert examples.full_transformation_monoid_MW24_a(5).contains_empty_word()
    assert examples.full_transformation_monoid_MW24_b(5).contains_empty_word()
    assert examples.hypo_plactic_monoid_Nov00(5).contains_empty_word()
    assert examples.monogenic_semigroup(0, 5).contains_empty_word()
    assert examples.motzkin_monoid_PHL13(5).contains_empty_word()
    assert examples.not_renner_type_B_monoid_Gay18(5, 0).contains_empty_word()
    assert examples.not_renner_type_B_monoid_Gay18(5, 1).contains_empty_word()
    assert examples.not_renner_type_D_monoid_God09(5, 0).contains_empty_word()
    assert examples.not_renner_type_D_monoid_God09(5, 1).contains_empty_word()
    assert examples.not_symmetric_group_GKKL08(5).contains_empty_word()
    assert examples.order_preserving_cyclic_inverse_monoid_Fer22(
        5
    ).contains_empty_word()
    assert examples.order_preserving_monoid_AR00(5).contains_empty_word()
    assert examples.orientation_preserving_monoid_AR00(5).contains_empty_word()
    assert examples.orientation_preserving_reversing_monoid_AR00(
        5
    ).contains_empty_word()
    assert examples.partial_brauer_monoid_KM07(5).contains_empty_word()
    assert examples.partial_isometries_cycle_graph_monoid_FP22(
        5
    ).contains_empty_word()
    assert examples.partial_transformation_monoid_MW24(5).contains_empty_word()
    assert examples.partial_transformation_monoid_Shu60(5).contains_empty_word()
    assert examples.partition_monoid_Eas11(5).contains_empty_word()
    assert examples.partition_monoid_HR05(5).contains_empty_word()
    assert examples.plactic_monoid_Knu70(5).contains_empty_word()
    assert examples.renner_type_B_monoid_Gay18(5, 0).contains_empty_word()
    assert examples.renner_type_B_monoid_Gay18(5, 1).contains_empty_word()
    assert examples.renner_type_D_monoid_Gay18(5, 0).contains_empty_word()
    assert examples.renner_type_D_monoid_Gay18(5, 1).contains_empty_word()
    assert examples.singular_brauer_monoid_MM07(5).contains_empty_word()
    assert examples.special_linear_group_2_CR80(5).contains_empty_word()
    assert examples.stellar_monoid_GH19(5).contains_empty_word()
    assert examples.stylic_monoid_AR22(5).contains_empty_word()
    assert examples.symmetric_group_Bur12(5).contains_empty_word()
    assert examples.symmetric_group_Car56(5).contains_empty_word()
    assert examples.symmetric_group_Moo97_a(5).contains_empty_word()
    assert examples.symmetric_group_Moo97_b(5).contains_empty_word()
    assert examples.symmetric_inverse_monoid_Sol04(5).contains_empty_word()
    assert examples.symmetric_inverse_monoid_MW24(5).contains_empty_word()
    assert examples.symmetric_inverse_monoid_Shu60(5).contains_empty_word()
    assert examples.temperley_lieb_monoid_Eas21(5).contains_empty_word()
    assert examples.uniform_block_bijection_monoid_Fit03(
        5
    ).contains_empty_word()
    assert examples.zero_rook_monoid_Gay18(5).contains_empty_word()


def test_symmetric_group():
    check_symmetric_group(2, 7, examples.symmetric_group)


def test_symmetric_group_Bur12():
    check_symmetric_group(2, 7, examples.symmetric_group_Bur12)


def test_symmetric_group_Car56():
    check_symmetric_group(2, 7, examples.symmetric_group_Car56)


def test_symmetric_group_Moo97_a():
    check_symmetric_group(2, 7, examples.symmetric_group_Moo97_a)


def test_symmetric_group_Moo97_b():
    check_symmetric_group(2, 7, examples.symmetric_group_Moo97_b)


def test_alternating_group_Moo97():
    ReportGuard(False)
    min_n = 4
    max_n = 9
    with pytest.raises(LibsemigroupsError):
        examples.alternating_group_Moo97(min_n - 1)
    for i in range(min_n, max_n):
        p = examples.alternating_group_Moo97(i)
        p.throw_if_bad_alphabet_or_rules()
        tc = ToddCoxeter(congruence_kind.twosided, p)
        assert tc.number_of_classes() == factorial(i) // 2


def test_full_transformation_monoid_Aiz58():
    check_full_transformation_monoid(
        [4, 5], examples.full_transformation_monoid_Aiz58
    )


def test_full_transformation_monoid_II74():
    check_full_transformation_monoid(
        [4, 5], examples.full_transformation_monoid_II74
    )


def test_full_transformation_monoid_MW24_a():
    check_full_transformation_monoid(
        [4, 5], examples.full_transformation_monoid_MW24_a
    )


def test_full_transformation_monoid():
    check_full_transformation_monoid([2], examples.full_transformation_monoid)


def test_partial_transformation_monoid_Shu60():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.partial_transformation_monoid_Shu60(3)

    n = 5
    p = examples.partial_transformation_monoid_Shu60(n)
    p.throw_if_bad_alphabet_or_rules()

    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 7776


def test_partial_transformation_monoid_MW24():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.partial_transformation_monoid_MW24(1)

    n = 5
    p = examples.partial_transformation_monoid_MW24(n)
    p.throw_if_bad_alphabet_or_rules()

    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 7776


def test_symmetric_inverse_monoid_Shu60():
    check_symmetric_inverse_monoid(examples.symmetric_inverse_monoid_Shu60)


def test_symmetric_inverse_monoid_Sol04():
    check_symmetric_inverse_monoid(examples.symmetric_inverse_monoid_Sol04)


def test_symmetric_inverse_monoid_MW24():
    check_symmetric_inverse_monoid(examples.symmetric_inverse_monoid_MW24)


def test_dual_symmetric_inverse_monoid_EEF07():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.dual_symmetric_inverse_monoid_EEF07(2)

    p = examples.dual_symmetric_inverse_monoid_EEF07(5)

    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 6721


def test_uniform_block_bijection_monoid_Fit03():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.uniform_block_bijection_monoid_Fit03(2)

    p = examples.uniform_block_bijection_monoid_Fit03(6)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 22482


def test_partition_monoid_Eas11():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.partition_monoid_Eas11(3)

    p = examples.partition_monoid_Eas11(4)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 4140


def test_partition_monoid_HR05():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.partition_monoid_HR05(0)

    p = examples.partition_monoid_HR05(4)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 4140


def test_brauer_monoid_KM07():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.brauer_monoid_KM07(0)

    p = examples.brauer_monoid_KM07(6)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 10395


def test_rectangular_band_ACOR00():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.rectangular_band_ACOR00(1, 0)
        examples.rectangular_band_ACOR00(0, 1)

    p = examples.rectangular_band_ACOR00(4, 5)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 20

    p = examples.rectangular_band_ACOR00(15, 1)
    tc.init(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 15


def test_stellar_monoid_GH19():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.stellar_monoid_GH19(0)
        examples.stellar_monoid_GH19(1)

    # TODO(1) adapt for new Sims API
    # p = examples.stellar_monoid_GH19(4)
    # C = Sims1(congruence_kind.right)
    # C.short_rules(p)
    # assert C.number_of_congruences(3) == 79237


def test_chinese_monoid_CEKNH01():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.chinese_monoid_CEKNH01(0)
        examples.chinese_monoid_CEKNH01(1)

    # TODO(1) adapt for new Sims API
    # p = examples.chinese_monoid_CEKNH01(5)
    # C = Sims1(congruence_kind.right)
    # C.short_rules(p)

    # assert C.number_of_congruences(3) == 23504


def test_monogenic_semigroup():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.monogenic_semigroup(4, 0)
        examples.monogenic_semigroup(0, 0)

    p = examples.monogenic_semigroup(5, 9)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 13


def test_plactic_monoid_Knu70():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.plactic_monoid_Knu70(0)
        examples.plactic_monoid_Knu70(1)

    # TODO(1) adapt for new Sims API
    # p = examples.plactic_monoid_Knu70(4)
    # C = Sims1(congruence_kind.right)
    # C.short_rules(p)

    # assert C.number_of_congruences(4) == 106264


def test_stylic_monoid_AR22():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.stylic_monoid_AR22(0)
        examples.stylic_monoid_AR22(1)

    # TODO(1) adapt for new Sims API
    # p = examples.stylic_monoid_AR22(5)
    # C = Sims1(congruence_kind.right)
    # C.short_rules(p)

    # assert C.number_of_congruences(5) == 19201


def test_fibonacci_semigroup_CRRT94():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.fibonacci_semigroup_CRRT94(0, 1)
        examples.fibonacci_semigroup_CRRT94(1, 0)
        examples.fibonacci_semigroup_CRRT94(0, 0)

    p = examples.fibonacci_semigroup_CRRT94(2, 5)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 11


def test_temperley_lieb_monoid_Eas21():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.temperley_lieb_monoid_Eas21(0)
        examples.temperley_lieb_monoid_Eas21(1)
        examples.temperley_lieb_monoid_Eas21(2)

    p = examples.temperley_lieb_monoid_Eas21(8)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 1430


def test_singular_brauer_monoid_MM07():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.singular_brauer_monoid_MM07(0)
        examples.singular_brauer_monoid_MM07(1)
        examples.singular_brauer_monoid_MM07(2)

    p = examples.singular_brauer_monoid(6)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 9676


def test_orientation_preserving_monoid_AR00():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.orientation_preserving_monoid_AR00(0)
        examples.orientation_preserving_monoid_AR00(1)
        examples.orientation_preserving_monoid_AR00(2)

    p = examples.orientation_preserving_monoid_AR00(6)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 2742


def test_orientation_preserving_reversing_monoid_AR00():
    ReportGuard(False)
    with pytest.raises(LibsemigroupsError):
        examples.orientation_preserving_reversing_monoid_AR00(0)
        examples.orientation_preserving_reversing_monoid_AR00(1)
        examples.orientation_preserving_reversing_monoid_AR00(2)

    p = examples.orientation_preserving_reversing_monoid_AR00(5)
    tc = ToddCoxeter(congruence_kind.twosided, p)
    assert tc.number_of_classes() == 1015
