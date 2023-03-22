# -*- coding: utf-8 -*-

# Copyright (c) 2021, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for ActionDigraph and related functionality in
libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name,
# pylint: disable=duplicate-code

import pytest
from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    UNDEFINED,
    ActionDigraph,
    action_digraph_helper,
    add_cycle,
    follow_path,
    is_acyclic,
    topological_sort,
    wilo,
    compare_version_numbers,
    libsemigroups_version,
)


def binary_tree(number_of_levels):
    ad = ActionDigraph()
    ad.add_nodes(2 ** number_of_levels - 1)
    ad.add_to_out_degree(2)
    ad.add_edge(0, 1, 0)
    ad.add_edge(0, 2, 1)

    for i in range(2, number_of_levels + 1):
        counter = 2 ** (i - 1) - 1
        for j in range(2 ** (i - 2) - 1, 2 ** (i - 1) - 1):
            ad.add_edge(j, counter, 0)
            counter += 1
            ad.add_edge(j, counter, 1)
            counter += 1
    return ad


def test_000():
    g = ActionDigraph()
    assert g.number_of_nodes() == 0
    assert g.number_of_edges() == 0


def test_001():
    g = ActionDigraph(42)
    assert g.number_of_nodes() == 42
    assert g.number_of_edges() == 0


def test_003():
    g = ActionDigraph(17, 31)
    for i in range(17):
        if compare_version_numbers("2.6.2", libsemigroups_version()):
            with pytest.raises(RuntimeError):
                g.number_of_scc()
        else:
            g.number_of_scc()

        for j in range(31):
            g.add_edge(i, (7 * i + 23 * j) % 17, j)

    assert g.number_of_edges() == 31 * 17
    assert g.number_of_nodes() == 17
    with pytest.raises(RuntimeError):
        g.add_edge(0, 0, 32)
    for i in range(17):
        for j in range(31):
            assert g.neighbor(i, j) == (7 * i + 23 * j) % 17

    g.add_to_out_degree(10)
    assert g.out_degree() == 41
    assert g.number_of_nodes() == 17
    assert not g.validate()

    for i in range(17):
        for j in range(10):
            g.add_edge(i, (7 * i + 23 * j) % 17, 31 + j)

    assert g.number_of_edges() == 41 * 17
    assert g.number_of_nodes() == 17


def test_004():
    g = ActionDigraph()
    g.add_to_out_degree(1)
    add_cycle(g, 32)
    assert g.scc_id(0) == 0

    g = ActionDigraph()
    g.add_to_out_degree(1)
    add_cycle(g, 33)
    assert list(g.sccs_iterator()) == [
        [
            32,
            31,
            30,
            29,
            28,
            27,
            26,
            25,
            24,
            23,
            22,
            21,
            20,
            19,
            18,
            17,
            16,
            15,
            14,
            13,
            12,
            11,
            10,
            9,
            8,
            7,
            6,
            5,
            4,
            3,
            2,
            1,
            0,
        ]
    ]
    for i in range(33):
        assert g.scc_id(i) == 0


def test_005():
    graph = ActionDigraph(0)
    for j in range(1, 100):
        graph.add_nodes(j)
        for i in range(j * (j + 1) // 2):
            assert graph.scc_id(i) == i


def test_008():
    graph = ActionDigraph(10, 5)
    with pytest.raises(RuntimeError):
        graph.neighbor(10, 0)
    assert graph.neighbor(0, 1) == UNDEFINED

    with pytest.raises(RuntimeError):
        graph.add_edge(0, 10, 0)
    with pytest.raises(RuntimeError):
        graph.add_edge(10, 0, 0)
    for i in range(5):
        graph.add_edge(0, 1, i)
        graph.add_edge(2, 2, i)
    graph.add_edge(0, 1, 0)
    graph.add_edge(2, 2, 0)

    with pytest.raises(RuntimeError):
        graph.scc_id(10)


def test_009():
    for k in range(2, 50):
        graph = ActionDigraph(k, k)
        for i in range(k):
            for j in range(k):
                graph.add_edge(i, j, j)

    assert graph.number_of_scc() == 1

    forest = graph.spanning_forest()
    assert forest.parent(k - 1) == UNDEFINED
    graph.reverse_spanning_forest()


def test_020():
    "cbegin/end_panislo - 100 node path"
    ad = ActionDigraph()
    n = 100
    ad.add_nodes(n)
    ad.add_to_out_degree(2)
    for i in range(n - 1):
        ad.add_edge(i, i + 1, i % 2)
    assert len(list(ad.panilo_iterator(0, 0, POSITIVE_INFINITY))) == 100
    assert sum(1 for _ in ad.panilo_iterator(50, 0, POSITIVE_INFINITY)) == 50
    pths = list(ad.panislo_iterator(0, 0, POSITIVE_INFINITY))
    assert len(pths) == 100
    assert pths[3][0] == [0, 1, 0]

    assert sum(1 for _ in ad.panislo_iterator(50, 0, POSITIVE_INFINITY)) == 50


def test_024():

    ad = ActionDigraph()
    ad.add_nodes(6)
    ad.add_to_out_degree(2)

    ad.add_edge(0, 1, 0)
    ad.add_edge(0, 2, 1)
    ad.add_edge(1, 3, 0)
    ad.add_edge(1, 4, 1)
    ad.add_edge(2, 4, 0)
    ad.add_edge(2, 2, 1)
    ad.add_edge(3, 1, 0)
    ad.add_edge(3, 5, 1)
    ad.add_edge(4, 5, 0)
    ad.add_edge(4, 4, 1)
    ad.add_edge(5, 4, 0)
    ad.add_edge(5, 5, 1)

    ad.validate()

    N = 18

    assert ad.number_of_paths(0, 4, 0, N) == 131062
    assert ad.number_of_paths(0, 4, 10, N) == 130556
    assert ad.number_of_paths(4, 1, 0, N) == 0
    assert ad.number_of_paths(0, 0, POSITIVE_INFINITY) == POSITIVE_INFINITY
    assert ad.number_of_paths(0, 0, 10) == 1023

    assert not action_digraph_helper.is_acyclic(ad)

    expected = sorted(
        [
            [0, 1],
            [1, 0],
            [0, 1, 1],
            [1, 1, 0],
            [1, 0, 1],
            [1, 1, 0, 1],
            [1, 0, 1, 1],
            [1, 1, 1, 0],
            [0, 1, 1, 1],
            [1, 0, 0, 0],
            [0, 0, 0, 1],
            [0, 0, 1, 0],
            [0, 1, 0, 0],
        ]
    )

    assert list(ad.pstilo_iterator(0, 4, 0, 5)) == expected

    expected = []
    for w in wilo(2, N, [], [1] * N):
        node = action_digraph_helper.follow_path(ad, 0, w)
        if node == 4:
            expected.append(w)
    assert len(expected) == 131_062

    result = list(ad.pstilo_iterator(0, 4, 0, N))
    assert len(result) == 131_062
    assert result == expected


def test_036():
    algorithm = ActionDigraph.algorithm

    ad = ActionDigraph()
    ad.add_nodes(10)
    ad.add_to_out_degree(20)
    ad.add_edge(0, 7, 5)
    ad.add_edge(0, 5, 7)
    ad.add_edge(1, 9, 14)
    ad.add_edge(1, 5, 17)
    ad.add_edge(3, 8, 5)
    ad.add_edge(5, 8, 1)
    ad.add_edge(6, 8, 14)
    ad.add_edge(7, 8, 10)
    ad.add_edge(8, 9, 12)
    ad.add_edge(8, 9, 13)

    assert is_acyclic(ad)
    assert not ad.validate()

    assert ad.number_of_paths_algorithm(0, 0, 16) == algorithm.acyclic
    assert ad.number_of_paths(0, 0, 30) == 9
    assert ad.number_of_paths(1, 0, 10, algorithm.matrix) == 6
    assert ad.number_of_paths(1, 9, 0, 10, algorithm.matrix) == 3


def test_037():  # pylint: disable=too-many-statements
    algorithm = ActionDigraph.algorithm
    n = 10
    ad = ActionDigraph()
    ad.add_nodes(10)
    ad.add_to_out_degree(20)
    ad.add_edge(0, 9, 0)
    ad.add_edge(0, 1, 1)
    ad.add_edge(0, 6, 2)
    ad.add_edge(0, 3, 3)
    ad.add_edge(0, 7, 4)
    ad.add_edge(0, 2, 5)
    ad.add_edge(0, 2, 6)
    ad.add_edge(0, 8, 7)
    ad.add_edge(0, 1, 8)
    ad.add_edge(0, 4, 9)
    ad.add_edge(0, 3, 10)
    ad.add_edge(0, 1, 11)
    ad.add_edge(0, 7, 12)
    ad.add_edge(0, 9, 13)
    ad.add_edge(0, 4, 14)
    ad.add_edge(0, 7, 15)
    ad.add_edge(0, 8, 16)
    ad.add_edge(0, 9, 17)
    ad.add_edge(0, 6, 18)
    ad.add_edge(0, 9, 19)
    ad.add_edge(1, 8, 0)
    ad.add_edge(1, 2, 1)
    ad.add_edge(1, 5, 2)
    ad.add_edge(1, 7, 3)
    ad.add_edge(1, 9, 4)
    ad.add_edge(1, 0, 5)
    ad.add_edge(1, 2, 6)
    ad.add_edge(1, 4, 7)
    ad.add_edge(1, 0, 8)
    ad.add_edge(1, 3, 9)
    ad.add_edge(1, 2, 10)
    ad.add_edge(1, 7, 11)
    ad.add_edge(1, 2, 12)
    ad.add_edge(1, 7, 13)
    ad.add_edge(1, 6, 14)
    ad.add_edge(1, 6, 15)
    ad.add_edge(1, 5, 16)
    ad.add_edge(1, 4, 17)
    ad.add_edge(1, 6, 18)
    ad.add_edge(1, 3, 19)
    ad.add_edge(2, 2, 0)
    ad.add_edge(2, 9, 1)
    ad.add_edge(2, 0, 2)
    ad.add_edge(2, 6, 3)
    ad.add_edge(2, 7, 4)
    ad.add_edge(2, 9, 5)
    ad.add_edge(2, 5, 6)
    ad.add_edge(2, 4, 7)
    ad.add_edge(2, 9, 8)
    ad.add_edge(2, 7, 9)
    ad.add_edge(2, 9, 10)
    ad.add_edge(2, 9, 11)
    ad.add_edge(2, 0, 12)
    ad.add_edge(2, 7, 13)
    ad.add_edge(2, 9, 14)
    ad.add_edge(2, 6, 15)
    ad.add_edge(2, 3, 16)
    ad.add_edge(2, 3, 17)
    ad.add_edge(2, 4, 18)
    ad.add_edge(2, 1, 19)
    ad.add_edge(3, 1, 0)
    ad.add_edge(3, 9, 1)
    ad.add_edge(3, 6, 2)
    ad.add_edge(3, 2, 3)
    ad.add_edge(3, 9, 4)
    ad.add_edge(3, 8, 5)
    ad.add_edge(3, 1, 6)
    ad.add_edge(3, 6, 7)
    ad.add_edge(3, 1, 8)
    ad.add_edge(3, 0, 9)
    ad.add_edge(3, 5, 10)
    ad.add_edge(3, 0, 11)
    ad.add_edge(3, 2, 12)
    ad.add_edge(3, 7, 13)
    ad.add_edge(3, 4, 14)
    ad.add_edge(3, 0, 15)
    ad.add_edge(3, 4, 16)
    ad.add_edge(3, 8, 17)
    ad.add_edge(3, 3, 18)
    ad.add_edge(3, 1, 19)
    ad.add_edge(4, 0, 0)
    ad.add_edge(4, 4, 1)
    ad.add_edge(4, 8, 2)
    ad.add_edge(4, 5, 3)
    ad.add_edge(4, 5, 4)
    ad.add_edge(4, 1, 5)
    ad.add_edge(4, 3, 6)
    ad.add_edge(4, 8, 7)
    ad.add_edge(4, 4, 8)
    ad.add_edge(4, 4, 9)
    ad.add_edge(4, 4, 10)
    ad.add_edge(4, 7, 11)
    ad.add_edge(4, 8, 12)
    ad.add_edge(4, 6, 13)
    ad.add_edge(4, 3, 14)
    ad.add_edge(4, 7, 15)
    ad.add_edge(4, 6, 16)
    ad.add_edge(4, 7, 17)
    ad.add_edge(4, 0, 18)
    ad.add_edge(4, 2, 19)
    ad.add_edge(5, 3, 0)
    ad.add_edge(5, 0, 1)
    ad.add_edge(5, 4, 2)
    ad.add_edge(5, 7, 3)
    ad.add_edge(5, 2, 4)
    ad.add_edge(5, 5, 5)
    ad.add_edge(5, 7, 6)
    ad.add_edge(5, 7, 7)
    ad.add_edge(5, 7, 8)
    ad.add_edge(5, 7, 9)
    ad.add_edge(5, 0, 10)
    ad.add_edge(5, 8, 11)
    ad.add_edge(5, 6, 12)
    ad.add_edge(5, 8, 13)
    ad.add_edge(5, 8, 14)
    ad.add_edge(5, 1, 15)
    ad.add_edge(5, 5, 16)
    ad.add_edge(5, 5, 17)
    ad.add_edge(5, 3, 18)
    ad.add_edge(5, 7, 19)
    ad.add_edge(6, 8, 0)
    ad.add_edge(6, 7, 1)
    ad.add_edge(6, 6, 2)
    ad.add_edge(6, 5, 3)
    ad.add_edge(6, 6, 4)
    ad.add_edge(6, 1, 5)
    ad.add_edge(6, 7, 6)
    ad.add_edge(6, 2, 7)
    ad.add_edge(6, 7, 8)
    ad.add_edge(6, 3, 9)
    ad.add_edge(6, 3, 10)
    ad.add_edge(6, 8, 11)
    ad.add_edge(6, 3, 12)
    ad.add_edge(6, 9, 13)
    ad.add_edge(6, 4, 14)
    ad.add_edge(6, 1, 15)
    ad.add_edge(6, 4, 16)
    ad.add_edge(6, 3, 17)
    ad.add_edge(6, 9, 18)
    ad.add_edge(6, 8, 19)
    ad.add_edge(7, 9, 0)
    ad.add_edge(7, 4, 1)
    ad.add_edge(7, 3, 2)
    ad.add_edge(7, 8, 3)
    ad.add_edge(7, 0, 4)
    ad.add_edge(7, 5, 5)
    ad.add_edge(7, 6, 6)
    ad.add_edge(7, 8, 7)
    ad.add_edge(7, 9, 8)
    ad.add_edge(7, 1, 9)
    ad.add_edge(7, 7, 10)
    ad.add_edge(7, 0, 11)
    ad.add_edge(7, 6, 12)
    ad.add_edge(7, 2, 13)
    ad.add_edge(7, 3, 14)
    ad.add_edge(7, 8, 15)
    ad.add_edge(7, 6, 16)
    ad.add_edge(7, 3, 17)
    ad.add_edge(7, 2, 18)
    ad.add_edge(7, 7, 19)
    ad.add_edge(8, 0, 0)
    ad.add_edge(8, 6, 1)
    ad.add_edge(8, 3, 2)
    ad.add_edge(8, 5, 3)
    ad.add_edge(8, 7, 4)
    ad.add_edge(8, 9, 5)
    ad.add_edge(8, 9, 6)
    ad.add_edge(8, 8, 7)
    ad.add_edge(8, 1, 8)
    ad.add_edge(8, 5, 9)
    ad.add_edge(8, 7, 10)
    ad.add_edge(8, 9, 11)
    ad.add_edge(8, 6, 12)
    ad.add_edge(8, 0, 13)
    ad.add_edge(8, 0, 14)
    ad.add_edge(8, 3, 15)
    ad.add_edge(8, 6, 16)
    ad.add_edge(8, 0, 17)
    ad.add_edge(8, 8, 18)
    ad.add_edge(8, 9, 19)
    ad.add_edge(9, 3, 0)
    ad.add_edge(9, 7, 1)
    ad.add_edge(9, 9, 2)
    ad.add_edge(9, 1, 3)
    ad.add_edge(9, 4, 4)
    ad.add_edge(9, 9, 5)
    ad.add_edge(9, 4, 6)
    ad.add_edge(9, 0, 7)
    ad.add_edge(9, 5, 8)
    ad.add_edge(9, 8, 9)
    ad.add_edge(9, 3, 10)
    ad.add_edge(9, 2, 11)
    ad.add_edge(9, 0, 12)
    ad.add_edge(9, 2, 13)
    ad.add_edge(9, 3, 14)
    ad.add_edge(9, 4, 15)
    ad.add_edge(9, 0, 16)
    ad.add_edge(9, 5, 17)
    ad.add_edge(9, 3, 18)
    ad.add_edge(9, 5, 19)
    assert not is_acyclic(ad)
    assert ad.validate()

    assert ad.number_of_paths_algorithm(0) == algorithm.acyclic
    assert ad.number_of_paths(0) == POSITIVE_INFINITY
    with pytest.raises(RuntimeError):
        ad.number_of_paths(0, 0, 10, algorithm.acyclic)
    with pytest.raises(RuntimeError):
        ad.number_of_paths(1, 9, 0, 10, algorithm.acyclic)

    ad = binary_tree(n)
    assert ad.number_of_paths_algorithm(0) == algorithm.acyclic
    assert ad.number_of_paths(0) == 1023

    add_cycle(ad, n)
    ad.add_edge(0, n + 1, 0)
    assert not is_acyclic(ad)
    assert not ad.validate()
    assert ad.number_of_paths(1) == 511
    assert (
        ad.number_of_paths_algorithm(1, 0, POSITIVE_INFINITY)
        == algorithm.acyclic
    )
    assert ad.number_of_paths(1, 0, POSITIVE_INFINITY) == 511
    assert len(topological_sort(ad)) == 0
    for m in ad.nodes_iterator():
        if len(topological_sort(ad, m)) == 0:
            assert m == 1023
            break


def test_010():
    j = 33
    graph = ActionDigraph()
    graph.add_to_out_degree(1)
    for k in range(10):
        graph.add_nodes(j)
        for i in range(k * j, (k + 1) * j - 1):
            graph.add_edge(i, i + 1, 0)
        graph.add_edge((k + 1) * j - 1, k * j, 0)
    for i in range(10 * j):
        assert graph.scc_id(i) == i // j

    forest = graph.spanning_forest()

    assert list(forest.parent_iterator()) == [
        32,
        0,
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        30,
        UNDEFINED,
        65,
        33,
        34,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        47,
        48,
        49,
        50,
        51,
        52,
        53,
        54,
        55,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        UNDEFINED,
        98,
        66,
        67,
        68,
        69,
        70,
        71,
        72,
        73,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        85,
        86,
        87,
        88,
        89,
        90,
        91,
        92,
        93,
        94,
        95,
        96,
        UNDEFINED,
        131,
        99,
        100,
        101,
        102,
        103,
        104,
        105,
        106,
        107,
        108,
        109,
        110,
        111,
        112,
        113,
        114,
        115,
        116,
        117,
        118,
        119,
        120,
        121,
        122,
        123,
        124,
        125,
        126,
        127,
        128,
        129,
        UNDEFINED,
        164,
        132,
        133,
        134,
        135,
        136,
        137,
        138,
        139,
        140,
        141,
        142,
        143,
        144,
        145,
        146,
        147,
        148,
        149,
        150,
        151,
        152,
        153,
        154,
        155,
        156,
        157,
        158,
        159,
        160,
        161,
        162,
        UNDEFINED,
        197,
        165,
        166,
        167,
        168,
        169,
        170,
        171,
        172,
        173,
        174,
        175,
        176,
        177,
        178,
        179,
        180,
        181,
        182,
        183,
        184,
        185,
        186,
        187,
        188,
        189,
        190,
        191,
        192,
        193,
        194,
        195,
        UNDEFINED,
        230,
        198,
        199,
        200,
        201,
        202,
        203,
        204,
        205,
        206,
        207,
        208,
        209,
        210,
        211,
        212,
        213,
        214,
        215,
        216,
        217,
        218,
        219,
        220,
        221,
        222,
        223,
        224,
        225,
        226,
        227,
        228,
        UNDEFINED,
        263,
        231,
        232,
        233,
        234,
        235,
        236,
        237,
        238,
        239,
        240,
        241,
        242,
        243,
        244,
        245,
        246,
        247,
        248,
        249,
        250,
        251,
        252,
        253,
        254,
        255,
        256,
        257,
        258,
        259,
        260,
        261,
        UNDEFINED,
        296,
        264,
        265,
        266,
        267,
        268,
        269,
        270,
        271,
        272,
        273,
        274,
        275,
        276,
        277,
        278,
        279,
        280,
        281,
        282,
        283,
        284,
        285,
        286,
        287,
        288,
        289,
        290,
        291,
        292,
        293,
        294,
        UNDEFINED,
        329,
        297,
        298,
        299,
        300,
        301,
        302,
        303,
        304,
        305,
        306,
        307,
        308,
        309,
        310,
        311,
        312,
        313,
        314,
        315,
        316,
        317,
        318,
        319,
        320,
        321,
        322,
        323,
        324,
        325,
        326,
        327,
        UNDEFINED,
    ]


def test_follow_path():
    ad = binary_tree(8)
    assert ad.number_of_nodes() == 255
    assert follow_path(ad, 0, [0, 1, 0, 1]) == 20
    assert follow_path(ad, 0, []) == 0
    for i, w in enumerate(ad.pislo_iterator(0, 0, 10)):
        assert follow_path(ad, 0, w) == i
    assert follow_path(ad, 1, [0] * 10) == UNDEFINED

    with pytest.raises(RuntimeError):
        follow_path(ad, 1, [2])
    with pytest.raises(RuntimeError):
        follow_path(ad, 312, [0, 1, 1, 0])


def test_out_neighbors():
    l = [[0, 1], [1, 0], [2, 2]]

    d = action_digraph_helper.make(3, l)
    assert action_digraph_helper.out_neighbors(d) == l
    assert d == action_digraph_helper.make(
        3, action_digraph_helper.out_neighbors(d)
    )

    d = action_digraph_helper.make(4, l)
    ll = [[0, 1], [1, 0], [2, 2], [18446744073709551615, 18446744073709551615]]
    assert action_digraph_helper.out_neighbors(d) == ll
    assert (
        action_digraph_helper.make(4, action_digraph_helper.out_neighbors(d))
        == d
    )


def test_dot():
    l = [[0, 1], [1, 0], [2, 2]]

    d = action_digraph_helper.make(3, l)
    assert (
        str(action_digraph_helper.dot(d))
        == 'digraph {\n\tnode [shape=circle]\n\t0\n\t1\n\t2\n\t0 -> 0 [label=0 color="#cc6677"]\n\t0 -> 1 [label=1 color="#ddcc77"]\n\t1 -> 1 [label=0 color="#cc6677"]\n\t1 -> 0 [label=1 color="#ddcc77"]\n\t2 -> 2 [label=0 color="#cc6677"]\n\t2 -> 2 [label=1 color="#ddcc77"]\n}\n'  # pylint: disable=line-too-long
    )

    l = list(12 * [i] for i in range(4))

    d = action_digraph_helper.make(4, l)
    assert (
        str(action_digraph_helper.dot(d))
        == 'digraph {\n\tnode [shape=circle]\n\t0\n\t1\n\t2\n\t3\n\t0 -> 0 [label=0 color="#cc6677"]\n\t0 -> 0 [label=1 color="#ddcc77"]\n\t0 -> 0 [label=2 color="#117733"]\n\t0 -> 0 [label=3 color="#88ccee"]\n\t0 -> 0 [label=4 color="#44aa99"]\n\t0 -> 0 [label=5 color="#882255"]\n\t0 -> 0 [label=6 color="#44aa99"]\n\t0 -> 0 [label=7 color="#999933"]\n\t0 -> 0 [label=8 color="#332288"]\n\t0 -> 0 [label=9 color="#cc6677"]\n\t0 -> 0 [label=10 color="#ddcc77"]\n\t0 -> 0 [label=11 color="#117733"]\n\t1 -> 1 [label=0 color="#cc6677"]\n\t1 -> 1 [label=1 color="#ddcc77"]\n\t1 -> 1 [label=2 color="#117733"]\n\t1 -> 1 [label=3 color="#88ccee"]\n\t1 -> 1 [label=4 color="#44aa99"]\n\t1 -> 1 [label=5 color="#882255"]\n\t1 -> 1 [label=6 color="#44aa99"]\n\t1 -> 1 [label=7 color="#999933"]\n\t1 -> 1 [label=8 color="#332288"]\n\t1 -> 1 [label=9 color="#cc6677"]\n\t1 -> 1 [label=10 color="#ddcc77"]\n\t1 -> 1 [label=11 color="#117733"]\n\t2 -> 2 [label=0 color="#cc6677"]\n\t2 -> 2 [label=1 color="#ddcc77"]\n\t2 -> 2 [label=2 color="#117733"]\n\t2 -> 2 [label=3 color="#88ccee"]\n\t2 -> 2 [label=4 color="#44aa99"]\n\t2 -> 2 [label=5 color="#882255"]\n\t2 -> 2 [label=6 color="#44aa99"]\n\t2 -> 2 [label=7 color="#999933"]\n\t2 -> 2 [label=8 color="#332288"]\n\t2 -> 2 [label=9 color="#cc6677"]\n\t2 -> 2 [label=10 color="#ddcc77"]\n\t2 -> 2 [label=11 color="#117733"]\n\t3 -> 3 [label=0 color="#cc6677"]\n\t3 -> 3 [label=1 color="#ddcc77"]\n\t3 -> 3 [label=2 color="#117733"]\n\t3 -> 3 [label=3 color="#88ccee"]\n\t3 -> 3 [label=4 color="#44aa99"]\n\t3 -> 3 [label=5 color="#882255"]\n\t3 -> 3 [label=6 color="#44aa99"]\n\t3 -> 3 [label=7 color="#999933"]\n\t3 -> 3 [label=8 color="#332288"]\n\t3 -> 3 [label=9 color="#cc6677"]\n\t3 -> 3 [label=10 color="#ddcc77"]\n\t3 -> 3 [label=11 color="#117733"]\n}\n'  # pylint: disable=line-too-long
    )
