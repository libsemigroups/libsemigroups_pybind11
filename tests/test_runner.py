# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This module contains some tests for the libsemigroups_pybind11 functionality
arising from runner.*pp in libsemigroups.
"""

from datetime import timedelta

from libsemigroups_pybind11 import Reporter, delta


def test_reporter_000():
    """
    Simple test case for the bindings of Reporter.
    """
    r = Reporter()
    assert not r.report()
    assert r.report_every() == timedelta(seconds=1)
    r.report_every(timedelta(seconds=2))
    assert r.report_every() == timedelta(seconds=2)
    r.last_report()
    r.reset_last_report()
    assert delta(r.last_report()) < timedelta(seconds=1)
    r.report_prefix("Banana")
    assert r.report_prefix() == "Banana"
    r.init()
    assert r.report_prefix() == ""
    assert r.report_every() == timedelta(seconds=1)
    assert r.report_prefix("Banana") is r
    r.report_every(timedelta(seconds=32))

    s = r.copy()
    assert s is not r
    assert s.report_prefix() == "Banana"
    assert s.report_every() == timedelta(seconds=32)
    assert s.last_report() == r.last_report()
    s.init()
    assert s.report_prefix() == ""
    assert s.report_every() == timedelta(seconds=1)

    assert s.report_every() == timedelta(seconds=1)
