# Copyright (c) 2021-2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This package provides the user-facing python part of libsemigroups_pybind11"""

from . import (
    action,
    adapters,
    aho_corasick,
    bipartition,
    blocks,
    bmat8,
    congruence,
    forest,
    froidure_pin,
    hpcombi,
    is_obviously_infinite as is_obviously_infinite_module,
    kambites,
    knuth_bendix,
    konieczny,
    matrix,
    paths,
    pbr,
    presentation,
    schreier_sims,
    sims,
    stephen,
    to as to_module,
    todd_coxeter,
    transf,
    ukkonen,
    word_graph,
    words,
)
from ._version import __version__
from .action import *
from .adapters import *
from .aho_corasick import *
from .bipartition import *
from .blocks import *
from .bmat8 import *
from .congruence import *
from .forest import *
from .froidure_pin import *
from .hpcombi import *
from .is_obviously_infinite import *
from .kambites import *
from .knuth_bendix import *
from .konieczny import *
from .matrix import *
from .paths import *
from .pbr import *
from .presentation import *
from .schreier_sims import *
from .sims import *
from .stephen import *
from .to import *
from .todd_coxeter import *
from .transf import *
from .ukkonen import *
from .word_graph import *
from .words import *

_DISCLAIMER = (
    "(You should not see this message unless you are installing libsemigroups_pybind11 from its "
    "sources. If you are not installing from the sources, please raise an issue at "
    "https://github.com/libsemigroups/libsemigroups_pybind11)"
)

try:
    from _libsemigroups_pybind11 import (
        LIBSEMIGROUPS_EIGEN_ENABLED,
        LIMIT_MAX,
        NEGATIVE_INFINITY,
        PBR,
        POSITIVE_INFINITY,
        UNDEFINED,
        AhoCorasick,
        BMat8,
        Dot,
        Forest,
        Gabow,
        Joiner,
        LibsemigroupsError,
        LimitMax,
        Meeter,
        NegativeInfinity,
        Order,
        Paths,
        PositiveInfinity,
        Reporter,
        ReportGuard,
        Runner,
        SimsStats,
        StringRange,
        ToString,
        ToWord,
        Ukkonen,
        Undefined,
        WordGraph,
        WordRange,
        congruence_kind,
        delta,
        error_message_with_prefix,
        freeband_equal_to,
        lexicographical_compare,
        number_of_words,
        random_string,
        random_strings,
        random_word,
        recursive_path_compare,
        shortlex_compare,
        side,
        tril,
    )
except ModuleNotFoundError as e:
    raise ModuleNotFoundError(
        f'{e.msg}, did you forget to run "pip install ." in the libsemigroups_pybind11 '
        f"directory? {_DISCLAIMER}"
    ) from e


__all__ = [
    "__version__",
    "LIBSEMIGROUPS_EIGEN_ENABLED",
    "LIMIT_MAX",
    "NEGATIVE_INFINITY",
    "PBR",
    "POSITIVE_INFINITY",
    "UNDEFINED",
    "AhoCorasick",
    "BMat8",
    "Dot",
    "Forest",
    "Gabow",
    "Joiner",
    "LibsemigroupsError",
    "LimitMax",
    "Meeter",
    "NegativeInfinity",
    "Order",
    "Paths",
    "PositiveInfinity",
    "Reporter",
    "ReportGuard",
    "Runner",
    "SimsStats",
    "StringRange",
    "ToString",
    "ToWord",
    "Ukkonen",
    "Undefined",
    "WordGraph",
    "WordRange",
    "congruence_kind",
    "delta",
    "error_message_with_prefix",
    "freeband_equal_to",
    "lexicographical_compare",
    "number_of_words",
    "random_string",
    "random_strings",
    "random_word",
    "recursive_path_compare",
    "shortlex_compare",
    "side",
    "tril",
]

__all__ += action.__all__
__all__ += adapters.__all__
__all__ += aho_corasick.__all__
__all__ += bipartition.__all__
__all__ += blocks.__all__
__all__ += bmat8.__all__
__all__ += congruence.__all__
__all__ += forest.__all__
__all__ += froidure_pin.__all__
__all__ += hpcombi.__all__
__all__ += is_obviously_infinite_module.__all__
__all__ += kambites.__all__
__all__ += knuth_bendix.__all__
__all__ += konieczny.__all__
__all__ += matrix.__all__
__all__ += paths.__all__
__all__ += pbr.__all__
__all__ += presentation.__all__
__all__ += schreier_sims.__all__
__all__ += sims.__all__
__all__ += stephen.__all__
__all__ += todd_coxeter.__all__
__all__ += to_module.__all__
__all__ += transf.__all__
__all__ += ukkonen.__all__
__all__ += word_graph.__all__
__all__ += words.__all__
