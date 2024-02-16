# -*- coding: utf-8 -*-

# Copyright (c) 2021-2022, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

# pylint: disable=no-name-in-module

"""
This package provides the user-facing python part of libsemigroups_pybind11
"""

import os

from .tools import ld_library_path
import pkgconfig

DISCLAIMER = "(You should not see this message unless you are installing libsemigroups_pybind11 from its sources. If you are not installing from the sources, please raise an issue at https://github.com/libsemigroups/libsemigroups_pybind11)"

assert pkgconfig.exists("libsemigroups")
# raise ImportError(
#    "cannot locate libsemigroups library, is it installed? see https://libsemigroups.github.io/libsemigroups_pybind11/install.html for more details"
# )
# except ImportError as e:
#     if "LD_LIBRARY_PATH" in os.environ:
#         ld_library_path_val = os.environ["LD_LIBRARY_PATH"]
#     else:
#         ld_library_path_val = ""
#     raise ImportError(
#         f'{e.what()}, is the environment LD_LIBRARY_PATH set correctly? The current value is "{ld_library_path_val}", pkg-config indicates it should include "{ld_library_path()}" {DISCLAIMER}'
#     )


try:
    from _libsemigroups_pybind11 import (
        NEGATIVE_INFINITY,
        POSITIVE_INFINITY,
        UNDEFINED,
        Forest,
        Gabow,
        Paths,
        ReportGuard,
        Strings,
        ToStrings,
        ToWord,
        WordGraph,
        Words,
        algorithm,
        congruence_kind,
        number_of_words,
        order,
        overlap,
        parse_relations,
        random_word,
        to_string,
        to_word,
    )
except ModuleNotFoundError as e:
    raise ModuleNotFoundError(
        f'{e.msg}, did you forget to run "pip install ." in the libsemigroups_pybind11 director? {DISCLAIMER}'
    )


from .knuth_bendix import KnuthBendix
from .presentation import Presentation, redundant_rule
from .transf import PPerm, Transf

"""
from .froidure_pin import FroidurePin
from .konieczny import Konieczny
from .matrix import Matrix, MatrixKind, make_identity
from .tools import compare_version_numbers, libsemigroups_version
from .transf import PPerm, Transf
"""
