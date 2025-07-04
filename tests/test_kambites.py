# -*- coding: utf-8 -*-
# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Kambites class.
"""

# pylint: disable=missing-function-docstring

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    Kambites,
    ReportGuard,
    number_of_words,
    Presentation,
    presentation,
    congruence_kind,
    ukkonen,
    is_obviously_infinite,
    kambites,
    StringRange,
)

from .cong_common import check_congruence_common_return_policy

###############################################################################
# Helper functions
###############################################################################


def number_of_subwords(lst):
    mp = {}
    for w in lst:
        for suffix in range(len(w)):
            for prefix in range(suffix + 1, len(w)):
                mp[w[suffix:prefix]] = True
    print(mp)
    return len(mp.keys())


def sum_lengths(lst):
    result = 0
    for x in lst:
        result += len(x[0])
        result += len(x[1])

    return result


###############################################################################
# Test functions begin
###############################################################################


def test_case_000():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    k = Kambites(congruence_kind.twosided, p)

    assert k.contains("abcd", "aaaeaa")
    assert k.contains("ef", "dg")
    assert k.contains("aaaaaef", "aaaaadg")
    assert k.contains("efababa", "dgababa")
    # TODO(1) move to tests for to_froidure_pin
    # k.froidure_pin().enumerate(100)
    # assert k.froidure_pin().current_size() == 8205)


def test_case_002():
    p = Presentation("aAbBcCe")

    presentation.add_rule(p, "aaa", "e")
    presentation.add_rule(p, "bbb", "e")
    presentation.add_rule(p, "ccc", "e")
    presentation.add_rule(p, "ABa", "BaB")
    presentation.add_rule(p, "bcB", "cBc")
    presentation.add_rule(p, "caC", "aCa")
    presentation.add_rule(p, "abcABCabcABCabcABC", "e")
    presentation.add_rule(p, "BcabCABcabCABcabCA", "e")
    presentation.add_rule(p, "cbACBacbACBacbACBa", "e")

    k = Kambites(congruence_kind.twosided, p)
    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[0]) == 2
    assert (
        ukkonen.number_of_pieces(k.ukkonen(), p.rules[1]) == POSITIVE_INFINITY
    )

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[2]) == 2
    assert (
        ukkonen.number_of_pieces(k.ukkonen(), p.rules[3]) == POSITIVE_INFINITY
    )

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[4]) == 2
    assert (
        ukkonen.number_of_pieces(k.ukkonen(), p.rules[5]) == POSITIVE_INFINITY
    )

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[6]) == 2
    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[7]) == 2

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[8]) == 2
    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[9]) == 2

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[10]) == 2
    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[11]) == 2

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[12]) == 2
    assert (
        ukkonen.number_of_pieces(k.ukkonen(), p.rules[13]) == POSITIVE_INFINITY
    )

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[14]) == 2
    assert (
        ukkonen.number_of_pieces(k.ukkonen(), p.rules[15]) == POSITIVE_INFINITY
    )

    assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[16]) == 2
    assert (
        ukkonen.number_of_pieces(k.ukkonen(), p.rules[17]) == POSITIVE_INFINITY
    )

    assert k.small_overlap_class() == 2


def test_case_004():
    for i in range(4, 20, 1):
        lhs = ""
        for b in range(1, i + 1, 1):
            lhs += "a" + "b" * b
        rhs = ""
        for b in range(i + 1, 2 * i + 1, 1):
            rhs += "a" + "b" * b

        p = Presentation("ab")
        presentation.add_rule(p, lhs, rhs)
        k = Kambites(congruence_kind.twosided, p)

        assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[0]) == i
        assert ukkonen.number_of_pieces(k.ukkonen(), p.rules[1]) == i + 1

        assert k.small_overlap_class() == i


def test_case_006_a():
    p = Presentation("abcdefghi")
    presentation.add_rule(
        p,
        "eiehiegiggfaigcdfdfdgiidcebacgfaf",
        "cgfaeiehiegiggfaigcdfdfdgigcccbddchbbhgaaedfiiahhehihcba",
    )
    presentation.add_rule(
        p, "hihcbaeiehiegiggfaigcdfdfdgiefhbidhbdgb", "chhfgafiiddg"
    )
    presentation.add_rule(
        p,
        "gcccbddchbbhgaaedfiiahheidcebacbdefegcehgffedacddiaiih",
        "eddfcfhbedecacheahcdeeeda",
    )
    presentation.add_rule(p, "dfbiccfeagaiffcfifg", "dceibahghaedhefh")

    k = Kambites(congruence_kind.twosided, p)

    assert k.small_overlap_class() == 4
    assert number_of_subwords(p.rules) == 3762

    n = presentation.length(p)
    assert n == 254
    assert n * n == 64516


def test_case_006_b():
    p = Presentation("abcdefghi")
    presentation.add_rule(
        p,
        "feffgccdgcfbeagiifheabecdfbgebfcibeifibccahaafabeihfgfieadebciheddeigbaf",
        "ifibccahaafabeihfgfiefeffgccdgcfbeagiifheabecfeibghddfgbaiaacghhdhggagaide",
    )
    presentation.add_rule(
        p, "ghhdhggagaidefeffgccdgcfbeagiifheabeccbeiddgdcbcf", "ahccccffdeb"
    )
    presentation.add_rule(
        p, "feibghddfgbaiaacdfbgebfcibeieaacdbdb", "gahdfgbghhhbcci"
    )
    presentation.add_rule(
        p,
        "dgibafaahiabfgeiiibadebciheddeigbaficfbfdbfbbiddgdcifbe",
        "iahcfgdbggaciih",
    )
    k = Kambites(congruence_kind.twosided, p)

    assert k.small_overlap_class() == 4
    assert number_of_subwords(p.rules) == 7197
    n = presentation.length(p)
    assert n == 327
    assert n * n == 106929


def test_case_006_c():
    p = Presentation("abcdefghi")
    presentation.add_rule(
        p,
        "adichhbhibfchbfbbibaidfibifgagcgdedfeeibhggdbchfdaefbefcbaahcbhbidgaahbahhahhb",
        "edfeeibhggdbchfdaefbeadichhbhibfchbfbbibaiihebabeabahcgdbicbgiciffhfggbfadf",
    )
    presentation.add_rule(
        p,
        "bgiciffhfggbfadfadichhbhibfchbfbbibaaggfdcfcebehhbdegiaeaf",
        "hebceeicbhidcgahhcfbb",
    )
    presentation.add_rule(
        p, "iihebabeabahcgdbicidfibifgagcgdedehed", "ecbcgaieieicdcdfdbgagdbf"
    )
    presentation.add_rule(p, "iagaadbfcbaahcbhbidgaahbahhahhbd", "ddddh")

    k = Kambites(congruence_kind.twosided, p)
    assert k.small_overlap_class() == 3
    assert number_of_subwords(p.rules) == 7408
    n = presentation.length(p)
    assert n == 330
    assert n * n == 108900


def test_case_006_d():
    p = Presentation("abcdefghi")
    presentation.add_rule(
        p,
        "ibddgdgddiabcahbidbedffeddciiabahbbiacbfehdfccacbhgafbgcdg",
        "iabahibddgdgddbdfacbafhcgfhdheieihd",
    )
    presentation.add_rule(
        p, "hdheieihdibddgdgddebhaeaicciidebegg", "giaeehdeeec"
    )
    presentation.add_rule(
        p, "bdfacbafhcgfiabcahbidbedffeddcifdfcdcdadhhcbcbebhei", "icaebehdff"
    )
    presentation.add_rule(
        p,
        "aggiiacdbbiacbfehdfccacbhgafbgcdghiahfccdchaiagaha",
        "hhafbagbhghhihg",
    )

    k = Kambites(congruence_kind.twosided, p)
    assert k.small_overlap_class() == 4
    assert number_of_subwords(p.rules) == 4560
    n = presentation.length(p)
    assert n == 265
    assert n * n == 70225


def test_case_006_e():
    p = Presentation("abcdefghi")
    presentation.add_rule(
        p,
        "fibehffegdeggaddgfdaeaiacbhbgbbccceaibfcabbiedhecggbbdgihddd",
        "ceafibehffegdeggafidbaefcebegahcbhciheceaehaaehih",
    )
    presentation.add_rule(
        p, "haaehihfibehffegdeggaecbedccaeabifeafi", "bfcccibgefiidgaih"
    )
    presentation.add_rule(
        p,
        "fidbaefcebegahcbhciheceaeddgfdaeaiacbhbgbbcccgiahbibehgbgabefdieiggc",
        "abigdadaecdfdeeciggbdfdf",
    )
    presentation.add_rule(
        p,
        "eeaaiicigieiabibfcabbiedhecggbbdgihdddifadgbgidbfeg",
        "daheebdgdiaeceeiicddg",
    )

    k = Kambites(congruence_kind.twosided, p)
    assert k.small_overlap_class() == 4
    assert number_of_subwords(p.rules) == 6398
    n = presentation.length(p)
    assert n == 328
    assert n * n == 107584


def test_case_008():
    ReportGuard(False)
    p = Presentation("abcdefg")

    presentation.add_rule(p, "abcd", "ce")
    presentation.add_rule(p, "df", "dg")

    k = Kambites(congruence_kind.twosided, p)

    assert k.small_overlap_class() == POSITIVE_INFINITY
    assert is_obviously_infinite(k)

    assert k.contains("dfabcdf", "dfabcdg")
    assert k.contains("abcdf", "ceg")
    assert k.contains("abcdf", "cef")
    assert k.contains("dfabcdf", "dfabcdg")
    assert k.contains("abcdf", "ceg")
    assert k.contains("abcdf", "cef")
    assert k.reduce("abcdfceg") == "abcdfabcdf"

    assert k.number_of_classes() == POSITIVE_INFINITY
    assert number_of_words(len(p.alphabet()), 0, 6) == 19608

    nf = kambites.normal_forms(k)
    count = 0
    while len(next(nf)) < 6:
        count += 1
    assert count == 17921

    assert number_of_subwords(p.rules) == 7


def test_case_010():
    ReportGuard(False)
    p = Presentation("cab")

    presentation.add_rule(p, "aabc", "acba")

    k = Kambites(congruence_kind.twosided, p)
    assert not k.contains("a", "b")
    assert k.contains("aabcabc", "aabccba")

    assert k.number_of_classes() == POSITIVE_INFINITY
    assert number_of_words(3, 4, 16) == 21523320

    strings = StringRange().alphabet("cab").first("aabc").last("aaabc")
    assert len(strings) == 81
    strings = StringRange().alphabet("cab").first("cccc").last("ccccc")
    assert sum(1 for x in strings if k.contains(x, "acba")) == 2


def test_kambites_return_policy():
    k = check_congruence_common_return_policy(Kambites)

    assert k.ukkonen() is k.ukkonen()
