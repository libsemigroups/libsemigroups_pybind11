# -*- coding: utf-8 -*-
# Copyright (c) 2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the Presentation class.
"""

# pylint: disable=fixme, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    Kambites,
    ReportGuard,
    number_of_words,
    sislo,
)

###############################################################################
# Helper functions
###############################################################################


def number_of_subwords(lst):
    mp = {}
    for x in lst:
        w = x[0]
        for suffix in range(len(w)):
            for prefix in range(suffix + 1, len(w)):
                mp[w[suffix:prefix]] = True
        w = x[1]
        for suffix in range(len(w)):
            for prefix in range(suffix + 1, len(w)):
                mp[w[suffix:prefix]] = True
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
    k = Kambites()
    k.set_alphabet("abcdefg")
    k.add_rule("abcd", "aaaeaa")
    k.add_rule("ef", "dg")

    assert k.equal_to("abcd", "aaaeaa")
    assert k.equal_to("ef", "dg")
    assert k.equal_to("aaaaaef", "aaaaadg")
    assert k.equal_to("efababa", "dgababa")
    # k.froidure_pin().enumerate(100)
    # assert k.froidure_pin().current_size() == 8205)


def test_case_002():
    k = Kambites()
    k.set_alphabet("aAbBcCe")

    k.add_rule("aaa", "e")
    k.add_rule("bbb", "e")
    k.add_rule("ccc", "e")
    k.add_rule("ABa", "BaB")
    k.add_rule("bcB", "cBc")
    k.add_rule("caC", "aCa")
    k.add_rule("abcABCabcABCabcABC", "e")
    k.add_rule("BcabCABcabCABcabCA", "e")
    k.add_rule("cbACBacbACBacbACBa", "e")

    assert k.number_of_pieces(0) == 2
    assert k.number_of_pieces(1) == POSITIVE_INFINITY

    assert k.number_of_pieces(2) == 2
    assert k.number_of_pieces(3) == POSITIVE_INFINITY

    assert k.number_of_pieces(4) == 2
    assert k.number_of_pieces(5) == POSITIVE_INFINITY

    assert k.number_of_pieces(6) == 2
    assert k.number_of_pieces(7) == 2

    assert k.number_of_pieces(8) == 2
    assert k.number_of_pieces(9) == 2

    assert k.number_of_pieces(10) == 2
    assert k.number_of_pieces(11) == 2

    assert k.number_of_pieces(12) == 2
    assert k.number_of_pieces(13) == POSITIVE_INFINITY

    assert k.number_of_pieces(14) == 2
    assert k.number_of_pieces(15) == POSITIVE_INFINITY

    assert k.number_of_pieces(16) == 2
    assert k.number_of_pieces(17) == POSITIVE_INFINITY

    assert k.small_overlap_class() == 2


def test_case_004():

    for i in range(4, 20, 1):
        lhs = ""
        for b in range(1, i + 1, 1):
            lhs += "a" + "b" * b
        rhs = ""
        for b in range(i + 1, 2 * i + 1, 1):
            rhs += "a" + "b" * b

        k = Kambites()
        k.set_alphabet("ab")
        k.add_rule(lhs, rhs)

        assert k.number_of_pieces(0) == i
        assert k.number_of_pieces(1) == i + 1

        assert k.small_overlap_class() == i


def test_case_006_a():

    k = Kambites()
    k.set_alphabet("abcdefghi")
    k.add_rule(
        "eiehiegiggfaigcdfdfdgiidcebacgfaf",
        "cgfaeiehiegiggfaigcdfdfdgigcccbddchbbhgaaedfiiahhehihcba",
    )
    k.add_rule("hihcbaeiehiegiggfaigcdfdfdgiefhbidhbdgb", "chhfgafiiddg")
    k.add_rule(
        "gcccbddchbbhgaaedfiiahheidcebacbdefegcehgffedacddiaiih",
        "eddfcfhbedecacheahcdeeeda",
    )
    k.add_rule("dfbiccfeagaiffcfifg", "dceibahghaedhefh")

    assert k.small_overlap_class() == 4
    assert number_of_subwords(k.rules()) == 3762

    n = sum_lengths(k.rules())
    assert n == 254
    assert n * n == 64516


def test_case_006_b():
    k = Kambites()
    k.set_alphabet("abcdefghi")
    k.add_rule(
        "feffgccdgcfbeagiifheabecdfbgebfcibeifibccahaafabeihfgfieade"
        "bciheddeigbaf",
        "ifibccahaafabeihfgfiefeffgccdgcfbeagiifheabecfeibghddfgbaia"
        "acghhdhggagaide",
    )
    k.add_rule(
        "ghhdhggagaidefeffgccdgcfbeagiifheabeccbeiddgdcbcf", "ahccccffdeb"
    )
    k.add_rule("feibghddfgbaiaacdfbgebfcibeieaacdbdb", "gahdfgbghhhbcci")
    k.add_rule(
        "dgibafaahiabfgeiiibadebciheddeigbaficfbfdbfbbiddgdcifbe",
        "iahcfgdbggaciih",
    )
    assert k.small_overlap_class() == 4
    assert number_of_subwords(k.rules()) == 7197
    n = sum_lengths(k.rules())
    assert n == 327
    assert n * n == 106929


def test_case_006_c():
    k = Kambites()
    k.set_alphabet("abcdefghi")
    k.add_rule(
        "adichhbhibfchbfbbibaidfibifgagcgdedfeeibhggdbchfdaefbefcbaa"
        "hcbhbidgaahbahhahhb",
        "edfeeibhggdbchfdaefbeadichhbhibfchbfbbibaiihebabeabahcgdbic"
        "bgiciffhfggbfadf",
    )
    k.add_rule(
        "bgiciffhfggbfadfadichhbhibfchbfbbibaaggfdcfcebehhbdegiaeaf",
        "hebceeicbhidcgahhcfbb",
    )
    k.add_rule(
        "iihebabeabahcgdbicidfibifgagcgdedehed", "ecbcgaieieicdcdfdbgagdbf"
    )
    k.add_rule("iagaadbfcbaahcbhbidgaahbahhahhbd", "ddddh")
    assert k.small_overlap_class() == 3
    assert number_of_subwords(k.rules()) == 7408
    n = sum_lengths(k.rules())
    assert n == 330
    assert n * n == 108900


def test_case_006_d():
    k = Kambites()
    k.set_alphabet("abcdefghi")
    k.add_rule(
        "ibddgdgddiabcahbidbedffeddciiabahbbiacbfehdfccacbhgafbgcdg",
        "iabahibddgdgddbdfacbafhcgfhdheieihd",
    )
    k.add_rule("hdheieihdibddgdgddebhaeaicciidebegg", "giaeehdeeec")
    k.add_rule(
        "bdfacbafhcgfiabcahbidbedffeddcifdfcdcdadhhcbcbebhei", "icaebehdff"
    )
    k.add_rule(
        "aggiiacdbbiacbfehdfccacbhgafbgcdghiahfccdchaiagaha", "hhafbagbhghhihg"
    )

    assert k.small_overlap_class() == 4
    assert number_of_subwords(k.rules()) == 4560
    n = sum_lengths(k.rules())
    assert n == 265
    assert n * n == 70225


def test_case_006_e():
    k = Kambites()
    k.set_alphabet("abcdefghi")
    k.add_rule(
        "fibehffegdeggaddgfdaeaiacbhbgbbccceaibfcabbiedhecggbbdgihddd",
        "ceafibehffegdeggafidbaefcebegahcbhciheceaehaaehih",
    )
    k.add_rule("haaehihfibehffegdeggaecbedccaeabifeafi", "bfcccibgefiidgaih")
    k.add_rule(
        "fidbaefcebegahcbhciheceaeddgfdaeaiacbhbgbbcccgiahbibehgbgab"
        "efdieiggc",
        "abigdadaecdfdeeciggbdfdf",
    )
    k.add_rule(
        "eeaaiicigieiabibfcabbiedhecggbbdgihdddifadgbgidbfeg",
        "daheebdgdiaeceeiicddg",
    )
    assert k.small_overlap_class() == 4
    assert number_of_subwords(k.rules()) == 6398
    n = sum_lengths(k.rules())
    assert n == 328
    assert n * n == 107584


def test_case_008():

    ReportGuard(False)
    k = Kambites()
    k.set_alphabet("abcdefg")

    k.add_rule("abcd", "ce")
    k.add_rule("df", "dg")

    assert k.small_overlap_class() == POSITIVE_INFINITY
    assert k.is_obviously_infinite()

    assert k.equal_to("dfabcdf", "dfabcdg")
    assert k.equal_to("abcdf", "ceg")
    assert k.equal_to("abcdf", "cef")
    assert k.equal_to("dfabcdf", "dfabcdg")
    assert k.equal_to("abcdf", "ceg")
    assert k.equal_to("abcdf", "cef")
    assert k.normal_form("abcdfceg") == "abcdfabcdf"

    assert k.size() == POSITIVE_INFINITY
    assert number_of_words(len(k.alphabet()), 0, 6) == 19608
    assert k.number_of_normal_forms(0, 6) == 17921

    assert number_of_subwords(k.rules()) == 7


def test_case_010():

    ReportGuard(False)
    k = Kambites()
    k.set_alphabet("cab")

    k.add_rule("aabc", "acba")
    assert not k.equal_to("a", "b")
    assert k.equal_to("aabcabc", "aabccba")

    assert k.size() == POSITIVE_INFINITY
    assert number_of_words(3, 4, 16) == 21523320
    assert len(list(sislo("cab", "aabc", "aaabc"))) == 162
    assert (
        len([x for x in sislo("cab", "cccc", "ccccc") if k.equal_to(x, "acba")])
        == 2
    )
