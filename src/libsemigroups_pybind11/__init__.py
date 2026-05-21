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
    todd_coxeter,
    transf,
    ukkonen,
    word_graph,
    words,
)
from ._version import __version__
from .action import Action, LeftAction, RightAction
from .adapters import ImageLeftAction, ImageRightAction
from .bipartition import Bipartition
from .blocks import Blocks
from .congruence import Congruence
from .forest import PathsFromRoots, PathsToRoots
from .froidure_pin import FroidurePin
from .hpcombi import LIBSEMIGROUPS_HPCOMBI_ENABLED
from .is_obviously_infinite import is_obviously_infinite
from .kambites import Kambites
from .knuth_bendix import KnuthBendix
from .konieczny import Konieczny
from .matrix import Matrix, MatrixKind
from .presentation import InversePresentation, Presentation
from .schreier_sims import SchreierSims
from .sims import MinimalRepOrc, RepOrc, Sims1, Sims2, SimsRefinerFaithful, SimsRefinerIdeals
from .stephen import Stephen
from .to import to
from .todd_coxeter import ToddCoxeter
from .transf import Perm, PPerm, Transf

if LIBSEMIGROUPS_HPCOMBI_ENABLED:
    from .hpcombi import Perm16, PPerm16, PTransf16, Transf16, Vect16


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
        PBR,
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
    # Constants from _libsemigruops_pybind11
    "LIBSEMIGROUPS_EIGEN_ENABLED",
    "LIMIT_MAX",
    "NEGATIVE_INFINITY",
    "POSITIVE_INFINITY",
    "UNDEFINED",
    # Classes from _libsemigroups_pybind11
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
    "PBR",
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
    # Free functions from _libsemigroups_pybind11
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
    # Submodules
    "action",
    "adapters",
    "aho_corasick",
    "bipartition",
    "blocks",
    "bmat8",
    "congruence",
    "forest",
    "froidure_pin",
    "hpcombi",
    "is_obviously_infinite",
    "kambites",
    "knuth_bendix",
    "konieczny",
    "matrix",
    "paths",
    "pbr",
    "presentation",
    "schreier_sims",
    "sims",
    "stephen",
    "to",
    "todd_coxeter",
    "transf",
    "ukkonen",
    "word_graph",
    "words",
    # Classes defined in submodules
    "Action",
    "Bipartition",
    "Blocks",
    "Congruence",
    "FroidurePin",
    "ImageLeftAction",
    "ImageRightAction",
    "InversePresentation",
    "Kambites",
    "KnuthBendix",
    "Konieczny",
    "LeftAction",
    "LIBSEMIGROUPS_HPCOMBI_ENABLED",
    "Matrix",
    "MatrixKind",
    "MinimalRepOrc",
    "PathsFromRoots",
    "PathsToRoots",
    "Perm",
    "PPerm",
    "Presentation",
    "RepOrc",
    "RightAction",
    "SchreierSims",
    "Sims1",
    "Sims2",
    "SimsRefinerFaithful",
    "SimsRefinerIdeals",
    "Stephen",
    "ToddCoxeter",
    "Transf",
    # Free functions from submodules
    "to",
    "is_obviously_infinite",
]

if LIBSEMIGROUPS_HPCOMBI_ENABLED:
    __all__ += ["Perm16", "PPerm16", "PTransf16", "Transf16", "Vect16"]
