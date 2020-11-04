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
//
// TODO:
// 1) regenerate
// 2) fix the doc

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {
  void init_todd_coxeter(py::module &m) {
    using sort_function_type
        = std::function<bool(word_type const &, word_type const &)>;

    py::class_<congruence::ToddCoxeter> tc(m, "ToddCoxeter");

    py::enum_<congruence::ToddCoxeter::order>(tc, "order")
        .value("none", congruence::ToddCoxeter::order::none)
        .value("shortlex", congruence::ToddCoxeter::order::shortlex)
        .value("lex", congruence::ToddCoxeter::order::lex)
        .value("recursive", congruence::ToddCoxeter::order::recursive);

    py::enum_<congruence::ToddCoxeter::policy::strategy>(tc, "strategy_options")
        .value("hlt", congruence::ToddCoxeter::policy::strategy::hlt)
        .value("felsch", congruence::ToddCoxeter::policy::strategy::felsch)
        .value("random", congruence::ToddCoxeter::policy::strategy::random);

    py::enum_<congruence::ToddCoxeter::policy::lookahead>(tc,
                                                          "lookahead_options")
        .value("full", congruence::ToddCoxeter::policy::lookahead::full)
        .value("partial", congruence::ToddCoxeter::policy::lookahead::partial);

    py::enum_<congruence::ToddCoxeter::policy::froidure_pin>(
        tc, "froidure_pin_options")
        .value("none", congruence::ToddCoxeter::policy::froidure_pin::none)
        .value("use_relations",
               congruence::ToddCoxeter::policy::froidure_pin::use_relations)
        .value("use_cayley_graph",
               congruence::ToddCoxeter::policy::froidure_pin::use_cayley_graph);

    tc.def(py::init<congruence_type>())
        .def(py::init<congruence_type, congruence::ToddCoxeter &>())
        .def(py::init<congruence_type, fpsemigroup::KnuthBendix &>())
        .def(py::init<congruence::ToddCoxeter const &>())
        .def("__repr__",
             [](congruence::ToddCoxeter const &tc) {
               auto n = (tc.nr_generators() == UNDEFINED
                             ? "-"
                             : detail::to_string(tc.nr_generators()));

               return std::string("<ToddCoxeter object with ") + n
                      + " generators and "
                      + detail::to_string(tc.nr_generating_pairs()) + " pairs>";
             })
        .def("set_nr_generators",
             &congruence::ToddCoxeter::set_nr_generators,
             py::arg("n"),
             R"pbdoc(
           Set the number of generators of the congruence.

           :param n: the number of generators.
           :type n: int

           :return: (None)
           )pbdoc")
        .def("nr_generators",
             &congruence::ToddCoxeter::nr_generators,
             R"pbdoc(
           Returns the number of generators.

           :return: The number of generators of the semigroup of the congruence that an object of this type represents, or UNDEFINED.
           )pbdoc")
        .def("add_pair",
             py::overload_cast<word_type const &, word_type const &>(
                 &congruence::ToddCoxeter::add_pair),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Add a generating pair to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: (None)
           )pbdoc")
        .def("nr_generating_pairs",
             &congruence::ToddCoxeter::nr_generating_pairs,
             R"pbdoc(
           Returns the number of generating pairs added by
           CongruenceInterface::add_pair.

           :return: The number of generating pairs of the congruence that an object of this type represents.
           )pbdoc")
        .def("froidure_pin_policy",
             py::overload_cast<congruence::ToddCoxeter::policy::froidure_pin>(
                 &congruence::ToddCoxeter::froidure_pin_policy),
             R"pbdoc(
           Sets the value of the Froidure-Pin policy specified by the argument
           ToddCoxeter.policy.froidure_pin.
           )pbdoc")
        .def("lookahead",
             &congruence::ToddCoxeter::lookahead,
             R"pbdoc(
           Sets the type of lookahead to be used when using the HLT strategy.
           )pbdoc")
        .def("lower_bound",
             &congruence::ToddCoxeter::lower_bound,
             R"pbdoc(
           Sets a lower bound for the number of classes of the congruence
           represented by a ToddCoxeter instance.
           )pbdoc")
        .def("next_lookahead",
             &congruence::ToddCoxeter::next_lookahead,
             R"pbdoc(
           If the number of cosets active exceeds the value set by this function,
           then a lookahead, of the type set by lookahead, is triggered.
           )pbdoc")
        .def("save",
             &congruence::ToddCoxeter::save,
             R"pbdoc(
           If the argument of this function is true and the HLT strategy is being
           used, then deductions are processed during the enumeration.
           )pbdoc")
        .def("standardize",
             py::overload_cast<bool>(&congruence::ToddCoxeter::standardize),
             R"pbdoc(
           If the argument of this function is true, then the coset table is
           standardized (according to the short-lex order) during the coset
           enumeration.
           )pbdoc")
        .def("strategy",
             (congruence::ToddCoxeter::policy::strategy(
                 congruence::ToddCoxeter::*)() const)
                 & congruence::ToddCoxeter::strategy,
             R"pbdoc(
           Returns the value of the strategy used during the coset enumeration.
           )pbdoc")
        .def("strategy",
             py::overload_cast<congruence::ToddCoxeter::policy::strategy>(
                 &congruence::ToddCoxeter::strategy),
             R"pbdoc(
           Set the strategy used during the coset enumeration can be specified
           using this function.
           )pbdoc")
        .def("random_interval",
             (congruence::ToddCoxeter
              & (congruence::ToddCoxeter::*) (std::chrono::nanoseconds))
                 & congruence::ToddCoxeter::random_interval,
             R"pbdoc(
           Sets the duration in nanoseconds that a given randomly selected
           strategy will run for, when using the random strategy
           (policy::strategy::random).
           )pbdoc")
        .def("sort_generating_pairs",
             py::overload_cast<sort_function_type>(
                 &congruence::ToddCoxeter::sort_generating_pairs),
             py::arg("func"),
             R"pbdoc(
           Sorts all existing generating pairs according to the binary function
           func.

           :param func: a value of type binary predicate that defines a linear order on the relations in a ToddCoxeter instance.
           :type func: a function.

           :return: ``self``.
           )pbdoc")
        .def("random_shuffle_generating_pairs",
             &congruence::ToddCoxeter::random_shuffle_generating_pairs,
             R"pbdoc(
           Randomly shuffle all existing generating pairs.

           :return: ``self``.
           )pbdoc")
        .def("report_every",
             (void (congruence::ToddCoxeter::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
           Set the minimum elapsed time between reports.

           :param t: the amount of time (in nanoseconds) between reports.
           :type t: int

           :return: (None)
           )pbdoc")
        .def("report",
             &congruence::ToddCoxeter::report,
             R"pbdoc(
           Check if it is time to report.

           :return: A bool.
           )pbdoc")
        .def("report_why_we_stopped",
             &congruence::ToddCoxeter::report_why_we_stopped,
             R"pbdoc(
           Report why Runner::run stopped.

           :return: (None)
           )pbdoc")
        .def("kill",
             &congruence::ToddCoxeter::kill,
             R"pbdoc(
           Stop Runner::run from running (thread-safe).

           :return: (None).
           )pbdoc")
        .def("run",
             &congruence::ToddCoxeter::run,
             R"pbdoc(
           Run the algorithm until it finishes.

           :return: (None)
           )pbdoc")
        .def("run_for",
             (void (congruence::ToddCoxeter::*)(std::chrono::nanoseconds))
                 & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
           Run for a specified amount of time.

           :param t: the time to run for.
           :type t: datetime.timedelta

           :return: (None)

           Example
           -------

           .. code-block:: python

              from datetime import timedelta
              from libsemigroups_pybind11 import ToddCoxeter, congruence_type

              tc = ToddCoxeter(congruence_type.twosided)
              tc.set_nr_generators(1)
              tc.add_pair([0] * 1000, [0] * 999)
              tc.run_for(timedelta(microseconds=10))
           )pbdoc")
        .def("run_until",
             (void (congruence::ToddCoxeter::*)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
           Run until a nullary predicate returns true or the algorithm
           finishes.

           :param func: a function.
           :type func: func

           :return: (None)
           )pbdoc")
        .def("less",
             &congruence::ToddCoxeter::less,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           This function returns true if the congruence class of v is less than
           the class of v in a total ordering of congruence classes.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: true if the class of u is less than that of .
           )pbdoc")
        .def("const_contains",
             &congruence::ToddCoxeter::const_contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if a pair of words is known to belong to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: ``tril.True`` if the words ``u`` and ``v`` are known to belong to the same congruence class, ``tril.False`` if the words are known to not belong to the same congruence class, ``tril::unknown`` otherwise.
           )pbdoc")
        .def("contains",
             &congruence::ToddCoxeter::contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
           Check if a pair of words belongs to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: true if the words u and v belong to the same congruence class, and false otherwise.
           )pbdoc")
        .def("empty",
             &congruence::ToddCoxeter::empty,
             R"pbdoc(
           Returns true if there are no relations or generating pairs in the
           ToddCoxeter instance, and the number of active cosets is 1 (the
           minimum possible).

     )pbdoc")
        .def("nr_classes",
             &congruence::ToddCoxeter::nr_classes,
             R"pbdoc(
           Computes the total number of classes in the congruence represented
           by an instance of this type.

           :return: The number of congruences classes of this if this number is finite, or POSITIVE_INFINITY in some cases if this number is not finite.
           )pbdoc")
        .def("nr_non_trivial_classes",
             &congruence::ToddCoxeter::nr_non_trivial_classes,
             R"pbdoc(
           Returns the number of non-trivial classes (size > 1) of the congruence.

           :return: The number of non-trivial classes of the congruence.
           )pbdoc")
        .def("reserve",
             &congruence::ToddCoxeter::reserve,
             R"pbdoc(
           Reserves the capacity specified by the argument in the data
           structures for cosets used in a ToddCoxeter instance.
           )pbdoc")
        .def("shrink_to_fit",
             &congruence::ToddCoxeter::shrink_to_fit,
             R"pbdoc(
           Release all memory used to store free cosets, and any other unnecessary
           data if the enumeration is finished.
           )pbdoc")
        .def("quotient_froidure_pin",
             &congruence::ToddCoxeter::quotient_froidure_pin,
             R"pbdoc(
           Returns a semigroup represented as an instance of a derived class of
           FroidurePinBase that is isomorphic to the quotient of the parent
           semigroup of this by the 2-sided congruence that this represents.

           :return: A std::shared_ptr to FroidurePinBase.
           )pbdoc")
        .def("has_quotient_froidure_pin",
             &congruence::ToddCoxeter::has_quotient_froidure_pin,
             R"pbdoc(
           Returns true if the congruence represented by this object knows an
           isomorphic quotient semigroup represented by an instance of
           FroidurePin.

           :return: A bool.
           )pbdoc")
        .def("parent_froidure_pin",
             &congruence::ToddCoxeter::parent_froidure_pin,
             R"pbdoc(
           Returns a shared_ptr to the parent FroidurePin over which the
           congruence represented by this object was defined, if it exists.

           :return: A std::shared_ptr to FroidurePinBase.
           )pbdoc")
        .def("has_parent_froidure_pin",
             &congruence::ToddCoxeter::has_parent_froidure_pin,
             R"pbdoc(
           Returns true if the congruence represented by this was created from
           a FroidurePin instance.

           :return: A bool.
           )pbdoc")
        .def("is_quotient_obviously_finite",
             &congruence::ToddCoxeter::is_quotient_obviously_finite,
             R"pbdoc(
          Return true if the number of classes in the congruence represented by
          this is obviously finite, and false if it is not obviously finite.

          :return: A bool.
          )pbdoc")
        .def("is_quotient_obviously_infinite",
             &congruence::ToddCoxeter::is_quotient_obviously_infinite,
             R"pbdoc(
           Return ``True`` if the number of classes in the congruence
           represented by this is obviously infinite, and ``False`` if it is
           not obviously infinite.

           :return: A bool.
           )pbdoc")
        .def("word_to_class_index",
             &congruence::ToddCoxeter::word_to_class_index,
             py::arg("w"),
             R"pbdoc(
           If the congruence, that an object of this type represents, is
           defined over a semigroup with generators $A$, then this function
           defines a surjective function from the set of all words over $A$ to
           either $\{0, 1, \ldots, n - 1\}$, where $n$ is the number of
           classes, or to the non-negative integers $\{0, 1, \ldots\}$ if this
           has infinitely many classes.

           :param w: the word (list of integers) whose class index we want to find.
           :type w: list

           :return: The index of the congruence class corresponding to word.
           )pbdoc")
        .def("class_index_to_word",
             &congruence::ToddCoxeter::class_index_to_word,
             py::arg("i"),
             R"pbdoc(
           If the congruence, that an object of this type represents, is
           defined over a semigroup with generators :math:`A`, then this
           function defines a injective function from :math:`\{0, 1, \ldots, n
           - 1\}`, where $n$ is the number of classes, or to the non-negative
           integers :math:`\{0, 1, \ldots\}` if this has infinitely many
           classes, to a fixed set of words over :math:`A` representing
           distinct congruences classes.

           :param i: the index of the class whose representative we want to find.
           :type i: int

           :return: A word representing the i-th class of the congruence.
           )pbdoc")
        .def("kind",
             &congruence::ToddCoxeter::kind,
             R"pbdoc(
           Return if the congruence represented by this object was created as a
           left, right, or two-sided congruence.

           :return: A ``congruence_type``.
           )pbdoc")
        .def("complete",
             &congruence::ToddCoxeter::complete,
             R"pbdoc(
           Returns ``True`` if the coset table is complete, and ``False`` if it
           is not.
           )pbdoc")
        .def("compatible",
             &congruence::ToddCoxeter::compatible,
             R"pbdoc(
           Returns true if the coset table is compatible with the relations and
           generating pairs used to create this, and false if it is not.

           )pbdoc")
        .def("dead",
             &congruence::ToddCoxeter::dead,
             R"pbdoc(
           Check if the runner is dead.

           :return: A bool.
           )pbdoc")
        .def("finished",
             &congruence::ToddCoxeter::finished,
             R"pbdoc(
           Check if Runner::run has been run to completion or not.

           :return: A bool.
           )pbdoc")
        .def("started",
             &congruence::ToddCoxeter::started,
             R"pbdoc(
           Check if Runner::run has already been called.

           :return: A bool.
           )pbdoc")
        .def("stopped",
             &congruence::ToddCoxeter::stopped,
             R"pbdoc(
           Check if the runner is stopped.

           :return: A bool.
           )pbdoc")
        .def("timed_out",
             &congruence::ToddCoxeter::timed_out,
             R"pbdoc(
           Check if the amount of time specified to Runner::run_for has elapsed.

           :return: A bool
           )pbdoc")
        .def("stopped_by_predicate",
             &congruence::ToddCoxeter::stopped_by_predicate,
             R"pbdoc(
           Check if the runner was, or should, stop because the nullary
           predicate passed as first argument to Runner::run_until.

           :return: A bool.
           )pbdoc")
        .def("running",
             &congruence::ToddCoxeter::running,
             R"pbdoc(
           Check if a Runner instance is currently running.

           :return: true if Runner::run is in the process to run and false it is not.
           )pbdoc")
        .def("is_standardized",
             &congruence::ToddCoxeter::is_standardized,
             R"pbdoc(
           Returns true if the ToddCoxeter instance is standardized.
           )pbdoc")
        .def("standardize",
             py::overload_cast<congruence::ToddCoxeter::order>(
                 &congruence::ToddCoxeter::standardize),
             R"pbdoc(
           If the argument of this function is true, then the coset table is
           standardized (according to the short-lex order) during the coset
           enumeration.
           )pbdoc")
        .def(
            "generating_pairs",
            [](congruence::ToddCoxeter const &tc) {
              return py::make_iterator(tc.cbegin_generating_pairs(),
                                       tc.cend_generating_pairs());
            },
            R"pbdoc(
          Returns an iterator to the generating pairs of the congruence.
          )pbdoc")
        .def(
            "non_trivial_classes",
            [](congruence::ToddCoxeter &tc) {
              return py::make_iterator(tc.cbegin_ntc(), tc.cend_ntc());
            },
            R"pbdoc(
          Returns an iterator pointing to the first non-trivial (size > 1)
          class of the congruence.
          )pbdoc")
        .def(
            "normal_forms",
            [](congruence::ToddCoxeter &tc) {
              return py::make_iterator(tc.cbegin_normal_forms(),
                                       tc.cend_normal_forms());
            },
            R"pbdoc(
          Returns an iterator to the normal forms of the congruence represented
          by an instance of ToddCoxeter.
          )pbdoc");
  }
}  // namespace libsemigroups
