//
// libsemigroups_pybind11
// Copyright (C) 2024 Joseph Edwards, James D. Mitchell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// C std headers....
#include <ctype.h>   // for isprint
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <algorithm>      // for for_each, none_of, search
#include <cmath>          // for pow
#include <iterator>       // for distance
#include <string>         // for string, basic_string, oper...
#include <unordered_set>  // for operator!=, operator==
#include <utility>        // for move, swap
#include <vector>         // for vector

// libsemigroups....
#include <libsemigroups/constants.hpp>     // for operator==, UNDEFINED
#include <libsemigroups/exception.hpp>     // for LibsemigroupsException
#include <libsemigroups/order.hpp>         // for shortlex_compare
#include <libsemigroups/presentation.hpp>  // for Presentation
#include <libsemigroups/ranges.hpp>        // for is_sorted
#include <libsemigroups/types.hpp>         // for word_type

// pybind11....
#include <pybind11/cast.h>           // for arg
#include <pybind11/detail/common.h>  // for const_, overload_cast, ove...
#include <pybind11/detail/descr.h>   // for operator+
#include <pybind11/functional.h>     // for std::function conversion
#include <pybind11/pybind11.h>       // for class_, init, module
#include <pybind11/pytypes.h>        // for sequence, str_attr_accessor
#include <pybind11/stl.h>            // for std::vector conversion

// libsemigroups_pybind11....
#include "main.hpp"  // for init_present

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename Word>
    void bind_present(py::module& m, std::string const& name) {
      using Presentation_ = Presentation<Word>;
      using size_type     = typename Presentation_::size_type;

      py::class_<Presentation_> thing(m,
                                      name.c_str(),
                                      R"pbdoc(
For an implementation of presentations for semigroups or monoids.

This class can be used to construction presentations for semigroups or monoids
and is intended to be used as the input to other algorithms in
``libsemigroups_pybind11``. The idea is to provide a shallow wrapper around a
collection of words of type :ref:`Word<pseudo_word_type_class>`. We refer to this vector of words as the rules
of the presentation. The :any:`PresentationStrings` class also provides some checks
that the rules really define a presentation, (i.e. it's consistent with its
alphabet), and some related functionality is available in the module
:any:`libsemigroups_pybind11.presentation`.)pbdoc");
      thing.def("__repr__", [](Presentation_ const& p) -> std::string {
        return to_human_readable_repr(p);
      });
      thing.def("__eq__",
                [](Presentation_ const& lhop, Presentation_ rhop) -> bool {
                  return lhop == rhop;
                });
      thing.def_readwrite("rules",
                          &Presentation_::rules,
                          R"pbdoc(
Data member holding the rules of the presentation.

The rules can be altered using the member functions of ``list``, and the
presentation can be checked for validity using :any:`throw_if_bad_alphabet_or_rules`.)pbdoc");
      thing.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs an empty presentation with no rules and no alphabet.)pbdoc");
      thing.def(
          "__copy__",
          [](const Presentation_& that) { return Presentation_(that); },
          R"pbdoc(
Default copy constructor.

Default copy constructor)pbdoc");
      thing.def(
          "alphabet",
          [](Presentation_ const& self) { return self.alphabet(); },
          R"pbdoc(
:sig=(self: PresentationStrings) -> Word:
Return the alphabet of the presentation.

:returns: The alphabet of the presentation.
:rtype: :ref:`Word<pseudo_word_type_class>`

:complexity: Constant.

)pbdoc");
      thing.def(
          "alphabet",
          [](Presentation_& self, size_type n) { return self.alphabet(n); },
          py::arg("n"),
          R"pbdoc(
:sig=(self: PresentationStrings, int: n) -> PresentationStrings:
Set the alphabet by size.

Sets the alphabet to the the first :math:`n` values with type
:ref:`Letter<pseudo_letter_type_class>`. For :any:`str`-types, we assume the
order of letters to be a-zA-Z0-9.

:param n: the size of the alphabet.
:type n: int

:returns: ``self``
:rtype: PresentationStrings

:raises LibsemigroupsError:  if the value of ``n`` is greater than the
  maximum number of letters supported by :ref:`Letter<pseudo_letter_type_class>`.

.. seealso::
      * :any:`throw_if_alphabet_has_duplicates`
      * :any:`throw_if_bad_rules`
      * :any:`throw_if_bad_alphabet_or_rules`
)pbdoc");
      thing.def(
          "alphabet",
          [](Presentation_&                           self,
             typename Presentation_::word_type const& lphbt) {
            return self.alphabet(lphbt);
          },
          py::arg("lphbt"),
          R"pbdoc(
:sig=(self: PresentationStrings, lphbt: Word) -> PresentationStrings:
Set the alphabet.

Sets the alphabet to be the letters in *lphbt*.

:param lphbt: the alphabet.
:type lphbt: :ref:`Word<pseudo_word_type_class>`

:returns: ``self``
:rtype: PresentationStrings

:raises LibsemigroupsError:  if there are duplicate letters in *lphbt*.

.. seealso::
      * :any:`throw_if_bad_rules`
      * :any:throw_if_bad_alphabet_or_rules

)pbdoc");
      thing.def("alphabet_from_rules",
                &Presentation_::alphabet_from_rules,
                R"pbdoc(
Set the alphabet to be the letters in the rules.

:returns: ``self``
:rtype: PresentationStrings

:complexity: At most :math:`O(mn)` where :math:`m` is the number of rules,
  and :math:`n` is the length of the longest rule.

.. seealso::
      * :any:`throw_if_bad_rules`
      * :any:throw_if_bad_alphabet_or_rules

)pbdoc");
      thing.def(
          "contains_empty_word",
          [](Presentation_ const& self) { return self.contains_empty_word(); },
          R"pbdoc(
Return whether the empty word is a valid relation word.

Returns ``True`` if the empty word is a valid relation word, and ``False`` if
the empty word is not a valid relation word.

If the presentation is not allowed to contain the empty word (according
to this function), the presentation may still be isomorphic to a monoid,
but is not given as a quotient of a free monoid.

:returns: whether the presentation can contain the empty word.
:rtype: bool

:complexity: Constant.

)pbdoc");
      thing.def(
          "contains_empty_word",
          [](Presentation_& self, bool val) {
            return self.contains_empty_word(val);
          },
          py::arg("val"),
          R"pbdoc(
Set whether whether the empty word is a valid relation word.

Specify whether the empty word should be a valid relation word (corresponding
to *val* being ``True``), or not (corresponding to *val* being ``False``).

If the presentation is not allowed to contain the empty word (according to
the value specified here), the presentation may still be isomorphic to a
monoid, but is not given as a quotient of a free monoid.

:param val: whether the presentation can contain the empty word.
:type val: bool

:returns: ``self``
:rtype: PresentationStrings

:complexity: Constant
)pbdoc");
      thing.def("in_alphabet",
                &Presentation_::in_alphabet,
                py::arg("val"),
                R"pbdoc(
:sig=(self: PresentationStrings, val: Letter) -> bool:
Check if a letter belongs to the alphabet or not.

:param val: the letter to check.
:type val: :ref:`Letter<pseudo_letter_type_class>`

:returns:  whether the letter belongs to the alphabet
:rtype: bool

:complexity: Constant on average, worst case linear in the size of the
  alphabet.
)pbdoc");
      thing.def("index",
                &Presentation_::index,
                py::arg("val"),
                R"pbdoc(
Return the index of a letter in the alphabet.

After checking that *val* is in the the alphabet, get the index of a letter in
the alphabet

:param val: the letter.
:type val: :ref:`Letter<pseudo_letter_type_class>`

:returns: the index.
:rtype: int

:raises LibsemigroupsError:  if *val* does not belong to the alphabet.

:complexity: Constant.

)pbdoc");
      thing.def("init",
                &Presentation_::init,
                R"pbdoc(
Remove the alphabet and all rules.

This function clears the alphabet and all rules from the presentation,
putting it back into the state it would be in if it was newly constructed.

:returns: ``self``.
:rtype: PresentationStrings
)pbdoc");
      thing.def("letter",
                &Presentation_::letter,
                py::arg("i"),
                R"pbdoc(
:sig=(self: PresentationStrings, i: int) -> Letter:
Get a letter in the alphabet by index.

After checking that *i* is in the range :math:`[0, n)`, where :math:`n` is
the length of the alphabet, this function returns the letter of the alphabet in
position *i*.

:param i: the index.
:type i: int

:returns: the letter
:rtype: :ref:`Letter<pseudo_letter_type_class>`

:raises LibsemigroupsError:  if *i* is not in the range :math:`[0, n)`.)pbdoc");
      thing.def("throw_if_bad_alphabet_or_rules",
                &Presentation_::throw_if_bad_alphabet_or_rules,
                R"pbdoc(
Check if the alphabet and rules are valid.

:raises LibsemigroupsError:  if :any:`throw_if_alphabet_has_duplicates` or
      :any:`throw_if_bad_rules` does.

:complexity: Worst case :math:`O(mnp)` where :math:`m` is the length of
      length of the word, :math:`n` is the size of the alphabet and :math:`p` is
      the number of rules.)pbdoc");
      thing.def(
          "throw_if_alphabet_has_duplicates",
          [](Presentation_ const& self) {
            return self.throw_if_alphabet_has_duplicates();
          },
          R"pbdoc(
Check if the alphabet is valid.

:raises LibsemigroupsError:  if there are duplicate letters in the alphabet.

:complexity: Linear in the length of the alphabet.)pbdoc");
      thing.def(
          "throw_if_letter_not_in_alphabet",
          [](Presentation_ const& self, typename Word::value_type c) {
            return self.throw_if_letter_not_in_alphabet(c);
          },
          py::arg("c"),
          R"pbdoc(
:sig=(self: PresentationStrings, c: Letter) -> None:
Check if a letter belongs to the alphabet or not.

:param c: the letter to check.
:type c: :ref:`Letter<pseudo_letter_type_class>`

:raises LibsemigroupsError:  if ``c`` does not belong to the alphabet.

:complexity: Constant on average, worst case linear in the size of the
      alphabet.)pbdoc");

      thing.def(
          "throw_if_letter_not_in_alphabet",
          [](Presentation_ const& self, Word const& w) {
            self.throw_if_letter_not_in_alphabet(w.begin(), w.end());
          },
          py::arg("w"),
          R"pbdoc(
:sig=(self: PresentationStrings, w: Word) -> None:

Check if every letter in a word belongs to the alphabet or not.

:param w: the word to check.
:type c: :ref:`Word<pseudo_word_type_class>`

:raises LibsemigroupsError:  if any letter in *w* does not belong to the alphabet.
)pbdoc");
      thing.def("throw_if_bad_rules",
                &Presentation_::throw_if_bad_rules,
                R"pbdoc(
Check if every rule consists of letters belonging to the alphabet.

:raises LibsemigroupsError:  if any word contains a letter not in the
      alphabet.

:complexity: Worst case :math:`O(mnt)` where :math:`m` is the length of the
      longest word, :math:`n` is the size of the alphabet and :math:`t` is the
      number of rules.)pbdoc");
      thing.def(
          "add_generator",
          [](Presentation_& self) { return self.add_generator(); },
          R"pbdoc(
:sig=(self: PresentationStrings) -> Letter:
Add a generator.

Add the first letter not in the alphabet as a generator, and return this letter.

:returns:  the letter added to the alphabet.
:rtype: :ref:`Letter<pseudo_letter_type_class>`

)pbdoc");
      thing.def(
          "add_generator",
          [](Presentation_& self, typename Presentation_::letter_type x) {
            self.add_generator(x);
          },
          py::arg("x"),
          R"pbdoc(
:sig=(self: PresentationStrings, x: Letter) -> Letter:
Add the letter *x* as a generator.

:param x: the letter to add as a generator.
:type x: :ref:`Letter<pseudo_letter_type_class>`

:raises LibsemigroupsError:  if *x* is in ``alphabet()``.)pbdoc");
      thing.def(
          "remove_generator",
          [](Presentation_& self, typename Presentation_::letter_type x) {
            self.remove_generator(x);
          },
          py::arg("x"),
          R"pbdoc(
:sig=(self: PresentationStrings, x: Letter):
Remove the letter *x* as a generator.

:param x: the letter to remove as a generator.
:type x: :ref:`Letter<pseudo_letter_type_class>`

:raises LibsemigroupsError: if *x* is not in `p.alphabet()`.

:complexity: Average case: linear in the length of the alphabet, worst case:
      quadratic in the length of the alphabet.
)pbdoc");
      m.def("add_identity_rules",
            &presentation::add_identity_rules<Word>,
            py::arg("p"),
            py::arg("e"),
            R"pbdoc(
Add rules for an identity element.

Adds rules of the form :math:`ae = ea = a` for every letter :math:`a` in the
alphabet of *p*, and where :math:`e` is the second parameter.

:param p: the presentation to add rules to.
:type p: PresentationStrings

:param e: the identity element.
:type e: :ref:`Letter<pseudo_letter_type_helper>`

:raises LibsemigroupsError:  if *e* is not a letter in ``p.alphabet()``.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "add_inverse_rules",
          [](Presentation_&                      p,
             Word const&                         vals,
             typename Presentation_::letter_type e = UNDEFINED) {
            presentation::add_inverse_rules(p, vals, e);
          },
          py::arg("p"),
          py::arg("vals"),
          py::arg("e"),
          R"pbdoc(
:sig=(p: PresentationStrings, vals: Word, e: Letter = UNDEFINED)->None:
:only-document-once:
Add rules for inverses.

The letter *a* with index ``i`` in *vals* is the inverse of the letter in
``alphabet()`` with index ``i``. The rules added are :math:`a_ib_i = e` where
the alphabet is :math:`\{a_1, \ldots, a_n\}` ; the 2nd parameter *vals* is
:math:`\{b_1, \ldots, b_n\}` ; and :math:`e` is the 3rd parameter.

:param p: the presentation to add rules to.
:type p: PresentationStrings

:param vals: the inverses.
:type vals: :ref:`Word<pseudo_word_type_helper>`

:param e: the identity element (defaults to :any:`UNDEFINED`, meaning use the empty word).
:type e: :ref:`Letter<pseudo_letter_type_helper>`

:raises LibsemigroupsError: if any of the following apply:

    *  the length of *vals* is not equal to ``alphabet().size()`` ;
    *  the letters in *vals* are not exactly those in ``alphabet()`` (perhaps in a different order);
    *  :math:`(a_i ^ {-1}) ^ {-1} = a_i` does not hold for some :math:`i` ;
    *  :math:`e ^ {-1} = e` does not hold

:complexity:  :math:`O(n)` where :math:`n` is ``p.alphabet().size()``.)pbdoc");
      m.def(
          "add_inverse_rules",
          [](Presentation_& p, Word const& vals) {
            presentation::add_inverse_rules(p, vals);
          },
          py::arg("p"),
          py::arg("vals"));
      m.def(
          "add_rule",
          [](Presentation_& p, Word const& lhop, Word const& rhop) {
            presentation::add_rule(p, lhop, rhop);
          },
          py::arg("p"),
          py::arg("lhop"),
          py::arg("rhop"),
          R"pbdoc(
:sig=(p: PresentationStrings, lhop: Word, rhop: Word)->None:
:only-document-once:
Add a rule to the presentation by reference and check.

Adds the rule with left-hand side *lhop* and right-hand side *rhop* to the
rules, after checking that *lhop* and *rhop* consist entirely of letters in the
alphabet of *p*.

:param p: the presentation.
:type p: PresentationStrings

:param lhop: the left-hand side of the rule.
:type lhop: :ref:`Word<pseudo_word_type_helper>`

:param rhop: the right-hand side of the rule.
:type rhop: :ref:`Word<pseudo_word_type_helper>`

:raises LibsemigroupsError:  if *lhop* or *rhop* contains any letters not
  belonging to ``p.alphabet()``.)pbdoc");
      m.def(
          "add_rules",
          [](Presentation_& p, Presentation_ const& q) {
            presentation::add_rules(p, q);
          },
          R"pbdoc(
:sig=(p: PresentationStrings, q: PresentationStrings)->None:
:only-document-once:
Add the rules of *q* to *p*.

Before it is added, each rule is validated to check it contains only letters of
the alphabet of *p*. If the :math:`n`th rule causes this function to throw, the
first :math:`n-1` rules will still be added to *p*.

:param p: the presentation to add rules to.
:type p: Presentation

:param q: the presentation to add words from.
:type first: Presentation

:raises LibsemigroupsError:
  if any rule contains any letters not belonging to
  ``p.alphabet()``.)pbdoc");
      m.def("add_zero_rules",
            &presentation::add_zero_rules<Word>,
            py::arg("p"),
            py::arg("z"),
            R"pbdoc(
:sig=(p: PresentationStrings, z: Letter)->None:
:only-document-once:
Add rules for a zero element.

Adds rules of the form :math:`az = za = z` for every letter :math:`a` in the
alphabet of *p*, and where :math:`z` is the second parameter.

:param p: the presentation to add rules to.
:type p: PresentationStrings

:param z: the zero element.
:type z: :ref:`Letter<pseudo_letter_type_helper>`

:raises LibsemigroupsError:  if *z* is not a letter in ``p.alphabet()``.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "are_rules_sorted",
          [](Presentation_ const& p) {
            return presentation::are_rules_sorted(p);
          },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->bool:
:only-document-once:
Check the rules are sorted relative to shortlex.

Check if the rules :math:`u_1 = v_1, \ldots, u_n = v_n` satisfy
:math:`u_1v_1 < \cdots < u_nv_n` where :math:`<` is shortlex order.

:param p: the presentation to check.
:type p: PresentationStrings

:returns: whether the rules are sorted.
:rtype: bool

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

.. seealso::
      * :any:`sort_rules`.
)pbdoc");
      m.def(
          "change_alphabet",
          [](Presentation_& p, Word const& new_alphabet) {
            presentation::change_alphabet(p, new_alphabet);
          },
          py::arg("p"),
          py::arg("new_alphabet"),
          R"pbdoc(
:sig=(p: PresentationStrings, new_alphabet: Word)->None:
:only-document-once:
Change or re-order the alphabet.

This function replaces ``p.alphabet()`` with ``new_alphabet``, where possible,
and re-writes the rules in the presentation using the new alphabet.

:param p: the presentation .
:type p: PresentationStrings

:param new_alphabet: the replacement alphabet.
:type new_alphabet: :ref:`Letter<pseudo_letter_type_helper>`

:raises LibsemigroupsError:  if the size of ``p.alphabet()`` and
  ``new_alphabet`` do not agree.)pbdoc");
      m.def("contains_rule",
            &presentation::contains_rule<Word>,
            py::arg("p"),
            py::arg("lhs"),
            py::arg("rhs"),
            R"pbdoc(
:sig=(p: PresentationStrings, lhs: Word, rhs: Word)->bool:
:only-document-once:
Check if a presentation contains a rule.

Checks if the rule with left-hand side *lhs* and right-hand side *rhs* is
contained in *p*.

:param p: the presentation.
:type p: PresentationStrings

:param lhs: the left-hand side of the rule.
:type lhs: :ref:`Word<pseudo_word_type_helper>`

:param rhs: the right-hand side of the rule.
:type rhs: :ref:`Word<pseudo_word_type_helper>`

:returns: whether the presentation contains the rule
:rtype: bool

:complexity: Linear in the number of rules.
)pbdoc");
      m.def("first_unused_letter",
            &presentation::first_unused_letter<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->Letter:
:only-document-once:
Return the first letter not in the alphabet of a presentation.

This function returns ``letter(p, i)`` when ``i`` is the least possible value
such that ``!p.in_alphabet(letter(p, i))`` if such a letter exists.

:param p: the presentation.
:type p: PresentationStrings

:returns: the letter.
:rtype: :ref:`Letter<pseudo_word_type_helper>`

:raises LibsemigroupsError:  if *p* already has an alphabet of the maximum
  possible size.

)pbdoc");
      m.def("greedy_reduce_length",
            &presentation::greedy_reduce_length<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Greedily reduce the length of the presentation using
:any:`longest_subword_reducing_length`.

This function repeatedly calls :any:`longest_subword_reducing_length` and
:any:`replace_subword` to introduce a new generator and reduce the length of the
presentation *p* until :any:`longest_subword_reducing_length` returns the empty
word.

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:  if :any:`longest_subword_reducing_length` or
  :any:`replace_word` does.)pbdoc");
      m.def("greedy_reduce_length_and_number_of_gens",
            &presentation::greedy_reduce_length_and_number_of_gens<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Greedily reduce the length and number of generators of the presentation.

This function repeatedly calls `longest_subword_reducing_length` and
`replace_subword` to introduce a new generator to try to reduce the
length of the presentation \p p and the number of generators. This is
done until either `longest_subword_reducing_length` returns the empty
word, or the new length and number of generators is greater than or
equal to that of the presentation in the previous iteration.

In the latter case, the presentation \p p gets restored to the state it
was in after the previous iteration.

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:  if :any:`longest_subword_reducing_length` or
  :any`replace_word` does.)pbdoc");
      m.def("is_strongly_compressible",
            &presentation::is_strongly_compressible<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->bool:
:only-document-once:
Return true if the :math:`1`-relation presentation can be strongly compressed.

A :math:`1` -relation presentation is *strongly compressible* if both relation
words start with the same letter and end with the same letter. In other words,
if the alphabet of the presentation *p* is :math:`A` and the relation words are
of the form :math:`aub = avb` where :math:`a, b\in A` (possibly :math:` a = b` )
and :math:`u, v\in A ^ *`, then *p* is strongly compressible.
See`Section 3.2 <https://doi.org/10.1007/s00233-021-10216-8>`_ for details.

:param p: the presentation.
:type p: PresentationStrings

:returns: whether the presentation is strongly compressible
:rtype: bool

.. seealso::
      * :any:`strongly_compress`

)pbdoc");
      m.def(
          "length",
          [](Presentation_ const& p) { return presentation::length(p); },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->int:
:only-document-once:
Return the sum of the lengths of the rules.

:param p: the presentation.
:type p: PresentationStrings

:returns: the length of the presentation
:rtype: int

)pbdoc");
      m.def(
          "longest_rule",
          [](Presentation_ const& p) {
            if (p.rules.empty()) {
              LIBSEMIGROUPS_EXCEPTION("expected non-zero number of rules.");
            }
            return std::distance(p.rules.cbegin(),
                                 presentation::longest_rule(p));
          },
          R"pbdoc(
:sig=(p: PresentationStrings)->int:
:only-document-once:
Return the index of the left-hand side of the longest rule.

Returns the index of the left-hand side of the first rule in the presentation
with maximal length. The *length* of a rule is defined to be the sum of the
lengths of its left-hand and right-hand sides.

:param p: the presentation.
:type p: PresentationStrings

:returns: the index of the rule
:rtype: int

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.
)pbdoc");
      m.def(
          "longest_rule_length",
          [](Presentation_ const& p) {
            if (p.rules.empty()) {
              LIBSEMIGROUPS_EXCEPTION("expected non-zero number of rules.");
            }
            return presentation::longest_rule_length(p);
          },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->int:
:only-document-once:
Return the maximum length of a rule in the presentation.

Returns the maximum length of a rule in the presentation. The *length* of a rule
is defined to be the sum of the lengths of its left-hand and right-hand sides.

:param p: the presentation.
:type p: PresentationStrings

:returns: the maximum length
:rtype: int

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.
)pbdoc");
      m.def("longest_subword_reducing_length",
            &presentation::longest_subword_reducing_length<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->Word:
:only-document-once:
Return the longest common subword of the rules.

If it is possible to find a subword :math:`w` of the rules :math:`u_1 = v_1,
\ldots, u_n = v_n` such that the introduction of a new generator :math:`z` and
the relation :math:`z = w` reduces the :any`presentation.length` of the
presentation, then this function returns the longest such word :math:`w`.
If no such word can be found, then a word of length :math:`0` is returned.

:param p: the presentation.
:type p: PresentationStrings

:returns: the longest common subword, if it exists.
:rtype: :ref:`Word<pseudo_word_type_helper>`.

)pbdoc");
      m.def("make_semigroup",
            &presentation::make_semigroup<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->Word:
:only-document-once:
Convert a monoid presentation to a semigroup presentation.

This function modifies its argument in-place by replacing the empty word in all
relations by a new generator, and the identity rules for that new generator.
If ``p.contains_empty_word()`` is ``False``, then the presentation is not
modified and :any:`UNDEFINED` is returned. If a new generator is added as the
identity, then this generator is returned.

:param p: the presentation.
:type p: PresentationStrings

:returns: The new generator added, if any, and :any:`UNDEFINED` if not.
:rtype: :ref:`Word<pseudo_word_type_helper>`

:raises LibsemigroupsError:  if :any:`replace_word` or
  :any:`add_identity_rules` does.
)pbdoc");
      m.def("normalize_alphabet",
            &presentation::normalize_alphabet<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Normalize the alphabet to :math:`\{0, \ldots, n - 1\}`.

Modify the presentation in-place so that the alphabet is :math:`\{0, \ldots,
n - 1\}` (or equivalent) and rewrites the rules to use this alphabet. If the
alphabet is already normalized, then no changes are made to the presentation.

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:
  if :any:`PresentationStrings.throw_if_bad_alphabet_or_rules` throws on the initial
  presentation.)pbdoc");
      m.def("reduce_complements",
            &presentation::reduce_complements<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
If there are rules :math:`u = v` and :math:`v = w` where :math:`|w| < |v|`,
then replace :math:`u = v` by :math:`u = w`.

Attempts to reduce the length of the words by finding the equivalence relation
on the relation words generated by the pairs of identical relation words. If
:math:`\{u_1, u_2, \ldots, u_n\}` are the distinct words in an equivalence class
and :math:`u_1` is the short-lex minimum word in the class, then the relation
words are replaced by :math:`u_1 = u_2, u_1 = u_3, \cdots, u_1 = u_n`.
The rules may be reordered by this function even if there are no reductions
found.

:param p: the presentation to add rules to.
:type p: PresentationStrings

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.)pbdoc");
      m.def("reduce_to_2_generators",
            &presentation::reduce_to_2_generators<Word>,
            py::arg("p"),
            py::arg("index") = 0,
            R"pbdoc(
:sig=(p: PresentationStrings, index: int)->bool:
:only-document-once:
Reduce the number of generators in a :math:`1`-relation presentation to 2.

Reduce the number of generators in a :math:`1` -relation presentation to ``2``.
Returns ``True`` if the :math:`1` -relation presentation *p* has been modified
and ``False`` if not. A :math:`1` -relation presentation is *left cycle-free* if
the relation words start with distinct letters. In other words, if the alphabet
of the presentation *p* is :math:`A` and the relation words are of the form
:math:`au = bv` where :math:`a, b\in A` with :math:`a \neq b` and
:math:`u, v \in A ^ *`, then *p* is left cycle-free.

The word problem for a left cycle-free :math:`1` -relation monoid is solvable if
the word problem for the modified version obtained from this function is
solvable.

:param p: the presentation.
:type p: PresentationStrings

:param index: determines the choice of letter to use, 0 uses p.rules[0].front() and 1 uses p.rules[1].front() (defaults to: 0).
:type index: int

:returns: whether the presentation has been modified
:rtype: bool

:raises LibsemigroupsError:  if *index* is not ``0`` or ``1``.
)pbdoc");
      m.def("remove_duplicate_rules",
            &presentation::remove_duplicate_rules<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Remove duplicate rules.

Removes all but one instance of any duplicate rules (if any). Note that rules of
the form :math:`u = v` and :math:`v = u` (if any) are considered duplicates.
Also note that the rules may be reordered by this function even if there are no
duplicate rules.

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.)pbdoc");
      m.def("remove_redundant_generators",
            &presentation::remove_redundant_generators<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Remove any trivially redundant generators.

If one side of any of the rules in the presentation *p* is a letter ``a`` and
the other side of the rule does not contain ``a``, then this function replaces
every occurrence of ``a`` in every rule by the other side of the rule. This
substitution is performed for every such rule in the presentation; and the
trivial rules (with both sides being identical) are removed. If both sides of a
rule are letters, then the greater letter is replaced by the lesser one.

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.)pbdoc");
      m.def("remove_trivial_rules",
            &presentation::remove_trivial_rules<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Remove rules consisting of identical words.

Removes all instance of rules (if any) where the left-hand side and the
right-hand side are identical.

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "replace_subword",
          [](Presentation_& p, Word const& existing, Word const& replacement) {
            return presentation::replace_subword(p, existing, replacement);
          },
          py::arg("p"),
          py::arg("existing"),
          py::arg("replacement"),
          R"pbdoc(
:sig=(p: PresentationStrings, existing: Word, replacement: Word)->None:
:only-document-once:
Replace non-overlapping instances of a subword by another word.

If *existing* and *replacement* are words, then this function replaces every
non-overlapping instance of *existing* in every rule by *replacement*. The
presentation *p* is changed in-place.

:param p: the presentation .
:type p: PresentationStrings

:param existing: the word to be replaced.
:type existing: :ref:`Word<pseudo_word_type_helper>`

:param replacement: the replacement word.
:type replacement: :ref:`Word<pseudo_word_type_helper>`

:raises LibsemigroupsError:  if *existing* is empty.)pbdoc");
      m.def("replace_word",
            &presentation::replace_word<Word>,
            py::arg("p"),
            py::arg("existing"),
            py::arg("replacement"),
            R"pbdoc(
:sig=(p: PresentationStrings, existing: Word, replacement: Word)->None:
:only-document-once:
Replace instances of a word on either side of a rule by another word.

If *existing* and *replacement* are words, then this function replaces every
instance of *existing* in every rule of the form *existing* :math:`= w` or :math:`w =`
*existing*, with the word *replacement*. The presentation *p* is changed in-place.

:param p: the presentation.
:type p: PresentationStrings

:param existing: the word to be replaced.
:type existing: :ref:`Word<pseudo_word_type_helper>`

:param replacement: the replacement word.
:type replacement: :ref:`Word<pseudo_word_type_helper>`

)pbdoc");
      m.def(
          "replace_word_with_new_generator",
          [](Presentation_& p, Word const& w) {
            return presentation::replace_word_with_new_generator(p, w);
          },
          py::arg("p"),
          py::arg("w"),
          R"pbdoc(
:sig=(p: PresentationStrings, w: Word)->Letter:
:only-document-once:
Replace non-overlapping instances of a word with a new generator via const reference.

This function replaces every non-overlapping instance (from left to right) of
*w* in every rule, adds a new generator :math:`z`, and the rule :math:`w = z`.
The new generator and rule are added even if *w* is not a subword of any rule.

:param p: the presentation.
:type p: PresentationStrings

:param w: the subword to replace.
:type w: :ref:`Word<pseudo_word_type_helper>`

:returns: The new generator added.
:rtype: :ref:`Letter<pseudo_letter_type_helper>`

:raises LibsemigroupsError:  if *w* is empty.)pbdoc");
      m.def(
          "reverse",
          [](Presentation<Word>& p) { presentation::reverse(p); },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Reverse every rule.

:param p: the presentation.
:type p: PresentationStrings
)pbdoc");
      m.def(
          "shortest_rule",
          [](Presentation_ const& p) {
            if (p.rules.empty()) {
              LIBSEMIGROUPS_EXCEPTION("expected non-zero number of rules.");
            }
            return std::distance(p.rules.cbegin(),
                                 presentation::shortest_rule(p));
          },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->int:
:only-document-once:
Return the index of the left-hand side of the shortest rule.

Returns the index of the left-hand side of the first rule in the presentation
with minimal length. The *length* of a rule is defined to be the sum of the
lengths of its left-hand and right-hand sides.

:param p: the presentation.
:type p: PresentationStrings

:returns: the index of the rule.
:rtype: int

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.
)pbdoc");
      m.def(
          "shortest_rule_length",
          [](Presentation_ const& p) {
            if (p.rules.empty()) {
              LIBSEMIGROUPS_EXCEPTION("expected non-zero number of rules.");
            }
            return presentation::shortest_rule_length(p);
          },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->int:
:only-document-once:
Return the minimum length of a rule in the presentation.

Returns the minimum length of a rule in the presentation. The *length* of a rule
is defined to be the sum of the lengths of its left-hand and right-hand sides.

:param p: the presentation.
:type p: PresentationStrings

:returns: the length of the shortest rule
:rtype: int

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.
)pbdoc");
      // TODO add functionality for different comparison functions when they've
      // been bound
      m.def(
          "sort_each_rule",
          [](Presentation_& p) { presentation::sort_each_rule(p); },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Sort the left-hand and right-hand side of each rule by shortlex.

Sort each rule :math:`u = v` so that the left-hand side is shortlex greater than
the right-hand side.

:param p: the presentation whose rules should be sorted.
:type p: PresentationStrings

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "sort_each_rule",
          [](Presentation<Word>&                            p,
             std::function<bool(Word const&, Word const&)>& cmp) {
            return presentation::sort_each_rule(p, cmp);
          },
          py::arg("p"),
          py::arg("cmp"),
          R"pbdoc(
:sig=(p: PresentationStrings, cmp: Callable[[Word, Word], bool]) -> bool:
:only-document-once:
Sort the left-hand and right-hand side of each rule relative to cmp.

Sort each rule :math:`u = v` so that the left-hand side is greater than the right-hand side with respect to ``cmp``.

:param p: the presentation whose rules should be sorted.
:type p: PresentationStrings

:param cmp: the comparison function.
:type cmp: Callable[[:ref:`Word<pseudo_word_type_helper>`, :ref:`Word<pseudo_word_type_helper>`], bool]

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.
)pbdoc");
      m.def(
          "sort_rules",
          [](Presentation_& p) { presentation::sort_rules(p); },
          py::arg("p"),
          R"pbdoc(
:sig=(p: PresentationStrings)->None:
:only-document-once:
Sort all of the rules by shortlex.

Sort the rules :math:`u_1 = v_1, \ldots, u_n = v_n` so that :math:`u_1v_1 <
\cdots < u_nv_n` where :math:`<` is the shortlex order.

:param p: the presentation to sort.
:type p: PresentationStrings

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.)pbdoc");
      m.def("strongly_compress",
            &presentation::strongly_compress<Word>,
            py::arg("p"),
            R"pbdoc(
:sig=(p: PresentationStrings)->bool:
:only-document-once:
Strongly compress a :math:`1` -relation presentation.

Strongly compress a :math:`1` -relation presentation. Returns ``True`` if the
:math:`1` -relation presentation *p* has been modified and ``False`` if not. The
word problem is solvable for the input presentation if it is solvable for the
modified version.

:param p: the presentation.
:type p: PresentationStrings

:returns: whether the presentation has been modified
:rtype: bool

.. seealso::
      * :any:`is_strongly_compressible`

)pbdoc");
      m.def(
          "to_gap_string",
          [](Presentation<std::string> const& p, std::string const& var_name) {
            return presentation::to_gap_string(p, var_name);
          },
          py::arg("p"),
          py::arg("var_name"),
          R"pbdoc(
:sig=(p: PresentationStrings, var_name: str)->str:
:only-document-once:
Return the code that would create \p p in GAP.

This function returns the string of GAP code that could be used to create an
object with the same alphabet and rules as *p* in GAP. Presentations in GAP
are created by taking quotients of free semigroups or monoids.

:param p: the presentation.
:type p: PresentationStrings

:param var_name:  the name of the variable to be used in GAP.
:type var_name: str
)pbdoc");
      m.def(
          "to_gap_string",
          [](Presentation<word_type> const& p, std::string const& var_name) {
            return presentation::to_gap_string(p, var_name);
          },
          py::arg("p"),
          py::arg("var_name"));
      m.def("throw_if_bad_inverses",
            &presentation::throw_if_bad_inverses<Word>,
            py::arg("p"),
            py::arg("vals"),
            R"pbdoc(
:sig=(p: PresentationStrings, vals: Word)->None:
:only-document-once:
Validate if *vals* act as semigroup inverses in *p*.

Check if the values in *vals* act as semigroup inverses for the letters of the
alphabet of *p*. Specifically, it checks that the :math:`i`th value in *vals*
acts as an inverse for the :math:`i`th value in ``p.alphabet()``.

Let :math:`x_i` be the :math:`i`th letter in ``p.alphabet()``, and suppose
that :math:`x_i=v_j` is in the :math:`j`th position of *vals*. This function
checks that :math:`v_i = x_j`, and therefore that :math:`(x_i^{-1})^{-1} = x`.

:param p: the presentation.
:type p: PresentationStrings

:param vals: the values to check if the act as inverses.
:type vals: :ref:`Word<pseudo_word_type_helper>`

:raises Libsemigroups_Exception:  if any of the following apply:

      *  the length of ``vals`` is not the same as the length of ``p.alphabet()``
      *  ``vals`` contains letters not in the alphabet
      *  ``vals`` contains duplicate letters
      *  the values in ``vals`` do not serve as semigroup inverses.

)pbdoc");
    }  // bind_present

    template <typename Word>
    void bind_inverse_present(py::module& m, std::string const& name) {
      using InversePresentation_ = InversePresentation<Word>;

      py::class_<InversePresentation_, Presentation<Word>> thing(m,
                                                                 name.c_str(),
                                                                 R"pbdoc(
For an implementation of inverse presentations for semigroups or monoids.

This class can be used to construction inverse presentations for semigroups or
monoids and is intended to be used as the input to other algorithms in
``libsemigroups_pybind11`` .

This class inherits from :any:`PresentationStrings`.)pbdoc");
      thing.def("__repr__", [](InversePresentation_ const& p) -> std::string {
        return to_human_readable_repr(p);
      });
      thing.def(py::init<Presentation<Word> const&>(), R"pbdoc(
Construct an InversePresentationStrings from a Presentation reference.

Construct an :any:`InversePresentationStrings`, initially with empty inverses,
from a :any:`PresentationStrings`.

:param p: the presentation to construct from.
:type p: PresentationStrings
)pbdoc");
      thing.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs an empty :any:`InversePresentationStrings` with no rules, no alphabet and
no inverses.)pbdoc");
      thing.def(
          "__copy__",
          [](const InversePresentation_& that) {
            return InversePresentation_(that);
          },
          R"pbdoc(
Default copy constructor.

Default copy constructor)pbdoc");
      thing.def("inverse",
                &InversePresentation_::inverse,
                py::arg("x"),
                R"pbdoc(
:sig=(self: InversePresentationStrings, x: Letter) -> Letter:
Return the inverse of a letter in the alphabet.

Returns the inverse of the letter *x*.

:param x: the letter whose index is sought.
:type x: :ref:`Letter<pseudo_letter_type_inv_class>`

:returns: the index of *x*.
:rtype: :ref:`Letter<pseudo_letter_type_inv_class>`

:raises LibsemigroupsError:  if no inverses have been set, or if ``index(x)`` throws.
)pbdoc");
      thing.def(
          "inverses",
          [](InversePresentation_& self) { return self.inverses(); },
          R"pbdoc(
:sig=(self: InversePresentationStrings) -> Word:
Return the inverse of each letter in the alphabet.

Returns the inverse of each letter in the alphabet.

:returns: the inverses.
:rtype: :ref:`Word<pseudo_word_type_inv_class>`
)pbdoc");
      thing.def(
          "inverses",
          [](InversePresentation_&                           self,
             typename InversePresentation_::word_type const& w) {
            return self.inverses(w);
          },
          py::arg("w"),
          R"pbdoc(
:sig=(self: InversePresentationStrings, w: Word) -> InversePresentationStrings:
Set the inverse of each letter in the alphabet.

:param w: a word containing the inverses.
:type w: :ref:`Word<pseudo_word_type_inv_class>`

:returns: ``self``.
:rtype: InversePresentationStrings

:raises LibsemigroupsException: if:

      * the alphabet contains duplicate letters
      * the inverses do not act as semigroup inverses

.. note::

      Whilst the alphabet is not specified as an argument to this function, it
      is necessary to validate the alphabet here; a specification of inverses
      cannot make sense if the alphabet contains duplicate letters.

.. seealso::

      * :any:`PresentationStrings.throw_if_alphabet_has_duplicates`
      * :any:`presentation.throw_if_bad_inverses`

)pbdoc");
      thing.def("throw_if_bad_alphabet_rules_or_inverses",
                &InversePresentation_::throw_if_bad_alphabet_rules_or_inverses,
                R"pbdoc(
Check if the :any:`InversePresentationStrings` is valid.

Check if the :any:`InversePresentationStrings` is valid. Specifically, check that the
alphabet does not contain duplicate letters, that all rules only contain letters
defined in the alphabet, and that the inverses act as semigroup inverses.

:raises LibsemigroupsError:  if:

      * the alphabet contains duplicate letters
      * the rules contain letters not defined in the alphabet
      * the inverses do not act as semigroup inverses

.. seealso::

      * :any:`PresentationStrings.throw_if_bad_alphabet_or_rules`
      * :any:`presentation.throw_if_bad_inverses`

)pbdoc");
    }  // bind_inverse_present
  }  // namespace
  void init_present(py::module& m) {
    bind_present<word_type>(m, "PresentationWords");
    bind_present<std::string>(m, "PresentationStrings");
  }

  void init_inverse_present(py::module& m) {
    bind_inverse_present<word_type>(m, "InversePresentationWords");
    bind_inverse_present<std::string>(m, "InversePresentationStrings");
  }
}  // namespace libsemigroups
