# Copyright (c) 2025, James D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring

"""This module contains some tests for the libsemigroups_pybind11 functionality
arising from hpcombi.*pp in libsemigroups.
"""

from copy import copy

import pytest

from libsemigroups_pybind11 import LIBSEMIGROUPS_HPCOMBI_ENABLED, LibsemigroupsError

if LIBSEMIGROUPS_HPCOMBI_ENABLED:
    from libsemigroups_pybind11.hpcombi import Perm16, PPerm16, PTransf16, Transf16, Vect16

    ########################################################################
    # Vect16
    ########################################################################

    def test_hpcombi_vect16_hash():
        d = {Vect16([0, 10]): 0}
        assert d[Vect16([0, 10])] == 0

    def test_hpcombi_vect16_copy():
        x = Vect16([1, 2, 0])
        assert copy(x) is not x
        assert x.copy() is not x
        assert x.copy() == copy(x)
        assert isinstance(x.copy(), Vect16)

    def test_hpcombi_vect16_construct_from_list():
        x = Vect16([0, 10])
        assert x[0] == 0
        assert x[1] == 10
        for i in range(2, 16):
            assert x[i] == 0
        with pytest.raises(LibsemigroupsError):
            x = Vect16([0] * 17)

    def test_hpcombi_vect16_set_item():
        x = Vect16([0] * 16)
        x[1] = 10
        assert x == Vect16([0, 10] + [0] * 14)

    def test_hpcombi_vect16_lt():
        x = Vect16([0])
        y = Vect16([1])
        assert x < y
        assert not x > y  # pylint: disable=unnecessary-negation
        assert not y < x  # pylint: disable=unnecessary-negation
        assert y > x

    def test_hpcombi_vect16_eq():
        x = Vect16([0])
        y = Vect16([1])
        assert x != y
        assert y != x
        assert x == Vect16([0])

    def test_hpcombi_vect16_size():
        assert len(Vect16()) == 16

    def test_hpcombi_vect16_first_diff():
        assert Vect16([1, 2, 3, 4, 255]).first_diff(Vect16([1, 2, 3, 4, 245])) == 4
        assert Vect16([1, 2, 3, 4, 255]).first_diff(Vect16([1, 2, 3, 4, 245]), 4) == 16
        assert Vect16().first_diff(Vect16()) == 16
        assert Vect16().first_diff(Vect16(), 12) == 16
        assert Vect16().first_diff(Vect16(), 100) == 16
        assert Vect16().first_diff(Vect16(), 256) == 16

    def test_hpcombi_vect16_last_diff():
        assert Vect16([1, 2, 3, 4, 255]).last_diff(Vect16([1, 2, 3, 4, 245])) == 4
        assert Vect16().last_diff(Vect16()) == 16

    def test_hpcombi_vect16_first_zero():
        assert Vect16([1, 2, 3, 4, 255]).first_zero() == 5
        assert Vect16().first_zero() == 0
        assert Vect16([1, 2, 3, 4, 255]).first_zero(3) == 16

    def test_hpcombi_vect16_last_zero():
        assert Vect16([1, 2, 3, 4, 255]).last_zero() == 15
        assert Vect16([1, 2, 3, 4, 255]).last_zero(3) == 16
        assert Vect16().last_zero() == 15

    def test_hpcombi_vect16_first_non_zero():
        assert Vect16([1, 2, 3, 4, 255]).first_non_zero() == 0
        assert Vect16([1, 2, 3, 4, 255]).first_non_zero(3) == 0
        assert Vect16().first_non_zero() == 16

    def test_hpcombi_vect16_last_non_zero():
        assert Vect16([1, 2, 3, 4, 255]).last_non_zero() == 4
        assert Vect16([1, 2, 3, 4, 255]).last_non_zero(3) == 2
        assert Vect16().last_non_zero() == 16

    def test_hpcombi_vect16_iterator():
        v = Vect16([1, 2, 3, 4, 255, 0, 10, 69, 0, 0, 66, 0, 0, 0, 43])
        assert list(v) == [1, 2, 3, 4, 255, 0, 10, 69, 0, 0, 66, 0, 0, 0, 43, 0]

    def test_hpcombi_vect16_less_partial():
        u = Vect16([0, 1, 2, 10])
        v = Vect16([0, 1, 2, 3])

        assert v.less_partial(u, 3) == 0
        assert v.less_partial(u, 4) == -7
        assert u.less_partial(v, 4) == 7
        assert u.less_partial(v, 16) == 7
        assert v.less_partial(u, 16) == -7

    def test_hpcombi_vect16_sum():
        assert Vect16([0, 1, 2, 3]).sum() == 6
        assert Vect16([0, 1, 2, 10]).sum() == 13
        assert Vect16([1, 2, 3, 255]).sum() == 5
        assert Vect16([0, 1, 2, 3]).sum() == 6

    def test_hpcombi_vect16_partial_sums():
        assert Vect16([0, 1, 2, 3]).partial_sums() == Vect16([0, 1, 3] + [6] * 13)
        assert Vect16([0, 1, 2, 255]).partial_sums() == Vect16([0, 1, 3] + [2] * 13)

    def test_hpcombi_vect16_eval16():
        assert Vect16([5, 5, 2, 5, 1, 6, 12, 4, 0, 3, 2, 11, 12, 13, 14, 15]).eval16() == Vect16(
            [1, 1, 2, 1, 1, 3, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1]
        )

    def test_hpcombi_vect16_is_permutation():
        assert not Vect16([5, 5, 2, 5, 1, 6, 12, 4, 0, 3, 2, 11, 12, 13, 14, 15]).is_permutation()
        assert not Vect16([1, 0, 2, 3, 4, 4]).is_permutation()
        assert Vect16([1, 0, 2, 3, 4] + list(range(5, 16))).is_permutation(4)
        assert Vect16([1, 0, 2, 3, 4] + list(range(5, 16))).is_permutation(5)
        assert Vect16([1, 0, 2, 3, 4] + list(range(5, 16))).is_permutation()

    ########################################################################
    # PTransf16
    ########################################################################

    def test_hpcombi_ptransf16_hash():
        d = {PTransf16([0, 10]): 0}
        assert d[PTransf16([0, 10])] == 0

    def test_hpcombi_ptransf16_copy():
        x = PTransf16([1, 2, 0])
        assert copy(x) is not x
        assert x.copy() is not x
        assert x.copy() == copy(x)
        assert isinstance(x.copy(), PTransf16)

    def test_hpcombi_ptransf16_construct_from_list():
        x = PTransf16([0, 10])
        assert x[0] == 0
        assert x[1] == 10
        for i in range(2, 16):
            assert x[i] == i

    def test_hpcombi_ptransf16_construct_dom_range():
        x = PTransf16([0, 10], [10, 0])
        assert x == PTransf16(
            [10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 255, 255, 255, 255]
        )

        with pytest.raises(LibsemigroupsError):
            x = PTransf16([0, 0], [10, 0])

        with pytest.raises(LibsemigroupsError):
            x = PTransf16([0, 17], [10, 0])

        with pytest.raises(LibsemigroupsError):
            x = PTransf16([0, 1], [10, 17])

        with pytest.raises(LibsemigroupsError):
            x = PTransf16([0, 1], [10])

        with pytest.raises(LibsemigroupsError):
            x = PTransf16(list(range(17)), list(range(17)))

    def test_hpcombi_ptransf16_set_item():
        x = PTransf16([0] * 16)
        x[1] = 10
        assert x == PTransf16([0, 10] + [0] * 14)

    def test_hpcombi_ptransf16_lt():
        x = PTransf16()
        y = PTransf16([1])
        assert x < y
        assert not x > y  # pylint: disable=unnecessary-negation
        assert not y < x  # pylint: disable=unnecessary-negation
        assert y > x

    def test_hpcombi_ptransf16_mul():
        x = PTransf16([1, 0, 1])
        y = PTransf16([0, 0, 2])
        assert y * x == PTransf16([1, 1, 1])

    def test_hpcombi_ptransf16_one():
        assert PTransf16.one() == PTransf16(list(range(16)))
        x = PTransf16([1, 0, 1])
        assert x.one() == PTransf16.one()
        assert isinstance(PTransf16.one(), PTransf16)

    def test_hpcombi_ptransf16_len():
        assert len(PTransf16()) == 16

    def test_hpcombi_ptransf16_validate():
        assert PTransf16.one().validate()
        assert PTransf16.one().validate(3)
        assert PTransf16([0, 1, 255]).validate(3)

    def test_hpcombi_ptransf16_image_mask():
        x = PTransf16([1, 0, 1])
        assert x.image_mask_ref(True) == Vect16([0, 0, 255] + [0] * 13)
        assert x.image_mask_ref(False) == Vect16([255, 255, 0] + [255] * 13)
        assert x.image_mask(True) == Vect16([0, 0, 255] + [0] * 13)
        assert x.image_mask(False) == Vect16([255, 255, 0] + [255] * 13)

    def test_hpcombi_ptransf16_image_bitset():
        x = PTransf16([1, 3, 1, 255, 10])
        assert x.image_bitset() == 65514
        assert bin(x.image_bitset()) == "0b1111111111101010"
        assert x.image_bitset() & 1 << 1
        assert x.image_bitset() & 1 << 3
        assert x.image_bitset() & 1 << 10

        assert bin(x.image_bitset(True)) == "0b10101"
        assert not x.image_bitset(True) & 1 << 1
        assert not x.image_bitset(True) & 1 << 3
        assert not x.image_bitset(True) & 1 << 10
        assert x.image_bitset(False) == x.image_bitset()

    def test_hpcombi_ptransf16_domain_mask():
        assert PTransf16([1, 0, 1]).domain_mask(True) == Vect16([0] * 16)
        assert PTransf16([1, 0, 1]).domain_mask(False) == Vect16([255] * 16)

    def test_hpcombi_ptransf16_domain_bitset():
        x = PTransf16([1, 3, 1, 255, 10])
        assert x.domain_bitset() == 65527
        assert bin(x.domain_bitset()) == "0b1111111111110111"
        assert x.domain_bitset() & 1
        assert x.domain_bitset() & 1 << 1
        assert x.domain_bitset() & 1 << 2
        assert x.domain_bitset() & 1 << 4

        assert bin(x.domain_bitset(True)) == "0b1000"
        assert not x.domain_bitset(True) & 1
        assert not x.domain_bitset(True) & 1 << 1
        assert not x.domain_bitset(True) & 1 << 2
        assert not x.domain_bitset(True) & 1 << 4

        assert x.domain_bitset(False) == x.domain_bitset()

    def test_hpcombi_ptransf16_right_one():
        x = PTransf16([1, 3, 1, 255, 10])
        assert x.image_bitset() == x.right_one().image_bitset()
        assert x * x.right_one() == x

    def test_hpcombi_ptransf16_left_one():
        x = PTransf16([1, 3, 1, 255, 10])
        assert x.domain_bitset() == x.left_one().domain_bitset()
        assert x.left_one() * x == x

    def test_hpcombi_ptransf16_rank():
        x = PTransf16([1, 3, 1, 255, 10])
        assert x.rank_ref() == 13
        assert x.rank() == 13
        assert x.rank_cmpestrm() == 13

    def test_hpcombi_ptransf16_fix_points_mask():
        x = PTransf16([1, 3, 1, 255, 10])
        assert x.fix_points_mask() == Vect16([0] * 5 + [255] * 11)
        assert x.fix_points_mask(True) == Vect16([255] * 5 + [0] * 11)
        assert x.one().fix_points_mask() == Vect16([255] * 16)
        assert x.one().fix_points_mask(True) == Vect16()

    def test_hpcombi_ptransf16_fix_points_bitset():
        x = PTransf16([1, 3, 2, 255, 10])
        assert x.fix_points_bitset() == 65508
        assert x.fix_points_bitset(False) == 65508
        assert x.fix_points_bitset(True) == 27
        assert bin(x.fix_points_bitset()) == "0b1111111111100100"
        assert bin(x.fix_points_bitset(True)) == "0b11011"

    def test_hpcombi_ptransf16_smallest_fix_point():
        assert PTransf16([1, 3, 2, 255, 10]).smallest_fix_point() == 2
        assert PTransf16([1, 3, 3, 255, 10]).smallest_fix_point() == 5

    def test_hpcombi_ptransf16_smallest_moved_point():
        assert PTransf16([1, 3, 2, 255, 10]).smallest_moved_point() == 0
        assert PTransf16.one().smallest_moved_point() == 255

    def test_hpcombi_ptransf16_largest_fix_point():
        assert PTransf16([1, 3, 2, 255, 10]).largest_fix_point() == 15
        assert PTransf16([1, 3, 3, 255, 10]).largest_fix_point() == 15
        assert PTransf16.one().largest_fix_point() == 15

    def test_hpcombi_ptransf16_largest_moved_point():
        assert PTransf16([1, 3, 2, 255, 10]).largest_moved_point() == 4
        # TODO update if
        # https://github.com/libsemigroups/HPCombi/issues/65
        # is ever resolved.
        assert PTransf16([255] * 16).largest_moved_point() == 15

    def test_hpcombi_ptransf_nb_fix_points():
        assert PTransf16([1, 3, 2, 255, 10]).nb_fix_points() == 12
        assert PTransf16.one().nb_fix_points() == 16

    ########################################################################
    # Transf16
    ########################################################################

    def test_hpcombi_transf16_hash():
        d = {Transf16([0, 10]): 0}
        assert d[Transf16([0, 10])] == 0

    def test_hpcombi_transf16_copy():
        x = Transf16([1, 2, 0])
        assert copy(x) is not x
        assert x.copy() is not x
        assert x.copy() == copy(x)
        assert isinstance(x.copy(), Transf16)

    def test_hpcombi_transf16_construct_from_list():
        x = Transf16([0, 10])
        assert x[0] == 0
        assert x[1] == 10
        for i in range(2, 16):
            assert x[i] == i
        with pytest.raises(LibsemigroupsError):
            x = Transf16([0] * 17)
        with pytest.raises(LibsemigroupsError):
            x = Transf16([255])
        with pytest.raises(TypeError):
            x = Transf16([300])

    def test_hpcombi_transf16_set_item():
        x = Transf16()
        x[1] = 10
        assert x == Transf16([0, 10] + [0] * 14)

    def test_hpcombi_transf16_to_from_ints():
        x = Transf16([0, 15])
        assert int(x) == 17863200012815736704
        assert Transf16(int(x)) == x

    def test_hpcombi_transf16_lt():
        x = Transf16()
        y = Transf16([1])
        assert x < y
        assert not x > y  # pylint: disable=unnecessary-negation
        assert not y < x  # pylint: disable=unnecessary-negation
        assert y > x

    def test_hpcombi_transf16_mul():
        x = Transf16([1, 0, 1])
        y = Transf16([0, 0, 2])
        assert y * x == Transf16([1, 1, 1])

    def test_hpcombi_transf16_one():
        assert Transf16.one() == Transf16(list(range(16)))
        assert isinstance(Transf16.one(), Transf16)

    ########################################################################
    # Perm16
    ########################################################################

    def test_hpcombi_perm_hash():
        d = {Perm16([1, 0]): 0}
        assert d[Perm16([1, 0])] == 0

    def test_hpcombi_perm16_copy():
        x = Perm16([1, 2, 0])
        assert copy(x) is not x
        assert x.copy() is not x
        assert x.copy() == copy(x)
        assert isinstance(x.copy(), Perm16)

    def test_hpcombi_perm16_one():
        assert Perm16.one() == Perm16(list(range(16)))
        x = Perm16([1, 2, 0])
        assert x.one() == Perm16.one()
        assert isinstance(x.one(), Perm16)

    def test_hpcombi_perm16_inverse():
        x = Perm16([1, 2, 0])
        assert x * x.inverse() == x.one() == x.inverse() * x
        assert x**-1 == x.inverse()

    def test_hpcombi_perm16_inverse_ref():
        x = Perm16([1, 2, 0])
        assert x * x.inverse_ref() == x.one() == x.inverse_ref() * x
        assert x**-1 == x.inverse_ref()

    def test_hpcombi_perm16_inverse_arr():
        x = Perm16([1, 2, 0])
        assert x * x.inverse_arr() == x.one() == x.inverse_arr() * x
        assert x**-1 == x.inverse_arr()

    def test_hpcombi_perm16_inverse_sort():
        x = Perm16([1, 2, 0])
        assert x * x.inverse_sort() == x.one() == x.inverse_sort() * x
        assert x**-1 == x.inverse_sort()

    def test_hpcombi_perm16_inverse_find():
        x = Perm16([1, 2, 0])
        assert x * x.inverse_find() == x.one() == x.inverse_find() * x
        assert x**-1 == x.inverse_find()

    def test_hpcombi_perm16_inverse_pow():
        x = Perm16([1, 2, 0])
        assert x * x.inverse_pow() == x.one() == x.inverse_pow() * x
        assert x**-1 == x.inverse_pow()

    def test_hpcombi_perm16_inverse_cycl():
        x = Perm16([1, 2, 0])
        assert x * x.inverse_cycl() == x.one() == x.inverse_cycl() * x
        assert x**-1 == x.inverse_cycl()

    def test_hpcombi_perm16_lehmer():
        assert Perm16([0, 3, 2, 4, 1]).lehmer() == Vect16([0, 2, 1, 1])

    def test_hpcombi_perm16_lehmer_ref():
        assert Perm16([0, 3, 2, 4, 1]).lehmer_ref() == Vect16([0, 2, 1, 1])

    def test_hpcombi_perm16_length():
        assert Perm16([0, 3, 2, 4, 1]).length() == 4

    def test_hpcombi_perm16_length_ref():
        assert Perm16([0, 3, 2, 4, 1]).length_ref() == 4

    def test_hpcombi_perm16_length_arr():
        assert Perm16([0, 3, 2, 4, 1]).length_arr() == 4

    def test_hpcombi_perm16_elementary_transposition():
        with pytest.raises(LibsemigroupsError):
            Perm16.elementary_transposition(16)
        assert [Perm16.elementary_transposition(i) for i in range(15)] == [
            Perm16([1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 2, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 3, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 4, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 5, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 6, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 7, 6, 8, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 8, 7, 9, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 9, 11, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 10, 12, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 11, 13, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 12, 14, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 13, 15]),
            Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 14]),
        ]

    def test_hpcombi_perm16_nb_descents():
        assert Perm16([0, 3, 2, 4, 1]).nb_descents() == 2

    def test_hpcombi_perm16_nb_descents_ref():
        assert Perm16([0, 3, 2, 4, 1]).nb_descents_ref() == 2

    def test_hpcombi_perm16_cycles_partition():
        assert Perm16(
            [1, 2, 3, 6, 0, 5, 4, 7, 8, 9, 10, 11, 12, 15, 14, 13]
        ).cycles_partition() == Vect16([0, 0, 0, 0, 0, 5, 0, 7, 8, 9, 10, 11, 12, 13, 14, 13])

    def test_hpcombi_perm16_nb_cycles():
        x = Perm16([1, 2, 3, 6, 0, 5, 4])
        assert x.nb_cycles() == 11

    def test_hpcombi_perm16_nb_cycles_ref():
        x = Perm16([1, 2, 3, 6, 0, 5, 4])
        assert x.nb_cycles_ref() == 11

    def test_hpcombi_perm16_nb_cycles_unroll():
        x = Perm16([1, 2, 3, 6, 0, 5, 4])
        assert x.nb_cycles_unroll() == 11

    def test_hpcombi_perm16_left_weak_leq():
        x, y = Perm16([2, 0, 3, 1]), Perm16([3, 0, 2, 1])
        assert x.left_weak_leq(y)
        assert y.left_weak_leq(y)
        assert not y.left_weak_leq(x)

    def test_hpcombi_perm16_left_weak_leq_ref():
        x, y = Perm16([2, 0, 3, 1]), Perm16([3, 0, 2, 1])
        assert x.left_weak_leq_ref(y)
        assert y.left_weak_leq_ref(y)
        assert not y.left_weak_leq_ref(x)

    def test_hpcombi_perm16_left_weak_leq_length():
        x, y = Perm16([2, 0, 3, 1]), Perm16([3, 0, 2, 1])
        assert x.left_weak_leq_length(y)
        assert y.left_weak_leq_length(y)
        assert not y.left_weak_leq_length(x)

    def test_hpcombi_perm16_unrankSJT():  # pylint: disable=invalid-name
        assert Perm16.unrankSJT(2) == Perm16([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 13, 14])

    ########################################################################
    # PPerm16
    ########################################################################

    def test_hpcombi_pperm16_hash():
        d = {PPerm16([1, 0]): 0}
        assert d[PPerm16([1, 0])] == 0

    def test_hpcombi_pperm16_copy():
        x = PPerm16([1, 2, 0])
        assert copy(x) is not x
        assert x.copy() is not x
        assert x.copy() == copy(x)
        assert isinstance(x.copy(), PPerm16)

    def test_hpcombi_pperm16_one():
        assert PPerm16.one() == PPerm16(list(range(16)))
        x = PPerm16([1, 2, 0])
        assert x.one() == PPerm16.one()
        assert isinstance(x.one(), PPerm16)

    def test_hpcombi_pperm16_init_from_list():
        with pytest.raises(LibsemigroupsError):
            assert PPerm16([255, 0, 255, 0])
        with pytest.raises(LibsemigroupsError):
            assert PPerm16([255, 0, 255, 17])
        with pytest.raises(TypeError):
            assert PPerm16([255, 0, 255, 300])
        x = PPerm16([255, 0, 255, 1])
        assert x == PPerm16([255, 0, 255, 1])

    def test_hpcombi_pperm16_init_from_dom_img():
        with pytest.raises(LibsemigroupsError):
            assert PPerm16([0, 0], [1, 2])
        with pytest.raises(LibsemigroupsError):
            assert PPerm16([1, 2], [0, 0])
        with pytest.raises(LibsemigroupsError):
            assert PPerm16([255, 0], [1, 2])
        with pytest.raises(LibsemigroupsError):
            assert PPerm16([1, 0], [1, 255])
        with pytest.raises(TypeError):
            assert PPerm16([1, 0], [1, 300])
        x = PPerm16([1, 0], [0, 1])
        assert x == PPerm16([1, 0], [0, 1])

    def test_hpcombi_pperm16_mult():
        x = PPerm16([1, 2], [3, 4])
        assert x * x == PPerm16([255] * 16)

    def test_hpcombi_pperm16_right_one():
        x = PPerm16([1, 3, 255, 10])
        assert x * x.right_one() == x

    def test_hpcombi_pperm16_left_one():
        x = PPerm16([1, 3, 255, 10])
        assert x.left_one() * x == x

    def test_hpcombi_pperm16_inverse_ref():
        x = PPerm16([1, 2, 0])
        assert x * x.inverse_ref() * x == x
        assert x.inverse_ref() * x * x.inverse_ref() == x.inverse_ref()
        assert x * x.inverse_ref() == x.left_one()
        assert x.inverse_ref() * x == x.right_one()
        assert x**-1 == x.inverse_ref()
