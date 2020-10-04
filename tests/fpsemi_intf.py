"""
This module contains some functions used in tests for FpSemigroupInterface
derived classes, i.e. KnuthBendix, FpSemigroup, etc.
"""


def check_validation(self, t):
    # ReportGuard(False)
    x = t()
    x.set_alphabet("ab")

    with self.assertRaises(RuntimeError):
        x.validate_letter("c")
    try:
        x.validate_letter("a")
    except Exception as e:
        self.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )

    with self.assertRaises(RuntimeError):
        x.validate_letter(3)
    try:
        x.validate_letter(0)
    except Exception as e:
        self.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )
    with self.assertRaises(RuntimeError):
        x.validate_word("abc")
    try:
        x.validate_word("abab")
    except Exception as e:
        self.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )

    with self.assertRaises(RuntimeError):
        x.validate_word([0, 1, 2])

    try:
        x.validate_word([0, 1, 0, 1])
    except Exception as e:
        self.fail(
            "unexpected exception raised for FpSemigroupInterface::validate_letter: "
            + e
        )


def check_converters(self, t):
    # ReportGuard(False)
    x = t()
    x.set_alphabet("ba")
    self.assertEqual(x.char_to_uint("a"), 1)
    self.assertEqual(x.char_to_uint("b"), 0)

    self.assertEqual(x.string_to_word("ab"), [1, 0])
    self.assertEqual(x.string_to_word("aaaaaa"), [1] * 6)
    with self.assertRaises(RuntimeError):
        x.string_to_word("c")

    self.assertEqual(x.uint_to_char(0), "b")
    self.assertEqual(x.uint_to_char(1), "a")
    with self.assertRaises(RuntimeError):
        x.uint_to_char(2)

    self.assertEqual(x.word_to_string([1, 0]), "ab")
    self.assertEqual(x.word_to_string([1] * 6), "a" * 6)
    with self.assertRaises(RuntimeError):
        x.word_to_string([2])


def check_initialisation(self, t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("ba")
    x.add_rule([0, 1], [1, 0])

    with self.assertRaises(TypeError):
        x.add_rule([0, 1], [2])

    S = FroidurePin(Transformation([1, 2, 0]), Transformation([1, 0, 2]))
    x.add_rules(S)
    self.assertEqual(x.size(), 2)

    x = t()
    x.set_alphabet("abBe")
    x.set_identity("e")

    x.set_inverses("aBbe")

    x.add_rule("bb", "B")
    x.add_rule("BaBa", "abab")
    self.assertEqual(x.size(), 24)

    x = t()
    x.set_alphabet(1)
    x.set_identity(0)
    self.assertEqual(x.size(), 1)


def check_attributes(self, t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("abBe")
    x.set_identity("e")
    x.set_inverses("aBbe")

    x.add_rule("bb", "B")
    x.add_rule("BaBa", "abab")
    x.run()

    if compare_version_numbers("1.1.0", libsemigroups_version()):
        self.assertEqual(
            x.rules(),
            [
                ["ae", "a"],
                ["ea", "a"],
                ["be", "b"],
                ["eb", "b"],
                ["Be", "B"],
                ["eB", "B"],
                ["ee", "e"],
                ["aa", "e"],
                ["aa", "e"],
                ["bB", "e"],
                ["Bb", "e"],
                ["Bb", "e"],
                ["bB", "e"],
                ["ee", "e"],
                ["ee", "e"],
                ["bb", "B"],
                ["BaBa", "abab"],
            ],
        )
        self.assertEqual(x.nr_rules(), 17)
    else:
        self.assertEqual(
            x.rules(),
            [
                ["ae", "a"],
                ["ea", "a"],
                ["be", "b"],
                ["eb", "b"],
                ["Be", "B"],
                ["eB", "B"],
                ["ee", "e"],
                ["aa", "e"],
                ["bB", "e"],
                ["Bb", "e"],
                ["bb", "B"],
                ["BaBa", "abab"],
            ],
        )
        self.assertEqual(x.nr_rules(), 12)

    self.assertEqual(x.alphabet(), "abBe")
    self.assertFalse(x.has_froidure_pin())
    self.assertEqual(x.froidure_pin().size(), 24)
    self.assertEqual(x.identity(), "e")
    self.assertEqual(x.inverses(), "aBbe")
    self.assertFalse(x.is_obviously_infinite())
    self.assertTrue(x.is_obviously_finite())
    self.assertEqual(x.size(), 24)


def check_operators(self, t):
    x = t()
    x.set_alphabet("abBe")
    x.set_identity("e")
    x.set_inverses("aBbe")

    x.add_rule("bb", "B")
    x.add_rule("BaBa", "abab")
    x.run()
    self.assertTrue(x.equal_to("bb", "B"))
    self.assertTrue(x.equal_to([1, 1], [2]))

    if compare_version_numbers(cppyy_version(), "1.7.1"):
        with self.assertRaises(LibsemigroupsException):
            x.equal_to([1, 1], [5])
    else:
        with self.assertRaises(TypeError):
            x.equal_to([1, 1], [5])

    with self.assertRaises(TypeError):
        x.equal_to("aa", "z")

    self.assertEqual(x.normal_form("bb"), "B")
    self.assertEqual(x.normal_form("B"), "B")
    self.assertEqual(x.normal_form([1, 1]), [2])
    self.assertEqual(x.normal_form([0, 0]), [3])

    with self.assertRaises(LibsemigroupsCppyyException):
        x.normal_form([1, 5])
    with self.assertRaises(LibsemigroupsCppyyException):
        x.normal_form("z")

    if hasattr(t, "rewrite"):
        self.assertEqual(x.rewrite("aa"), "")
        self.assertEqual(x.rewrite("bb"), "B")

        with self.assertRaises(TypeError):
            self.assertEqual(x.rewrite("z"))


def check_running_and_state(self, t):
    ReportGuard(False)
    x = t()
    x.set_alphabet("abce")
    x.set_identity("e")
    x.add_rule("aa", "e")
    x.add_rule("bc", "e")
    x.add_rule("bbb", "e")
    x.add_rule("ababababababab", "e")
    x.add_rule("abacabacabacabacabacabacabacabac", "e")
    x.run_for(microseconds(1))

    if t != FpSemigroup or compare_version_numbers(libsemigroups_version(), "1.1.0"):
        # FpSemigroup doesn't behave correctly wrt run_until or run_for in
        # versions less than 1.1.0
        self.assertTrue(x.stopped())
        self.assertFalse(x.finished())
        self.assertFalse(x.running())
        self.assertTrue(x.started())
        self.assertFalse(x.stopped_by_predicate())
        self.assertTrue(x.timed_out())

    x = t()
    x.set_alphabet("abce")
    x.set_identity("e")
    x.add_rule("aa", "e")
    x.add_rule("bc", "e")
    x.add_rule("bbb", "e")
    x.add_rule("ababababababab", "e")
    x.add_rule("abacabacabacabacabacabacabacabac", "e")

    n = 0

    def func():
        nonlocal n
        n += 1
        return n == 2

    x.run_until(func)

    if t != FpSemigroup or compare_version_numbers(libsemigroups_version(), "1.1.0"):
        self.assertTrue(x.stopped())
        self.assertFalse(x.finished())
        self.assertFalse(x.running())
        self.assertTrue(x.started())
        self.assertTrue(x.stopped_by_predicate())
        self.assertFalse(x.timed_out())
