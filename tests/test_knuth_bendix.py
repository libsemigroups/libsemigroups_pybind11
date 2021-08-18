# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell + Maria Tsalakou
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for number_of_words.
"""

import unittest
from libsemigroups_pybind11 import ReportGuard, KnuthBendix


class TestKnuthBendix(unittest.TestCase):
    def test_validation(self):
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
