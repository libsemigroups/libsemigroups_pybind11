# -*- coding: utf-8 -*-

# Copyright (c) 2023, M. T. Whyte
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""
This file contains tests for WordGraph and related functionality in
libsemigroups_pybind11.
"""

# pylint: disable=no-name-in-module, missing-function-docstring, invalid-name,
# pylint: disable=duplicate-code, too-many-lines

import pytest

from libsemigroups_pybind11 import (
    POSITIVE_INFINITY,
    UNDEFINED,
    Gabow,
    WordGraph,
    word_graph,
)


def test_001():
    g = WordGraph(17, 31)
    for i in range(17):
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


def test_002():
    g = WordGraph()
    g.add_to_out_degree(1)
    add_cycle(g, 32)
    assert g.scc_id(0) == 0

    g = WordGraph()
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


def test_003():
    graph = WordGraph(0)
    for j in range(1, 100):
        graph.add_nodes(j)
        for i in range(j * (j + 1) // 2):
            assert graph.scc_id(i) == i


def test_004():
    graph = WordGraph(10, 5)
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


def test_005():
    for k in range(2, 50):
        graph = WordGraph(k, k)
        for i in range(k):
            for j in range(k):
                graph.add_edge(i, j, j)

    assert graph.number_of_scc() == 1

    forest = graph.spanning_forest()
    assert forest.parent(k - 1) == UNDEFINED
    graph.reverse_spanning_forest()


def test_006():
    j = 33
    graph = WordGraph()
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
