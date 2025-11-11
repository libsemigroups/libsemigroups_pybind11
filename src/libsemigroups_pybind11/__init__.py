# Copyright (c) 2021-2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This package provides the user-facing python part of libsemigroups_pybind11"""

import libsemigroups_pybind11.aho_corasick
import libsemigroups_pybind11.bipartition
import libsemigroups_pybind11.blocks
import libsemigroups_pybind11.bmat8
import libsemigroups_pybind11.congruence
import libsemigroups_pybind11.forest
import libsemigroups_pybind11.froidure_pin
import libsemigroups_pybind11.hpcombi
import libsemigroups_pybind11.kambites
import libsemigroups_pybind11.knuth_bendix
import libsemigroups_pybind11.matrix
import libsemigroups_pybind11.paths
import libsemigroups_pybind11.pbr
import libsemigroups_pybind11.sims
import libsemigroups_pybind11.stephen
import libsemigroups_pybind11.todd_coxeter
import libsemigroups_pybind11.ukkonen
import libsemigroups_pybind11.word_graph
import libsemigroups_pybind11.words

from ._version import version as __version__
from .action import Action, LeftAction, RightAction
from .adapters import ImageLeftAction, ImageRightAction
from .congruence import Congruence
from .detail.dot import _Dot as Dot
from .froidure_pin import FroidurePin
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

DISCLAIMER = (
    "(You should not see this message unless you are installing libsemigroups_pybind11 from its "
    "sources. If you are not installing from the sources, please raise an issue at "
    "https://github.com/libsemigroups/libsemigroups_pybind11)"
)

try:
    from _libsemigroups_pybind11 import (
        LIBSEMIGROUPS_HPCOMBI_ENABLED,
        LIMIT_MAX,
        NEGATIVE_INFINITY,
        PBR,
        POSITIVE_INFINITY,
        UNDEFINED,
        AhoCorasick,
        Bipartition,
        Blocks,
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
        f"directory? {DISCLAIMER}"
    ) from e

# The following fools sphinx into thinking that MatrixKind + Matrix are not
# aliases.
Matrix.__module__ = __name__
Matrix.__name__ = "Matrix"
MatrixKind.__module__ = __name__
MatrixKind.__name__ = "MatrixKind"
