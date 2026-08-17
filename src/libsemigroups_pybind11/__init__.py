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
    alphabet,
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
    order,
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
from .alphabet import Alphabet, validate
from .bipartition import Bipartition
from .blocks import Blocks
from .congruence import Congruence
from .detail.cxx_wrapper import wrap_cxx_free_fn as _wrap_cxx_free_fn
from .detail.dot import Dot
from .du_narendran_rusinowitch import du_narendran_rusinowitch
from .forest import PathsFromRoots, PathsToRoots
from .froidure_pin import FroidurePin
from .hpcombi import LIBSEMIGROUPS_HPCOMBI_ENABLED
from .is_obviously_infinite import is_obviously_infinite
from .kambites import Kambites
from .knuth_bendix import KnuthBendix
from .konieczny import Konieczny
from .matrix import Matrix, MatrixKind
from .order import (
    LenLexCmp,
    LexCmp,
    RevLenLexCmp,
    RevLexCmp,
    RevRPOCmp,
    RevWrCmp,
    RPOCmp,
    WrCmp,
    WtLenLexCmp,
)
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
        len_wt_lex_cmp as _len_wt_lex_cmp,
        lenlex_cmp as _lenlex_cmp,
        lex_cmp as _lex_cmp,
        lexicographical_compare,
        number_of_words,
        random_string,
        random_strings,
        random_word,
        recursive_path_compare,
        rev_len_wt_lex_cmp as _rev_len_wt_lex_cmp,
        rev_lenlex_cmp as _rev_lenlex_cmp,
        rev_lex_cmp as _rev_lex_cmp,
        rev_rpo_cmp as _rev_rpo_cmp,
        rev_wr_cmp as _rev_wr_cmp,
        rev_wt_lenlex_cmp as _rev_wt_lenlex_cmp,
        rev_wt_lex_cmp as _rev_wt_lex_cmp,
        rpo_cmp as _rpo_cmp,
        shortlex_compare,
        side,
        tril,
        wr_cmp as _wr_cmp,
        wt_lenlex_cmp as _wt_lenlex_cmp,
        wt_lex_cmp as _wt_lex_cmp,
    )
except ModuleNotFoundError as e:
    raise ModuleNotFoundError(
        f'{e.msg}, did you forget to run "pip install ." in the libsemigroups_pybind11 '
        f"directory? {_DISCLAIMER}"
    ) from e


len_wt_lex_cmp = _wrap_cxx_free_fn(_len_wt_lex_cmp)
lenlex_cmp = _wrap_cxx_free_fn(_lenlex_cmp)
lex_cmp = _wrap_cxx_free_fn(_lex_cmp)
rev_len_wt_lex_cmp = _wrap_cxx_free_fn(_rev_len_wt_lex_cmp)
rev_lenlex_cmp = _wrap_cxx_free_fn(_rev_lenlex_cmp)
rev_lex_cmp = _wrap_cxx_free_fn(_rev_lex_cmp)
rev_rpo_cmp = _wrap_cxx_free_fn(_rev_rpo_cmp)
rev_wr_cmp = _wrap_cxx_free_fn(_rev_wr_cmp)
rev_wt_lenlex_cmp = _wrap_cxx_free_fn(_rev_wt_lenlex_cmp)
rev_wt_lex_cmp = _wrap_cxx_free_fn(_rev_wt_lex_cmp)
rpo_cmp = _wrap_cxx_free_fn(_rpo_cmp)
wr_cmp = _wrap_cxx_free_fn(_wr_cmp)
wt_lenlex_cmp = _wrap_cxx_free_fn(_wt_lenlex_cmp)
wt_lex_cmp = _wrap_cxx_free_fn(_wt_lex_cmp)


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
    "Forest",
    "Gabow",
    "Joiner",
    "LibsemigroupsError",
    "LimitMax",
    "LenLexCmp",
    "LexCmp",
    "RPOCmp",
    "RevLenLexCmp",
    "RevLexCmp",
    "RevRPOCmp",
    "RevWrCmp",
    "WrCmp",
    "WtLenLexCmp",
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
    "len_wt_lex_cmp",
    "lenlex_cmp",
    "lex_cmp",
    "lexicographical_compare",
    "number_of_words",
    "random_string",
    "random_strings",
    "random_word",
    "recursive_path_compare",
    "rev_len_wt_lex_cmp",
    "rev_lenlex_cmp",
    "rev_lex_cmp",
    "rev_rpo_cmp",
    "rev_wr_cmp",
    "rev_wt_lenlex_cmp",
    "rev_wt_lex_cmp",
    "rpo_cmp",
    "shortlex_compare",
    "side",
    "tril",
    "wr_cmp",
    "wt_lenlex_cmp",
    "wt_lex_cmp",
    # Submodules
    "action",
    "adapters",
    "aho_corasick",
    "alphabet",
    "bipartition",
    "blocks",
    "bmat8",
    "congruence",
    "forest",
    "froidure_pin",
    "hpcombi",
    "kambites",
    "knuth_bendix",
    "konieczny",
    "matrix",
    "order",
    "paths",
    "pbr",
    "presentation",
    "schreier_sims",
    "sims",
    "stephen",
    "todd_coxeter",
    "transf",
    "ukkonen",
    "word_graph",
    "words",
    # Classes defined in submodules
    "Action",
    "Alphabet",
    "Bipartition",
    "Blocks",
    "Congruence",
    "Dot",
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
    "du_narendran_rusinowitch",
    "is_obviously_infinite",
    "to",
    "validate",
]

if LIBSEMIGROUPS_HPCOMBI_ENABLED:
    __all__ += ["Perm16", "PPerm16", "PTransf16", "Transf16", "Vect16"]
