//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021 James D. Mitchell
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

// C++ stl headers....
#include <array>             // for array
#include <chrono>            // for nanoseconds
#include <functional>        // for function
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <memory>            // for shared_ptr
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/fpsemi-intf.hpp>  // for FpSemigroupInterface
#include <libsemigroups/fpsemi.hpp>  // for FpSemigroup, FpSemigroup::equal_to
#include <libsemigroups/runner.hpp>  // for Runner
#include <libsemigroups/types.hpp>   // for word_type, letter_type, relation...

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, init, make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_fpsemi

namespace libsemigroups {
  class FroidurePinBase;
}

namespace py = pybind11;

namespace libsemigroups {
  using rule_type = FpSemigroupInterface::rule_type;
  void init_fpsemi(py::module& m) {
    py::class_<FpSemigroup>(m, "FpSemigroup")
        .def(py::init<>())
        .def(py::init<std::shared_ptr<FroidurePinBase>>())
        .def(py::init<FpSemigroup const&>())
        .def("validate_letter",
             py::overload_cast<char>(&FpSemigroup::validate_letter, py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter.

               :Parameters: **c** (str) - the letter to validate.

               :Returns: (None)
               )pbdoc")
        .def("validate_letter",
             py::overload_cast<letter_type>(&FpSemigroup::validate_letter,
                                            py::const_),
             py::arg("c"),
             R"pbdoc(
               Validates a letter.

               :Parameters: **c** (int) - the letter to validate.

               :Returns: (None)
               )pbdoc")
        .def("validate_word",
             py::overload_cast<word_type const&>(&FpSemigroup::validate_word,
                                                 py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word.

               :Parameters: **w** (List[int]) - the word to validate.

               :Returns: (None)
               )pbdoc")
        .def("validate_word",
             py::overload_cast<std::string const&>(&FpSemigroup::validate_word,
                                                   py::const_),
             py::arg("w"),
             R"pbdoc(
               Validates a word.

               :Parameters: **w** (str) - the word to validate.

               :Returns: (None)
               )pbdoc")
        .def("set_alphabet",
             py::overload_cast<size_t>(&FpSemigroup::set_alphabet),
             py::arg("n"),
             R"pbdoc(
               Set the size of the alphabet.

               :Parameters: **n** (int) - the number of letters.

               :Returns: (None)
               )pbdoc")
        .def("set_alphabet",
             py::overload_cast<std::string const&>(&FpSemigroup::set_alphabet),
             py::arg("a"),
             R"pbdoc(
               Set the alphabet of the finitely presented semigroup.

               :Parameters: **a** (str) - the alphabet.

               :Returns: (None)
               )pbdoc")
        .def("alphabet",
             py::overload_cast<>(&FpSemigroup::alphabet, py::const_),
             R"pbdoc(
               Returns the alphabet of the finitely presented semigroup
               represented.


               :Returns: A string.
               )pbdoc")
        .def("alphabet",
             py::overload_cast<size_t>(&FpSemigroup::alphabet, py::const_),
             py::arg("i"),
             R"pbdoc(
               Returns the i-th letter of the alphabet of the finitely
               presented semigroup.

               :Parameters: **i** (int) - the index of the letter.

               :Returns: A string.
               )pbdoc")
        .def("set_identity",
             py::overload_cast<letter_type>(&FpSemigroup::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a string of length 1 belonging to
               :py:meth:`~FpSemigroup.alphabet` to be the identity using its
               index.

               :Parameters: **id** (int) - the index of the character to be the identity.

               :Returns: (None)
               )pbdoc")
        .def("set_identity",
             py::overload_cast<std::string const&>(&FpSemigroup::set_identity),
             py::arg("id"),
             R"pbdoc(
               Set a string of length 1 belonging to
               :py:meth:`~FpSemigroup.alphabet` to be the identity.

               :Parameters: **id** (str) - a string containing the character to be the identity.

               :Returns: (None)
               )pbdoc")
        .def("identity",
             &FpSemigroup::identity,
             R"pbdoc(
               Returns the identity of this, or raises an exception if there
               isn't one.

               :return: A string.
               )pbdoc")
        .def("set_inverses",
             &FpSemigroup::set_inverses,
             py::arg("a"),
             R"pbdoc(
               Set the inverses of letters in :py:meth:`~FpSemigroup.alphabet`.

               :param a: a string of length ``alphabet().size()``.
               :type a: str

               :return: (None)
               )pbdoc")
        .def("inverses",
             &FpSemigroup::inverses,
             R"pbdoc(
               Returns the inverses of this, or raises an exception if there
               aren't any.

               :return: A string.
               )pbdoc")
        .def("add_rule",
             py::overload_cast<relation_type>(&FpSemigroup::add_rule),
             py::arg("rel"),
             R"pbdoc(
               Add a rule.

               :Parameters: **rel** (Tuple[List[int], List[int]]) - the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rule",
             py::overload_cast<rule_type>(&FpSemigroup::add_rule),
             py::arg("rel"),
             R"pbdoc(
               Add a rule.

               :Parameters: **rel** (Tuple[str, str]) - the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rule",
             py::overload_cast<std::string const&, std::string const&>(
                 &FpSemigroup::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule.

               :Parameters: - **u** (str) - the left-hand side of the rule being added.
                            - **v** (str) - the right-hand side of the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rule",
             py::overload_cast<word_type const&, word_type const&>(
                 &FpSemigroup::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a rule.

               :Parameters: - **u** (List[int]) - the left-hand side of the rule being added.
                            - **v** (List[int]) - the right-hand side of the rule being added.

               :Returns: (None)
               )pbdoc")
        .def("add_rules",
             py::overload_cast<FroidurePinBase&>(&FpSemigroup::add_rules),
             py::arg("S"),
             R"pbdoc(
               Add the rules of a finite presentation for S to this.

               :Parameters: **S** (:py:class:`FroidurePin`) - a FroidurePin object representing a semigroup.

               :Returns: (None)
               )pbdoc")
        .def("add_rules",
             py::overload_cast<std::vector<rule_type> const&>(
                 &FpSemigroup::add_rules),
             py::arg("rels"),
             R"pbdoc(
               Add the rules in the given list to the finitely presented
               semigroup.

               :Parameters: **rels** (List[Tuple[str, str]]) - the rules to add.

               :Returns: (None)
               )pbdoc")
        .def(
            "number_of_rules",
            [](FpSemigroup const& fp) { return fp.number_of_rules(); },
            R"pbdoc(
               Returns the number of rules currently used to define the
               finitely presented semigroups.

               :return: An int.
               )pbdoc")
        .def("report_every",
             (void (FpSemigroup::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
               Set the minimum elapsed time between reports.

               :Parameters: **t** (datatime.timedelta) - the amount of time between reports.

               :Returns: (None)
               )pbdoc")
        .def("report",
             &FpSemigroup::report,
             R"pbdoc(
               Check if it is time to report.

               :return: A bool.
               )pbdoc")
        .def("report_why_we_stopped",
             &FpSemigroup::report_why_we_stopped,
             R"pbdoc(
               Report why the algorithm stopped.

               :return: (None)
               )pbdoc")
        .def("kill",
             &FpSemigroup::kill,
             R"pbdoc(
               Stop the algorithm from running (thread-safe).

               :return: (None).
               )pbdoc")
        .def("run",
             &FpSemigroup::run,
             R"pbdoc(
               Run the algorithm.

               :return: (None)
               )pbdoc")
        .def("run_for",
             (void (FpSemigroup::*)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
               Run for a specified amount of time.

               :Parameters: **t** (datetime.timedelta) - the time to run for.

               :Returns: (None)
               )pbdoc")
        .def("run_until",
             (void (FpSemigroup::*)(std::function<bool()>&))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
               Run until a nullary predicate returns ``True`` or the algorithm
               is finished.

               :Parameters: **func** (Callable[], bool) - the nullary predicate.

               :Returns: (None)
               )pbdoc")
        .def("dead",
             &FpSemigroup::dead,
             R"pbdoc(
               Check if the algorithm was killed.

               :return: A ``bool``.
               )pbdoc")
        .def("finished",
             &FpSemigroup::finished,
             R"pbdoc(
               Check if the algorithm has been run to completion or not.

               :return: A ``bool``.
               )pbdoc")
        .def("started",
             &FpSemigroup::started,
             R"pbdoc(
               Check if the algorithm has started.

               :return: A ``bool``.
               )pbdoc")
        .def("stopped",
             &FpSemigroup::stopped,
             R"pbdoc(
               Check if the algorithm is stopped.

               :return: A ``bool``.
               )pbdoc")
        .def("timed_out",
             &FpSemigroup::timed_out,
             R"pbdoc(
               Check if the amount of time specified to
               :py:meth:`~FpSemigroup.run_for` has elapsed.

               :return: A ``bool``.
               )pbdoc")
        .def(
            "running",
            [](FpSemigroup const& fp) { return fp.running(); },
            R"pbdoc(
               Check if the algorithm is currently running.

               :return: ``True`` if algorithm is in the process of running and ``False`` it is not.
               )pbdoc")
        .def("stopped_by_predicate",
             &FpSemigroup::stopped_by_predicate,
             R"pbdoc(
               Check if the runner was, or should, stop because the nullary
               predicate passed as first argument to
               :py:meth:`~FpSemigroup.run_until` return ``True``.

               :return: A ``bool``.
               )pbdoc")
        .def("normal_form",
             py::overload_cast<std::string const&>(&FpSemigroup::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a string.

               :Parameters: **w** (str) - the word whose normal form we want to find. The parameter w must consist of letters in :py:meth:`~FpSemigroup.alphabet()`.

               :Returns: A string.
               )pbdoc")
        .def("normal_form",
             py::overload_cast<word_type const&>(&FpSemigroup::normal_form),
             py::arg("w"),
             R"pbdoc(
               Returns a normal form for a list of integers.

               :Parameters: **w** (List[int]) - the word whose normal form we want to find. The parameter ``w`` consist of indices of the generators of the finitely presented semigroup that ``self`` represents.

               :Returns: A list of integers.
               )pbdoc")
        .def("equal_to",
             py::overload_cast<std::string const&, std::string const&>(
                 &FpSemigroup::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (str) - the first word, must be a string over :py:meth:`~FpSemigroup.alphabet`.
                            - **v** (str) - the second word, must be a string over :py:meth:`~FpSemigroup.alphabet`.

               :Returns: ``True`` if the strings ``u`` and ``v`` represent the same element of the finitely presented semigroup, and ``False`` otherwise.
               )pbdoc")
        .def("equal_to",
             py::overload_cast<word_type const&, word_type const&>(
                 &FpSemigroup::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if two words represent the same element.

               :Parameters: - **u** (List[int]) - the first word.
                            - **v** (List[int]) - the second word.

               :Returns: ``True`` if the words ``u`` and ``v`` represent the same element of the finitely presented semigroup, and ``False`` otherwise.
               )pbdoc")
        .def("word_to_string",
             &FpSemigroup::word_to_string,
             py::arg("w"),
             R"pbdoc(
               Convert a list of integers to a string representing the same
               element of the finitely presented semigroup.

               :param w: the word to convert.
               :type w: List[int]

               :return: A string.
               )pbdoc")
        .def("string_to_word",
             &FpSemigroup::string_to_word,
             py::arg("w"),
             R"pbdoc(
               Convert a string to a list of integers representing the
               same element of the finitely presented semigroup.

               :param w: the string to convert.
               :type w: str

               :return: A list of integers.
               )pbdoc")
        .def("uint_to_char",
             &FpSemigroup::uint_to_char,
             py::arg("a"),
             R"pbdoc(
               Convert an ``int`` to a string of length 1 representing the same
               generator of the finitely presented semigroup.

               :param a: the int to convert.
               :type a: int

               :return: A string of length 1.
               )pbdoc")
        .def("char_to_uint",
             &FpSemigroup::char_to_uint,
             py::arg("a"),
             R"pbdoc(
               Convert a string of length 1 to an ``int`` representing the same
               generator of the finitely presented semigroup.

               :param a: the string to convert.
               :type a: str

               :return: An ``int``.
             )pbdoc")
        .def(
            "has_froidure_pin",
            [](FpSemigroup const& x) { return x.has_froidure_pin(); },
            R"pbdoc(
              Returns True if a ``FroidurePin`` instance isomorphic to the
              finitely presented semigroup has already been
              computed, and False if not.

              :return: A ``bool``.
            )pbdoc")
        .def(
            "froidure_pin",
            [](FpSemigroup& x) { return x.froidure_pin(); },
            R"pbdoc(
              Returns a ``FroidurePin`` instance isomorphic to the finitely
              presented semigroup.

              :return: A ``FroidurePin`` instance.
            )pbdoc")
        .def("has_knuth_bendix",
             &FpSemigroup::has_knuth_bendix,
             R"pbdoc(
               Checks if a ``KnuthBendix`` instance is being used to
               compute the finitely presented semigroup.

               :return: A ``bool``.
               )pbdoc")
        .def("knuth_bendix",
             &FpSemigroup::knuth_bendix,
             R"pbdoc(
               Returns the ``KnuthBendix`` instance used to compute
               the finitely presented semigroup (if any).

               :return: A ``KnuthBendix`` instance.
               )pbdoc")
        .def("has_todd_coxeter",
             &FpSemigroup::has_todd_coxeter,
             R"pbdoc(
               Checks if a ``ToddCoxeter`` instance is being used to
               compute the finitely presented semigroup.

               :return: A ``bool``.
               )pbdoc")
        .def("todd_coxeter",
             &FpSemigroup::todd_coxeter,
             R"pbdoc(
               Returns the ``ToddCoxeter`` instance
               used to compute the finitely presented semigroup (if any).

               :return: A ``ToddCoxeter`` instance.
               )pbdoc")
        .def("is_obviously_finite",
             &FpSemigroup::is_obviously_finite,
             R"pbdoc(
               Return ``True`` if the finitely presented semigroup is obviously
               finite, and ``False`` if it is not obviously finite.

               :return: A ``bool``.
               )pbdoc")
        .def("is_obviously_infinite",
             &FpSemigroup::is_obviously_infinite,
             R"pbdoc(
               Return ``True`` if the finitely presented semigroup is obviously
               infinite, and ``False`` if it is not obviously infinite.

               :return: A ``bool``.
               )pbdoc")
        .def("size",
             &FpSemigroup::size,
             R"pbdoc(
               Returns the size of the finitely presented semigroup.

               :return:
                An ``int`` the value of which equals the size of this
                if this number is finite, or ``POSITIVE_INFINITY`` if this
                number is not finite.
              )pbdoc")
        .def(
            "rules",
            [](FpSemigroup const& fp) {
              return py::make_iterator(fp.cbegin_rules(), fp.cend_rules());
            },
            R"pbdoc(
               Returns an iterator to the rules used to define the finitely
               presented semigroup.

               :return: An iterator.
               )pbdoc")
        .def("to_gap_string",
             &FpSemigroup::to_gap_string,
             R"pbdoc(
               Returns a string containing GAP commands for defining a finitely
               presented semigroup equal to that represented by ``self``.

               :return: A string.
               )pbdoc");
  }
}  // namespace libsemigroups
