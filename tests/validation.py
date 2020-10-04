def validation(self):
    ReportGuard(False)
    kb = KnuthBendix()
    kb.set_alphabet("ab")

    with self.assertRaises(TypeError):
        kb.validate_letter("c")
    try:
        kb.validate_letter("a")
    except Exception as e:
        self.fail("unexpected exception raised for KnuthBendix::validate_letter: " + e)

    with self.assertRaises(LibsemigroupsException):
        kb.validate_letter(3)
    try:
        kb.validate_letter(0)
    except Exception as e:
        self.fail("unexpected exception raised for KnuthBendix::validate_letter: " + e)

    with self.assertRaises(TypeError):
        kb.validate_word("abc")
    try:
        kb.validate_word("abab")
    except Exception as e:
        self.fail("unexpected exception raised for KnuthBendix::validate_letter: " + e)

    with self.assertRaises(TypeError):
        kb.validate_word([0, 1, 2])
    try:
        kb.validate_word([0, 1, 0, 1])
    except Exception as e:
        self.fail("unexpected exception raised for KnuthBendix::validate_letter: " + e)
