# -*- coding: utf-8 -*-
# Copyright (c) 2021-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some functions used in tests for derived classes, i.e.
KnuthBendix, etc.
"""

from datetime import timedelta

from libsemigroups_pybind11 import ReportGuard

N = 0


def check_runner(
    x, t=timedelta(microseconds=1000)
):  # pylint: disable=missing-function-docstring
    global N  # pylint: disable=global-statement
    N = 0
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
        x = x.copy()  # copy
    except:  # noqa: E722 pylint: disable=bare-except
        pass

    def func():
        global N  # pylint: disable=global-statement
        N += 1
        return N >= 2

    x.run_until(func)

    assert x.stopped()
    assert not x.finished()
    assert not x.running()
    assert x.started()
    assert x.stopped_by_predicate()
    assert not x.timed_out()
