# -*- coding: utf-8 -*-
# pylint: disable=no-name-in-module, missing-function-docstring
# pylint: disable=missing-class-docstring, invalid-name

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for FpSemigroupInterface
derived classes, i.e. KnuthBendix, FpSemigroup, etc.
"""

from datetime import timedelta

from libsemigroups_pybind11 import ReportGuard

n = 0


def check_runner(self, x, t=timedelta(microseconds=1000)):
    global n  # pylint: disable=global-statement
    n = 0
    ReportGuard(False)

    self.assertFalse(x.stopped())
    self.assertFalse(x.finished())
    self.assertFalse(x.running())
    self.assertFalse(x.started())
    self.assertFalse(x.stopped_by_predicate())
    self.assertFalse(x.timed_out())

    x.run_for(t)

    self.assertTrue(x.stopped())
    self.assertFalse(x.finished())
    self.assertFalse(x.running())
    self.assertTrue(x.started())
    self.assertFalse(x.stopped_by_predicate())
    self.assertTrue(x.timed_out())

    try:
        x = type(x)(x)  # copy construct
    except:  # pylint: disable=bare-except
        pass

    def func():
        global n  # pylint: disable=global-statement
        n += 1
        return n >= 2

    x.run_until(func)

    self.assertTrue(x.stopped())
    self.assertFalse(x.finished())
    self.assertFalse(x.running())
    self.assertTrue(x.started())
    self.assertTrue(x.stopped_by_predicate())
    self.assertFalse(x.timed_out())
