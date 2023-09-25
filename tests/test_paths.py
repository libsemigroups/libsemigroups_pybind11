def test_020():
    "cbegin/end_panislo - 100 node path"
    wg = WordGraph()
    n = 100
    wg.add_nodes(n)
    wg.add_to_out_degree(2)
    for i in range(n - 1):
        wg.add_edge(i, i + 1, i % 2)
    assert len(list(wg.panilo_iterator(0, 0, POSITIVE_INFINITY))) == 100
    assert sum(1 for _ in wg.panilo_iterator(50, 0, POSITIVE_INFINITY)) == 50
    pths = list(wg.panislo_iterator(0, 0, POSITIVE_INFINITY))
    assert len(pths) == 100
    assert pths[3][0] == [0, 1, 0]

    assert sum(1 for _ in wg.panislo_iterator(50, 0, POSITIVE_INFINITY)) == 50


def test_024():

    wg = WordGraph()
    wg.add_nodes(6)
    wg.add_to_out_degree(2)

    wg.add_edge(0, 1, 0)
    wg.add_edge(0, 2, 1)
    wg.add_edge(1, 3, 0)
    wg.add_edge(1, 4, 1)
    wg.add_edge(2, 4, 0)
    wg.add_edge(2, 2, 1)
    wg.add_edge(3, 1, 0)
    wg.add_edge(3, 5, 1)
    wg.add_edge(4, 5, 0)
    wg.add_edge(4, 4, 1)
    wg.add_edge(5, 4, 0)
    wg.add_edge(5, 5, 1)

    wg.validate()

    N = 18

    assert wg.number_of_paths(0, 4, 0, N) == 131062
    assert wg.number_of_paths(0, 4, 10, N) == 130556
    assert wg.number_of_paths(4, 1, 0, N) == 0
    assert wg.number_of_paths(0, 0, POSITIVE_INFINITY) == POSITIVE_INFINITY
    assert wg.number_of_paths(0, 0, 10) == 1023

    assert not action_digraph_helper.is_acyclic(wg)

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

    assert list(wg.pstilo_iterator(0, 4, 0, 5)) == expected

    expected = []
    for w in wilo(2, N, [], [1] * N):
        node = action_digraph_helper.follow_path(wg, 0, w)
        if node == 4:
            expected.append(w)
    assert len(expected) == 131_062

    result = list(wg.pstilo_iterator(0, 4, 0, N))
    assert len(result) == 131_062
    assert result == expected


def test_036():
    algorithm = WordGraph.algorithm

    wg = WordGraph()
    wg.add_nodes(10)
    wg.add_to_out_degree(20)
    wg.add_edge(0, 7, 5)
    wg.add_edge(0, 5, 7)
    wg.add_edge(1, 9, 14)
    wg.add_edge(1, 5, 17)
    wg.add_edge(3, 8, 5)
    wg.add_edge(5, 8, 1)
    wg.add_edge(6, 8, 14)
    wg.add_edge(7, 8, 10)
    wg.add_edge(8, 9, 12)
    wg.add_edge(8, 9, 13)

    assert is_acyclic(wg)
    assert not wg.validate()

    assert wg.number_of_paths_algorithm(0, 0, 16) == algorithm.acyclic
    assert wg.number_of_paths(0, 0, 30) == 9
    assert wg.number_of_paths(1, 0, 10, algorithm.matrix) == 6
    assert wg.number_of_paths(1, 9, 0, 10, algorithm.matrix) == 3


def test_037():  # pylint: disable=too-many-statements
    algorithm = WordGraph.algorithm
    n = 10
    wg = WordGraph()
    wg.add_nodes(10)
    wg.add_to_out_degree(20)
    wg.add_edge(0, 9, 0)
    wg.add_edge(0, 1, 1)
    wg.add_edge(0, 6, 2)
    wg.add_edge(0, 3, 3)
    wg.add_edge(0, 7, 4)
    wg.add_edge(0, 2, 5)
    wg.add_edge(0, 2, 6)
    wg.add_edge(0, 8, 7)
    wg.add_edge(0, 1, 8)
    wg.add_edge(0, 4, 9)
    wg.add_edge(0, 3, 10)
    wg.add_edge(0, 1, 11)
    wg.add_edge(0, 7, 12)
    wg.add_edge(0, 9, 13)
    wg.add_edge(0, 4, 14)
    wg.add_edge(0, 7, 15)
    wg.add_edge(0, 8, 16)
    wg.add_edge(0, 9, 17)
    wg.add_edge(0, 6, 18)
    wg.add_edge(0, 9, 19)
    wg.add_edge(1, 8, 0)
    wg.add_edge(1, 2, 1)
    wg.add_edge(1, 5, 2)
    wg.add_edge(1, 7, 3)
    wg.add_edge(1, 9, 4)
    wg.add_edge(1, 0, 5)
    wg.add_edge(1, 2, 6)
    wg.add_edge(1, 4, 7)
    wg.add_edge(1, 0, 8)
    wg.add_edge(1, 3, 9)
    wg.add_edge(1, 2, 10)
    wg.add_edge(1, 7, 11)
    wg.add_edge(1, 2, 12)
    wg.add_edge(1, 7, 13)
    wg.add_edge(1, 6, 14)
    wg.add_edge(1, 6, 15)
    wg.add_edge(1, 5, 16)
    wg.add_edge(1, 4, 17)
    wg.add_edge(1, 6, 18)
    wg.add_edge(1, 3, 19)
    wg.add_edge(2, 2, 0)
    wg.add_edge(2, 9, 1)
    wg.add_edge(2, 0, 2)
    wg.add_edge(2, 6, 3)
    wg.add_edge(2, 7, 4)
    wg.add_edge(2, 9, 5)
    wg.add_edge(2, 5, 6)
    wg.add_edge(2, 4, 7)
    wg.add_edge(2, 9, 8)
    wg.add_edge(2, 7, 9)
    wg.add_edge(2, 9, 10)
    wg.add_edge(2, 9, 11)
    wg.add_edge(2, 0, 12)
    wg.add_edge(2, 7, 13)
    wg.add_edge(2, 9, 14)
    wg.add_edge(2, 6, 15)
    wg.add_edge(2, 3, 16)
    wg.add_edge(2, 3, 17)
    wg.add_edge(2, 4, 18)
    wg.add_edge(2, 1, 19)
    wg.add_edge(3, 1, 0)
    wg.add_edge(3, 9, 1)
    wg.add_edge(3, 6, 2)
    wg.add_edge(3, 2, 3)
    wg.add_edge(3, 9, 4)
    wg.add_edge(3, 8, 5)
    wg.add_edge(3, 1, 6)
    wg.add_edge(3, 6, 7)
    wg.add_edge(3, 1, 8)
    wg.add_edge(3, 0, 9)
    wg.add_edge(3, 5, 10)
    wg.add_edge(3, 0, 11)
    wg.add_edge(3, 2, 12)
    wg.add_edge(3, 7, 13)
    wg.add_edge(3, 4, 14)
    wg.add_edge(3, 0, 15)
    wg.add_edge(3, 4, 16)
    wg.add_edge(3, 8, 17)
    wg.add_edge(3, 3, 18)
    wg.add_edge(3, 1, 19)
    wg.add_edge(4, 0, 0)
    wg.add_edge(4, 4, 1)
    wg.add_edge(4, 8, 2)
    wg.add_edge(4, 5, 3)
    wg.add_edge(4, 5, 4)
    wg.add_edge(4, 1, 5)
    wg.add_edge(4, 3, 6)
    wg.add_edge(4, 8, 7)
    wg.add_edge(4, 4, 8)
    wg.add_edge(4, 4, 9)
    wg.add_edge(4, 4, 10)
    wg.add_edge(4, 7, 11)
    wg.add_edge(4, 8, 12)
    wg.add_edge(4, 6, 13)
    wg.add_edge(4, 3, 14)
    wg.add_edge(4, 7, 15)
    wg.add_edge(4, 6, 16)
    wg.add_edge(4, 7, 17)
    wg.add_edge(4, 0, 18)
    wg.add_edge(4, 2, 19)
    wg.add_edge(5, 3, 0)
    wg.add_edge(5, 0, 1)
    wg.add_edge(5, 4, 2)
    wg.add_edge(5, 7, 3)
    wg.add_edge(5, 2, 4)
    wg.add_edge(5, 5, 5)
    wg.add_edge(5, 7, 6)
    wg.add_edge(5, 7, 7)
    wg.add_edge(5, 7, 8)
    wg.add_edge(5, 7, 9)
    wg.add_edge(5, 0, 10)
    wg.add_edge(5, 8, 11)
    wg.add_edge(5, 6, 12)
    wg.add_edge(5, 8, 13)
    wg.add_edge(5, 8, 14)
    wg.add_edge(5, 1, 15)
    wg.add_edge(5, 5, 16)
    wg.add_edge(5, 5, 17)
    wg.add_edge(5, 3, 18)
    wg.add_edge(5, 7, 19)
    wg.add_edge(6, 8, 0)
    wg.add_edge(6, 7, 1)
    wg.add_edge(6, 6, 2)
    wg.add_edge(6, 5, 3)
    wg.add_edge(6, 6, 4)
    wg.add_edge(6, 1, 5)
    wg.add_edge(6, 7, 6)
    wg.add_edge(6, 2, 7)
    wg.add_edge(6, 7, 8)
    wg.add_edge(6, 3, 9)
    wg.add_edge(6, 3, 10)
    wg.add_edge(6, 8, 11)
    wg.add_edge(6, 3, 12)
    wg.add_edge(6, 9, 13)
    wg.add_edge(6, 4, 14)
    wg.add_edge(6, 1, 15)
    wg.add_edge(6, 4, 16)
    wg.add_edge(6, 3, 17)
    wg.add_edge(6, 9, 18)
    wg.add_edge(6, 8, 19)
    wg.add_edge(7, 9, 0)
    wg.add_edge(7, 4, 1)
    wg.add_edge(7, 3, 2)
    wg.add_edge(7, 8, 3)
    wg.add_edge(7, 0, 4)
    wg.add_edge(7, 5, 5)
    wg.add_edge(7, 6, 6)
    wg.add_edge(7, 8, 7)
    wg.add_edge(7, 9, 8)
    wg.add_edge(7, 1, 9)
    wg.add_edge(7, 7, 10)
    wg.add_edge(7, 0, 11)
    wg.add_edge(7, 6, 12)
    wg.add_edge(7, 2, 13)
    wg.add_edge(7, 3, 14)
    wg.add_edge(7, 8, 15)
    wg.add_edge(7, 6, 16)
    wg.add_edge(7, 3, 17)
    wg.add_edge(7, 2, 18)
    wg.add_edge(7, 7, 19)
    wg.add_edge(8, 0, 0)
    wg.add_edge(8, 6, 1)
    wg.add_edge(8, 3, 2)
    wg.add_edge(8, 5, 3)
    wg.add_edge(8, 7, 4)
    wg.add_edge(8, 9, 5)
    wg.add_edge(8, 9, 6)
    wg.add_edge(8, 8, 7)
    wg.add_edge(8, 1, 8)
    wg.add_edge(8, 5, 9)
    wg.add_edge(8, 7, 10)
    wg.add_edge(8, 9, 11)
    wg.add_edge(8, 6, 12)
    wg.add_edge(8, 0, 13)
    wg.add_edge(8, 0, 14)
    wg.add_edge(8, 3, 15)
    wg.add_edge(8, 6, 16)
    wg.add_edge(8, 0, 17)
    wg.add_edge(8, 8, 18)
    wg.add_edge(8, 9, 19)
    wg.add_edge(9, 3, 0)
    wg.add_edge(9, 7, 1)
    wg.add_edge(9, 9, 2)
    wg.add_edge(9, 1, 3)
    wg.add_edge(9, 4, 4)
    wg.add_edge(9, 9, 5)
    wg.add_edge(9, 4, 6)
    wg.add_edge(9, 0, 7)
    wg.add_edge(9, 5, 8)
    wg.add_edge(9, 8, 9)
    wg.add_edge(9, 3, 10)
    wg.add_edge(9, 2, 11)
    wg.add_edge(9, 0, 12)
    wg.add_edge(9, 2, 13)
    wg.add_edge(9, 3, 14)
    wg.add_edge(9, 4, 15)
    wg.add_edge(9, 0, 16)
    wg.add_edge(9, 5, 17)
    wg.add_edge(9, 3, 18)
    wg.add_edge(9, 5, 19)
    assert not is_acyclic(wg)
    assert wg.validate()

    assert wg.number_of_paths_algorithm(0) == algorithm.acyclic
    assert wg.number_of_paths(0) == POSITIVE_INFINITY
    with pytest.raises(RuntimeError):
        wg.number_of_paths(0, 0, 10, algorithm.acyclic)
    with pytest.raises(RuntimeError):
        wg.number_of_paths(1, 9, 0, 10, algorithm.acyclic)

    wg = binary_tree(n)
    assert wg.number_of_paths_algorithm(0) == algorithm.acyclic
    assert wg.number_of_paths(0) == 1023

    add_cycle(wg, n)
    wg.add_edge(0, n + 1, 0)
    assert not is_acyclic(wg)
    assert not wg.validate()
    assert wg.number_of_paths(1) == 511
    assert (
        wg.number_of_paths_algorithm(1, 0, POSITIVE_INFINITY)
        == algorithm.acyclic
    )
    assert wg.number_of_paths(1, 0, POSITIVE_INFINITY) == 511
    assert len(topological_sort(wg)) == 0
    for m in wg.nodes_iterator():
        if len(topological_sort(wg, m)) == 0:
            assert m == 1023
            break


def test_follow_path():
    wg = binary_tree(8)
    assert wg.number_of_nodes() == 255
    assert follow_path(wg, 0, [0, 1, 0, 1]) == 20
    assert follow_path(wg, 0, []) == 0
    for i, w in enumerate(wg.pislo_iterator(0, 0, 10)):
        assert follow_path(wg, 0, w) == i
    assert follow_path(wg, 1, [0] * 10) == UNDEFINED

    with pytest.raises(RuntimeError):
        follow_path(wg, 1, [2])
    with pytest.raises(RuntimeError):
        follow_path(wg, 312, [0, 1, 1, 0])
