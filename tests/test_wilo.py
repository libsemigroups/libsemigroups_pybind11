import unittest
from libsemigroups_pybind11 import wilo, number_of_words


class TestWilo(unittest.TestCase):
    def test_001(self):
        u = [0, 0, 0, 0]
        v = [1, 1, 1, 1]
        self.assertEqual([x for x in wilo(2, 1, v, u)], [])
        self.assertEqual([x for x in wilo(2, 1, u, u)], [])
        w1 = [x for x in wilo(2, 2, [], [1, 1])]
        self.assertEqual(len(w1), 3)
        w2 = []
        for i in range(0, len(w1)):
            w2.append([x for x in w1[i]])
        self.assertEqual(w2, [[], [0], [1]])
        w3 = [x for x in wilo(2, 1, [], [1, 1])]
        self.assertEqual(len(w3), 1)
        self.assertEqual([x for x in w3[0]], [])
        w4 = [x for x in wilo(2, 1, [], [0])]
        self.assertEqual(len(w4), 1)
        self.assertEqual([x for x in w4[0]], [])

    def test_002(self):
        w = [x for x in wilo(2, 4, [0], [1, 1, 1, 1])]
        u = []
        for i in range(0, len(w)):
            u.append([x for x in w[i]])
        self.assertEqual(
            u,
            [
                [0],
                [0, 0],
                [0, 0, 0],
                [0, 0, 1],
                [0, 1],
                [0, 1, 0],
                [0, 1, 1],
                [1],
                [1, 0],
                [1, 0, 0],
                [1, 0, 1],
                [1, 1],
                [1, 1, 0],
                [1, 1, 1],
            ],
        )
        self.assertEqual(len(u), 14)

    def test_003(self):
        first = [0]
        last = [2, 2, 2, 2]
        w = [x for x in wilo(3, 4, first, last)]
        u = []
        self.assertEqual(len(w), 39)
        self.assertEqual(len(w), number_of_words(3, 1, 4))
        for i in range(0, len(w)):
            u.append([x for x in w[i]])
        self.assertEqual(
            u,
            [
                [0],
                [0, 0],
                [0, 0, 0],
                [0, 0, 1],
                [0, 0, 2],
                [0, 1],
                [0, 1, 0],
                [0, 1, 1],
                [0, 1, 2],
                [0, 2],
                [0, 2, 0],
                [0, 2, 1],
                [0, 2, 2],
                [1],
                [1, 0],
                [1, 0, 0],
                [1, 0, 1],
                [1, 0, 2],
                [1, 1],
                [1, 1, 0],
                [1, 1, 1],
                [1, 1, 2],
                [1, 2],
                [1, 2, 0],
                [1, 2, 1],
                [1, 2, 2],
                [2],
                [2, 0],
                [2, 0, 0],
                [2, 0, 1],
                [2, 0, 2],
                [2, 1],
                [2, 1, 0],
                [2, 1, 1],
                [2, 1, 2],
                [2, 2],
                [2, 2, 0],
                [2, 2, 1],
                [2, 2, 2],
            ],
        )

    def test_004(self):
        first = []
        last = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
        w = [x for x in wilo(3, 10, first, last)]
        self.assertEqual(len(w), 29524)
        self.assertEqual(len(w), number_of_words(3, 0, 10))

    def test_004(self):
        first = []
        last = [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
        w = [x for x in wilo(3, 13, first, last)]
        self.assertEqual(len(w), 797161)
        self.assertEqual(len(w), number_of_words(3, 0, 13))

    def test_005(self):
        first = []
        last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        w = [x for x in wilo(1, 10, first, last)]
        self.assertEqual(len(w), 10)
        u = []
        for i in range(0, len(w)):
            u.append([x for x in w[i]])
        self.assertEqual(
            u,
            [
                [],
                [0],
                [0, 0],
                [0, 0, 0],
                [0, 0, 0, 0],
                [0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0, 0],
            ],
        )
        w = [x for x in wilo(0, 0, first, last)]
        self.assertEqual(w, [])
        first = [0, 0]
        last = [0, 0, 0, 0]
        w = [x for x in wilo(1, 4, first, last)]
        self.assertEqual(len(w), 2)
        self.assertEqual([x for x in w[0]], [0, 0])
        self.assertEqual([x for x in w[1]], [0, 0, 0])
        w = [x for x in wilo(1, 1, first, last)]
        self.assertEqual(w, [])
        w = [x for x in wilo(1, 5, [0], [1])]
        self.assertEqual(len(w), 4)
        u = []
        for i in range(0, len(w)):
            u.append([x for x in w[i]])
        self.assertEqual(u, [[0], [0, 0], [0, 0, 0], [0, 0, 0, 0]])

    def test_006(self):
        first = [0, 1, 2, 3]
        last = [4, 4, 4, 4, 4]
        w = [x for x in wilo(4, 5, first, last)]
        self.assertEqual(len(w), 303)

    def test_007(self):
        first = [0, 1]
        last = [1, 1, 1]
        w = [x for x in wilo(2, 3, first, last)]
        self.assertEqual(len(w), 4)
        u = []
        for i in range(0, len(w)):
            u.append([x for x in w[i]])
        self.assertEqual(u, [[0, 1], [1], [1, 0], [1, 1]])
        w = [x for x in wilo(2, 1, first, last)]
        self.assertEqual(w, [])
