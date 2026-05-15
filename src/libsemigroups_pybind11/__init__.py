# Copyright (c) 2021-2024, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""This package provides the user-facing python part of libsemigroups_pybind11"""

from . import action
from .action import Action, LeftAction, RightAction

from . import adapters
from .adapters import ImageLeftAction, ImageRightAction

from . import aho_corasick
from .aho_corasick import add_word, dot, rm_word, traverse_word

from . import bipartition
from .bipartition import Bipartition, one, random, underlying_partition, uniform_random

from . import blocks
from .blocks import Blocks, underlying_partition

from . import bmat8
from .bmat8 import (
    col_space_basis,
    col_space_size,
    is_regular_element,
    minimum_dim,
    number_of_cols,
    number_of_rows,
    one,
    random,
    row_space_basis,
    row_space_size,
    rows,
    transpose,
)

from . import congruence
from .congruence import Congruence, non_trivial_classes, partition

from . import forest
from .forest import (
    PathsFromRoots,
    PathsToRoots,
    depth,
    dot,
    is_root,
    max_label,
    path_from_root,
    path_to_root,
)

from . import froidure_pin
from .froidure_pin import (
    FroidurePin,
    current_minimal_factorisation,
    current_normal_forms,
    current_position,
    current_rules,
    equal_to,
    factorisation,
    minimal_factorisation,
    normal_forms,
    position,
    product_by_reduction,
    rules,
    to_element,
)

from . import hpcombi
from .hpcombi import LIBSEMIGROUPS_HPCOMBI_ENABLED

if LIBSEMIGROUPS_HPCOMBI_ENABLED:
    from .hpcombi import Perm16, PPerm16, PTransf16, Transf16, Vect16

from . import is_obviously_infinite
from .is_obviously_infinite import is_obviously_infinite

from . import kambites
from .kambites import Kambites, non_trivial_classes, normal_forms, partition

from . import knuth_bendix
from .knuth_bendix import (
    KnuthBendix,
    by_overlap_length,
    is_reduced,
    non_trivial_classes,
    normal_forms,
    partition,
    redundant_rule,
)

from . import konieczny
from .konieczny import Konieczny

from . import matrix
from .matrix import Matrix, MatrixKind, period, row_basis, row_space_size, threshold

from . import paths
from .paths import algorithm, number_of_paths, number_of_paths_algorithm

from . import pbr
from .pbr import one

from . import presentation
from .presentation import (
    Presentation,
    InversePresentation,
    add_commutator_rule,
    add_identity_rules,
    add_inverse_rules,
    add_rule,
    add_rules,
    add_zero_rules,
    are_rules_sorted,
    change_alphabet,
    commutator,
    contains_rule,
    first_unused_letter,
    greedy_reduce_length,
    greedy_reduce_length_and_number_of_gens,
    is_strongly_compressible,
    length,
    longest_rule,
    longest_rule_length,
    longest_subword_reducing_length,
    make_semigroup,
    normalize_alphabet,
    reduce_complements,
    reduce_to_2_generators,
    remove_duplicate_rules,
    remove_redundant_generators,
    remove_trivial_rules,
    replace_subword,
    replace_word,
    replace_word_with_new_generator,
    reverse,
    shortest_rule,
    shortest_rule_length,
    sort_each_rule,
    sort_rules,
    strongly_compress,
    throw_if_bad_inverses,
    to_gap_string,
    balance,
    add_cyclic_conjugates,
    index_rule,
    is_normalized,
    is_rule,
    try_detect_inverses,
)

from . import schreier_sims
from .schreier_sims import SchreierSims, intersection

from . import sims
from .sims import (
    MinimalRepOrc,
    RepOrc,
    Sims1,
    Sims2,
    SimsRefinerFaithful,
    SimsRefinerIdeals,
    is_maximal_right_congruence,
    is_right_congruence,
    is_right_congruence_of_dual,
    is_two_sided_congruence,
    poset,
    right_generating_pairs,
    two_sided_generating_pairs,
)

from . import stephen
from .stephen import (
    Stephen,
    accepts,
    dot,
    is_left_factor,
    left_factors,
    number_of_left_factors,
    number_of_words_accepted,
    words_accepted,
)

from . import to
from .to import to

from . import todd_coxeter
from .todd_coxeter import (
    ToddCoxeter,
    class_by_index,
    class_of,
    is_non_trivial,
    non_trivial_classes,
    normal_forms,
    partition,
    perform_lookbehind,
    redundant_rule,
)

from . import transf
from .transf import Perm, PPerm, Transf, domain, image, inverse, left_one, one, right_one

from . import ukkonen
from .ukkonen import (
    add_word,
    add_words,
    dot,
    is_piece,
    is_subword,
    is_suffix,
    length_maximal_piece_prefix,
    length_maximal_piece_suffix,
    maximal_piece_prefix,
    maximal_piece_suffix,
    number_of_distinct_subwords,
    number_of_pieces,
    pieces,
    traverse,
)

from . import word_graph
from .word_graph import (
    add_cycle,
    adjacency_matrix,
    dot,
    equal_to,
    follow_path,
    is_acyclic,
    is_compatible,
    is_complete,
    is_connected,
    is_reachable,
    is_strictly_cyclic,
    last_node_on_path,
    nodes_reachable_from,
    number_of_nodes_reachable_from,
    random_acyclic,
    spanning_tree,
    standardize,
    topological_sort,
)

from . import words
from .words import human_readable_index, human_readable_letter, parse_relations, pow, prod

from ._version import version as __version__

_DISCLAIMER = (
    "(You should not see this message unless you are installing libsemigroups_pybind11 from its "
    "sources. If you are not installing from the sources, please raise an issue at "
    "https://github.com/libsemigroups/libsemigroups_pybind11)"
)

try:
    from _libsemigroups_pybind11 import (
        LIBSEMIGROUPS_EIGEN_ENABLED,
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
        f"directory? {_DISCLAIMER}"
    ) from e


__all__ = [
    "__version__",
    "LIBSEMIGROUPS_EIGEN_ENABLED",
    "LIBSEMIGROUPS_HPCOMBI_ENABLED",
    "LIMIT_MAX",
    "NEGATIVE_INFINITY",
    "PBR",
    "POSITIVE_INFINITY",
    "UNDEFINED",
    "AhoCorasick",
    "Bipartition",
    "Blocks",
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
# __all__ += is_obviously_infinite.__all__
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
# __all__ += to.__all__
__all__ += transf.__all__
__all__ += ukkonen.__all__
__all__ += word_graph.__all__
__all__ += words.__all__
