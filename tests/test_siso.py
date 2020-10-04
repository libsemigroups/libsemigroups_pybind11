import unittest
from libsemigroups_pybind11 import silo, sislo, number_of_words


class TestSilo(unittest.TestCase):
    def test_001(self):
        u = ""
        v = "aaaaaaaaaa"
        w = [x for x in silo("a", 10, "", "aaaaaaaaaa")]
        self.assertEqual(len(w), 10)
        self.assertEqual(
            w,
            [
                "",
                "a",
                "aa",
                "aaa",
                "aaaa",
                "aaaaa",
                "aaaaaa",
                "aaaaaaa",
                "aaaaaaaa",
                "aaaaaaaaa",
            ],
        )
        w = [x for x in silo("", 10, "", "aaaaaaaaaa")]
        self.assertEqual(len(w), 1)
        w = [x for x in silo("a", 4, "aa", v)]
        self.assertEqual(w, ["aa", "aaa"])

    def test_002(self):
        u = ""
        v = "aaaaaaaaaa"
        w1 = [x for x in silo("ab", 4, v, u)]
        self.assertEqual(w1, [])
        w2 = [x for x in silo("ab", 4, u, u)]
        self.assertEqual(w2, [])
        w3 = [x for x in silo("ab", 2, "a", "bb")]
        self.assertEqual(w3, ["a", "b"])
        w4 = [x for x in silo("ab", 1, u, v)]
        self.assertEqual(w4, [""])

    def test_003(self):
        w = [x for x in silo("ab", 4, "a", "bbbbb")]
        self.assertEqual(len(w), 14)
        self.assertEqual(
            w,
            [
                "a",
                "aa",
                "aaa",
                "aab",
                "ab",
                "aba",
                "abb",
                "b",
                "ba",
                "baa",
                "bab",
                "bb",
                "bba",
                "bbb",
            ],
        )
        w = [x for x in silo("ba", 4, "b", "aaaaa")]
        self.assertEqual(
            w,
            [
                "b",
                "bb",
                "bbb",
                "bba",
                "ba",
                "bab",
                "baa",
                "a",
                "ab",
                "abb",
                "aba",
                "aa",
                "aab",
                "aaa",
            ],
        )

    def test_004(self):
        w = [x for x in silo("abc", 13, "", "ccccccccccccc")]
        self.assertEqual(len(w), 797161)
        self.assertEqual(len(w), number_of_words(3, 0, 13))

    def test_005(self):
        first = ""
        last = "aaaaaaaaaa"
        w = [x for x in sislo("a", first, last)]
        self.assertEqual(len(w), 10)
        self.assertEqual(
            w,
            [
                "",
                "a",
                "aa",
                "aaa",
                "aaaa",
                "aaaaa",
                "aaaaaa",
                "aaaaaaa",
                "aaaaaaaa",
                "aaaaaaaaa",
            ],
        )
        w = [x for x in sislo("a", "aa", "aaaa")]
        self.assertEqual(w, ["aa", "aaa"])

    def test_006(self):
        first = ""
        last = "bbaaab"
        w1 = [x for x in sislo("ab", last, first)]
        self.assertEqual(w1, [])
        w2 = [x for x in sislo("ab", first, first)]
        self.assertEqual(w2, [])
        w3 = [x for x in sislo("ab", "a", "aa")]
        self.assertEqual(w3, ["a", "b"])
        w4 = [x for x in sislo("ab", first, last)]
        self.assertEqual(len(w4), 112)
        self.assertEqual(
            w4,
            [
                "",
                "a",
                "b",
                "aa",
                "ab",
                "ba",
                "bb",
                "aaa",
                "aab",
                "aba",
                "abb",
                "baa",
                "bab",
                "bba",
                "bbb",
                "aaaa",
                "aaab",
                "aaba",
                "aabb",
                "abaa",
                "abab",
                "abba",
                "abbb",
                "baaa",
                "baab",
                "baba",
                "babb",
                "bbaa",
                "bbab",
                "bbba",
                "bbbb",
                "aaaaa",
                "aaaab",
                "aaaba",
                "aaabb",
                "aabaa",
                "aabab",
                "aabba",
                "aabbb",
                "abaaa",
                "abaab",
                "ababa",
                "ababb",
                "abbaa",
                "abbab",
                "abbba",
                "abbbb",
                "baaaa",
                "baaab",
                "baaba",
                "baabb",
                "babaa",
                "babab",
                "babba",
                "babbb",
                "bbaaa",
                "bbaab",
                "bbaba",
                "bbabb",
                "bbbaa",
                "bbbab",
                "bbbba",
                "bbbbb",
                "aaaaaa",
                "aaaaab",
                "aaaaba",
                "aaaabb",
                "aaabaa",
                "aaabab",
                "aaabba",
                "aaabbb",
                "aabaaa",
                "aabaab",
                "aababa",
                "aababb",
                "aabbaa",
                "aabbab",
                "aabbba",
                "aabbbb",
                "abaaaa",
                "abaaab",
                "abaaba",
                "abaabb",
                "ababaa",
                "ababab",
                "ababba",
                "ababbb",
                "abbaaa",
                "abbaab",
                "abbaba",
                "abbabb",
                "abbbaa",
                "abbbab",
                "abbbba",
                "abbbbb",
                "baaaaa",
                "baaaab",
                "baaaba",
                "baaabb",
                "baabaa",
                "baabab",
                "baabba",
                "baabbb",
                "babaaa",
                "babaab",
                "bababa",
                "bababb",
                "babbaa",
                "babbab",
                "babbba",
                "babbbb",
                "bbaaaa",
            ],
        )

    def test_007(self):
        w = [x for x in sislo("ab", "a", "aaaa")]
        self.assertEqual(
            w,
            [
                "a",
                "b",
                "aa",
                "ab",
                "ba",
                "bb",
                "aaa",
                "aab",
                "aba",
                "abb",
                "baa",
                "bab",
                "bba",
                "bbb",
            ],
        )
        self.assertEqual(len(w), 14)
        w = [x for x in sislo("ba", "b", "bbbb")]
        self.assertEqual(
            w,
            [
                "b",
                "a",
                "bb",
                "ba",
                "ab",
                "aa",
                "bbb",
                "bba",
                "bab",
                "baa",
                "abb",
                "aba",
                "aab",
                "aaa",
            ],
        )

    def test_008(self):
        w = [x for x in sislo("abc", "", "aaaaaaaaaaaaa")]
        self.assertEqual(len(w), 797161)
        self.assertEqual(len(w), number_of_words(3, 0, 13))
