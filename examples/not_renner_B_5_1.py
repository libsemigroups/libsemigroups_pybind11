"""
In this file we give the "proof" that the monoid defined by the presentation
not_renner_type_B_monoid(5, 1) is not isomorphic to the monoid defined by
renner_type_B_monoid(5, 1).
"""

from libsemigroups_pybind11 import Sims2, Presentation, WordGraph
from libsemigroups_pybind11 import word_graph
from libsemigroups_pybind11.presentation import examples


def check_incompat(wg: WordGraph, p: Presentation) -> bool:
    for i in range(0, len(p.rules), 2):
        if not word_graph.is_compatible(wg, 0, 4096, p.rules[i], p.rules[i + 1]):
            return True
    return False


def witness(wg: WordGraph, p: Presentation) -> tuple[list[int], list[int]]:
    for i in range(0, len(p.rules), 2):
        if not word_graph.is_compatible(wg, 0, 1, p.rules[i], p.rules[i + 1]):
            return (p.rules[i], p.rules[i + 1])


p = examples.not_renner_type_B_monoid(5, 1)
q = examples.renner_type_B_monoid(5, 1)

# We iterate 2-sided congruences of the monoid defined by "p" with the aim of
# finding one which is not compatible with the relations in "q"
twosided_congs = Sims2(p)
wg = twosided_congs.find_if(4096, lambda wg: check_incompat(wg, q))
word_graph.number_of_nodes_reachable_from(wg, 0)  # 2177
wg.induced_subgraph(0, 2177)
witness(wg, q)  # ([5, 0, 1, 2, 3, 4, 0, 1, 2, 3, 0, 1, 2, 0, 1, 0, 5], [10])
# meaning that the paths with source 0 labelled by these two words should lead to the same
# node but do not.
word_graph.follow_path(wg, 0, [5, 0, 1, 2, 3, 4, 0, 1, 2, 3, 0, 1, 2, 0, 1, 0, 5])  # 6
word_graph.follow_path(wg, 0, [10])  # 7
