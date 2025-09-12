# Copyright (c) 2022-2024 J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.
"""
This module contains some tests for the delta function.
"""

from datetime import datetime, timedelta
from time import sleep
import pytest

from libsemigroups_pybind11 import delta


def test_delta():
    """Simple test case for the bindings of delta"""
    current_time = datetime.now()
    wait_time = 10**-3
    sleep(wait_time)
    with pytest.deprecated_call():
        diff = delta(current_time)
    assert isinstance(diff, timedelta)
    assert diff > timedelta(seconds=wait_time)
