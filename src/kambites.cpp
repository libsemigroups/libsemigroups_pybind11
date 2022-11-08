//
// libsemigroups_pybind11
// Copyright (C) 2022 James D. Mitchell
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
// C++ stl headers....

// libsemigroups....
#include <libsemigroups/kambites.hpp>  // for Kambites
#include <libsemigroups/types.hpp>     // for rule_type

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for init_kambites
#include "main.hpp"         // for init_kambites

namespace py = pybind11;

namespace libsemigroups {
  void init_kambites(py::module& m) {
    using detail::MultiStringView;
    using string_type =
        typename fpsemigroup::Kambites<MultiStringView>::string_type;

    py::class_<fpsemigroup::Kambites<MultiStringView>> k(m, "Kambites");

    k.def(py::init<>())
        .def(py::init<fpsemigroup::Kambites<MultiStringView> const&>())
        .def("small_overlap_class",
             &fpsemigroup::Kambites<MultiStringView>::small_overlap_class,
             R"pbdoc(
               Get the small overlap class.

               :return:
                 The greatest positive integer :math:`n` such that the finitely
                 semigroup represented by this satisfies the condition
                 :math:`C(n)`; or :py:obj:`POSITIVE_INFINITY` if no word
                 occurring in a relation can be written as a product of pieces.
             )pbdoc")
        .def("number_of_pieces",
             &fpsemigroup::Kambites<MultiStringView>::number_of_pieces,
             py::arg("i"),
             R"pbdoc(
               Returns the minimum number of pieces required to factorise the
               :math:`i`-th relation word.

               :param i: the index of the relation word
               :type i: int

               :return: An ``int``.
             )pbdoc")
        .def("number_of_normal_forms",
             &fpsemigroup::Kambites<MultiStringView>::number_of_normal_forms,
             py::arg("min"),
             py::arg("max"),
             R"pbdoc(
               Returns the number of normal forms with length in a given range.

               :param min: the minimum length of a normal form to count
               :type min: int
               :param max: one larger than the maximum length of a normal form to count.
               :type max: int

               :return: An ``int``.
             )pbdoc")
        .def("uint_to_char",
             &fpsemigroup::Kambites<MultiStringView>::uint_to_char,
             py::arg("a"),
             R"pbdoc(
               Convert a ``int`` to a ``char``.

               :param a: the letter to convert.
               :type a: int

               :return: A ``char``.
             )pbdoc")
        .def("char_to_uint",
             &fpsemigroup::Kambites<MultiStringView>::char_to_uint,
             py::arg("a"),
             R"pbdoc(
               Convert a ``char`` to a ``letter_type``.

               :param a: the string to convert.
               :type a: str

               :return: An ``int``.
             )pbdoc")
        .def("string_to_word",
             &fpsemigroup::Kambites<MultiStringView>::string_to_word,
             py::arg("w"),
             R"pbdoc(
               Convert a string to a word.

               :param w: the string to convert.
               :type w: str

               :return: A  ``List[int]``.
             )pbdoc")
        .def("is_obviously_finite",
             &fpsemigroup::Kambites<MultiStringView>::is_obviously_finite,
             R"pbdoc(
               Check if the finitely presented semigroup is obviously finite.

               :return: A ``bool``.
             )pbdoc")
        .def("word_to_string",
             &fpsemigroup::Kambites<MultiStringView>::word_to_string,
             py::arg("w"),
             R"pbdoc(
               Convert a ``List[int]`` to a ``str``.

               :param w: the word to convert.
               :type w: List[int]

               :return: A string.
             )pbdoc")
        .def("is_obviously_infinite",
             &fpsemigroup::Kambites<MultiStringView>::is_obviously_infinite,
             R"pbdoc(
               Check if the finitely presented semigroup is obviously infinite.

               :return: A ``bool``.
             )pbdoc")
        .def("size",
             &fpsemigroup::Kambites<MultiStringView>::size,
             R"pbdoc(
               Returns the size of the finitely presented semigroup or
               :py:obj:`POSITIVE_INFINITY`.

               :return: An ``int``.
             )pbdoc")
        .def(
            "rules",
            [](fpsemigroup::Kambites<MultiStringView> const& k) {
              return py::make_iterator(k.cbegin_rules(), k.cend_rules());
            },
            R"pbdoc(
              Returns an iterator to the rules.
            )pbdoc")
        .def("alphabet",
             py::overload_cast<size_t>(
                 &fpsemigroup::Kambites<MultiStringView>::alphabet, py::const_),
             py::arg("i"),
             R"pbdoc(
               Returns the ith letter of the alphabet.

               :Parameters: **i** (int) - the index of the letter.

               :Returns: A string.
             )pbdoc")
        .def("alphabet",
             py::overload_cast<>(
                 &fpsemigroup::Kambites<MultiStringView>::alphabet, py::const_),
             R"pbdoc(
               Returns the alphabet.

               :Returns: A string.
             )pbdoc")
        .def("identity",
             &fpsemigroup::Kambites<MultiStringView>::identity,
             R"pbdoc(
               Returns the identity (if any).

               :return: A string.
             )pbdoc")
        .def("set_identity",
             py::overload_cast<letter_type>(
                 &fpsemigroup::Kambites<MultiStringView>::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a character in alphabet() to be the identity using its index.

               :Parameters:
                 **id** (int) - the index of the character to be the identity.

               :Returns: (None)
             )pbdoc")
        .def("inverses",
             &fpsemigroup::Kambites<MultiStringView>::inverses,
             R"pbdoc(
               Returns the inverses (if any).

               :return: A string.
             )pbdoc")
        .def("number_of_rules",
             &fpsemigroup::Kambites<MultiStringView>::number_of_rules,
             R"pbdoc(
               Returns the number of rules.

               :return: An ``int``.
             )pbdoc")
        .def("add_rule",
             py::overload_cast<relation_type>(
                 &fpsemigroup::Kambites<MultiStringView>::add_rule),
             py::arg("rel"),
             R"pbdoc(
               Add a rule using a tuple of lists of ints.

               :Parameters: **rel** (Tuple[List[int], List[int]]) - the rule being added.

               :Returns: (None)
             )pbdoc")
        .def("add_rule",
             py::overload_cast<std::string const&, std::string const&>(
                 &fpsemigroup::Kambites<MultiStringView>::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule using strings.

               :Parameters: - **u** (str) - the left-hand side of the rule being added.
                            - **v** (str) - the right-hand side of the rule being added.

               :Returns: (None)
             )pbdoc")
        .def("add_rule",
             py::overload_cast<word_type const&, word_type const&>(
                 &fpsemigroup::Kambites<MultiStringView>::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule using two word_type const references.

               :Parameters: - **u** (List[int]) - the left-hand side of the rule being added.
                            - **v** (List[int]) - the right-hand side of the rule being added.

               :Returns: (None)
             )pbdoc")
        .def("add_rules",
             py::overload_cast<FroidurePinBase&>(
                 &fpsemigroup::Kambites<MultiStringView>::add_rules),
             py::arg("S"),
             R"pbdoc(
               Add rules from a :py:obj:`FroidurePin` instance.

               :Parameters:
                 **S** (FroidurePin) - a :py:obj:`FroidurePin` object
                 representing a semigroup.

               :Returns: (None)
             )pbdoc")
        .def("has_froidure_pin",
             &fpsemigroup::Kambites<MultiStringView>::has_froidure_pin,
             R"pbdoc(
               Check if an isomorphic :py:obj:`FroidurePin` instance is known.

               :return: A bool.
             )pbdoc")
        .def("froidure_pin",
             &fpsemigroup::Kambites<MultiStringView>::froidure_pin,
             R"pbdoc(
               Returns an isomorphic :py:obj:`FroidurePin` instance.

               :return: A :py:obj:`FroidurePinBase`.
             )pbdoc")
        .def("set_alphabet",
             py::overload_cast<size_t>(
                 &fpsemigroup::Kambites<MultiStringView>::set_alphabet),
             py::arg("n"),
             R"pbdoc(
               Set the size of the alphabet.

               :Parameters: **n** (int) - the number of letters.

               :Returns: (None)
             )pbdoc")
        .def("set_alphabet",
             py::overload_cast<std::string const&>(
                 &fpsemigroup::Kambites<MultiStringView>::set_alphabet),
             py::arg("a"),
             R"pbdoc(
               Set the alphabet of the finitely presented semigroup.

               :Parameters: **a** (str) - the alphabet.

               :Returns: (None)
             )pbdoc")
        .def("set_identity",
             py::overload_cast<std::string const&>(
                 &fpsemigroup::Kambites<MultiStringView>::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a character in alphabet() to be the identity.

               :Parameters:
                 **id** (str) - a string containing the character to be the
                 identity.

               :Returns: (None)
             )pbdoc")
        .def("set_inverses",
             &fpsemigroup::Kambites<MultiStringView>::set_inverses,
             py::arg("a"),
             R"pbdoc(
               Set the inverses of letters in :py:meth:`~Kambites.alphabet()`.

               :param a: a string of length :py:meth:`len(~Kambites.alphabet())``.
               :type a: str

               :return: (None)
             )pbdoc")
        .def("validate_letter",
             py::overload_cast<char>(
                 &fpsemigroup::Kambites<MultiStringView>::validate_letter,
                 py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter specified by a string.

               :Parameters: **c** (str) - the letter to validate.

               :Returns: (None)
             )pbdoc")
        .def("validate_letter",
             py::overload_cast<letter_type>(
                 &fpsemigroup::Kambites<MultiStringView>::validate_letter,
                 py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter specified by an integer.

               :Parameters: **c** (int) - the letter to validate.

               :Returns: (None)
             )pbdoc")
        .def("validate_word",
             py::overload_cast<word_type const&>(
                 &fpsemigroup::Kambites<MultiStringView>::validate_word,
                 py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word given by a ``List[int]``.

               :Parameters: **w** (List[int]) - the word to validate.

               :Returns: (None)
             )pbdoc")
        .def("validate_word",
             py::overload_cast<std::string const&>(
                 &fpsemigroup::Kambites<MultiStringView>::validate_word,
                 py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word given by a string.

               :Parameters: **w** (str) - the word to validate.

               :Returns: (None)
             )pbdoc")
        .def("normal_form",
             py::overload_cast<string_type const&>(
                 &fpsemigroup::Kambites<MultiStringView>::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a string.

               :Parameters: **w** (str) - the word whose normal form we want to find.

               :Returns: A ``str``.
             )pbdoc")
        .def("normal_form",
             py::overload_cast<word_type const&>(
                 &FpSemigroupInterface::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a word_type.

               :Parameters:
                 **w** (List[int]) - the word whose normal form we want to find.

               :Returns:
                  The normal form of the parameter ``w``, a value of type
                  ``List[int]``.
             )pbdoc")
        .def("equal_to",
             py::overload_cast<string_type const&, string_type const&>(
                 &fpsemigroup::Kambites<MultiStringView>::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters:
                  - **u** (str) - first word for comparison.
                  - **v** (str) - second word for comparison.

               :Returns:
                 ``True`` if the strings ``u`` and ``v`` represent the same
                 element of the finitely presented semigroup, and ``False``
                 otherwise.
             )pbdoc")
        .def("equal_to",
             py::overload_cast<word_type const&, word_type const&>(
                 &fpsemigroup::Kambites<MultiStringView>::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (List[int]) - first word for comparison.
                            - **v** (List[int]) - second word for comparison.

               :Returns:
                 ``True`` if the words ``u`` and ``v`` represent the same
                 element of the finitely presented semigroup, and ``False``
                 otherwise.
             )pbdoc")
        .def("has_identity",
             &fpsemigroup::Kambites<MultiStringView>::has_identity,
             R"pbdoc(
               Check if an identity has been set.

               :return: A ``bool``.
             )pbdoc")
        .def("dead",
             &fpsemigroup::Kambites<MultiStringView>::dead,
             runner_doc_strings::dead)
        .def("finished",
             &fpsemigroup::Kambites<MultiStringView>::finished,
             runner_doc_strings::finished)
        .def("started",
             &fpsemigroup::Kambites<MultiStringView>::started,
             runner_doc_strings::started)
        .def("stopped",
             &fpsemigroup::Kambites<MultiStringView>::stopped,
             runner_doc_strings::stopped)
        .def("timed_out",
             &fpsemigroup::Kambites<MultiStringView>::timed_out,
             runner_doc_strings::timed_out)
        .def("running",
             &fpsemigroup::Kambites<MultiStringView>::running,
             runner_doc_strings::running)
        .def("stopped_by_predicate",
             &fpsemigroup::Kambites<MultiStringView>::stopped_by_predicate,
             runner_doc_strings::stopped_by_predicate)
        .def("kill",
             &fpsemigroup::Kambites<MultiStringView>::kill,
             runner_doc_strings::kill)
        .def("run",
             &fpsemigroup::Kambites<MultiStringView>::run,
             runner_doc_strings::run)
        .def("run_for",
             (void (fpsemigroup::Kambites<  // NOLINT(whitespace/parens)
                    MultiStringView>::*)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             runner_doc_strings::run_for)
        .def("run_until",
             (void (fpsemigroup::Kambites<  // NOLINT(whitespace/parens)
                    MultiStringView>::*)(std::function<bool()>&))
                 & Runner::run_until,
             py::arg("func"),
             runner_doc_strings::run_until)
        .def("report_every",
             (void (fpsemigroup::Kambites<  // NOLINT(whitespace/parens)
                    MultiStringView>::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             runner_doc_strings::report_every)
        .def("report",
             &fpsemigroup::Kambites<MultiStringView>::report,
             runner_doc_strings::report)
        .def("report_why_we_stopped",
             &fpsemigroup::Kambites<MultiStringView>::report_why_we_stopped,
             runner_doc_strings::report_why_we_stopped);
  }
}  // namespace libsemigroups
