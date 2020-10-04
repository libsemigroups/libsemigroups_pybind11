import unittest, libsemigroups_pybind11
from libsemigroups_pybind11 import wislo, number_of_words


class WisloTests(unittest.TestCase):
    def test_000(self):
        first = [0]
        last = [0, 0, 0, 0]
        w = [x for x in wislo(2, first, last)]
        self.assertEqual(len(w), 14)
        u = []
        for i in range(0, len(w)):
            u.append([x for x in w[i]])
        self.assertEqual(
            u,
            [
                [0],
                [1],
                [0, 0],
                [0, 1],
                [1, 0],
                [1, 1],
                [0, 0, 0],
                [0, 0, 1],
                [0, 1, 0],
                [0, 1, 1],
                [1, 0, 0],
                [1, 0, 1],
                [1, 1, 0],
                [1, 1, 1],
            ],
        )

    def test_001(self):
        first = [0, 0, 0, 0]
        last = [0, 0, 0, 0, 0]
        w1 = [x for x in wislo(2, last, first)]
        self.assertEqual(w1, [])
        w2 = [x for x in wislo(2, last, last)]
        self.assertEqual(w2, [])
        w3 = [x for x in wislo(2, last, [0, 0, 0, 0, 0, 0])]
        self.assertEqual(len(w3), 32)
        u = []
        for i in range(0, len(w3)):
            u.append([x for x in w3[i]])
        self.assertEqual(
            u,
            [
                [0, 0, 0, 0, 0],
                [0, 0, 0, 0, 1],
                [0, 0, 0, 1, 0],
                [0, 0, 0, 1, 1],
                [0, 0, 1, 0, 0],
                [0, 0, 1, 0, 1],
                [0, 0, 1, 1, 0],
                [0, 0, 1, 1, 1],
                [0, 1, 0, 0, 0],
                [0, 1, 0, 0, 1],
                [0, 1, 0, 1, 0],
                [0, 1, 0, 1, 1],
                [0, 1, 1, 0, 0],
                [0, 1, 1, 0, 1],
                [0, 1, 1, 1, 0],
                [0, 1, 1, 1, 1],
                [1, 0, 0, 0, 0],
                [1, 0, 0, 0, 1],
                [1, 0, 0, 1, 0],
                [1, 0, 0, 1, 1],
                [1, 0, 1, 0, 0],
                [1, 0, 1, 0, 1],
                [1, 0, 1, 1, 0],
                [1, 0, 1, 1, 1],
                [1, 1, 0, 0, 0],
                [1, 1, 0, 0, 1],
                [1, 1, 0, 1, 0],
                [1, 1, 0, 1, 1],
                [1, 1, 1, 0, 0],
                [1, 1, 1, 0, 1],
                [1, 1, 1, 1, 0],
                [1, 1, 1, 1, 1],
            ],
        )

    def test_002(self):
        first = []
        last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        w = [x for x in wislo(3, first, last)]
        self.assertEqual(len(w), 29524)
        self.assertEqual(len(w), number_of_words(3, 0, 10))

    def test_003(self):
        first = []
        last = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        w = [x for x in wislo(3, first, last)]
        self.assertEqual(len(w), 797161)
        self.assertEqual(len(w), number_of_words(3, 0, 13))
