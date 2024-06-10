
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
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/froidure-pin-base.hpp>  // for FroidurePinBase
#include <libsemigroups/presentation.hpp>       // for Presentation
#include <libsemigroups/to-presentation.hpp>    // for make
#include <libsemigroups/types.hpp>              // for word_type

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

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
For an implementations of presentations for semigroups or monoids.

This class can be used to construction presentations for semigroups or monoids
and is intended to be used as the input to other algorithms in
``libsemigroups_pybind11`` . The idea is to provide a shallow wrapper around a
vector of words of type ``Word`` . We refer to this vector of words as the rules 
of the presentation. The :any:`Presentation` class also provides some checks
that the rules really define a presentation, (i.e. it's consistent with its 
alphabet), and some related functionality is available in the module
``libsemigroups_pybind11.presentation``.)pbdoc");
      thing.def("__repr__", &to_human_readable_repr<Word>);
      thing.def_readwrite("rules",
                          &Presentation_::rules,
                          R"pbdoc(
Data member holding the rules of the presentation.

The rules can be altered using the member functions of ``list`` , and the
presentation can be checked for validity using :any:`validate`.)pbdoc");
      thing.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs an empty presentation with no rules and no alphabet.)pbdoc");
      thing.def(py::init<Presentation_ const&>(), R"pbdoc(
Default copy constructor.)pbdoc");
      thing.def(
          "alphabet",
          [](Presentation_ const& self) { return self.alphabet(); },
          R"pbdoc(
Returns the alphabet of the presentation.

:exceptions: This function is guaranteed never to throw.

:complexity: Constant.

:returns: The alphabet of the presentation.
:rtype: str | List[int]
)pbdoc");
      thing.def(
          "alphabet",
          [](Presentation_& self, size_type n) { return self.alphabet(n); },
          py::arg("n"),
          R"pbdoc(
Set the alphabet by size.

Sets the alphabet to the range :math:`[0, n)` consisting of values of type
:any:`letter_type`.

:param n: the size of the alphabet.
:type n: int

:returns: ``self``

:rtype: Presentation

:raises LibsemigroupsError:  if the value of ``n`` is greater than the
  maximum number of letters supported by :any:`letter_type`.

.. seealso::  :any:`validate_alphabet` , :any:`validate_rules` , and
  :any:`validate`.
)pbdoc");
      thing.def(
          "alphabet",
          [](Presentation_&                           self,
             typename Presentation_::word_type const& lphbt) {
            return self.alphabet(lphbt);
          },
          py::arg("lphbt"),
          R"pbdoc(
Set the alphabet.

Sets the alphabet to be the letters in ``lphbt``.

:param lphbt: the alphabet.
:type lphbt: str | List[int]
:returns: ``self``
:rtype: Presentation

:raises LibsemigroupsError:  if there are duplicate letters in ``lphbt``.

.. seealso::  :any:`validate_rules` and :any:`validate`.

)pbdoc");
      thing.def("alphabet_from_rules",
                &Presentation_::alphabet_from_rules,
                R"pbdoc(
Set the alphabet to be the letters in the rules.

Sets the alphabet to be the letters in :any:`rules`.

:returns: ``self``
:rtype: Presentation

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.

:complexity: At most :math:`O(mn)` where :math:`m` is the number of rules,
  and :math:`n` is the length of the longest rule.

.. seealso::  :any:`validate_rules` , and :any:`validate`.

)pbdoc");
      thing.def(
          "contains_empty_word",
          [](Presentation_ const& self) { return self.contains_empty_word(); },
          R"pbdoc(
Check if the presentation should contain the empty word.

Check if the presentation should contain the empty word.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:returns: A value of type ``bool``.

:rtype: bool
)pbdoc");
      thing.def(
          "contains_empty_word",
          [](Presentation_& self, bool val) {
            return self.contains_empty_word(val);
          },
          py::arg("val"),
          R"pbdoc(
Specify that the presentation should (not) contain the empty word.

:param val: whether the presentation should contain the empty word
:type val: bool
Specify that the presentation should (not) contain the empty word.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant

:returns: A const reference to ``self``

:rtype: Presentation
// )pbdoc");
      thing.def("in_alphabet",
                &Presentation_::in_alphabet,
                py::arg("val"),
                R"pbdoc(
Check if a letter belongs to the alphabet or not.

:param val: the letter to check
:type val: letter_type
Check if a letter belongs to the alphabet or not.

:raises This:  function guarantees not to throw a ``LibsemigroupsError``.

:complexity: Constant on average, worst case linear in the size of the
alphabet.

:returns:  a value of type ``bool``.

:rtype: bool
)pbdoc");
      thing.def("index",
                &Presentation_::index,
                py::arg("val"),
                R"pbdoc(
Get the index of a letter in the alphabet.
After checking that ``val`` is in the the alphabet, this function performs
the same as ``index_no_checks(letter_type val)``.

:raises LibsemigroupsError:  if ``val`` does not belong to the alphabet.

.. seealso::  :any:`index_no_checks`.)pbdoc");
      thing.def("init",
                &Presentation_::init,
                R"pbdoc(
Remove the alphabet and all rules.
This function clears the alphabet and all rules from the presentation,
putting it back into the state it would be in if it was newly constructed.

:returns: A reference to ``self``.

:rtype: Presentation
)pbdoc");
      thing.def("letter",
                &Presentation_::letter,
                py::arg("i"),
                R"pbdoc(
Get a letter in the alphabet by index.
After checking that ``i`` is in the range :math:`[0, n)` , where :math:`n` is
the length of the alphabet, this function performs the same as
``letter_no_checks(size_type i)``.

:raises LibsemigroupsError:  if ``i`` is not in the range :math:`[0, n)`.

.. seealso::  :any:`letter_no_checks`.)pbdoc");
      thing.def("validate",
                &Presentation_::validate,
                R"pbdoc(
Check if the alphabet and rules are valid.
Check if the alphabet and rules are valid.

:raises LibsemigroupsError:  if :any:`validate_alphabet` or
:any:`validate_rules` does.

:complexity: Worst case :math:`O(mnp)` where :math:`m` is the length of
length of the word, :math:`n` is the size of the alphabet and :math:`p` is
the number of rules.)pbdoc");
      thing.def(
          "validate_alphabet",
          [](Presentation_ const& self) { return self.validate_alphabet(); },
          R"pbdoc(
Check if the alphabet is valid.
Check if the alphabet is valid.

:raises LibsemigroupsError:  if there are duplicate letters in the alphabet.

:complexity: Linear in the length of the alphabet.)pbdoc");
      thing.def("validate_letter",
                &Presentation_::validate_letter,
                py::arg("c"),
                R"pbdoc(
Check if a letter belongs to the alphabet or not.

:param c: the letter to check.
:type c: letter_type
Check if a letter belongs to the alphabet or not.

:raises LibsemigroupsError:  if ``c`` does not belong to the alphabet.

:complexity: Constant on average, worst case linear in the size of the
alphabet.)pbdoc");
      thing.def("validate_rules",
                &Presentation_::validate_rules,
                R"pbdoc(
Check if every rule consists of letters belonging to the alphabet.
Check if every rule consists of letters belonging to the alphabet.

:raises LibsemigroupsError:  if any word contains a letter not in the
alphabet.

:complexity: Worst case :math:`O(mnt)` where :math:`m` is the length of the
longest word, :math:`n` is the size of the alphabet and :math:`t` is the
number of rules.)pbdoc");
      //       m.def("add_cyclic_conjugates",
      //             &presentation::add_cyclic_conjugates<Word>,
      //             py::arg("p"),
      //             py::arg("lhs"),
      //             py::arg("rhs"),
      //             R"pbdoc(
      // TODO.

      // :param p: the presentation
      // :type p: Presentation

      // :param lhs: TODO
      // :type lhs: Word

      // :param rhs: TODO
      // :type rhs: Word
      // TODO

      // :raises LibsemigroupsError:  if either ``lhs`` or ``rhs`` contain
      // letters not in ``p.alphabet()``)pbdoc");
      m.def(
          "add_generator",
          [](Presentation<Word>& p) { return presentation::add_generator(p); },
          py::arg("p"),
          R"pbdoc(
Add a generator to p.

:param p: the presentation.
:type p: Presentation
Add the first letter not in the alphabet of ``p`` as a generator of ``p`` , and return this letter.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.


:returns:  a value of type :any:`letter_type`.

:rtype: typename Word
)pbdoc");
      m.def(
          "add_generator",
          [](Presentation<Word>& p, typename Presentation_::letter_type x) {
            presentation::add_generator(p, x);
          },
          py::arg("p"),
          py::arg("x"),
          R"pbdoc(
Add x as a generator of p.

:param p: the presentation. 
:type p: Presentation

:param x: the letter to add as a generator.Add the letter ``x`` as a generator of ``p``.
:type x: str | int

:raises LibsemigroupsError:  if ``x`` is in ``p.alphabet()``.)pbdoc");
      m.def("add_identity_rules",
            &presentation::add_identity_rules<Word>,
            py::arg("p"),
            py::arg("e"),
            R"pbdoc(
Add rules for an identity element.

:param p: the presentation to add rules to 
:type p: Presentation

:param e: the identity elementAdds rules of the form :math:`ae = ea = a` for every letter :math:`a` in the alphabet of ``p`` , and where :math:`e` is the second parameter.
:type e: str | int

:raises LibsemigroupsError:  if ``e`` is not a letter in ``p.alphabet()``.

:complexity: Linear in the number of rules)pbdoc");
      m.def(
          "add_inverse_rules",
          [](Presentation<Word>&                      p,
             Word const&                              vals,
             typename Presentation<Word>::letter_type e = UNDEFINED) {
            presentation::add_inverse_rules(p, vals, e);
          },
          py::arg("p"),
          py::arg("vals"),
          py::arg("e"),
          R"pbdoc(
Add rules for inverses.

:param p: the presentation to add rules to 
:type p: Presentation

:param vals: the inverses 
:type vals: Word

:param e: the identity element (defaults to UNDEFINED, meaning use the empty word)
:type e: str | int

The letter in ``a`` with index ``i`` in ``vals`` is the inverse of the letter in ``alphabet()`` with index ``i`` . The rules added are :math:`a_ib_i
= e` where the alphabet is :math:`\{a_1, \ldots, a_n\}` ; the 2nd parameter ``vals`` is :math:`\{b_1, \ldots, b_n\}` ; and :math:`e` is the 3rd parameter.

:raises LibsemigroupsError:  if any of the following apply:

*  the length of ``vals`` is not equal to ``alphabet().size()`` ;
*  the letters in ``vals`` are not exactly those in ``alphabet()`` (perhaps in a different order);
*  :math:`(a_i ^ {-1}) ^ {-1} = a_i` does not hold for some :math:`i` ;
*  :math:`e ^ {-1} = e` does not hold



:complexity:  :math:`O(n)` where :math:`n` is ``p.alphabet().size()``.)pbdoc");
      m.def(
          "add_rule",
          [](Presentation<Word>& p, Word const& lhop, Word const& rhop) {
            presentation::add_rule(p, lhop, rhop);
          },
          py::arg("p"),
          py::arg("lhop"),
          py::arg("rhop"),
          R"pbdoc(
Add a rule to the presentation by reference and check.

:param p: the presentation. 
:type p: Presentation

:param lhop: the left-hand side of the rule. 
:type lhop: Word

:param rhop: the right-hand side of the rule.
:type rhop: Word
Adds the rule with left-hand side ``lhop`` and right-hand side ``rhop`` to the rules, after checking that ``lhop`` and ``rhop`` consist entirely of letters in the alphabet of ``p``.

:raises LibsemigroupsError:  if ``lhop`` or ``rhop`` contains any letters not belonging to ``p.alphabet()``.)pbdoc");
      m.def("add_zero_rules",
            &presentation::add_zero_rules<Word>,
            py::arg("p"),
            py::arg("z"),
            R"pbdoc(
Add rules for a zero element.

:param p: the presentation to add rules to 
:type p: Presentation

:param z: the zero element
:type z: str | int

Adds rules of the form :math:`az = za = z` for every letter :math:`a` in the alphabet of ``p`` , and where :math:`z` is the second parameter.

:raises LibsemigroupsError:  if ``z`` is not a letter in ``p.alphabet()``.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "are_rules_sorted",
          [](Presentation<Word> const& p) {
            presentation::are_rules_sorted(p);
          },
          py::arg("p"),
          R"pbdoc(
Check the rules are sorted relative to shortlex.

:param p: the presentation to check
:type p: Presentation
Check if the rules :math:`u_1 = v_1, \ldots, u_n = v_n` satisfy :math:`u_1v_1 < \cdots < u_nv_n` where :math:`<` is shortlex order.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

.. seealso::  :any:`sort_rules(Presentation p)`.


:returns: A value of type ``bool``.

:rtype: typename Word
)pbdoc");
      m.def(
          "change_alphabet",
          [](Presentation<Word>& p, Word const& new_alphabet) {
            presentation::change_alphabet(p, new_alphabet);
          },
          py::arg("p"),
          py::arg("new_alphabet"),
          R"pbdoc(
Change or re-order the alphabet.

:param p: the presentation 
:type p: Presentation

:param new_alphabet: the replacement alphabet
:type new_alphabet: Word
This function replaces ``p.alphabet()`` with ``new_alphabet`` , where possible, and re-writes the rules in the presentation using the new alphabet.

:raises LibsemigroupsError:  if the size of ``p.alphabet()`` and ``new_alphabet`` do not agree.)pbdoc");
      m.def("contains_rule",
            &presentation::contains_rule<Word>,
            py::arg("p"),
            py::arg("lhs"),
            py::arg("rhs"),
            R"pbdoc(
Check if a presentation contains a rule.

:param p: the presentation. 
:type p: Presentation

:param lhs: the left-hand side of the rule. 
:type lhs: Word

:param rhs: the right-hand side of the rule.
:type rhs: Word
Checks if the rule with left-hand side ``lhs`` and right-hand side ``rhs`` is contained in ``p``.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.

:complexity: Linear in the number of rules


:returns:  a value of type ``bool``

:rtype: typename Word
)pbdoc");
      m.def("first_unused_letter",
            &presentation::first_unused_letter<Word>,
            py::arg("p"),
            R"pbdoc(
Return the first letter not in the alphabet of a presentation.

:param p: the presentation
:type p: Presentation
This function returns ``letter(p, i)`` when ``i`` is the least possible value such that ``!p.in_alphabet(letter(p, i))`` if such a letter exists.

:raises LibsemigroupsError:  if ``p`` already has an alphabet of the maximum possible size supported by ``letter_type``.


:returns: A ``letter_type``.

:rtype: typename Word
)pbdoc");
      m.def("greedy_reduce_length",
            &presentation::greedy_reduce_length<Word>,
            py::arg("p"),
            R"pbdoc(
Greedily reduce the length of the presentation using longest_subword_reducing_length.

:param p: the presentation
:type p: Presentation
This function repeatedly calls ``longest_subword_reducing_length`` and ``replace_subword`` to introduce a new generator and reduce the length of the presentation ``p`` until ``longest_subword_reducing_length`` returns the empty word.

:raises LibsemigroupsError:  if ``longest_subword_reducing_length`` or ``replace_word`` does.)pbdoc");
      m.def("greedy_reduce_length_and_number_of_gens",
            &presentation::greedy_reduce_length_and_number_of_gens<Word>,
            py::arg("p"),
            R"pbdoc(
Greedily reduce the length and number of generators of the presentation.

:param p: the presentation
:type p: Presentation
This function repeatedly calls ``longest_subword_reducing_length`` and ``replace_subword`` to temporarily introduce a new generator and try to reduce the length of the presentation ``p`` . If, for a given subword, this operation is successfully reduces the length of the presentation, the changes are kept. Otherwise, the presentation is reverted and the next subword is tried. This is done until ``longest_subword_reducing_length`` returns the empty word.

:raises LibsemigroupsError:  if ``longest_subword_reducing_length`` or ``replace_word`` does.)pbdoc");
      m.def(
          "human_readable_letter",
          [](Presentation<std::string> const& p, size_t i) {
            return presentation::human_readable_letter(p, i);
          },
          py::arg("p"),
          py::arg("i"),
          R"pbdoc(
Return a possible letter by index.

:param p: a presentation (unused) 
:type p: Presentation

:param i: the index
:type i: int
This function returns the :math:`i` -th letter in the alphabet consisting of all possible letters of type :any:`Presentation::letter_type` . This function exists so that visible ASCII characters occur before invisible ones, so that when manipulating presentations over ``str`` s the human readable characters are used before non-readable ones.

:raises LibsemigroupsError:  if ``i`` exceeds the number of letters in supported by ``letter_type``.


:returns: A ``letter_type``.

:rtype: Presentation::letter_type
)pbdoc");
      m.def(
          "human_readable_letter",
          [](Presentation<Word> const& p, size_t i) {
            presentation::human_readable_letter(p, i);
          },
          py::arg("p"),
          py::arg("i"),
          R"pbdoc(
Return a possible letter by index.

:param p: the presentation (unused) 
:type p: Presentation

:param i: the index of the letter to find
:type i: int
This function returns the :math:`i` -th letter in the alphabet consisting of all possible letters of type :any:`Presentation::letter_type` . This function exists so that visible ASCII characters occur before invisible ones, so that when manipulating presentations over ``str`` s the human readable characters are used before non-readable ones.

:raises LibsemigroupsError:  if ``i`` exceeds the number of letters in supported by ``letter_type``.


:returns: A ``letter_type``.

:rtype: typename Word
)pbdoc");
      m.def("is_strongly_compressible",
            &presentation::is_strongly_compressible<Word>,
            py::arg("p"),
            R"pbdoc(
Returns true if the $1$-relation presentation can be strongly compressed.

:param p: the presentation
:type p: Presentation
A :math:`1` -relation presentation is *strongly compressible* if both relation words start with the same letter and end with the same letter. In other words, if the alphabet of the presentation ``p`` is :math:`A` and the relation words are of the form :math:`aub = avb` where :math:`a, b\in A` (possibly :math:` a = b` ) and :math:`u, v\in A ^ *` , then ``p`` is strongly compressible. See`Section 3.2 <https://doi.org/10.1007/s00233-021-10216-8>`_ for details.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.

.. seealso::  ``strongly_compress``


:returns: A value of type ``bool``.

:rtype: typename Word
)pbdoc");
      m.def(
          "length",
          [](Presentation<Word> const& p) { presentation::length(p); },
          py::arg("p"),
          R"pbdoc(
Return the sum of the lengths of the rules.

:param p: the presentation
:type p: Presentation
Return the sum of the lengths of the rules.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.


:returns: A value of type ``int``.

:rtype: typename Word
)pbdoc");
      m.def(
          "longest_rule",
          [](Presentation<Word> const& p) { presentation::longest_rule(p); },
          py::arg("p"),
          R"pbdoc(
Returns an iterator pointing at the left-hand side of the first rule in the presentation with maximal length.

:param p: the presentation
:type p: Presentation
Returns an iterator pointing at the left-hand side of the first rule in the presentation with maximal length.The *length* of a rule is defined to be the sum of the lengths of its left-hand and right-hand sides.

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.


:returns: A value of type ``Iterator``.

:rtype: typename Word
)pbdoc");
      m.def(
          "longest_rule_length",
          [](Presentation<Word> const& p) {
            presentation::longest_rule_length(p);
          },
          py::arg("p"),
          R"pbdoc(
the maximum length of a rule in the presentation.

:param p: the presentation
:type p: Presentation
Returns the maximum length of a rule in the presentation.The *length* of a rule is defined to be the sum of the lengths of its left-hand and right-hand sides.

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.


:returns: A value of type ``Word::size_type``.

:rtype: typename Word
)pbdoc");
      m.def("longest_subword_reducing_length",
            &presentation::longest_subword_reducing_length<Word>,
            py::arg("p"),
            R"pbdoc(
Returns the longest common subword of the rules.

:param p: the presentation
:type p: Presentation
If it is possible to find a subword :math:`w` of the rules :math:`u_1 = v_1,
\ldots, u_n = v_n` such that the introduction of a new generator :math:`z` and the relation :math:`z = w` reduces the ``presentation::length`` of the presentation, then this function returns the longest such word :math:`w` . If no such word can be found, then a word of length :math:`0` is returned.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.


:returns: A value of type ``Word``.

:rtype: typename Word
)pbdoc");
      m.def("make_semigroup",
            &presentation::make_semigroup<Word>,
            py::arg("p"),
            R"pbdoc(
Convert a monoid presentation to a semigroup presentation.

:param p: the presentation
:type p: Presentation
This function modifies its argument in-place by replacing the empty word in all relations by a new generator, and the identity rules for that new generator. If ``p.contains_empty_word()`` is ``False`` , then the presentation is not modified and :any:`UNDEFINED` is returned. If a new generator is added as the identity, then this generator is returned.

:raises LibsemigroupsError:  if ``replace_word`` or ``add_identity_rules`` does.


:returns: The new generator added, if any, and :any:`UNDEFINED` if not.

:rtype: typename Word
)pbdoc");
      m.def("normalize_alphabet",
            &presentation::normalize_alphabet<Word>,
            py::arg("p"),
            R"pbdoc(
Normalize the alphabet to $\{0, \ldots, n - 1\}$.

:param p: the presentation
:type p: Presentation
Modify the presentation in-place so that the alphabet is :math:`\{0, \ldots,
n - 1\}` (or equivalent) and rewrites the rules to use this alphabet.If the alphabet is already normalized, then no changes are made to the presentation.

:raises LibsemigroupsError:  if :any:`validate` throws on the initial presentation.)pbdoc");
      m.def("reduce_complements",
            &presentation::reduce_complements<Word>,
            py::arg("p"),
            R"pbdoc(
If there are rules $u = v$ and $v = w$ where $|w| <
|v|$, then replace $u = v$ by $u = w$.

:param p: the presentation to add rules to
:type p: Presentation
Attempts to reduce the length of the words by finding the equivalence relation on the relation words generated by the pairs of identical relation words. If :math:`\{u_1, u_2, \ldots, u_n\}` are the distinct words in an equivalence class and :math:`u_1` is the short-lex minimum word in the class, then the relation words are replaced by :math:`u_1 =
u_2, u_1 = u_3, \cdots, u_1 = u_n`.The rules may be reordered by this function even if there are no reductions found.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.)pbdoc");
      m.def("reduce_to_2_generators",
            &presentation::reduce_to_2_generators<Word>,
            py::arg("p"),
            py::arg("index"),
            R"pbdoc(
Reduce the number of generators in a $1$-relation presentation to 2.

:param p: the presentation 
:type p: Presentation

:param index: determines the choice of letter to use, 0 uses p.rules[0].front() and 1 uses p.rules[1].front() (defaults to: 0).
:type index: int
Reduce the number of generators in a :math:`1` -relation presentation to ``2``.Returns ``True`` if the :math:`1` -relation presentation ``p`` has been modified and ``False`` if not.A :math:`1` -relation presentation is *left cycle-free* if the relation words start with distinct letters. In other words, if the alphabet of the presentation ``p`` is :math:`A` and the relation words are of the form :math:`au = bv` where :math:`a, b\in A` with :math:`a \neq b` and :math:`u, v \in A ^ *` , then ``p`` is left cycle-free. The word problem for a left cycle-free :math:`1` -relation monoid is solvable if the word problem for the modified version obtained from this function is solvable.

:raises LibsemigroupsError:  if ``index`` is not ``0`` or ``1``.


:returns: A value of type ``bool``.

:rtype: typename Word
)pbdoc");
      m.def("remove_duplicate_rules",
            &presentation::remove_duplicate_rules<Word>,
            py::arg("p"),
            R"pbdoc(
Remove duplicate rules.

:param p: the presentation
:type p: Presentation
Removes all but one instance of any duplicate rules (if any). Note that rules of the form :math:`u = v` and :math:`v = u` (if any) are considered duplicates. Also note that the rules may be reordered by this function even if there are no duplicate rules.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.)pbdoc");
      m.def("remove_redundant_generators",
            &presentation::remove_redundant_generators<Word>,
            py::arg("p"),
            R"pbdoc(
Remove any trivially redundant generators.

:param p: the presentation
:type p: Presentation
If one side of any of the rules in the presentation ``p`` is a letter ``a`` and the other side of the rule does not contain ``a`` , then this function replaces every occurrence of ``a`` in every rule by the other side of the rule. This substitution is performed for every such rule in the presentation; and the trivial rules (with both sides being identical) are removed. If both sides of a rule are letters, then the greater letter is replaced by the lesser one.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.)pbdoc");
      m.def("remove_trivial_rules",
            &presentation::remove_trivial_rules<Word>,
            py::arg("p"),
            R"pbdoc(
Remove rules consisting of identical words.

:param p: the presentation
:type p: Presentation
Removes all instance of rules (if any) where the left-hand side and the right-hand side are identical.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "replace_subword",
          [](Presentation<Word>& p,
             Word const&         existing,
             Word const&         replacement) {
            presentation::replace_subword(p, existing, replacement);
          },
          py::arg("p"),
          py::arg("existing"),
          py::arg("replacement"),
          R"pbdoc(
Replace non-overlapping instances of a subword by another word.

:param p: the presentation 
:type p: Presentation

:param existing: the word to be replaced 
:type existing: Word

:param replacement: the replacement word.
:type replacement: Word
If ``existing`` and ``replacement`` are words, then this function replaces every non-overlapping instance of ``existing`` in every rule by ``replacement`` . The presentation ``p`` is changed in-place.

:raises LibsemigroupsError:  if ``existing`` is empty.)pbdoc");
      m.def("replace_word",
            &presentation::replace_word<Word>,
            py::arg("p"),
            py::arg("existing"),
            py::arg("replacement"),
            R"pbdoc(
Replace instances of a word on either side of a rule by another word.

:param p: the presentation. 
:type p: Presentation

:param existing: the word to be replaced. 
:type existing: Word

:param replacement: the replacement word.
:type replacement: Word
If ``existing`` and ``replacement`` are words, then this function replaces every instance of ``existing`` in every rule of the form ``existing`` :math:`= w` or :math:`w = ` ``existing`` , with the word ``replacement`` . The presentation ``p`` is changed in-place.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.)pbdoc");
      m.def(
          "replace_word_with_new_generator",
          [](Presentation<Word>& p, Word const& w) {
            presentation::replace_word_with_new_generator(p, w);
          },
          py::arg("p"),
          py::arg("w"),
          R"pbdoc(
Replace non-overlapping instances of a word with a new generator via const reference.

:param p: the presentation. 
:type p: Presentation

:param w: the subword to replace.
:type w: Word
If :math:`w=` ``[first, last)`` is a word, then this function replaces every non-overlapping instance (from left to right) of :math:`w` in every rule, adds a new generator :math:`z` , and the rule :math:`w = z` . The new generator and rule are added even if :math:`w` is not a subword of any rule.

:raises LibsemigroupsError:  if ``w`` is empty.


:returns: The new generator added.

:rtype: typename Word
)pbdoc");
      m.def("reverse",
            &presentation::reverse<Word>,
            py::arg("p"),
            R"pbdoc(
Reverse every rule.

:param p: the presentation
:type p: Presentation
Reverse every rule.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.)pbdoc");
      m.def(
          "shortest_rule",
          [](Presentation<Word> const& p) { presentation::shortest_rule(p); },
          py::arg("p"),
          R"pbdoc(
Returns an iterator pointing at the left-hand side of the first rule in the presentation with minimal length.

:param p: the presentation
:type p: Presentation
Returns an iterator pointing at the left-hand side of the first rule in the presentation with minimal length.The *length* of a rule is defined to be the sum of the lengths of its left-hand and right-hand sides.

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.


:returns: A value of type ``Iterator``.

:rtype: typename Word
)pbdoc");
      m.def(
          "shortest_rule_length",
          [](Presentation<Word> const& p) {
            presentation::shortest_rule_length(p);
          },
          py::arg("p"),
          R"pbdoc(
Returns the minimum length of a rule in the presentation.

:param p: the presentation
:type p: Presentation
Returns the minimum length of a rule in the presentation.The *length* of a rule is defined to be the sum of the lengths of its left-hand and right-hand sides.

:raises LibsemigroupsError:  if the length of ``p.rules`` is odd.


:returns: A value of type ``Word::size_type``.

:rtype: typename Word
)pbdoc");
      m.def(
          "sort_each_rule",
          [](Presentation<Word>& p) { presentation::sort_each_rule(p); },
          py::arg("p"),
          R"pbdoc(
Sort the left-hand and right-hand side of each rule by shortlex.

:param p: the presentation whose rules should be sorted.
:type p: Presentation
Sort each rule :math:`u = v` so that the left-hand side is shortlex greater than the right-hand side.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.

:complexity: Linear in the number of rules.)pbdoc");
      m.def(
          "sort_rules",
          [](Presentation<Word>& p) { presentation::sort_rules(p); },
          py::arg("p"),
          R"pbdoc(
Sort all of the rules by shortlex.

:param p: the presentation to sort
:type p: Presentation
Sort the rules :math:`u_1 = v_1, \ldots, u_n = v_n` so that :math:`u_1v_1 < \cdots < u_nv_n` where :math:`<` is the shortlex order.

:raises LibsemigroupsError:  if ``p.rules.size()`` is odd.)pbdoc");
      m.def("strongly_compress",
            &presentation::strongly_compress<Word>,
            py::arg("p"),
            R"pbdoc(
Strongly compress a $1$-relation presentation.

:param p: the presentation
:type p: Presentation
Strongly compress a :math:`1` -relation presentation.Returns ``True`` if the :math:`1` -relation presentation ``p`` has been modified and ``False`` if not. The word problem is solvable for the input presentation if it is solvable for the modified version.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.

.. seealso::  ``is_strongly_compressible``


:returns: A value of type ``bool``.

:rtype: typename Word
)pbdoc");
      m.def(
          "to_gap_string",
          [](Presentation<std::string> const& p, std::string const& var_name) {
            presentation::to_gap_string(p, var_name);
          },
          py::arg("p"),
          py::arg("var_name"),
          R"pbdoc(
TODO.

:param p: the presentation 
:type p: Presentation

:param var_name: TODO
:type var_name: str
TODO

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.)pbdoc");
      m.def(
          "to_gap_string",
          [](Presentation<word_type> const& p, std::string const& var_name) {
            presentation::to_gap_string(p, var_name);
          },
          py::arg("p"),
          py::arg("var_name"),
          R"pbdoc(
TODO.

:param p: the presentation 
:type p: Presentation

:param var_name: TODO
:type var_name: str
TODO

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.)pbdoc");
      m.def("validate_semigroup_inverses",
            &presentation::validate_semigroup_inverses<Word>,
            py::arg("p"),
            py::arg("vals"),
            R"pbdoc(
Validate if vals act as semigroup inverses in p.

:param p: the presentation. 
:type p: Presentation

:param vals: the values to check if the act as inverses.
:type vals: Word
Check if the values in ``vals`` act as semigroup inverses for the letters of the alphabet of ``p`` . Specifically, it checks that the :math:`i` th value in ``vals`` acts as an inverse for the :math:`i` th value in ``p.alphabet()``.Let :math:`x_i` be the :math:`i` th letter in ``p.alphabet()`` , and suppose that :math:`x_i=v_j` is in the :math:`j` th position of ``vals`` . This function checks that :math:`v_i = x_j` , and therefore that :math:`(x_i^{-1})^{-1} = x`.

:raises Libsemigroups_Exception:  if any of the following apply:

*  the length of ``vals`` is not the same as the length of ``p.alphabet()``
*  ``p.validate_word(vals)`` throws
*  ``vals`` contains duplicate letters
*  the values in ``vals`` do not serve as semigroup inverses.

)pbdoc");

    }  // bind_present
  }    // namespace
  void init_present(py::module& m) {
    bind_present<word_type>(m, "PresentationWords");
    bind_present<std::string>(m, "PresentationStrings");
  }
}  // namespace libsemigroups

// #####################################################
// # Old
// #####################################################
//
// libsemigroups_pybind11
// Copyright (C) 2022-2024 James D. Mitchell
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

// TODO:
// * iwyu
/*
// C std headers....
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/froidure-pin-base.hpp>  // for FroidurePinBase
#include <libsemigroups/presentation.hpp>       // for Presentation
#include <libsemigroups/to-presentation.hpp>    // for make

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_present

namespace py = pybind11;

namespace libsemigroups {

  namespace {

    template <typename T>
    std::string presentation_repr(Presentation<T> const& p) {
      std::ostringstream out;

      out << "<" << (p.contains_empty_word() ? "monoid" : "semigroup")
          << " presentation of length "
          << presentation::length(p) + p.alphabet().size() << ", with "
          << p.alphabet().size() << " generators and " << p.rules.size()
          << " relations>";
      return out.str();
    }

    template <typename T>
    void bind_present(py::module& m, std::string const& name) {
      using size_type = typename Presentation<T>::size_type;

      py::class_<Presentation<T>>(m, name.c_str())
          .def(py::init<>())
          .def(py::init<Presentation<T> const&>())
          .def("alphabet",
               py::overload_cast<>(&Presentation<T>::alphabet, py::const_))
          .def("alphabet",
               py::overload_cast<size_type>(&Presentation<T>::alphabet))
          .def("alphabet",
               py::overload_cast<T const&>(&Presentation<T>::alphabet))
          .def("alphabet_from_rules", &Presentation<T>::alphabet_from_rules)
          .def("letter", &Presentation<T>::letter)
          .def("index", &Presentation<T>::index)
          .def("contains_empty_word",
               py::overload_cast<>(&Presentation<T>::contains_empty_word,
                                   py::const_))
          .def("contains_empty_word",
               py::overload_cast<bool>(&Presentation<T>::contains_empty_word))
          .def_readwrite("rules", &Presentation<T>::rules)
          .def("validate_alphabet",
               py::overload_cast<>(&Presentation<T>::validate_alphabet,
                                   py::const_))
          .def("validate_letter", &Presentation<T>::validate_letter)
          .def("validate_rules", &Presentation<T>::validate_rules)
          .def("validate", &Presentation<T>::validate)
          .def("__repr__", &presentation_repr<T>);

      m.def("add_rule", [](Presentation<T>& p, T const& lhs, T const& rhs) {
        presentation::add_rule(p, lhs, rhs);
      });
      m.def("add_rule_no_checks", &presentation::add_rule_no_checks<T>);
      m.def("add_rules",
            py::overload_cast<Presentation<T>&, Presentation<T> const&>(
                &presentation::add_rules_no_checks<T>));
      m.def("add_identity_rules", &presentation::add_identity_rules<T>);
      m.def("add_zero_rules", &presentation::add_zero_rules<T>);
      // There are two definitions here to handle the default parameters
      m.def("add_inverse_rules",
            [](Presentation<T>&                      p,
               T const&                              vals,
               typename Presentation<T>::letter_type e) {
              presentation::add_inverse_rules(p, vals, e);
            });
      m.def("add_inverse_rules", [](Presentation<T>& p, T const& vals) {
        presentation::add_inverse_rules(p, vals);
      });
      m.def("remove_duplicate_rules", &presentation::remove_duplicate_rules<T>);
      m.def("remove_trivial_rules", &presentation::remove_trivial_rules<T>);
      m.def("reduce_complements", &presentation::reduce_complements<T>);
      m.def("sort_each_rule", &presentation::sort_each_rule<T>);
      m.def("sort_rules", &presentation::sort_rules<T>);
      m.def("longest_subword_reducing_length",
            &presentation::longest_subword_reducing_length<T>);
      m.def("replace_word_with_new_generator",
            [](Presentation<T>& p, T const& w) -> void {
              presentation::replace_word_with_new_generator(p, w);
            });
      m.def(
          "replace_subword",
          [](Presentation<T>& p, T const& existing, T const& replace) -> void {
            presentation::replace_subword(p, existing, replace);
          });
      m.def("replace_word", &presentation::replace_word<T>);
      m.def("length", [](Presentation<T> const& p) -> size_t {
        return presentation::length<T>(p);
      });
      m.def("reverse", &presentation::reverse<T>);
      m.def("normalize_alphabet", &presentation::normalize_alphabet<T>);
      m.def("remove_redundant_generators",
            &presentation::remove_redundant_generators<T>);
      m.def("add_zero_rules", &presentation::add_zero_rules<T>);
      m.def("are_rules_sorted", &presentation::are_rules_sorted<T>);
      m.def("change_alphabet", &presentation::change_alphabet<T>);
      m.def("first_unused_letter", &presentation::first_unused_letter<T>);
      m.def("greedy_reduce_length", &presentation::greedy_reduce_length<T>);
      m.def("longest_rule", [](Presentation<T> const& p) {
        return std::distance(p.rules.cbegin(), presentation::longest_rule(p));
      });
      m.def("longest_rule_length", [](Presentation<T> const& p) {
        return presentation::longest_rule_length(p);
      });
      m.def("shortest_rule", [](Presentation<T> const& p) {
        return std::distance(p.rules.cbegin(), presentation::shortest_rule(p));
      });
      m.def("shortest_rule_length", [](Presentation<T> const& p) {
        return presentation::shortest_rule_length(p);
      });
      m.def("make_semigroup", presentation::make_semigroup<T>);

      m.def("to_presentation",
            py::overload_cast<FroidurePinBase&>(to_presentation<word_type>));

      m.def("to_presentation",
            [](Presentation<std::string> const& p) -> Presentation<word_type> {
              return to_presentation<word_type>(p);
            });

      m.def("to_presentation",
            [](Presentation<word_type> const& p) -> Presentation<std::string> {
              return to_presentation<std::string>(p);
            });

      // m.def("to_presentation",
      //       [](Presentation<word_type> const& p,
      //          std::string const&             s) -> Presentation<std::string>
      //          {
      //         return to_presentation<std::string>(p, s);
      //       });
      m.def("is_strongly_compressible",
            &presentation::is_strongly_compressible<T>);
      m.def("strongly_compress", &presentation::strongly_compress<T>);
      m.def("reduce_to_2_generators",
            &presentation::reduce_to_2_generators<T>,
            py::arg("p"),
            py::arg("index") = 0);
    }
  }  // namespace

  void init_present(py::module& m) {
    bind_present<word_type>(m, "PresentationWords");
    bind_present<std::string>(m, "PresentationStrings");
  }
}  // namespace libsemigroups

*/
