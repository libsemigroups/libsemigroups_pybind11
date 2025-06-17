# -*- coding: utf-8 -*-

# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for Congruence from libsemigroups_pybind11.
"""

# pylint: disable=missing-function-docstring

from datetime import timedelta

import pytest

from libsemigroups_pybind11 import (
    Congruence,
    Kambites,
    KnuthBendix,
    LibsemigroupsError,
    POSITIVE_INFINITY,
    Presentation,
    ReportGuard,
    StringRange,
    ToddCoxeter,
    congruence,
    congruence_kind,
    kambites,
    knuth_bendix,
    presentation,
)

from .cong_common import check_congruence_common_return_policy


def test_018():
    ReportGuard(False)
    p = Presentation([0, 1, 2])
    presentation.add_rule(p, [0, 0], [0, 0])
    presentation.add_rule(p, [0, 1], [1, 0])
    presentation.add_rule(p, [0, 2], [2, 0])
    presentation.add_rule(p, [0, 0], [0])
    presentation.add_rule(p, [0, 2], [0])
    presentation.add_rule(p, [2, 0], [0])
    presentation.add_rule(p, [1, 0], [0, 1])
    presentation.add_rule(p, [1, 1], [1, 1])
    presentation.add_rule(p, [1, 2], [2, 1])
    presentation.add_rule(p, [1, 1, 1], [1])
    presentation.add_rule(p, [1, 2], [1])
    presentation.add_rule(p, [2, 1], [1])

    cong = Congruence(congruence_kind.twosided, p)
    cong.add_generating_pair([0], [1])

    kb2 = cong.get(KnuthBendix)

    assert not cong.finished()
    assert cong.number_of_classes() == POSITIVE_INFINITY
    assert cong.finished()

    kb1 = KnuthBendix(congruence_kind.twosided, p)
    assert cong.has(KnuthBendix)
    assert not kb2.dead()
    assert kb2 is not cong.get(KnuthBendix)
    assert not kb2.dead()
    assert not kb2.finished()
    assert kb1.number_of_classes() == POSITIVE_INFINITY
    assert kb2.number_of_classes() == POSITIVE_INFINITY

    assert kb2.contains([0], [1])
    assert not cong.has(ToddCoxeter)
    assert not cong.has(Kambites)

    kb2 = cong.get(KnuthBendix)
    assert sorted(knuth_bendix.non_trivial_classes(kb1, kb2)) == [
        [[1], [0, 1], [1, 1], [0, 1, 1], [0]]
    ]
    # The next line does nothing except check that it's possible to call
    # `run_for` with a timedelta
    cong.run_for(timedelta(seconds=1))

    kb3 = kb2.copy()
    assert kb3.finished()
    assert kb3.contains([0], [1])
    assert sorted(knuth_bendix.non_trivial_classes(kb1, kb3)) == [
        [[1], [0, 1], [1, 1], [0, 1, 1], [0]]
    ]


def test_has_get():
    p = Presentation("ab")
    presentation.add_rule(p, "abab", "a" * 6)
    presentation.add_rule(p, "ba", "ababbb")

    c = Congruence(congruence_kind.twosided, p)

    assert c.number_of_runners() == 4
    assert c.has(ToddCoxeter)
    tc = c.get(ToddCoxeter)

    assert c.number_of_classes() == POSITIVE_INFINITY
    assert c.number_of_runners() == 1
    assert not c.has(ToddCoxeter)
    assert not c.has(Kambites)
    assert c.has(KnuthBendix)
    assert c.get(KnuthBendix).confluent()

    # Check that the ToddCoxeter returned above outlives its deletion in
    # Congruence
    assert tc.number_of_classes() == POSITIVE_INFINITY

    with pytest.raises(LibsemigroupsError):
        c.get(ToddCoxeter)


def test_normal_forms():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    cong = Congruence(congruence_kind.twosided, p)
    assert cong.number_of_classes() == POSITIVE_INFINITY
    k = cong.get(Kambites)
    it = kambites.normal_forms(k)
    assert [next(it) for _ in range(50)] == [
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "aa",
        "ab",
        "ac",
        "ad",
        "ae",
        "af",
        "ag",
        "ba",
        "bb",
        "bc",
        "bd",
        "be",
        "bf",
        "bg",
        "ca",
        "cb",
        "cc",
        "cd",
        "ce",
        "cf",
        "cg",
        "da",
        "db",
        "dc",
        "dd",
        "de",
        "df",
        "dg",
        "ea",
        "eb",
        "ec",
        "ed",
        "ee",
        "eg",
        "fa",
        "fb",
        "fc",
        "fd",
        "fe",
        "ff",
        "fg",
        "ga",
        "gb",
    ]


def test_partition():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    cong = Congruence(congruence_kind.twosided, p)
    assert cong.number_of_classes() == POSITIVE_INFINITY
    words = StringRange().alphabet(p.alphabet()).min(1).max(4)
    # TODO(1) allow passing the StringRange itself
    # codespell:begin-ignore
    assert congruence.partition(cong, list(words)) == [
        ["a"],
        ["b"],
        ["c"],
        ["d"],
        ["e"],
        ["f"],
        ["g"],
        ["aa"],
        ["ab"],
        ["ac"],
        ["ad"],
        ["ae"],
        ["af"],
        ["ag"],
        ["ba"],
        ["bb"],
        ["bc"],
        ["bd"],
        ["be"],
        ["bf"],
        ["bg"],
        ["ca"],
        ["cb"],
        ["cc"],
        ["cd"],
        ["ce"],
        ["cf"],
        ["cg"],
        ["da"],
        ["db"],
        ["dc"],
        ["dd"],
        ["de"],
        ["df"],
        ["dg", "ef"],
        ["ea"],
        ["eb"],
        ["ec"],
        ["ed"],
        ["ee"],
        ["eg"],
        ["fa"],
        ["fb"],
        ["fc"],
        ["fd"],
        ["fe"],
        ["ff"],
        ["fg"],
        ["ga"],
        ["gb"],
        ["gc"],
        ["gd"],
        ["ge"],
        ["gf"],
        ["gg"],
        ["aaa"],
        ["aab"],
        ["aac"],
        ["aad"],
        ["aae"],
        ["aaf"],
        ["aag"],
        ["aba"],
        ["abb"],
        ["abc"],
        ["abd"],
        ["abe"],
        ["abf"],
        ["abg"],
        ["aca"],
        ["acb"],
        ["acc"],
        ["acd"],
        ["ace"],
        ["acf"],
        ["acg"],
        ["ada"],
        ["adb"],
        ["adc"],
        ["add"],
        ["ade"],
        ["adf"],
        ["adg", "aef"],
        ["aea"],
        ["aeb"],
        ["aec"],
        ["aed"],
        ["aee"],
        ["aeg"],
        ["afa"],
        ["afb"],
        ["afc"],
        ["afd"],
        ["afe"],
        ["aff"],
        ["afg"],
        ["aga"],
        ["agb"],
        ["agc"],
        ["agd"],
        ["age"],
        ["agf"],
        ["agg"],
        ["baa"],
        ["bab"],
        ["bac"],
        ["bad"],
        ["bae"],
        ["baf"],
        ["bag"],
        ["bba"],
        ["bbb"],
        ["bbc"],
        ["bbd"],
        ["bbe"],
        ["bbf"],
        ["bbg"],
        ["bca"],
        ["bcb"],
        ["bcc"],
        ["bcd"],
        ["bce"],
        ["bcf"],
        ["bcg"],
        ["bda"],
        ["bdb"],
        ["bdc"],
        ["bdd"],
        ["bde"],
        ["bdf"],
        ["bdg", "bef"],
        ["bea"],
        ["beb"],
        ["bec"],
        ["bed"],
        ["bee"],
        ["beg"],
        ["bfa"],
        ["bfb"],
        ["bfc"],
        ["bfd"],
        ["bfe"],
        ["bff"],
        ["bfg"],
        ["bga"],
        ["bgb"],
        ["bgc"],
        ["bgd"],
        ["bge"],
        ["bgf"],
        ["bgg"],
        ["caa"],
        ["cab"],
        ["cac"],
        ["cad"],
        ["cae"],
        ["caf"],
        ["cag"],
        ["cba"],
        ["cbb"],
        ["cbc"],
        ["cbd"],
        ["cbe"],
        ["cbf"],
        ["cbg"],
        ["cca"],
        ["ccb"],
        ["ccc"],
        ["ccd"],
        ["cce"],
        ["ccf"],
        ["ccg"],
        ["cda"],
        ["cdb"],
        ["cdc"],
        ["cdd"],
        ["cde"],
        ["cdf"],
        ["cdg", "cef"],
        ["cea"],
        ["ceb"],
        ["cec"],
        ["ced"],
        ["cee"],
        ["ceg"],
        ["cfa"],
        ["cfb"],
        ["cfc"],
        ["cfd"],
        ["cfe"],
        ["cff"],
        ["cfg"],
        ["cga"],
        ["cgb"],
        ["cgc"],
        ["cgd"],
        ["cge"],
        ["cgf"],
        ["cgg"],
        ["daa"],
        ["dab"],
        ["dac"],
        ["dad"],
        ["dae"],
        ["daf"],
        ["dag"],
        ["dba"],
        ["dbb"],
        ["dbc"],
        ["dbd"],
        ["dbe"],
        ["dbf"],
        ["dbg"],
        ["dca"],
        ["dcb"],
        ["dcc"],
        ["dcd"],
        ["dce"],
        ["dcf"],
        ["dcg"],
        ["dda"],
        ["ddb"],
        ["ddc"],
        ["ddd"],
        ["dde"],
        ["ddf"],
        ["ddg", "def"],
        ["dea"],
        ["deb"],
        ["dec"],
        ["ded"],
        ["dee"],
        ["deg"],
        ["dfa"],
        ["dfb"],
        ["dfc"],
        ["dfd"],
        ["dfe"],
        ["dff"],
        ["dfg"],
        ["dga", "efa"],
        ["dgb", "efb"],
        ["dgc", "efc"],
        ["dgd", "efd"],
        ["dge", "efe"],
        ["dgf", "eff"],
        ["dgg", "efg"],
        ["eaa"],
        ["eab"],
        ["eac"],
        ["ead"],
        ["eae"],
        ["eaf"],
        ["eag"],
        ["eba"],
        ["ebb"],
        ["ebc"],
        ["ebd"],
        ["ebe"],
        ["ebf"],
        ["ebg"],
        ["eca"],
        ["ecb"],
        ["ecc"],
        ["ecd"],
        ["ece"],
        ["ecf"],
        ["ecg"],
        ["eda"],
        ["edb"],
        ["edc"],
        ["edd"],
        ["ede"],
        ["edf"],
        ["edg", "eef"],
        ["eea"],
        ["eeb"],
        ["eec"],
        ["eed"],
        ["eee"],
        ["eeg"],
        ["ega"],
        ["egb"],
        ["egc"],
        ["egd"],
        ["ege"],
        ["egf"],
        ["egg"],
        ["faa"],
        ["fab"],
        ["fac"],
        ["fad"],
        ["fae"],
        ["faf"],
        ["fag"],
        ["fba"],
        ["fbb"],
        ["fbc"],
        ["fbd"],
        ["fbe"],
        ["fbf"],
        ["fbg"],
        ["fca"],
        ["fcb"],
        ["fcc"],
        ["fcd"],
        ["fce"],
        ["fcf"],
        ["fcg"],
        ["fda"],
        ["fdb"],
        ["fdc"],
        ["fdd"],
        ["fde"],
        ["fdf"],
        ["fdg", "fef"],
        ["fea"],
        ["feb"],
        ["fec"],
        ["fed"],
        ["fee"],
        ["feg"],
        ["ffa"],
        ["ffb"],
        ["ffc"],
        ["ffd"],
        ["ffe"],
        ["fff"],
        ["ffg"],
        ["fga"],
        ["fgb"],
        ["fgc"],
        ["fgd"],
        ["fge"],
        ["fgf"],
        ["fgg"],
        ["gaa"],
        ["gab"],
        ["gac"],
        ["gad"],
        ["gae"],
        ["gaf"],
        ["gag"],
        ["gba"],
        ["gbb"],
        ["gbc"],
        ["gbd"],
        ["gbe"],
        ["gbf"],
        ["gbg"],
        ["gca"],
        ["gcb"],
        ["gcc"],
        ["gcd"],
        ["gce"],
        ["gcf"],
        ["gcg"],
        ["gda"],
        ["gdb"],
        ["gdc"],
        ["gdd"],
        ["gde"],
        ["gdf"],
        ["gdg", "gef"],
        ["gea"],
        ["geb"],
        ["gec"],
        ["ged"],
        ["gee"],
        ["geg"],
        ["gfa"],
        ["gfb"],
        ["gfc"],
        ["gfd"],
        ["gfe"],
        ["gff"],
        ["gfg"],
        ["gga"],
        ["ggb"],
        ["ggc"],
        ["ggd"],
        ["gge"],
        ["ggf"],
        ["ggg"],
    ]
    # codespell:end-ignore

    assert congruence.non_trivial_classes(cong, list(words)) == [
        ["dg", "ef"],
        ["adg", "aef"],
        ["bdg", "bef"],
        ["cdg", "cef"],
        ["ddg", "def"],
        ["dga", "efa"],
        ["dgb", "efb"],
        ["dgc", "efc"],
        ["dgd", "efd"],
        ["dge", "efe"],
        ["dgf", "eff"],
        ["dgg", "efg"],
        ["edg", "eef"],
        ["fdg", "fef"],
        ["gdg", "gef"],
    ]


def test_congruence_return_policy():
    c = check_congruence_common_return_policy(Congruence)

    assert c.max_threads(2) is c
