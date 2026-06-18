//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2026 James D. Mitchell
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

// libsemigroups headers
#include <libsemigroups/alphabet.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_alphabet

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename Word>
    void bind_alphabet(py::module& m, std::string name) {
      using Alphabet_ = Alphabet<Word>;

      using size_type          = typename Alphabet_::size_type;
      using native_word_type   = typename Alphabet_::native_word_type;
      using native_letter_type = typename Alphabet_::native_letter_type;

      py::class_<Alphabet_> thing(m,
                                  name.c_str(),
                                  R"pbdoc(
Class for storing and indexing an alphabet.

An :any:`Alphabet` object stores an ordered list of distinct letters and
maintains a map from each letter to its position in that list. The order of the
letters is significant: it is the order used by :any:`letter` and
:any:`Alphabet.index`.)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Operators
      ////////////////////////////////////////////////////////////////////////

      thing.def("__repr__", [](Alphabet_ const& thing) {
        return to_human_readable_repr(thing);
      });

      thing.def("__str__", [](Alphabet_ const& self) {
        return to_input_string(self, "[]");
      });

      thing.def("__len__", &Alphabet_::size);

      thing.def("__copy__",
                [](Alphabet_ const& self) { return Alphabet(self); });

      thing.def(
          "__iter__",
          [](Alphabet_ const& self) {
            return py::make_iterator(self.letters().begin(),
                                     self.letters().end());
          },
          py::keep_alive<0, 1>());

      thing.def(py::self == py::self);
      thing.def(py::self != py::self);

      ////////////////////////////////////////////////////////////////////////
      // Constructors + initializers
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::init<>(), R"pbdoc(
:sig=(self: Alphabet, *, word: type) -> None:
::only-document-once:
Default constructor.
This function constructs an empty alphabet.)pbdoc");

      thing.def(
          "init",
          [](Alphabet_& self) -> Alphabet_& { return self.init(); },
          R"pbdoc(
:sig=(self: Alphabet) -> Alphabet:

Remove all letters.

This function clears the alphabet, putting it back into the state it would be in
if it was newly constructed.

:returns: ``self``.
:rtype: Alphabet
)pbdoc");

      thing.def(py::init<native_word_type const&>(), R"pbdoc(
:sig=(self: Alphabet, letters: str | list[int]) -> None:
::only-document-once:
Construct from letters.

Sets the alphabet to be the letters in *letters*.

:param letters: the alphabet.
:type letters: str | list[int]

:raises LibsemigroupsError:  if there are duplicate letters in *letters*.
)pbdoc");

      thing.def(
          "init",
          [](Alphabet_& self, native_word_type const& letters) -> Alphabet_& {
            return self.init(letters);
          },
          py::arg("letters"),
          R"pbdoc(
:sig=(self: Alphabet, letters: str | list[int]) -> Alphabet:

Re-initialize from letters.

This function sets the alphabet to be the letters in *letters*.

:param letters: the alphabet.
:type letters: str | list[int]

:returns: ``self``.
:rtype: Alphabet

:raises LibsemigroupsError:  if there are duplicate letters in *letters*.
)pbdoc");

      thing.def(py::init<size_type>(), R"pbdoc(
:sig=(self: Alphabet, n: int, *, word: type) -> None:
::only-document-once:

Construct the alphabet by size.

This function constructs an alphabet containing the first *n* human-readable letters.

:param n: the size of the alphabet.
:type n: int

:Keyword Arguments:
  * **word** (*type*) -- the type of words to use. Must be either ``str`` or
    ``list[int]``.

:raises LibsemigroupsError:
  if the value of *n* is greater than the maximum number of supported
  letters.
)pbdoc");

      thing.def(
          "init",
          [](Alphabet_& self, size_type n) -> Alphabet_& {
            return self.init(n);
          },
          py::arg("n"),
          R"pbdoc(
:sig=(self: Alphabet, n: int) -> Alphabet:

Re-initialize the alphabet by size.

This function replaces the alphabet by the first *n* human-readable letters.

:param n: the size of the alphabet.
:type n: int

:returns: ``self``.
:rtype: Alphabet

:raises LibsemigroupsError:
  if the value of *n* is greater than the maximum number of supported
  letters.
)pbdoc");

      thing.def(
          "copy",
          [](Alphabet_ const& self) { return Alphabet(self); },
          R"pbdoc(
:sig=(self: Alphabet) -> Alphabet:

Copy a :any:`Alphabet` object.

:returns: A copy.
:rtype: Alphabet
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Mem fns
      ////////////////////////////////////////////////////////////////////////

      thing.def("add_letter",
                &Alphabet_::add_letter,
                py::arg("x"),
                R"pbdoc(
:sig=(self: Alphabet, x: str | int) -> Alphabet:

Add a letter.

This function adds the letter *x* to the alphabet.

:param x: the letter to add.
:type x: str | int

:returns: ``self``.
:rtype: Alphabet

:raises LibsemigroupsError:  if *x* is in :any:`letters`.
)pbdoc");

      thing.def("contains",
                &Alphabet_::contains,
                py::arg("x"),
                R"pbdoc(
:sig=(self: Alphabet, x: str | int) -> bool:

Check if a letter belongs to the alphabet or not.

This function checks whether *x* belongs to the alphabet.

:param x: the letter to check.
:type x: str | int

:returns: Whether or not *x* belongs to the alphabet.
:rtype: bool

:complexity: Constant on average, worst case linear in the size of the alphabet.
)pbdoc");

      thing.def("empty",
                &Alphabet_::empty,
                R"pbdoc(
:sig=(self: Alphabet) -> bool:

Check if the alphabet is empty.

This function returns ``True`` if the alphabet contains no letters, and
``False`` otherwise.

:returns: Whether or not the alphabet is empty.
:rtype: bool

:complexity: Constant.
)pbdoc");

      thing.def("index",
                &Alphabet_::index,
                py::arg("x"),
                R"pbdoc(
:sig=(self: Alphabet, x: str | int) -> int:

Return the index of a letter in the alphabet.

After checking that *x* is in the alphabet, this function returns the index
of *x* in the alphabet.

:param x: the possible letter to check.
:type x: str | int

:returns: The index of *x* in the alphabet.
:rtype: int

:raises LibsemigroupsError:  if ``x`` does not belong to the alphabet.
)pbdoc");

      thing.def("letter",
                &Alphabet_::letter,
                py::arg("i"),
                R"pbdoc(
:sig=(self: Alphabet, i: int) -> str | int:

Return a letter in the alphabet by index.

After checking that *i* is in the range :math:`[0, n)`, where :math:`n` is
the length of the alphabet, this function returns the letter with index *i*.

:param i: the index.
:type i: int

:returns: The letter with index *i*.
:rtype: str | int

:raises LibsemigroupsError:  if *i* is not in the range :math:`[0, n)`.
)pbdoc");

      thing.def("letters",
                &Alphabet_::letters,
                R"pbdoc(
:sig=(self: Alphabet) -> str | list[int]:
Return the letters of the alphabet.

This function returns the letters of the alphabet.

:returns: The letters of the alphabet.
:rtype: str | list[int]

:complexity: Constant.
)pbdoc");

      thing.def("remove_letter",
                &Alphabet_::remove_letter,
                py::arg("x"),
                R"pbdoc(
:sig=(self: Alphabet, x: str | int) -> Alphabet:

Remove a letter.

This function removes the letter *x* from the alphabet.

:param x: the letter to remove.
:type x: str | int

:returns: ``self``.
:rtype: Alphabet

:raises LibsemigroupsError:  if *x* is not in :any`letters`.

:complexity:
  Average case: linear in the length of the alphabet, worst case: quadratic in
  the length of the alphabet.
)pbdoc");

      thing.def(
          "throw_if_duplicate_letters",
          [](Alphabet_ const& self) {
            return self.throw_if_duplicate_letters();
          },
          R"pbdoc(
:sig=(self: Alphabet) -> None:

Check if the alphabet contains duplicates.

This function checks if the alphabet contains duplicates..

:raises LibsemigroupsError:  if there are duplicate letters in the alphabet.

:complexity: Linear in the length of the alphabet.)pbdoc");
      //       thing.def(
      //           "throw_if_letter_not_in_alphabet",
      //           [](Alphabet_ const& self, Iterator1 first, Iterator2 last) {
      //             return self.throw_if_letter_not_in_alphabet(first, last);
      //           },
      //           py::arg("first"),
      //           py::arg("last"),
      //           R"pbdoc(
      // Check if every letter in a range belongs to the alphabet.
      //
      // :param first: iterator pointing at the first letter to check.
      // :type first: Iterator1
      //
      // :param last: iterator pointing one beyond the last letter to check.
      // :type last: Iterator2
      // Checks whether every letter in the range ``[first, last)`` belongs to
      // the alphabet.
      //
      // :raises LibsemigroupsError:  if any letter in ``[first, last)`` does
      // not belong to the alphabet.
      //
      // :complexity: Linear in the length of the range on average, worst case
      // proportional to the product of the length of the range and the size of
      // the alphabet.)pbdoc");
      thing.def(
          "throw_if_letter_not_in_alphabet",
          [](Alphabet_ const& self, native_letter_type x) {
            return self.throw_if_letter_not_in_alphabet(x);
          },
          py::arg("x"),
          R"pbdoc(
:sig=(self: Alphabet, x : str | int) -> None:

Check if a letter belongs to the alphabet or not.

This function checks whether or not *x* belongs to the alphabet.

:param x: the letter to check.
:type x: str | int

:raises LibsemigroupsError:  if *x* does not belong to the alphabet.

:complexity: Constant on average, worst case linear in the size of the alphabet.)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Helpers in libsemigroups namespace
      ////////////////////////////////////////////////////////////////////////

      m.def(
          "validate",
          [](Alphabet<Word> const& alphabet) { return validate(alphabet); },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(alphabet: Alphabet) -> None:
::only-document-once:

Check if an alphabet is valid.

This function checks whether the parameter *alphabet* is valid.

:param alphabet: the alphabet to validate.
:type alphabet: Alphabet

:raises LibsemigroupsError:  if *alphabet* contains duplicate letters.

:complexity: Linear in the size of *alphabet*.)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Helpers in alphabet namespace
      ////////////////////////////////////////////////////////////////////////

      m.def(
          "alphabet_first_unused_letter",
          [](Alphabet<Word> const& alphabet) {
            return alphabet::first_unused_letter(alphabet);
          },
          py::arg("alphabet"),
          R"pbdoc(
:sig=(alphabet: Alphabet) -> str | int:
::only-document-once:

Return the first letter not in the alphabet.

This function returns ``words.human_readable_letter(i)``, where ``i`` is the
least possible value such that the returned letter does not belong to
*alphabet*.

:param alphabet: the alphabet.
:type alphabet: Alphabet

:returns: The first unused letter.
:rtype: str | int

:raises LibsemigroupsError:
  if ``alphabet`` already contains the maximum possible number of supported
  letters.
)pbdoc");
    }  // bind_alphabet

    template <typename InputWord, typename OutputWord>
    void bind_to_alphabet(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_alphabet_") + name;
      // NOTE: the following prevents the _cxx_obj of a python Alphabet from
      // being copied here, but doesn't prevent the python Alphabet itself from
      // being copied.
      using Result = std::conditional_t<std::is_same_v<InputWord, OutputWord>,
                                        Alphabet<OutputWord> const&,
                                        Alphabet<OutputWord>>;

      m.def(fn_name.c_str(), [](Alphabet<InputWord> const& p) -> Result {
        return to<Alphabet<OutputWord>>(p);
      });
    }
  }  // namespace

  void init_alphabet(py::module& m) {
    bind_alphabet<std::string>(m, "AlphabetString");
    bind_alphabet<word_type>(m, "AlphabetWord");

    bind_to_alphabet<std::string, std::string>(m, "string");
    bind_to_alphabet<word_type, std::string>(m, "string");
    bind_to_alphabet<std::string, word_type>(m, "word");
    bind_to_alphabet<word_type, word_type>(m, "word");
  }
}  // namespace libsemigroups
