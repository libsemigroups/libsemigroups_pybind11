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
        .def(py::init<congruence_kind>(),
             py::arg("kind"),
             R"pbdoc(
               Construct from kind (left/right/2-sided) and options.

               Constructs an empty instance of an interface to a congruence of
               type specified by the argument.

               :Parameters: - **kind** (congruence_kind) the handedness of the congruence.

               :Complexity: Constant.

               .. seealso:: :py:meth:`set_number_of_generators` and
                            :py:meth:`add_pair`.
             )pbdoc")
        .def(py::init<congruence_kind, std::shared_ptr<FroidurePinBase>>(),
             py::arg("kind"),
             py::arg("S"),
             R"pbdoc(
               Construct from kind (left/right/2-sided) and
               :py:class:`FroidurePin`.

               Constructs a Congruence over the FroidurePin instance ``S``
               representing a left/right/2-sided congruence according to
               ``kind``.

               :Parameters: - **kind** (congruence_kind) the handedness of the
                              congruence.
                            - **S** (FroidurePin) semigroup over which the
                              congruence is defined.

               :Complexity: Linear in the size of ``S``.
             )pbdoc")
        .def(py::init<congruence_kind, FpSemigroup &>(),
             py::arg("kind"),
             py::arg("S"),
             R"pbdoc(
               Construct from kind (left/right/2-sided) and
               :py:class:`FpSemigroup`.

               Constructs a Congruence over the FpSemigroup instance ``S``
               representing a left/right/2-sided congruence according to ``type``.

               :Parameters: - **kind** (congruence_kind) the handedness of the
                              congruence.
                            - **S** (FpSemigroup) semigroup over which the
                              congruence is defined.

               :Complexity: Constant.
             )pbdoc")
        .def("set_number_of_generators",
             &Congruence::set_number_of_generators,
             py::arg("n"),
             R"pbdoc(
               Set the number of generators of the congruence.

               :param n: the number of generators.
               :type n: int

               :return: (None)
             )pbdoc")
        .def("number_of_generators",
             &Congruence::number_of_generators,
             R"pbdoc(
               Returns the number of generators specified by
               :py:meth:`set_number_of_generators`.

               :Parameters: None

               :return:
                 The number of generators of the semigroup of the congruence,
                 or :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("add_pair",
             py::overload_cast<word_type const &, word_type const &>(
                 &Congruence::add_pair),
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Add a generating pair to the congruence.

               :param u: a word over the generators of the semigroup.
               :type u: List[int]
               :param v: a word over the generators of the semigroup.
               :type v: List[int]

               :return: (None)
             )pbdoc")
        .def("number_of_generating_pairs",
             &Congruence::number_of_generating_pairs,
             R"pbdoc(
               Returns the number of generating pairs added by
               :py:meth:`add_pair`.

               :return:
                 The number of generating pairs of the congruence that an
                 object of this type represents.
             )pbdoc")
        .def("report_every",
             (void (Congruence::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             R"pbdoc(
               Set the minimum elapsed time between reports.

               :param t: the amount of time between reports.
               :type t: datetime.timedelta

               :Returns: (None)
             )pbdoc")
        .def("report",
             &Congruence::report,
             R"pbdoc(
               Check if it is time to report.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("report_why_we_stopped",
             &Congruence::report_why_we_stopped,
             R"pbdoc(
               Report why we stopped.

               :Parameters: None
               :return: (None)
             )pbdoc")
        .def("kill",
             &Congruence::kill,
             R"pbdoc(
               Stop Runner::run from running (thread-safe).

               :Parameters: None
               :return: (None).
             )pbdoc")
        .def("run",
             &Congruence::run,
             R"pbdoc(
               Run all the underlying algorithms to determine the structure of
               the congruence.

               :Parameters: None
               :return: (None)
             )pbdoc")
        .def("run_for",
             (void (Congruence::*)(std::chrono::nanoseconds)) & Runner::run_for,
             py::arg("t"),
             R"pbdoc(
               Run for a specified amount of time.

               :param t: the time in nanoseconds to run for.
               :type t: datetime.timedelta

               :Returns: (None)
             )pbdoc")
        .def("run_until",
             (void (Congruence::*)(std::function<bool()> &))
                 & Runner::run_until,
             py::arg("func"),
             R"pbdoc(
               Run until a nullary predicate returns ``True`` or
               :py:meth:`finished`.

               :param func: a function.
               :type func: Callable[], bool

               :Returns: (None)
             )pbdoc")
        .def("less",
             &Congruence::less,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               This function returns ``True`` if the congruence class of ``v``
               is less than the class of ``v`` in a total ordering of
               congruence classes.

               :param u: a word over the generators of the semigroup.
               :type u: List[int]
               :param v: a word over the generators of the semigroup.
               :type v: List[int]

               :return:
                 ``True`` if the class of ``u`` is less than that of ``v``.
             )pbdoc")
        .def("const_contains",
             &Congruence::const_contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if a pair of words is known to belong to the congruence.

               :param u: a word  over the generators of the semigroup.
               :type u: List[int]
               :param v: a word  over the generators of the semigroup.
               :type v: List[int]

               :return:
                 :py:obj:`tril.TRUE` if the words ``u`` and ``v`` are known to
                 belong to the same congruence class :py:obj:`tril.FALSE` if
                 the words are known to not belong to the same congruence class
                 :py:obj:`tril.unknown` otherwise.
             )pbdoc")
        .def("contains",
             &Congruence::contains,
             py::arg("u"),
             py::arg("v"),
             R"pbdoc(
               Check if a pair of words belongs to the congruence.

               :param u: a word over the generators of the semigroup.
               :type u: List[int]
               :param v: a word over the generators of the semigroup.
               :type v: List[int]

               :return:
                 ``True`` if the words ``u`` and ``v`` belong to the same
                 congruence class, and ``False`` otherwise.
             )pbdoc")
        .def("number_of_classes",
             &Congruence::number_of_classes,
             R"pbdoc(
               Returns the number of classes in the congruence.

               :Parameters: None

               :return:
                 The number of congruences classes of this if this
                 number is finite, or :py:obj:`POSITIVE_INFINITY` in some cases
                 if this number is not finite.
             )pbdoc")
        .def("number_of_non_trivial_classes",
             &Congruence::number_of_non_trivial_classes,
             R"pbdoc(
               Returns the number of non-trivial classes (size > 1) of the
               congruence.

               :Parameters: None

               :return: The number of non-trivial classes of the congruence.
             )pbdoc")
        .def(
            "non_trivial_classes",
            [](Congruence &C, size_t i) {
              return C.non_trivial_classes()->at(i);
            },
            py::arg("i"),
            R"pbdoc(
              Returns the words belonging to non-trivial class with given index.

              :param i: the index of the non-trivial class.
              :type i: int

              :return: A ``List[List[int]]``.
            )pbdoc")
        .def("quotient_froidure_pin",
             &Congruence::quotient_froidure_pin,
             R"pbdoc(
               Returns a semigroup represented as :py:class:`FroidurePin` that
               is isomorphic to the quotient of the parent semigroup by
               the 2-sided congruence that this represents.

               :Parameters: None

               :return: A :py:class:`FroidurePin`.
             )pbdoc")
        .def("has_quotient_froidure_pin",
             &Congruence::has_quotient_froidure_pin,
             R"pbdoc(
               Returns ``True`` if the congruence knows an isomorphic quotient
               semigroup represented by an instance of :py:class:`FroidurePin`.

               :Parameters: None

               :return: A ``bool``.
             )pbdoc")
        .def("parent_froidure_pin",
             &Congruence::parent_froidure_pin,
             R"pbdoc(
               Returns the :py:class:`FroidurePin` over which the
               congruence was defined, if it exists.

               :Parameters: None

               :return: A :py:class:`FroidurePin`.
             )pbdoc")
        .def("has_parent_froidure_pin",
             &Congruence::has_parent_froidure_pin,
             R"pbdoc(
               Returns ``True`` if the congruence was created from a
               :py:obj:`FroidurePin` instance.

               :Parameters: None

               :return: A ``bool``.
             )pbdoc")
        .def("is_quotient_obviously_finite",
             &Congruence::is_quotient_obviously_finite,
             R"pbdoc(
               Return ``True`` if the number of classes in the congruence
               is obviously finite, and ``False`` if it is not obviously
               finite.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("is_quotient_obviously_infinite",
             &Congruence::is_quotient_obviously_infinite,
             R"pbdoc(
               Return true if the number of classes in the congruence
               represented by this is obviously infinite, and false if it is
               not obviously infinite.

               :Parameters: None
               :return: A ``bool``.
               )pbdoc")
        .def("has_todd_coxeter",
             &Congruence::has_todd_coxeter,
             R"pbdoc(
               Checks if a :py:class:`ToddCoxeter` instance is being used to
               compute the congruence.

               :Parameters: None
               :return: A ``bool``.
               )pbdoc")
        .def("todd_coxeter",
             &Congruence::todd_coxeter,
             R"pbdoc(
               Returns the :py:class:`ToddCoxeter` being used to compute the
               congruence (if any).

               :Parameters: None
               :return: A :py:class:`ToddCoxeter` or ``None``.
             )pbdoc")
        .def("has_knuth_bendix",
             &Congruence::has_knuth_bendix,
             R"pbdoc(
               Checks if a :py:class:`KnuthBendix` instance is being used to
               compute the congruence.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("knuth_bendix",
             &Congruence::knuth_bendix,
             R"pbdoc(
               Returns the :py:class:`KnuthBendix` being used to compute the
               congruence (if any).

               :Parameters: None
               :return: A :py:class:`KnuthBendix` or ``None``.
             )pbdoc")
        .def("word_to_class_index",
             &Congruence::word_to_class_index,
             py::arg("w"),
             R"pbdoc(
               If the congruence, that an object of this type represents, is
               defined over a semigroup with generators :math:`A`, then this
               function defines a surjective function from the set of all words
               over :math:`A` to either :math:`\{0, 1, \ldots, n - 1\}`, where
               :math:`n` is the number of classes, or to the non-negative
               integers :math:`\{0, 1, \ldots\}` if this has infinitely many
               classes.

               :param w:
                 the word whose class index we want to find.
               :type w: List[int]

               :return:
                  The index of the congruence class corresponding to the word.
               )pbdoc")
        .def("class_index_to_word",
             &Congruence::class_index_to_word,
             py::arg("i"),
             R"pbdoc(
               If the congruence is defined over a semigroup with generators
               :math:`A`, then this function defines a injective function from
               :math:`\{0, 1, \ldots, n - 1\}`, where :math:`n` is the number
               of classes, or to the non-negative integers :math:`\{0, 1,
               \ldots\}` if this has infinitely many classes, to a fixed set of
               words over :math:`A` representing distinct congruences classes.

               :param i:
                  the index of the class whose representative we want to find
               :type i: int

               :return:
                 The word representing the ``i``-th class of the congruence
             )pbdoc")
        .def("kind",
             &Congruence::kind,
             R"pbdoc(
               Return if the congruence was created as a left, right, or
               two-sided congruence.

               :Parameters: None
               :return: A :py:class:`congruence_kind`.
             )pbdoc")
        .def("dead",
             &Congruence::dead,
             R"pbdoc(
               Check if the runner is dead.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("finished",
             &Congruence::finished,
             R"pbdoc(
               Check if an algorithm used to determine the structure of the
               congruence has been run to completion or not.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("timed_out",
             &Congruence::timed_out,
             R"pbdoc(
               Check if the algorithms used to find the structure of the
               congruence have timed out.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("stopped_by_predicate",
             &Congruence::stopped_by_predicate,
             R"pbdoc(
               Check if the algorithms used to find the structure of the
               congruence were stopped by the nullary predicate passed as first
               argument to :py:meth:`run_until`.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def(
            "generating_pairs",
            [](Congruence const &c) {
              return py::make_iterator(c.cbegin_generating_pairs(),
                                       c.cend_generating_pairs());
            },
            R"pbdoc(
               Returns an iterator pointing to the first generating pair
               of the congruence (if any).

               :Parameters: None
               :return: An iterator.
            )pbdoc");
  }
}  // namespace libsemigroups
