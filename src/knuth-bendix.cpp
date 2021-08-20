//
//// libsemigroups - C++ library for semigroups and monoids
//// Copyright (C) 2020 James D. Mitchell
////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU General Public License as published by
//// the Free Software Foundation, either version 3 of the License, or
//// (at your option) any later version.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU General Public License for more details.
////
//// You should have received a copy of the GNU General Public License
//// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////
//
//// Status: incomplete
////
//// TODO:
//// 1) regenerate
//// 2) fix the doc

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {
  using rule_type = FpSemigroupInterface::rule_type;
  void init_knuth_bendix(py::module &m) {
    py::class_<fpsemigroup::KnuthBendix> kb(m, "KnuthBendix");

    py::enum_<fpsemigroup::KnuthBendix::options::overlap>(kb, "overlap")
        .value("ABC", fpsemigroup::KnuthBendix::options::overlap::ABC)
        .value("AB_BC", fpsemigroup::KnuthBendix::options::overlap::AB_BC)
        .value("MAX_AB_BC",
               fpsemigroup::KnuthBendix::options::overlap::MAX_AB_BC);

    kb.def(py::init<>())
        .def("set_alphabet",
             py::overload_cast<std::string const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::set_alphabet),
             py::arg("a"),
             R"pbdoc(
           Set the alphabet of the finitely presented semigroup.

           :Parameters: **a** (str) - the alphabet.

           :Returns: (None)
           )pbdoc")
        .def("set_alphabet",
             py::overload_cast<size_t>(
                 &libsemigroups::fpsemigroup::KnuthBendix::set_alphabet),
             py::arg("n"),
             R"pbdoc(
           Set the size of the alphabet.

           :Parameters: **n** (??) - the number of letters.

           :Returns: (None)
           )pbdoc")
        .def(
            "alphabet",
            py::overload_cast<>(
                &libsemigroups::fpsemigroup::KnuthBendix::alphabet, py::const_),
            R"pbdoc(
           Returns the alphabet of the finitely presented semigroup represented by this.

           :Returns: A const reference to the alphabet, a value of type

           std::string.
           )pbdoc")
        .def(
            "alphabet",
            py::overload_cast<size_t>(
                &libsemigroups::fpsemigroup::KnuthBendix::alphabet, py::const_),
            py::arg("i"),
            R"pbdoc(
           Returns the i-th letter of the alphabet of the finitely presented semigroup represented by this.

           :Parameters: **i** (??) - the index of the letter.

           :Returns: A std::string by value.
           )pbdoc")
        .def("add_rule",
             py::overload_cast<std::string const &, std::string const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Add a rule.

           :Parameters: - **u** (??) - the left-hand side of the rule being added.

                        - **v** (??) - the right-hand side of the rule being added.
           :Returns: (None)
           )pbdoc")
        .def("size",
             &libsemigroups::fpsemigroup::KnuthBendix::size,
             R"pbdoc(
           Returns the size of the finitely presented semigroup.

           :return: A uint64_t, the value of which equals the size of this if this number is finite, or libsemigroups::POSITIVE_INFINITY in some cases if this number is not finite.
           )pbdoc")
        .def("number_of_active_rules",
             &libsemigroups::fpsemigroup::KnuthBendix::number_of_active_rules,
             R"pbdoc(
           Returns the current number of active rules in the KnuthBendix instance.

           :return: The current number of active rules, a value of type size_t.
           )pbdoc")
        .def("confluent",
             &libsemigroups::fpsemigroup::KnuthBendix::confluent,
             R"pbdoc(
           Check if the KnuthBendix instance is confluent.

           :return: true if the KnuthBendix instance is confluent and false if it is not.
           )pbdoc")
        .def("equal_to",
             py::overload_cast<word_type const &, word_type const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
            Check if two words represent the same element.

           :Parameters: - **u** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.
                        - **v** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.

           :Returns: true if the words u and v represent the same element of the finitely presented semigroup, and false otherwise.
           )pbdoc")
        .def("run",
             &libsemigroups::fpsemigroup::KnuthBendix::run,
             R"pbdoc(
            Member function that wraps run_impl which is implemented in a derived class.

            :return: (None)
            )pbdoc")
        .def("active_rules",
             &libsemigroups::fpsemigroup::KnuthBendix::active_rules,
             R"pbdoc(
            Returns a copy of the active rules of the KnuthBendix instance.
           :return: A copy of the currently active rules, a value of type std::vector<rule_type>.
           )pbdoc")
        .def("number_of_rules",
             &libsemigroups::fpsemigroup::KnuthBendix::number_of_rules,
             R"pbdoc(
            Returns the number of rules of the finitely presented semigroup represented by this.

            :return: A value of type size_t.
            )pbdoc")
        .def("set_identity",
             py::overload_cast<letter_type>(
                 &libsemigroups::fpsemigroup::KnuthBendix::set_identity),
             py::arg("id"),
             R"pbdoc(
             Set a character in alphabet() to be the identity using its index.

             :Parameters: **id** (??) - the index of the character to be the identity.

             :Returns: (None)
             )pbdoc")
        .def("set_identity",
             py::overload_cast<std::string const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::set_identity),
             py::arg("id"),
             R"pbdoc(
            Set a character in alphabet() to be the identity.

            :Parameters: **id** (??) - a string containing the character to be the identity.

            :Returns: (None)
            )pbdoc")
        .def("set_inverses",
             &libsemigroups::fpsemigroup::KnuthBendix::set_inverses,
             py::arg("a"),
             R"pbdoc(
           Set the inverses of letters in alphabet().

           :param a: a string of length alphabet().size().
           :type a: ??

           :return: (None)
           )pbdoc")
        .def("identity",
             &libsemigroups::fpsemigroup::KnuthBendix::identity,
             R"pbdoc(
           Returns the identity of this, or throws an exception if there isn't one.

           :return: A const reference to the identity, a value of type std::string.
           )pbdoc")
        .def("inverses",
             &libsemigroups::fpsemigroup::KnuthBendix::inverses,
             R"pbdoc(
           Returns the inverses of this, or throws an exception if there aren't any.

           :return: A const reference to the inverses, a value of type std::string.
           )pbdoc")
        .def("is_obviously_finite",
             &libsemigroups::fpsemigroup::KnuthBendix::is_obviously_finite,
             R"pbdoc(
           Return true if the finitely presented semigroup represented by this is obviously finite, and false if it is not obviously finite.

           :return: A bool.
           )pbdoc")
        .def("is_obviously_infinite",
             &libsemigroups::fpsemigroup::KnuthBendix::is_obviously_infinite,
             R"pbdoc(
            Return true if the finitely presented semigroup represented by this is obviously infinite, and false if it is not obviously infinite.

            :return: A bool.
            )pbdoc")
        .def("equal_to",
             py::overload_cast<std::initializer_list<letter_type>,
                               std::initializer_list<letter_type>>(
                 &libsemigroups::fpsemigroup::KnuthBendix::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
            Check if two words represent the same element.

           :Parameters: - **u** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.
                        - **v** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.

           :Returns: true if the words u and v represent the same element of the finitely presented semigroup, and false otherwise.
           )pbdoc")
        .def("equal_to",
             py::overload_cast<std::string const &, std::string const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
            Check if two words represent the same element.

           :Parameters: - **u** (??) - a string over the alphabet of the finitely presented semigroup.
                        - **v** (??) - a string over the alphabet of the finitely presented semigroup.

           :Returns: true if the strings u and v represent the same element of the finitely presented semigroup, and false otherwise.
           )pbdoc")
        .def("equal_to",
             py::overload_cast<word_type const &, word_type const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::equal_to),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if two words represent the same element.

           :Parameters: - **u** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.
                        - **v** (??) - a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup.

           :Returns: true if the words u and v represent the same element of the finitely presented semigroup, and false otherwise.
           )pbdoc")
        .def("rewrite",
             py::overload_cast<std::string>(
                 &libsemigroups::fpsemigroup::KnuthBendix::rewrite, py::const_),
             py::arg("w"),
             R"pbdoc(
            Rewrite a word.

           :Parameters: **w** (??) - the word to rewrite.

           :Returns: A copy of the argument w after it has been rewritten.
           )pbdoc")
        .def("rewrite",
             py::overload_cast<std::string *>(
                 &libsemigroups::fpsemigroup::KnuthBendix::rewrite, py::const_),
             py::arg("w"),
             R"pbdoc(
            Rewrite a word in-place.

            :Parameters: **w** (??) - the word to rewrite.

            :Returns: The argument w after it has been rewritten.
            )pbdoc")
        .def("normal_form",
             py::overload_cast<std::string const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::normal_form),
             py::arg("w"),
             R"pbdoc(
            Returns a normal form for a string.

           :Parameters: **w** (??) - the word whose normal form we want to find.
           The parameter w must be a std::string consisting of letters in alphabet().

           :Returns: the normal form of the parameter w, a value of type std::string.
           )pbdoc")
        .def("normal_form",
             py::overload_cast<word_type const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::normal_form),
             py::arg("w"),
             R"pbdoc(
            Returns a normal form for a libsemigroups::word_type.

           :Parameters: **w** (??) - the word whose normal form we want to find.
           The parameter w must be a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup that this represents.

           :Returns: the normal form of the parameter w, a value of type libsemigroups::word_type.
           )pbdoc")
        .def("normal_form",
             py::overload_cast<std::initializer_list<letter_type>>(
                 &libsemigroups::fpsemigroup::KnuthBendix::normal_form),
             py::arg("w"),
             R"pbdoc(
            Returns a normal form for a libsemigroups::word_type.

           :Parameters: **w** (??) - the word whose normal form we want to find.
           The parameter w must be a libsemigroups::word_type consisting of indices of the generators of the finitely presented semigroup that this represents.

           :Returns: the normal form of the parameter w, a value of type libsemigroups::word_type.
           )pbdoc")
        .def("add_rule",
             py::overload_cast<std::initializer_list<size_t>,
                               std::initializer_list<size_t>>(
                 &libsemigroups::fpsemigroup::KnuthBendix::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Add a rule.

           :Parameters: - **u** (??) - the left-hand side of the rule being added.
                        - **v** (??) - the right-hand side of the rule being added.

           :Returns: (None)
           )pbdoc")
        .def("add_rule",
             py::overload_cast<word_type const &, word_type const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::add_rule),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
            Add a rule.

           :Parameters: - **u** (??) - the left-hand side of the rule being added.
                        - **v** (??) - the right-hand side of the rule being added.

           :Returns: (None)
           )pbdoc")
        .def("contains_empty_string",
             &libsemigroups::fpsemigroup::KnuthBendix::contains_empty_string,
             R"pbdoc(
            Returns whether or not the empty string belongs to the finitely presented semigroup represented by this.

           :return: A value of type bool.
           )pbdoc")
        .def("number_of_normal_forms",
             &libsemigroups::fpsemigroup::KnuthBendix::number_of_normal_forms,
             py::arg("min"),
             py::arg("max"),
             R"pbdoc(
            Returns the number of normal forms with length in a given range.

            :param min: the minimum length of a normal form to count
            :type min: ??
            :param max: one larger than the maximum length of a normal form to count.
            :type max: ??

            :return: A value of type uint64_t.
            )pbdoc")
        .def("validate_letter",
             py::overload_cast<char>(
                 &libsemigroups::fpsemigroup::KnuthBendix::validate_letter,
                 py::const_),
             py::arg("c"),
             R"pbdoc(
             Validates a letter.

             :Parameters: **c** (??) - the letter to validate.

             :Returns: (None)
             )pbdoc")
        .def("validate_letter",
             py::overload_cast<letter_type>(
                 &libsemigroups::fpsemigroup::KnuthBendix::validate_letter,
                 py::const_),
             py::arg("c"),
             R"pbdoc(
            Validates a letter.

           :Parameters: **c** (??) - the letter to validate.

           :Returns: (None)
           )pbdoc")
        .def("validate_word",
             py::overload_cast<std::string const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::validate_word,
                 py::const_),
             py::arg("w"),
             R"pbdoc(
            Validates a word.

            :Parameters: **w** (??) - the word to validate.

            :Returns: (None)
            )pbdoc")
        .def("validate_word",
             py::overload_cast<word_type const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::validate_word,
                 py::const_),
             py::arg("w"),
             R"pbdoc(
            Validates a word.

           :Parameters: **w** (??) - the word to validate.

           :Returns: (None)
           )pbdoc")
        .def("froidure_pin",
             &libsemigroups::fpsemigroup::KnuthBendix::froidure_pin,
             R"pbdoc(
             Returns a FroidurePin instance isomorphic to the finitely presented semigroup defined by this.

            :return: A shared pointer to a FroidurePinBase.
            )pbdoc")
        .def("has_froidure_pin",
             &libsemigroups::fpsemigroup::KnuthBendix::has_froidure_pin,
             R"pbdoc(
             Returns true if a FroidurePin instance isomorphic to the finitely presented semigroup defined by this has already been computed, and false if not.

            :return: A bool.
            )pbdoc")
        .def("run_for",
             (void(libsemigroups::fpsemigroup::
                       KnuthBendix::  // NOLINT(whitespace/parens)
                           *)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
            Run for a specified amount of time.

            :Parameters: **t** (??) - the time in nanoseconds to run for.

            :Returns: (None)
            )pbdoc")
        .def("run_until",
             (void(libsemigroups::fpsemigroup::
                       KnuthBendix::  // NOLINT(whitespace/parens)
                           *)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
           Run until a nullary predicate returns true or Runner::finished.

           :Parameters: **func** (??) - a function pointer.

           :Returns: (None)
           )pbdoc")
        .def("kill",
             &libsemigroups::fpsemigroup::KnuthBendix::kill,
             R"pbdoc(
            Stop Runner::run from running (thread-safe).

           :return: (None).
           )pbdoc")
        .def("dead",
             &libsemigroups::fpsemigroup::KnuthBendix::dead,
             R"pbdoc(
            Check if the runner is dead.

           :return: A bool.
           )pbdoc")
        .def("finished",
             &libsemigroups::fpsemigroup::KnuthBendix::finished,
             R"pbdoc(
            Check if Runner::run has been run to completion or not.

            :return: A bool.
            )pbdoc")
        .def("started",
             &libsemigroups::fpsemigroup::KnuthBendix::started,
             R"pbdoc(
            Check if Runner::run has already been called.

           :return: A bool.
           )pbdoc")
        .def("stopped",
             &libsemigroups::fpsemigroup::KnuthBendix::stopped,
             R"pbdoc(
           Check if the runner is stopped.

           :return: A bool.
           )pbdoc")
        .def("timed_out",
             &libsemigroups::fpsemigroup::KnuthBendix::timed_out,
             R"pbdoc(
            Check if the amount of time specified to Runner::run_for has elapsed.
            :return: A bool
            )pbdoc")
        .def("stopped_by_predicate",
             &libsemigroups::fpsemigroup::KnuthBendix::stopped_by_predicate,
             R"pbdoc(
            Check if the runner was, or should, stop because the nullary predicate passed as first argument to Runner::run_until.

           :return: A bool.
           )pbdoc")
        .def("running",
             &libsemigroups::fpsemigroup::KnuthBendix::running,
             R"pbdoc(
            Check if a Runner instance is currently running.

           :return: true if Runner::run is in the process to run and false it is not.
           )pbdoc")
        .def("char_to_uint",
             &libsemigroups::fpsemigroup::KnuthBendix::char_to_uint,
             py::arg("a"),
             R"pbdoc(
           Convert a char to a libsemigroups::letter_type representing the same generator of the finitely presented semigroup represented by this.

           :param a: the string to convert.
           :type a: ??

           :return: a libsemigroups::letter_type.
           )pbdoc")
        .def("uint_to_char",
             &libsemigroups::fpsemigroup::KnuthBendix::uint_to_char,
             py::arg("a"),
             R"pbdoc(
            Convert a libsemigroups::letter_type to a char representing the same generator of the finitely presented semigroup represented by this.

           :param a: the libsemigroups::letter_type to convert.
           :type a: ??

           :return: A char.
           )pbdoc")
        .def("string_to_word",
             &libsemigroups::fpsemigroup::KnuthBendix::string_to_word,
             py::arg("w"),
             R"pbdoc(
            Convert a string to a libsemigroups::word_type representing the same element of the finitely presented semigroup represented by this.

           :param w: the string to convert.
           :type w: ??

           :return: a libsemigroups::word_type.
           )pbdoc")
        .def("word_to_string",
             &libsemigroups::fpsemigroup::KnuthBendix::word_to_string,
             py::arg("w"),
             R"pbdoc(
            Convert a libsemigroups::word_type to a std::string representing the same element of the finitely presented semigroup represented by this.

            :param w: the libsemigroups::word_type to convert.
            :type w: ??

            :return: a std::string.
            )pbdoc")
        .def("to_gap_string",
             &libsemigroups::fpsemigroup::KnuthBendix::to_gap_string,
             R"pbdoc(
            Returns a string containing GAP commands for defining a finitely presented semigroup equal to that represented by this.

            :return: A std::string.
            )pbdoc")
        .def(
            "check_confluence_interval",
            &libsemigroups::fpsemigroup::KnuthBendix::check_confluence_interval,
            py::arg("val"),
            R"pbdoc(
            Set the interval at which confluence is checked.

           :param val: the new value of the interval.
           :type val: ??

           :return: A reference to *this.
           )pbdoc")
        .def("overlap_policy",
             &libsemigroups::fpsemigroup::KnuthBendix::overlap_policy,
             py::arg("val"),
             R"pbdoc(
            Set the overlap policy.

           :param val: the maximum number of rules.
           :type val: ??

           :return: A reference to *this.
           )pbdoc")
        .def("max_overlap",
             &libsemigroups::fpsemigroup::KnuthBendix::max_overlap,
             py::arg("val"),
             R"pbdoc(
            Set the maximum length of overlaps to be considered.

           :param val: the new value of the maximum overlap length.
           :type val: ??

           :return: A reference to *this.
           )pbdoc")
        .def("max_rules",
             &libsemigroups::fpsemigroup::KnuthBendix::max_rules,
             py::arg("val"),
             R"pbdoc(
            Set the maximum number of rules.

           :param val: the maximum number of rules.
           :type val: ??

           :return: A reference to *this.
           )pbdoc")
        .def("report",
             &libsemigroups::fpsemigroup::KnuthBendix::report,
             R"pbdoc(
           Check if it is time to report.

           :return: A bool.
           )pbdoc")
        .def("report_every",
             (void(libsemigroups::fpsemigroup::
                       KnuthBendix::  // NOLINT(whitespace/parens)
                           *)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
            Set the minimum elapsed time between reports.

           :Parameters: **t** (??) - the amount of time (in nanoseconds) between reports.

           :Returns: (None)
           )pbdoc")
        .def("report_why_we_stopped",
             &libsemigroups::fpsemigroup::KnuthBendix::report_why_we_stopped,
             R"pbdoc(
            Report why Runner::run stopped.

           :return: (None)
           )pbdoc")
        .def("knuth_bendix_by_overlap_length",
             &libsemigroups::fpsemigroup::KnuthBendix::
                 knuth_bendix_by_overlap_length,
             R"pbdoc(
            Run the Knuth-Bendix algorithm on the KnuthBendix instance.

           :return: (None)
           )pbdoc")
        .def(
            "rules_iterator",
            [](fpsemigroup::KnuthBendix const &kb) {
              return py::make_iterator(kb.cbegin_rules(), kb.cend_rules());
            },
            R"pbdoc(
            Returns an iterator to the generating pairs of the congruence.
            )pbdoc")
        .def(
            "normal_forms",
            [](fpsemigroup::KnuthBendix &kb, size_t const mn, size_t const mx) {
              return py::make_iterator(kb.cbegin_normal_forms(mn, mx),
                                       kb.cend_normal_forms());
            },
            R"pbdoc(
          Returns an iterator to the generating pairs of the congruence.
          )pbdoc")
        .def(
            "normal_forms_alphabet",
            [](fpsemigroup::KnuthBendix &kb,
               std::string const &       lphbt,
               size_t const              mn,
               size_t const              mx) {
              return py::make_iterator(kb.cbegin_normal_forms(lphbt, mn, mx),
                                       kb.cend_normal_forms());
            },
            R"pbdoc(
          Returns an iterator to the generating pairs of the congruence.
          )pbdoc")
        .def("add_rule",
             py::overload_cast<rule_type>(
                 &libsemigroups::fpsemigroup::KnuthBendix::add_rule),
             py::arg("rel"),
             R"pbdoc(
           Add a rule.

           :Parameters: **rel** (??) - the rule being added.

           :Returns: (None)
           )pbdoc")
        .def("add_rules",
             py::overload_cast<FroidurePinBase &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::add_rules),
             py::arg("S"),
             R"pbdoc(
           Add the rules of a finite presentation for S to this.

           :Parameters: **S** (??) - a FroidurePin object representing a semigroup.

           :Returns: (None)
           )pbdoc")
        .def("add_rules",
             py::overload_cast<std::vector<rule_type> const &>(
                 &libsemigroups::fpsemigroup::KnuthBendix::add_rules),
             py::arg("rels"),
             R"pbdoc(
           Add the rules in a vector to this.

           :Parameters: **rels** (??) - a vector of FpSemigroupInterface::rule_type.

           :Returns: (None)
           )pbdoc")
        .def("gilman_digraph",
             &libsemigroups::fpsemigroup::KnuthBendix::gilman_digraph,
             R"pbdoc(
            Returns the Gilman digraph (or automata) of this.

           :return: A const reference to a ActionDigraph<size_t>.
           )pbdoc");

    /*
     *       .def(py::init<FroidurePinBase &>())
     *           .def(py::init<KnuthBendix const &>())
     *               .def(py::init<std::shared_ptr<FroidurePinBase>>())
     *
     *                         .def("cbegin_normal_forms",
     *                                  py::overload_cast<std::string const &,
     * size_t const, size_t const>(
     *                                               &libsemigroups::fpsemigroup::KnuthBendix::cbegin_normal_forms),
     *                                                        py::arg("lphbt"),
     *                                                                 py::arg("min"),
     *                                                                          py::arg("max"),
     *                                                                                   R"pbdoc(
     *                                                                                              Returns
     * a forward iterator pointing at the first normal form whose length is in
     * the given range using the specified alphabet.
     *
     *                                                                                                             :Parameters:
     * - **lphbt** (??) - the alphabet to use for the normal forms
     *                                                                                                                                         -
     * **min** (??) - the minimum length of a normal form
     *                                                                                                                                                                 -
     * **max** (??) - one larger than the maximum length of a normal form.
     *
     *                                                                                                                                                                                :Returns:
     * A value of type const_normal_form_iterator. )pbdoc")
     *
     *                                                                                                                                                                                                 .def("cbegin_normal_forms",
     *                                                                                                                                                                                                          py::overload_cast<size_t
     * const, size_t const>(
     *                                                                                                                                                                                                                       &libsemigroups::fpsemigroup::KnuthBendix::cbegin_normal_forms),
     *                                                                                                                                                                                                                                py::arg("min"),
     *                                                                                                                                                                                                                                         py::arg("max"),
     *                                                                                                                                                                                                                                                  R"pbdoc(
     *                                                                                                                                                                                                                                                             Returns
     * a forward iterator pointing at the first normal form whose length is in
     * the given range using the alphabet returned by KnuthBendix::alphabet.
     *
     *                                                                                                                                                                                                                                                                                :Parameters:
     * - **min** (??) - the minimum length of a normal form
     *                                                                                                                                                                                                                                                                                                        -
     * **max** (??) - one larger than the maximum length of a normal form.
     *
     *                                                                                                                                                                                                                                                                                                                       :Returns:
     * A value of type const_normal_form_iterator. )pbdoc")
     *
     *                                                                                                                                                                                                                                                                                                                                        .def("cend_normal_forms",
     *                                                                                                                                                                                                                                                                                                                                                 &libsemigroups::fpsemigroup::KnuthBendix::cend_normal_forms,
     *                                                                                                                                                                                                                                                                                                                                                          R"pbdoc(
     *                                                                                                                                                                                                                                                                                                                                                                     Returns
     * a forward iterator pointing to one after the last normal form.
     *
     *                                                                                                                                                                                                                                                                                                                                                                                    )pbdoc")
     *                                                                                                                                                                                                                                                                                                                                                                                       .*/
  }
}  // namespace libsemigroups
