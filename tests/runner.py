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


def check_runner(x, t=timedelta(microseconds=1000)):
    global n  # pylint: disable=global-statement
    n = 0
    ReportGuard(False)

    assert not x.stopped()
    assert not x.finished()
    assert not x.running()
    assert not x.started()
    assert not x.stopped_by_predicate()
    assert not x.timed_out()

    x.run_for(t)

    assert x.stopped()
    assert not x.finished()
    assert not x.running()
    assert x.started()
    assert not x.stopped_by_predicate()
    assert x.timed_out()

    try:
        x = type(x)(x)  # copy construct
    except:  # pylint: disable=bare-except
        pass

    def func():
        global n  # pylint: disable=global-statement
        n += 1
        return n >= 2

    x.run_until(func)

    assert x.stopped()
    assert not x.finished()
    assert not x.running()
    assert x.started()
    assert x.stopped_by_predicate()
    assert not x.timed_out()
