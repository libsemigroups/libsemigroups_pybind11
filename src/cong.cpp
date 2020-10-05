
//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2020 James D. Mitchell
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

// Status: incomplete

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {
  void init_cong(py::module &m) {
    py::class_<Congruence>(m, "Congruence")
        .def(py::init<congruence_type>())
        .def(py::init<congruence_type, std::shared_ptr<FroidurePinBase>>())
        .def(py::init<congruence_type, FpSemigroup &>())
        .def("set_nr_generators",
             &Congruence::set_nr_generators,
             py::arg("n"),
             R"pbdoc(
               Set the number of generators of the congruence.

               :param n: the number of generators.
               :type n: ??

               :return: (None)
               )pbdoc")
        .def("nr_generators",
             &Congruence::nr_generators,
             R"pbdoc(
               Returns the number of generators specified by CongruenceInterface::set_nr_generators.

               :return: The number of generators of the semigroup of the congruence that an object of this type represents, or UNDEFINED.
               )pbdoc")
        .def("add_pair",
             py::overload_cast<word_type const &, word_type const &>(
                 &Congruence::add_pair),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a generating pair to the congruence.

               :Parameters: - **u** (??) - a word (vector of integers) over the generators of the semigroup.
                            - **v** (??) - a word (vector of integers) over the generators of the semigroup.

               :Returns: (None)
               )pbdoc")
        .def("nr_generating_pairs",
             &Congruence::nr_generating_pairs,
             R"pbdoc(
               Returns the number of generating pairs added by CongruenceInterface::add_pair.

               :return: The number of generating pairs of the congruence that an object of this type represents.
               )pbdoc")
        .def("report_every",
             (void (Congruence::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
               Set the minimum elapsed time between reports.

               :Parameters: **t** (??) - the amount of time (in nanoseconds) between reports.

               :Returns: (None)
               )pbdoc")
        .def("report",
             &Congruence::report,
             R"pbdoc(
               Check if it is time to report.

               :return: A bool.
               )pbdoc")
        .def("report_why_we_stopped",
             &Congruence::report_why_we_stopped,
             R"pbdoc(
               Report why Runner::run stopped.

               :return: (None)
               )pbdoc")
        .def("kill",
             &Congruence::kill,
             R"pbdoc(
               Stop Runner::run from running (thread-safe).

               :return: (None).
               )pbdoc")
        .def("run",
             &Congruence::run,
             R"pbdoc(
               Member function that wraps run_impl which is implemented in a derived class.

               :return: (None)
               )pbdoc")
        .def("run_for",
             (void (Congruence::*)(std::chrono::nanoseconds)) & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
               Run for a specified amount of time.

               :Parameters: **t** (??) - the time in nanoseconds to run for.

               :Returns: (None)
               )pbdoc")
        .def("run_until",
             (void (Congruence::*)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
               Run until a nullary predicate returns true or Runner::finished.

               :Parameters: **func** (??) - a function pointer.

               :Returns: (None)
               )pbdoc")
        .def("less",
             &Congruence::less,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               This function returns true if the congruence class of v is less than the class of v in a total ordering of congruence classes.

               :param u: a word (vector of integers) over the generators of the semigroup.
               :type u: ??
               :param v: a word (vector of integers) over the generators of the semigroup.
               :type v: ??

               :return: true if the class of u is less than that of .
               )pbdoc")
        .def("const_contains",
             &Congruence::const_contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if a pair of words is known to belong to the congruence.

               :param u: a word (vector of integers) over the generators of the semigroup.
               :type u: ??
               :param v: a word (vector of integers) over the generators of the semigroup.
               :type v: ??

               :return:
tril::TRUE if the words u and v are known to belong to the same congruence class
tril::FALSE if the words are known to not belong to the same congruence class
tril::unknown otherwise.


               )pbdoc")
        .def("contains",
             &Congruence::contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if a pair of words belongs to the congruence.

               :param u: a word (vector of integers) over the generators of the semigroup.
               :type u: ??
               :param v: a word (vector of integers) over the generators of the semigroup.
               :type v: ??

               :return: true if the words u and v belong to the same congruence class, and false otherwise.
               )pbdoc")
        .def("nr_classes",
             &Congruence::nr_classes,
             R"pbdoc(
               Computes the total number of classes in the congruence represented by an instance of this type.

               :return: The number of congruences classes of this if this number is finite, or POSITIVE_INFINITY in some cases if this number is not finite.
               )pbdoc")
        .def("nr_non_trivial_classes",
             &Congruence::nr_non_trivial_classes,
             R"pbdoc(
               Returns the number of non-trivial classes (size > 1) of the congruence.

               :return: The number of non-trivial classes of the congruence.
               )pbdoc")
        .def("non_trivial_classes",
             &Congruence::non_trivial_classes,
             R"pbdoc(
               Returns a shared pointer to the non-trivial classes.

               :return: A std::shared_ptr to CongruenceInterface::non_trivial_classes_type.
               )pbdoc")
        .def("quotient_froidure_pin",
             &Congruence::quotient_froidure_pin,
             R"pbdoc(
               Returns a semigroup represented as an instance of a derived class of FroidurePinBase that is isomorphic to the quotient of the parent semigroup of this by the 2-sided congruence that this represents.

               :return: A std::shared_ptr to FroidurePinBase.
               )pbdoc")
        .def("has_quotient_froidure_pin",
             &Congruence::has_quotient_froidure_pin,
             R"pbdoc(
               Returns true if the congruence represented by this object knows an isomorphic quotient semigroup represented by an instance of FroidurePin.

               :return: A bool.
               )pbdoc")
        .def("parent_froidure_pin",
             &Congruence::parent_froidure_pin,
             R"pbdoc(
               Returns a shared_ptr to the parent FroidurePin over which the congruence represented by this object was defined, if it exists.

               :return: A std::shared_ptr to FroidurePinBase.
               )pbdoc")
        .def("has_parent_froidure_pin",
             &Congruence::has_parent_froidure_pin,
             R"pbdoc(
               Returns true if the congruence represented by this was created from a FroidurePin instance.

               :return: A bool.
               )pbdoc")
        .def("is_quotient_obviously_finite",
             &Congruence::is_quotient_obviously_finite,
             R"pbdoc(
               Return true if the number of classes in the congruence represented by this is obviously finite, and false if it is not obviously finite.

               :return: A bool.
               )pbdoc")
        .def("is_quotient_obviously_infinite",
             &Congruence::is_quotient_obviously_infinite,
             R"pbdoc(
               Return true if the number of classes in the congruence represented by this is obviously infinite, and false if it is not obviously infinite.

               :return: A bool.
               )pbdoc")
        .def("has_todd_coxeter",
             &Congruence::has_todd_coxeter,
             R"pbdoc(
               Checks if a congruence::ToddCoxeter instance is being used to compute the congruence.

               :return: A bool.
               )pbdoc")
        .def("todd_coxeter",
             &Congruence::todd_coxeter,
             R"pbdoc(
               Returns the congruence::ToddCoxeter instance used to compute the congruence (if any).

               :return: A shared_ptr to a congruence::ToddCoxeter or nullptr.
               )pbdoc")
        .def("has_knuth_bendix",
             &Congruence::has_knuth_bendix,
             R"pbdoc(
               Checks if a congruence::KnuthBendix instance is being used to compute the congruence.

               :return: A bool.
               )pbdoc")
        .def("knuth_bendix",
             &Congruence::knuth_bendix,
             R"pbdoc(
               Returns the congruence::KnuthBendix instance used to compute the congruence (if any).

               :return: A shared_ptr to a congruence::KnuthBendix or nullptr.
               )pbdoc")
        .def("word_to_class_index",
             &Congruence::word_to_class_index,
             py::arg("w"),
             R"pbdoc(
               If the congruence, that an object of this type represents, is defined over a semigroup with generators $A$, then this function defines a surjective function from the set of all words over $A$ to either $\{0, 1, \ldots, n - 1\}$, where $n$ is the number of classes, or to the non-negative integers $\{0, 1, \ldots\}$ if this has infinitely many classes.

               :param w: the word whose class index we want to find. The parameter w must be a word_type consisting of indices of the generators of the semigroup over which this is defined.
               :type w: ??

               :return: The index of the congruence class corresponding to word.
               )pbdoc")
        .def("class_index_to_word",
             &Congruence::class_index_to_word,
             py::arg("i"),
             R"pbdoc(
               If the congruence, that an object of this type represents, is defined over a semigroup with generators $A$, then this function defines a injective function from $\{0, 1, \ldots, n - 1\}$, where $n$ is the number of classes, or to the non-negative integers $\{0, 1, \ldots\}$ if this has infinitely many classes, to a fixed set of words over $A$ representing distinct congruences classes.

               :param i: the index of the class whose representative we want to find, a value of type word_type.
               :type i: ??

               :return: The word representing the i-th class of the congruence
               )pbdoc")
        .def("kind",
             &Congruence::kind,
             R"pbdoc(
               Return if the congruence represented by this object was created as a left, right, or two-sided congruence.

               :return: A congruence_type.
               )pbdoc")
        .def("dead",
             &Congruence::dead,
             R"pbdoc(
               Check if the runner is dead.

               :return: A bool.
               )pbdoc")
        .def("finished",
             &Congruence::finished,
             R"pbdoc(
               Check if Runner::run has been run to completion or not.

               :return: A bool.
               )pbdoc")
        .def("started",
             &Congruence::started,
             R"pbdoc(
               Check if Runner::run has already been called.

               :return: A bool.
               )pbdoc")
        .def("stopped",
             &Congruence::stopped,
             R"pbdoc(
               Check if the runner is stopped.

               :return: A bool.
               )pbdoc")
        .def("timed_out",
             &Congruence::timed_out,
             R"pbdoc(
               Check if the amount of time specified to Runner::run_for has elapsed.

               :return: A bool
               )pbdoc")
        .def("running",
             &Congruence::running,
             R"pbdoc(
               Check if a Runner instance is currently running.

               :return: true if Runner::run is in the process to run and false it is not.
               )pbdoc")
        .def("stopped_by_predicate",
             &Congruence::stopped_by_predicate,
             R"pbdoc(
               Check if the runner was, or should, stop because the nullary predicate passed as first argument to Runner::run_until.

               :return: A bool.
               )pbdoc")
        .def("cbegin_generating_pairs",
             &Congruence::cbegin_generating_pairs,
             R"pbdoc(
               Returns a const iterator pointing to the first generating pair of the congruence added by CongruenceInterface::add_pair (if any).

               :return: A CongruenceInterface::const_iterator pointing to a relation_type.
               )pbdoc")
        .def("cend_generating_pairs",
             &Congruence::cend_generating_pairs,
             R"pbdoc(
               Returns a const iterator pointing one-after-the-end of the last generating pair of the congruence added by CongruenceInterface::add_pair (if any).

               :return: A CongruenceInterface::const_iterator pointing to a relation_type.
               )pbdoc")
        .def("cbegin_ntc",
             &Congruence::cbegin_ntc,
             R"pbdoc(
               Returns a const iterator pointing to the first non-trivial (size > 1) class of the congruence.

               :return: A CongruenceInterface::non_trivial_class_iterator pointing to a std::vector<word_type>.
               )pbdoc")
        .def("cend_ntc",
             &Congruence::cend_ntc,
             R"pbdoc(
               Returns a const iterator pointing one-past-the-end of the non-trivial (size > 1) classes of the congruence.

               :return: A CongruenceInterface::non_trivial_class_iterator pointing to a std::vector<word_type>.
               )pbdoc");
  }
}  // namespace libsemigroups
