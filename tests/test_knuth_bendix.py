# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for KnuthBendix.
"""

import unittest

from fpsemi_intf import (
    check_validation,
    check_converters,
    check_initialisation,
    check_attributes,
    check_operators,
    check_running_and_state,
)
from libsemigroups_pybind11 import ReportGuard, KnuthBendix


class TestKnuthBendix(unittest.TestCase):
    def test_validation_other(self):
        ReportGuard(False)
        kb = KnuthBendix()
        kb.set_alphabet("ab")

        with self.assertRaises(RuntimeError):
            kb.validate_letter("c")
        kb.validate_letter("a")

        with self.assertRaises(RuntimeError):
            kb.validate_letter(3)
        kb.validate_letter(0)

        with self.assertRaises(RuntimeError):
            kb.validate_word("abc")
        kb.validate_word("abab")

        with self.assertRaises(RuntimeError):
            kb.validate_word([0, 1, 2])
        kb.validate_word([0, 1, 0, 1])

    def test_validation(self):
        check_validation(self, KnuthBendix)

    def test_converters(self):
        check_converters(self, KnuthBendix)

    def test_initialisation(self):
        check_initialisation(self, KnuthBendix)

    def test_attributes(self):
        check_attributes(self, KnuthBendix)

    def test_operators(self):
        check_operators(self, KnuthBendix)


def test_running_and_state():
    check_running_and_state(KnuthBendix)


     # Tests from libsemigroups
    def test_006(self):
        kb = KnuthBendix()
        kb.set_alphabet("ab")
        kb.add_rule("aa", "")
        kb.add_rule("bbb", "")
        kb.add_rule("ababab", "")
        self.assertFalse(kb.confluent())
        kb.run()
        self.assertEqual(kb.size(), 12)
        self.assertTrue(kb.confluent())
    
    def test_009(self):
        kb = KnuthBendix()
        kb.set_alphabet("012")
        kb.add_rule("000", "2")
        kb.add_rule("111", "2")
        kb.add_rule("010101", "2")
        kb.set_identity("2")
        self.assertEqual(kb.alphabet(), "012")
        self.assertFalse(kb.confluent())
        kb.run()
        self.assertTrue(kb.confluent())
        self.assertEqual(kb.number_of_active_rules(), 9)
        ad = kb.gilman_digraph()
        self.assertEqual(ad.number_of_nodes(), 9)
        self.assertEqual(ad.number_of_edges(), 13)
        
    def test_022(self):
        kb = KnuthBendix()
        kb.set_alphabet("aAbBcC")
        kb.set_identity("")
        kb.set_inverses("AaBbCc")
        kb.add_rule("Aba", "bb")
        kb.add_rule("Bcb", "cc")
        kb.add_rule("Cac", "aa")
        self.assertFalse(kb.confluent())
        kb.run()
        self.assertTrue(kb.confluent())
        self.assertTrue(kb.equal_to("Aba", "bb"))
        self.assertTrue(kb.equal_to("Bcb", "cc"))
        self.assertTrue(kb.equal_to("Cac", "aa"))
        self.assertEqual(kb.size(), 1)
        
    def test_024(self):
        kb = KnuthBendix()
        kb.set_alphabet("abAB")
        kb.set_identity("")
        kb.set_inverses("ABab")
        kb.add_rule("aaa", "")
        kb.add_rule("bbb", "")
        kb.add_rule("abababab", "")
        kb.add_rule("aBaBaBaBaB", "")
        self.assertFalse(kb.confluent())
        kb.run()
        self.assertTrue(kb.confluent())
        self.assertEqual(kb.number_of_active_rules(), 183)
        self.assertTrue(kb.equal_to("aaa", ""))
        self.assertTrue(kb.equal_to("bbb", ""))
        self.assertTrue(kb.equal_to("BaBaBaBaB", "aa"))
        self.assertTrue(kb.equal_to("bababa", "aabb"))
        self.assertTrue(kb.equal_to("ababab", "bbaa"))
        self.assertTrue(kb.equal_to("aabbaa", "babab"))
        self.assertTrue(kb.equal_to("bbaabb", "ababa"))
        self.assertTrue(kb.equal_to("bababbabab", "aabbabbaa"))
        self.assertTrue(kb.equal_to("ababaababa", "bbaabaabb"))
        self.assertTrue(kb.equal_to("bababbabaababa", "aabbabbaabaabb"))
        self.assertTrue(kb.equal_to("bbaabaabbabbaa", "ababaababbabab"))
        
    def test_025(self):
        kb = KnuthBendix()
        kb.set_alphabet("aAbB")
        kb.set_identity("")
        kb.set_inverses("AaBb")
        self.assertTrue(kb.confluent())
        kb.run()
        self.assertTrue(kb.confluent())
        self.assertTrue(kb.is_obviously_infinite())
        
    def test_027(self):
        kb = KnuthBendix()
        kb.set_alphabet("abB")
        kb.add_rule("bb", "B")
        kb.add_rule("BaB", "aba")
        self.assertFalse(kb.confluent())
        kb.run()
        self.assertTrue(kb.confluent())
        self.assertEqual(kb.number_of_active_rules(), 6)
        self.assertEqual(kb.active_rules(), [('Bb', 'bB'), ('bb', 'B'), ('BaB', 'aba'), ('BabB', 'abab'), ('Baaba', 'abaaB'), ('Bababa', 'ababaB')])
