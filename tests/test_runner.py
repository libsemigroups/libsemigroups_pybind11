# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This module contains some tests for the libsemigroups_pybind11 functionality
arising from runner.*pp in libsemigroups.
"""

from datetime import datetime, timedelta

import pytest

from libsemigroups_pybind11 import (
    FroidurePin,
    KnuthBendix,
    Presentation,
    Reporter,
    ToddCoxeter,
    Transf,
    congruence_kind,
)


def test_reporter_000():
    """Simple test case for the bindings of Reporter."""
    creation_time = datetime.now()
    r = Reporter()
    assert not r.report()
    assert r.report_every() == timedelta(seconds=1)
    r.report_every(timedelta(seconds=2))
    assert r.report_every() == timedelta(seconds=2)
    assert isinstance(r.start_time(), datetime)
    assert creation_time < r.start_time() < datetime.now()
    with pytest.deprecated_call():
        r.last_report()
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
    s.init()
    assert s.report_prefix() == ""
    assert s.report_every() == timedelta(seconds=1)

    assert s.report_every() == timedelta(seconds=1)


@pytest.fixture(
    name="reporter",
    params=["Reporter", "FroidurePin", "ToddCoxeter", "KnuthBendixSet", "KnuthBendixTrie"],
)
def make_reporter(request):
    """Return a reporter, including algorithms with inherited reporting methods."""
    if request.param == "Reporter":
        return Reporter()
    if request.param == "FroidurePin":
        return FroidurePin(Transf([1, 0]))
    if request.param == "ToddCoxeter":
        return ToddCoxeter(congruence_kind.twosided, Presentation("ab"))
    return KnuthBendix(
        congruence_kind.twosided,
        Presentation("ab"),
        rewriting_system=request.param.removeprefix("KnuthBendix"),
    )


@pytest.mark.quick
def test_reporter_inherited_report_every(reporter):
    """Inherited reporting methods must access the Reporter base subobject."""
    assert reporter.report_every() == timedelta(seconds=1)
    reporter.report_every(timedelta(seconds=2))
    assert reporter.report_every() == timedelta(seconds=2)


@pytest.mark.quick
def test_reporter_inherited_report_prefix(reporter):
    """Both prefix overloads must access the correct reporting prefix."""
    expected = "" if isinstance(reporter, Reporter) else type(reporter).__name__
    assert reporter.report_prefix() == expected
    for prefix in ("a report prefix longer than a small string buffer", "foo", ""):
        reporter.report_prefix(prefix)
        assert reporter.report_prefix() == prefix


@pytest.mark.quick
def test_reporter_inherited_reset_last_report(reporter):
    """Resetting the last report must preserve the start time."""
    start = reporter.start_time()
    before = datetime.now()
    reporter.reset_last_report()
    with pytest.deprecated_call():
        last_report = reporter.last_report()
    assert before <= last_report <= datetime.now()
    assert abs(reporter.start_time() - start) < timedelta(milliseconds=1)


@pytest.mark.quick
def test_reporter_inherited_reset_start_time(reporter):
    """Resetting the start time must also reset the last report time."""
    before = datetime.now()
    reporter.reset_start_time()
    start = reporter.start_time()
    with pytest.deprecated_call():
        last_report = reporter.last_report()
    assert before <= start <= datetime.now()
    # Each conversion from high_resolution_clock samples system_clock anew.
    assert abs(last_report - start) < timedelta(milliseconds=1)
