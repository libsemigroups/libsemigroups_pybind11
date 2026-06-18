"""This file contains the configuration for pytest."""

import pytest

from libsemigroups_pybind11 import ReportGuard


@pytest.fixture(autouse=True)
def setup_each_test():
    """Avoid having to write ReportGuard(false) in every test case"""
    rg = ReportGuard(False)
    yield
    del rg
