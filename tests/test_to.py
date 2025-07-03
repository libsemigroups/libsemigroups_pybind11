# -*- coding: utf-8 -*-
# Copyright (c) 2025 Joseph Edwards
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=missing-function-docstring, invalid-name

"""
This module contains some tests for the to function.
"""

import pytest

# TODO(1) be good to remove the imports from _libsemigroups_pybind11, but
# couldn't immediately figure out how to.
from _libsemigroups_pybind11 import (  # pylint: disable=no-name-in-module
    FroidurePinKBEStringRewriteFromLeft,
    FroidurePinKBEStringRewriteTrie,
    FroidurePinKBEWordRewriteFromLeft,
    FroidurePinKBEWordRewriteTrie,
    FroidurePinKEMultiViewString,
    FroidurePinKEString,
    FroidurePinKEWord,
    FroidurePinTCE,
    PresentationString,
    PresentationWord,
)

from libsemigroups_pybind11 import (
    Bipartition,
    congruence_kind,
    Congruence,
    FroidurePin,
    InversePresentation,
    Kambites,
    KnuthBendix,
    presentation,
    Presentation,
    ReportGuard,
    to,
    ToddCoxeter,
    Transf,
    WordGraph,
)
from libsemigroups_pybind11.detail.cxx_wrapper import to_cxx


ReportGuard(False)

###############################################################################
# Helper functions
###############################################################################


def sample_pres(Word):
    if Word is not str:
        p = Presentation([0, 1])
        presentation.add_rule(p, [0, 1], [1, 0])
        presentation.add_rule(p, [0, 0], [0])
        presentation.add_rule(p, [1, 1], [1])
        return p

    p = Presentation("ab")
    presentation.add_rule(p, "ab", "ba")
    presentation.add_rule(p, "aa", "a")
    presentation.add_rule(p, "bb", "b")
    return p


def sample_to_str(i):
    return "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM9876543210"[i]


def sample_to_int(x):
    return (
        "mnbvcxzlkjhgfdsapoiuytrewqMNBVCXZLKJHGFDSAPOIUYTREWQ5432167890".index(
            x
        )
    )


def sample_froidure_pin():
    b1 = Bipartition([[1, -1], [2, -2], [3, -3], [4, -4]])
    b2 = Bipartition([[1, -2], [2, -3], [3, -4], [4, -1]])
    b3 = Bipartition([[1, -2], [2, -1], [3, -3], [4, -4]])
    b4 = Bipartition([[1, 2], [3, -3], [4, -4], [-1, -2]])
    S = FroidurePin(b1, b2, b3, b4)
    return S


def cong_from_sample_pres(ReturnType, Word, **kwargs):
    p = sample_pres(Word)
    return ReturnType(congruence_kind.twosided, p, **kwargs)


def check_cong_to_froidure_pin(Type, Word, **kwargs):
    thing = cong_from_sample_pres(Type, Word, **kwargs)
    fp = to(thing, rtype=(FroidurePin,))
    fp.run()
    assert fp.is_finite()
    assert fp.number_of_idempotents() == 3
    assert isinstance(fp, FroidurePin)
    return fp


def check_cong_to_todd_coxeter(Type, Word, **kwargs):
    thing = cong_from_sample_pres(Type, Word, **kwargs)
    tc = to(congruence_kind.twosided, thing, rtype=(ToddCoxeter,))
    tc.run()
    assert tc.number_of_classes() == 3
    return tc


def check_knuth_bendix_to_pres(WordIn, WordOut, Rewriter):
    p = sample_pres(WordIn)
    kb = KnuthBendix(congruence_kind.twosided, p, rewriter=Rewriter)
    q = to(kb, rtype=(Presentation, WordOut))

    assert len(q.rules) == kb.number_of_active_rules() * 2

    if WordIn is WordOut:
        assert q == to(kb, rtype=(Presentation,))
        presentation.sort_each_rule(p)
        presentation.sort_each_rule(q)
        presentation.sort_rules(p)
        presentation.sort_rules(q)
        # This is because sample_pres is already confluent
        assert p == q
    elif WordOut is str:
        assert isinstance(to_cxx(q), PresentationString)
        assert q.alphabet() == "ab"
    else:
        assert isinstance(to_cxx(q), PresentationWord)
        assert q.alphabet() == [0, 1]


def check_froidure_pin_to_pres(Word):
    S = sample_froidure_pin()
    assert S.size() == 105

    p = to(S, rtype=(Presentation, Word))
    assert len(p.alphabet()) == 4
    assert len(p.rules) == 86
    assert isinstance(p, Presentation)


def check_froidure_pin_to_knuth_bendix(Word, Rewriter):
    S = sample_froidure_pin()
    kb = to(congruence_kind.twosided, S, rtype=(KnuthBendix, Word, Rewriter))
    assert S.size() == kb.number_of_classes()
    return kb


def check_todd_coxeter_to_knuth_bendix(Word, Rewriter):
    tc = cong_from_sample_pres(ToddCoxeter, Word)
    kb = to(congruence_kind.twosided, tc, rtype=(KnuthBendix, Rewriter))
    assert kb.number_of_classes() == tc.number_of_classes()
    return kb


def check_todd_coxeter_to_knuth_bendix_default(Word):
    tc = cong_from_sample_pres(ToddCoxeter, Word)
    kb = to(congruence_kind.twosided, tc, rtype=(KnuthBendix,))
    assert kb.number_of_classes() == tc.number_of_classes()
    return kb


def check_froidure_pin_to_congruence(Word):
    S = sample_froidure_pin()
    cong = to(
        congruence_kind.twosided,
        S,
        S.right_cayley_graph(),
        rtype=(Congruence, Word),
    )
    cong.run()
    assert S.size() == cong.number_of_classes()
    return cong


################################################################################
# FroidurePin
################################################################################

# From KnuthBendix


def test_to_FroidurePin_000():
    fp = check_cong_to_froidure_pin(
        KnuthBendix, str, rewriter="RewriteFromLeft"
    )
    assert isinstance(to_cxx(fp), FroidurePinKBEStringRewriteFromLeft)


def test_to_FroidurePin_001():
    fp = check_cong_to_froidure_pin(KnuthBendix, str, rewriter="RewriteTrie")
    assert isinstance(to_cxx(fp), FroidurePinKBEStringRewriteTrie)


def test_to_FroidurePin_002():
    fp = check_cong_to_froidure_pin(
        KnuthBendix, int, rewriter="RewriteFromLeft"
    )
    assert isinstance(to_cxx(fp), FroidurePinKBEWordRewriteFromLeft)


def test_to_FroidurePin_003():
    fp = check_cong_to_froidure_pin(KnuthBendix, int, rewriter="RewriteTrie")
    assert isinstance(to_cxx(fp), FroidurePinKBEWordRewriteTrie)


# From ToddCoxeter


def test_to_FroidurePin_004():
    fp = check_cong_to_froidure_pin(ToddCoxeter, str)
    assert isinstance(to_cxx(fp), FroidurePinTCE)


def test_to_FroidurePin_005():
    fp = check_cong_to_froidure_pin(ToddCoxeter, int)
    assert isinstance(to_cxx(fp), FroidurePinTCE)


# From Congruence


def test_to_FroidurePin_006():
    check_cong_to_froidure_pin(Congruence, str)


def test_to_FroidurePin_007():
    check_cong_to_froidure_pin(Congruence, int)


# From Kambites


def test_to_FroidurePin_008():
    k = Kambites(word=str)
    fp = to(k, rtype=(FroidurePin,))
    assert isinstance(fp, FroidurePin)


def test_to_FroidurePin_009():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    k = Kambites(congruence_kind.twosided, p)

    fp = to(k, rtype=(FroidurePin,))
    fp.enumerate(100)
    assert fp.current_size() == 8205
    assert isinstance(to_cxx(fp), FroidurePinKEMultiViewString)
    assert isinstance(fp, FroidurePin)


def test_to_FroidurePin_010():
    p = Presentation([0, 1, 2, 3, 4, 5, 6])
    presentation.add_rule(p, [0, 1, 2, 3], [0, 0, 0, 4, 0, 0])
    presentation.add_rule(p, [4, 5], [3, 6])
    k = Kambites(congruence_kind.twosided, p)

    fp = to(k, rtype=(FroidurePin,))
    fp.enumerate(100)
    assert fp.current_size() == 8205
    assert isinstance(to_cxx(fp), FroidurePinKEWord)
    assert isinstance(fp, FroidurePin)


def test_to_FroidurePin_011():
    p = Presentation("abcdefg")
    presentation.add_rule(p, "abcd", "aaaeaa")
    presentation.add_rule(p, "ef", "dg")
    # Kambites wins here, so this checks FroidurePinKEString
    c = Congruence(congruence_kind.twosided, p)

    fp = to(c, rtype=(FroidurePin,))
    fp.enumerate(100)
    assert fp.current_size() == 8205
    assert isinstance(to_cxx(fp), FroidurePinKEString)
    assert isinstance(fp, FroidurePin)


# From WordGraph


def test_to_FroidurePin_012():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, rtype=(FroidurePin,))
    fp.run()
    assert fp.number_of_rules() == 1
    assert isinstance(fp, FroidurePin)


def test_to_FroidurePin_013():
    w = WordGraph(3, 1)
    w.target(0, 0, 1)
    w.target(1, 0, 1)
    w.target(2, 0, 1)

    fp = to(w, 1, 2, rtype=(FroidurePin,))
    fp.run()
    assert fp.number_of_rules() == 1
    assert isinstance(fp, FroidurePin)


################################################################################
# ToddCoxeter
################################################################################

# From KnuthBendix


def test_to_ToddCoxeter_014():
    tc = check_cong_to_todd_coxeter(
        KnuthBendix, str, rewriter="RewriteFromLeft"
    )
    assert isinstance(tc, ToddCoxeter)
    assert tc.py_template_params == (str,)


def test_to_ToddCoxeter_015():
    tc = check_cong_to_todd_coxeter(KnuthBendix, str, rewriter="RewriteTrie")
    assert isinstance(tc, ToddCoxeter)
    assert tc.py_template_params == (str,)


def test_to_ToddCoxeter_016():
    tc = check_cong_to_todd_coxeter(
        KnuthBendix, int, rewriter="RewriteFromLeft"
    )
    assert isinstance(tc, ToddCoxeter)
    assert tc.py_template_params == (list[int],)


def test_to_ToddCoxeter_017():
    tc = check_cong_to_todd_coxeter(KnuthBendix, int, rewriter="RewriteTrie")
    assert isinstance(tc, ToddCoxeter)
    assert tc.py_template_params == (list[int],)


# From FroidurePin


def test_to_ToddCoxeter_018():
    S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
    tc = to(
        congruence_kind.twosided,
        S,
        S.right_cayley_graph(),
        rtype=(ToddCoxeter, str),
    )
    assert tc.current_word_graph().number_of_nodes() == S.size() + 1
    assert isinstance(tc, ToddCoxeter)


def test_to_ToddCoxeter_019():
    S = FroidurePin(Transf([1, 3, 4, 2, 3]), Transf([3, 2, 1, 3, 3]))
    tc = to(
        congruence_kind.twosided,
        S,
        S.right_cayley_graph(),
        rtype=(ToddCoxeter, list[int]),
    )
    assert tc.current_word_graph().number_of_nodes() == S.size() + 1
    assert isinstance(tc, ToddCoxeter)


###############################################################################
# Presentation
###############################################################################

# From Presentation


def test_to_Presentation_020():
    p = Presentation("abcdef")
    presentation.add_zero_rules(p, "e")
    assert p.rules == [
        "ae",
        "e",
        "ea",
        "e",
        "be",
        "e",
        "eb",
        "e",
        "ce",
        "e",
        "ec",
        "e",
        "de",
        "e",
        "ed",
        "e",
        "ee",
        "e",
        "fe",
        "e",
        "ef",
        "e",
    ]
    assert p == to(p, rtype=(Presentation, str))
    q = to(p, rtype=(Presentation, list[int]))
    assert isinstance(q, Presentation)
    assert q.alphabet() == [0, 1, 2, 3, 4, 5]
    assert q.rules == [
        [0, 4],
        [4],
        [4, 0],
        [4],
        [1, 4],
        [4],
        [4, 1],
        [4],
        [2, 4],
        [4],
        [4, 2],
        [4],
        [3, 4],
        [4],
        [4, 3],
        [4],
        [4, 4],
        [4],
        [5, 4],
        [4],
        [4, 5],
        [4],
    ]
    assert p == to(q, rtype=(Presentation, str))


# From func + Presentation


def test_to_Presentation_021():
    p = Presentation("abcde")
    presentation.add_zero_rules(p, "e")
    assert p.rules == [
        "ae",
        "e",
        "ea",
        "e",
        "be",
        "e",
        "eb",
        "e",
        "ce",
        "e",
        "ec",
        "e",
        "de",
        "e",
        "ed",
        "e",
        "ee",
        "e",
    ]
    with pytest.raises(TypeError):
        q = to(p, sample_to_str, rtype=(Presentation, str))

    q = to(p, sample_to_int, rtype=(Presentation, list[int]))
    assert isinstance(q, Presentation)
    assert q.alphabet() == [15, 2, 4, 13, 23]
    assert q.rules == [
        [15, 23],
        [23],
        [23, 15],
        [23],
        [2, 23],
        [23],
        [23, 2],
        [23],
        [4, 23],
        [23],
        [23, 4],
        [23],
        [13, 23],
        [23],
        [23, 13],
        [23],
        [23, 23],
        [23],
    ]

    with pytest.raises(TypeError):
        r = to(q, sample_to_int, rtype=(Presentation, list[int]))

    r = to(q, sample_to_str, rtype=(Presentation, str))
    assert isinstance(r, Presentation)
    assert r.alphabet() == "hetfb"
    assert r.rules == [
        "hb",
        "b",
        "bh",
        "b",
        "eb",
        "b",
        "be",
        "b",
        "tb",
        "b",
        "bt",
        "b",
        "fb",
        "b",
        "bf",
        "b",
        "bb",
        "b",
    ]


def test_to_Presentation_022():
    p = Presentation("abcde")
    presentation.add_zero_rules(p, "e")
    assert p.rules == [
        "ae",
        "e",
        "ea",
        "e",
        "be",
        "e",
        "eb",
        "e",
        "ce",
        "e",
        "ec",
        "e",
        "de",
        "e",
        "ed",
        "e",
        "ee",
        "e",
    ]
    q = to(p, lambda x: chr(ord(x) + 10), rtype=(Presentation, str))
    assert q.alphabet() == "klmno"
    assert q.rules == [
        "ko",
        "o",
        "ok",
        "o",
        "lo",
        "o",
        "ol",
        "o",
        "mo",
        "o",
        "om",
        "o",
        "no",
        "o",
        "on",
        "o",
        "oo",
        "o",
    ]


def test_to_Presentation_023():
    p = Presentation([0, 1, 2, 3, 4])
    presentation.add_zero_rules(p, 4)
    assert p.rules == [
        [0, 4],
        [4],
        [4, 0],
        [4],
        [1, 4],
        [4],
        [4, 1],
        [4],
        [2, 4],
        [4],
        [4, 2],
        [4],
        [3, 4],
        [4],
        [4, 3],
        [4],
        [4, 4],
        [4],
    ]
    q = to(p, lambda x: x + 10, rtype=(Presentation, list[int]))
    assert q.alphabet() == [10, 11, 12, 13, 14]
    assert q.rules == [
        [10, 14],
        [14],
        [14, 10],
        [14],
        [11, 14],
        [14],
        [14, 11],
        [14],
        [12, 14],
        [14],
        [14, 12],
        [14],
        [13, 14],
        [14],
        [14, 13],
        [14],
        [14, 14],
        [14],
    ]


# From KnuthBendix


def test_to_Presentation_024():
    check_knuth_bendix_to_pres(str, str, "RewriteFromLeft")
    check_knuth_bendix_to_pres(str, list[int], "RewriteFromLeft")


def test_to_Presentation_025():
    check_knuth_bendix_to_pres(str, str, "RewriteTrie")
    check_knuth_bendix_to_pres(str, list[int], "RewriteTrie")


def test_to_Presentation_026():
    check_knuth_bendix_to_pres(list[int], str, "RewriteFromLeft")
    check_knuth_bendix_to_pres(list[int], list[int], "RewriteFromLeft")


def test_to_Presentation_027():
    check_knuth_bendix_to_pres(list[int], str, "RewriteTrie")
    check_knuth_bendix_to_pres(list[int], list[int], "RewriteTrie")


# From FroidurePin


def test_to_Presentation_028():
    check_froidure_pin_to_pres(str)


def test_to_Presentation_029():
    check_froidure_pin_to_pres(list[int])


###############################################################################
# InversePresentation
###############################################################################

# From InversePresentation


def test_to_InversePresentation_030():
    ip = InversePresentation("abc")
    ip.inverses("cba")
    presentation.add_rule(ip, "aaa", "b")
    presentation.add_rule(ip, "bac", "cab")
    assert ip == to(ip, rtype=(InversePresentation, str))

    iq = to(ip, rtype=(InversePresentation, list[int]))
    assert iq.alphabet() == [0, 1, 2]
    assert iq.inverses() == [2, 1, 0]
    assert iq.rules == [[0, 0, 0], [1], [1, 0, 2], [2, 0, 1]]

    ir = to(iq, rtype=(InversePresentation, str))
    assert ir == ip
    assert iq == to(ir, rtype=(InversePresentation, list[int]))
    assert ip == to(iq, rtype=(Presentation, str))
    assert iq == to(ir, rtype=(Presentation, list[int]))


# From function + InversePresentation


def test_to_InversePresentation_031():
    ip = InversePresentation("abc")
    ip.inverses("cba")
    presentation.add_rule(ip, "aaa", "b")
    presentation.add_rule(ip, "bac", "cab")

    iq = to(ip, sample_to_int, rtype=(InversePresentation, list[int]))
    assert iq.alphabet() == [15, 2, 4]
    assert iq.inverses() == [4, 2, 15]
    assert iq.rules == [[15, 15, 15], [2], [2, 15, 4], [4, 15, 2]]

    ir = to(iq, sample_to_str, rtype=(InversePresentation, str))
    assert ir.alphabet() == "het"
    assert ir.inverses() == "teh"  # codespell:ignore teh
    assert ir.rules == ["hhh", "e", "eht", "the"]

    iu = to(ip, lambda x: chr(ord(x) + 11), rtype=(InversePresentation, str))
    assert iu.alphabet() == "lmn"
    assert iu.inverses() == "nml"
    assert iu.rules == ["lll", "m", "mln", "nlm"]

    iv = to(iq, lambda x: x + 11, rtype=(InversePresentation, list[int]))
    assert iv.alphabet() == [26, 13, 15]
    assert iv.inverses() == [15, 13, 26]
    assert iv.rules == [[26, 26, 26], [13], [13, 26, 15], [15, 26, 13]]


# From Presentation


def test_to_InversePresentation_032():
    p = Presentation("abc")
    presentation.add_rule(p, "aaa", "b")
    presentation.add_rule(p, "bac", "cab")
    ip = to(p, rtype=(InversePresentation,))
    assert ip.alphabet() == "abcdef"
    assert ip.inverses() == "defabc"
    assert ip.rules == p.rules

    q = to(p, rtype=(Presentation, list[int]))
    iq = to(q, rtype=(InversePresentation,))
    assert iq.alphabet() == [0, 1, 2, 3, 4, 5]
    assert iq.inverses() == [3, 4, 5, 0, 1, 2]
    assert iq.rules == q.rules

    assert to(
        to(p, rtype=(Presentation, list[int])), rtype=(InversePresentation,)
    ) == to(
        to(p, rtype=(InversePresentation,)), rtype=(Presentation, list[int])
    )

    assert to(
        to(q, rtype=(Presentation, str)), rtype=(InversePresentation,)
    ) == to(to(q, rtype=(InversePresentation,)), rtype=(Presentation, str))


###############################################################################
# KnuthBendix
###############################################################################

# From FroidurePin


def test_to_KnuthBendix_033():
    kb = check_froidure_pin_to_knuth_bendix(str, "RewriteFromLeft")
    assert isinstance(kb, KnuthBendix)
    assert kb.py_template_params == (str, "RewriteFromLeft")


def test_to_KnuthBendix_034():
    kb = check_froidure_pin_to_knuth_bendix(str, "RewriteTrie")
    assert isinstance(kb, KnuthBendix)


def test_to_KnuthBendix_035():
    kb = check_froidure_pin_to_knuth_bendix(list[int], "RewriteFromLeft")
    assert isinstance(kb, KnuthBendix)


def test_to_KnuthBendix_036():
    kb = check_froidure_pin_to_knuth_bendix(list[int], "RewriteTrie")
    assert isinstance(kb, KnuthBendix)


# From ToddCoxeter + Rewriter


def test_to_KnuthBendix_037():
    kb = check_todd_coxeter_to_knuth_bendix(str, "RewriteFromLeft")
    assert isinstance(kb, KnuthBendix)


def test_to_KnuthBendix_038():
    kb = check_todd_coxeter_to_knuth_bendix(str, "RewriteTrie")
    assert isinstance(kb, KnuthBendix)


def test_to_KnuthBendix_039():
    kb = check_todd_coxeter_to_knuth_bendix(list[int], "RewriteFromLeft")
    assert isinstance(kb, KnuthBendix)


def test_to_KnuthBendix_040():
    kb = check_todd_coxeter_to_knuth_bendix(list[int], "RewriteTrie")
    assert isinstance(kb, KnuthBendix)


# From ToddCoxeter


def test_to_KnuthBendix_041():
    kb = check_todd_coxeter_to_knuth_bendix_default(str)
    # RewriteTrie is the default rewriter
    assert isinstance(kb, KnuthBendix)


def test_to_KnuthBendix_042():
    kb = check_todd_coxeter_to_knuth_bendix_default(list[int])
    # RewriteTrie is the default rewriter
    assert isinstance(kb, KnuthBendix)


###############################################################################
# Congruence
###############################################################################

# From FroidurePin


def test_to_Congruence_043():
    c = check_froidure_pin_to_congruence(str)
    assert isinstance(c, Congruence)
    assert c.py_template_params == (str,)


def test_to_Congruence_044():
    c = check_froidure_pin_to_congruence(list[int])
    assert isinstance(c, Congruence)
    assert c.py_template_params == (list[int],)


###############################################################################
# Exceptions
###############################################################################


def test_to_999():
    x = 10
    with pytest.raises(TypeError):
        to(x, rtype=(FroidurePin,))
    with pytest.raises(TypeError):
        to(x, rtype=(ToddCoxeter,))
    with pytest.raises(TypeError):
        to(x, rtype=(str,))
