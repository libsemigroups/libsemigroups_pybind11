# -*- coding: utf-8 -*-

# Copyright (c) 2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint:disable=no-name-in-module, unused-import, protected-access,
# pylint:disable=missing-function-docstring, missing-class-docstring
# pylint:disable=no-member

"""
This package provides a class name Runner that shadows the same class in
libsemigroups. It exists for those python classes in libsemigroups_pybind11
that wrap multiple C++ types.
"""

from .detail.cxx_wrapper import CxxWrapper


class Runner(
    CxxWrapper
):  # pylint: disable=invalid-name, too-few-public-methods
    """
    This class exists so that python classes derived from CxxWrapper can
    also be derived from a class call Runner. I.e. Action objects in
    libsemigroups C++ are Runners, Action objects in python are
    CxxWrappers, but should also inherit from Runner, they cannot inherit
    from the C++ runner (because this already has run etc mem fns which
    won't work for python classes) This class is used so
    that classes such as Action have the same interface as Runner C++ objects.
    """
