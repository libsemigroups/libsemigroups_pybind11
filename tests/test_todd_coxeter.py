import unittest
from libsemigroups_cppyy import (
    ToddCoxeter,
    FroidurePin,
    Transformation,
    KnuthBendix,
    ReportGuard,
    LibsemigroupsException,
)


class TestToddCoxeter(unittest.TestCase):
    def test_constructors(self):
        try:
            ToddCoxeter("left")
        except:
            self.fail("unexpected exception thrown")
        try:
            ToddCoxeter("right")
        except:
            self.fail("unexpected exception thrown")
        try:
            ToddCoxeter("twosided")
        except:
            self.fail("unexpected exception thrown")

        with self.assertRaises(TypeError):
            ToddCoxeter(45)
        with self.assertRaises(ValueError):
            ToddCoxeter("lft")
        with self.assertRaises(TypeError):
            ToddCoxeter("twosided", "left", "right")

        S = FroidurePin(Transformation([0, 0, 1, 2, 3]))
        with self.assertRaises(TypeError):
            ToddCoxeter(S)
        try:
            ToddCoxeter("twosided", S)
        except:
            self.fail("unexpected exception thrown")

        K = KnuthBendix()
        with self.assertRaises(TypeError):
            ToddCoxeter("left", K)
        K.set_alphabet("a")
        try:
            ToddCoxeter("left", K)
        except:
            self.fail("unexpected exception thrown")

        T = ToddCoxeter("left")
        with self.assertRaises(TypeError):
            ToddCoxeter("left", T)
        T.set_nr_generators(1)
        with self.assertRaises(TypeError):
            ToddCoxeter("right", T)
        with self.assertRaises(TypeError):
            ToddCoxeter("twosided", T)
        try:
            ToddCoxeter("left", T)
        except:
            self.fail("unexpected exception thrown")

        T = ToddCoxeter("right")
        with self.assertRaises(TypeError):
            ToddCoxeter("right", T)
        T.set_nr_generators(1)
        with self.assertRaises(TypeError):
            ToddCoxeter("left", T)
        with self.assertRaises(TypeError):
            ToddCoxeter("twosided", T)
        try:
            ToddCoxeter("right", T)
        except:
            self.fail("unexpected exception thrown")

        T = ToddCoxeter("twosided")
        with self.assertRaises(TypeError):
            ToddCoxeter("twosided", T)
        T.set_nr_generators(1)
        try:
            ToddCoxeter("left", T)
        except:
            self.fail("unexpected exception thrown")
        try:
            ToddCoxeter("right", T)
        except:
            self.fail("unexpected exception thrown")
        try:
            ToddCoxeter("twosided", T)
        except:
            self.fail("unexpected exception thrown")

        tc = ToddCoxeter("left")
        tc.set_nr_generators(2)
        with self.assertRaises(LibsemigroupsException):
            tc.add_pair([0, 0, 1, 0], [1, 1, 2])

    def test_attributes(self):
        tc = ToddCoxeter("left")
        tc.set_nr_generators(1)
        tc.add_pair([0, 0, 0, 0, 0, 0], [0, 0, 0])
        self.assertEqual(tc.nr_classes(), 5)
        self.assertTrue(tc.contains([0, 0, 0, 0, 0, 0, 0], [0, 0, 0, 0]))
        self.assertFalse(tc.contains([0, 0, 0], [0, 0]))
        self.assertEqual(tc.const_contains([0, 0, 0], [0, 0]), 0)
        self.assertEqual(tc.kind(), "left")
        self.assertFalse(tc.empty())
        self.assertEqual(tc.class_index_to_word(1), [0, 0])
        self.assertEqual(tc.word_to_class_index([0, 0]), 1)
        self.assertFalse(tc.has_parent_froidure_pin())
        self.assertEqual(tc.nr_generators(), 1)
        self.assertEqual(tc.nr_generating_pairs(), 1)
        self.assertTrue(tc.less([0], [0, 0]))

        S = FroidurePin(Transformation([1, 2, 2]), Transformation([2, 0, 1]))
        tc = ToddCoxeter("twosided", S)
        self.assertEqual(tc.nr_classes(), 24)
        self.assertTrue(tc.has_parent_froidure_pin())
        try:
            tc.parent_froidure_pin()
        except:
            self.fail("unexpected exception thrown")
        try:
            tc.quotient_froidure_pin()
        except:
            self.fail("unexpected exception thrown")

        K = KnuthBendix()
        K.set_alphabet("a")
        K.add_rule("aaaa", "aa")
        tc = ToddCoxeter("left", K)
        self.assertEqual(tc.nr_classes(), 3)

        tc = ToddCoxeter("twosided")
        tc.set_nr_generators(1)
        self.assertTrue(tc.is_quotient_obviously_infinite())
        self.assertFalse(tc.is_quotient_obviously_finite())
        tc.add_pair([0, 0, 0], [0])
        self.assertFalse(tc.is_quotient_obviously_infinite())
        tc.run()
        self.assertTrue(tc.is_quotient_obviously_finite())

    def test_operators(self):
        tc = ToddCoxeter("left")
        tc.set_nr_generators(2)
        tc.add_pair([0, 0, 0, 0], [1])
        tc.add_pair([1, 1, 1, 1], [1])
        try:
            tc.run()
        except:
            self.fail("unexpected exception thrown")
        tc = ToddCoxeter("left")
        tc.set_nr_generators(1)
        tc.add_pair([0, 0, 0], [0])
        try:
            tc.run_for(2000)
        except:
            self.fail("unexpected exception thrown")
        tc = ToddCoxeter("right")
        tc.set_nr_generators(1)
        tc.add_pair([0, 0, 0, 0, 0, 0, 0, 0], [0])
        try:
            tc.run_until(lambda: tc.nr_cosets_active() >= 4)
        except:
            self.fail("unexpected exception thrown")
        self.assertTrue(tc.stopped_by_predicate())
        self.assertFalse(tc.finished())

        tc = ToddCoxeter("left")
        tc.set_nr_generators(2)
        tc.add_pair([0, 0, 0, 0], [1])
        tc.add_pair([1, 0], [0, 1])
        tc.add_pair([1, 1, 1, 1], [0])
        tc.run_for(1)  # If this is run on an incredibly fast computer, this
        # test of timed_out() may not work.
        self.assertTrue(tc.timed_out())
        tc.run()
        self.assertFalse(tc.timed_out())

    def test_settings(self):
        ReportGuard(False)
        tc = ToddCoxeter("left")
        tc.set_nr_generators(1)
        tc.add_pair([0, 0, 0, 0], [0, 0])
        try:
            tc.reserve(10)
        except:
            self.fail("unexpected exception thrown")
        tc.run()

        self.assertFalse(tc.is_standardized())
        try:
            tc.standardize("lex")
        except:
            self.fail("unexpected exception thrown")
        self.assertTrue(tc.is_standardized())
        try:
            tc.standardize("shortlex")
        except:
            self.fail("unexpected exception thrown")
        try:
            tc.standardize("recursive")
        except:
            self.fail("unexpected exception thrown")
        try:
            tc.standardize(True)
        except:
            self.fail("unexpected exception thrown")
        try:
            tc.standardize(False)
        except:
            self.fail("unexpected exception thrown")
        with self.assertRaises(TypeError):
            tc.standardize(1)
        with self.assertRaises(ValueError):
            tc.standardize("shooortlex")

        S = FroidurePin(
            Transformation([3, 1, 2, 1, 2]), Transformation([1, 1, 1, 2, 2])
        )
        tc = ToddCoxeter("twosided", S)
        try:
            tc.froidure_pin_policy()
        except:
            self.fail("unexpected exception thrown")
        try:
            tc.froidure_pin_policy("none")
        except:
            self.fail("unexpected exception thrown")
        self.assertEqual(tc.froidure_pin_policy(), "none")
        try:
            tc.froidure_pin_policy("use_relations")
        except:
            self.fail("unexpected exception thrown")
        self.assertEqual(tc.froidure_pin_policy(), "use_relations")
        try:
            tc.froidure_pin_policy("use_cayley_graph")
        except:
            self.fail("unexpected exception thrown")
        self.assertEqual(tc.froidure_pin_policy(), "use_cayley_graph")
        with self.assertRaises(TypeError):
            tc.froidure_pin_policy(1)
        with self.assertRaises(ValueError):
            tc.froidure_pin_policy("userelations")

        tc = ToddCoxeter("left")
        tc.set_nr_generators(2)
        tc.add_pair([0, 0, 0, 0], [0])
        tc.add_pair([1, 1, 1, 1], [0])
        with self.assertRaises(ValueError):
            tc.strategy("feelsch")
        with self.assertRaises(TypeError):
            tc.strategy(1)
        try:
            tc.strategy("felsch")
        except:
            self.fail("unexpected exception thrown")
        self.assertEqual(tc.strategy(), "felsch")
        try:
            tc.strategy("hlt")
        except:
            self.fail("unexpected exception thrown")
        self.assertEqual(tc.strategy(), "hlt")
        try:
            tc.strategy("random")
        except:
            self.fail("unexpected exception thrown")
        self.assertEqual(tc.strategy(), "random")

    # FIXME: This test was failing on Travis. I had tried renaming the test to see if
    #        modifying the run order helps, but this didn't work. The relevant Travis
    #         logs are:
    #        https://travis-ci.org/github/libsemigroups/libsemigroups_cppyy/jobs/698883155
    #        and
    #        https://travis-ci.org/github/libsemigroups/libsemigroups_cppyy/jobs/698500051

    #    def test_000_iterators(self):
    #        tc = ToddCoxeter("left")
    #        tc.set_nr_generators(2)
    #        tc.add_pair([0, 0, 0, 0], [0])
    #        tc.add_pair([1, 1, 1, 1], [1])
    #        tc.add_pair([0, 1], [1, 0])
    #        self.assertEqual(
    #            tc.generating_pairs(),
    #            [[[0, 0, 0, 0], [0]], [[1, 1, 1, 1], [1]], [[0, 1], [1, 0]]],
    #        )
    #        self.assertEqual(
    #            tc.normal_forms(),
    #            [
    #                [0],
    #                [1],
    #                [0, 0],
    #                [1, 0],
    #                [1, 1],
    #                [0, 0, 0],
    #                [1, 0, 0],
    #                [1, 1, 0],
    #                [1, 1, 1],
    #                [1, 0, 0, 0],
    #                [1, 1, 0, 0],
    #                [1, 1, 1, 0],
    #                [1, 1, 0, 0, 0],
    #                [1, 1, 1, 0, 0],
    #                [1, 1, 1, 0, 0, 0],
    #            ],
    #        )
    #        S = FroidurePin(
    #            Transformation([1, 3, 4, 2, 3]), Transformation([3, 2, 1, 3, 3])
    #        )
    #        tc = ToddCoxeter("left", S)
    #        tc.add_pair(
    #            S.factorisation(Transformation([3, 4, 4, 4, 4])),
    #            S.factorisation([3, 1, 3, 3, 3]),
    #        )
    #        self.assertEqual(
    #            tc.non_trivial_classes(),
    #            [
    #                [
    #                    [0, 0, 1],
    #                    [1, 0, 1],
    #                    [0, 0, 0, 1],
    #                    [0, 1, 0, 1],
    #                    [1, 0, 0, 1],
    #                    [0, 0, 0, 0, 1],
    #                    [0, 0, 1, 0, 1],
    #                    [1, 0, 0, 0, 1],
    #                    [1, 0, 1, 0, 1],
    #                    [0, 0, 0, 1, 0, 1],
    #                    [0, 1, 0, 0, 0, 1],
    #                    [0, 1, 0, 1, 0, 1],
    #                    [1, 0, 0, 1, 0, 1],
    #                    [0, 0, 0, 0, 1, 0, 1],
    #                    [0, 0, 1, 0, 0, 0, 1],
    #                    [1, 0, 0, 0, 1, 0, 1],
    #                    [0, 1, 0, 0, 0, 1, 0, 1],
    #                    [0, 0, 1, 0, 0, 0, 1, 0, 0],
    #                    [0, 0, 1, 0, 0, 0, 1, 0, 1],
    #                    [0, 1, 0, 0, 0, 1, 1, 0, 0],
    #                ]
    #            ],
    #        )

    # The following are some tests adapted from the libsemigroups test file

    def test_020(self):
        tc = ToddCoxeter("twosided")
        tc.set_nr_generators(1)
        tc.strategy("hlt")
        self.assertTrue(tc.contains([0, 0], [0, 0]))
        self.assertFalse(tc.contains([0, 0], [0]))

    def test_021(self):
        tc = ToddCoxeter("twosided")
        tc.set_nr_generators(5)
        with self.assertRaises(LibsemigroupsException):
            tc.run()

    def test_033(self):
        tc1 = ToddCoxeter("twosided")
        tc1.set_nr_generators(2)
        tc1.add_pair([0, 0, 0], [0])
        tc1.add_pair([0], [1, 1])
        self.assertEqual(tc1.nr_classes(), 5)
        tc2 = ToddCoxeter("left", tc1)
        tc2.next_lookahead(1)
        tc2.report_every(1)
        self.assertFalse(tc2.empty())
        tc2.add_pair([0], [0, 0])
        self.assertEqual(tc2.nr_classes(), 3)

    def test_036(self):
        S = FroidurePin(
            Transformation([1, 3, 4, 2, 3]), Transformation([3, 2, 1, 3, 3])
        )
        tc = ToddCoxeter("twosided")
        tc.set_nr_generators(2)
        tc.add_pair([0], [1])
        tc.add_pair([0, 0], [0])
        self.assertEqual(tc.nr_classes(), 1)
        with self.assertRaises(LibsemigroupsException):
            tc.prefill(S.right_cayley_graph())

    def test_096(self):
        tc = ToddCoxeter("twosided")
        tc.set_nr_generators(2)
        tc.add_pair([0], [1])
        tc.add_pair([0, 0], [0])
        tc.strategy("felsch")
        self.assertEqual(tc.strategy(), "felsch")
        self.assertFalse(tc.complete())
        self.assertTrue(tc.compatible())
        self.assertEqual(tc.nr_classes(), 1)
        self.assertEqual(tc.normal_forms(), [[0]])
        self.assertTrue(tc.complete())
        self.assertTrue(tc.compatible())

        copy = tc
        self.assertEqual(copy.nr_generators(), 2)
        self.assertEqual(copy.nr_generating_pairs(), 2)
        self.assertTrue(copy.finished())
        self.assertEqual(copy.nr_classes(), 1)
        self.assertEqual(copy.froidure_pin_policy(), "none")
        self.assertTrue(copy.complete())
        self.assertTrue(copy.compatible())
