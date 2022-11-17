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
#include <functional>        // for __base, function
#include <initializer_list>  // for initializer_list
#include <memory>            // for shared_ptr
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/cong-intf.hpp>  // for congruence_kind
#include <libsemigroups/cong.hpp>       // for Congruence
#include <libsemigroups/fpsemi.hpp>     // for FpSemigroup
#include <libsemigroups/runner.hpp>     // for Runner
#include <libsemigroups/types.hpp>      // for word_type

// pybind11....
#include <pybind11/chrono.h>    // for auto conversion of py types for run_for
#include <pybind11/pybind11.h>  // for class_, init, make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "doc-strings.hpp"  // for add_pair, class_index_to_word
#include "main.hpp"         // for init_cong

// Forward decls
namespace libsemigroups {
  class FroidurePinBase;
}

namespace py = pybind11;

namespace libsemigroups {
  void init_cong(py::module& m) {
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
        .def(py::init<congruence_kind, FpSemigroup&>(),
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
             cong_intf_doc_strings::set_number_of_generators)
        .def("number_of_generators",
             &Congruence::number_of_generators,
             cong_intf_doc_strings::number_of_generators)
        .def("add_pair",
             py::overload_cast<word_type const&, word_type const&>(
                 &Congruence::add_pair),
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::add_pair)
        .def("number_of_generating_pairs",
             &Congruence::number_of_generating_pairs,
             cong_intf_doc_strings::number_of_generating_pairs)
        .def("report_every",
             (void (Congruence::*)(std::chrono::nanoseconds))
                 & Runner::report_every,
             py::arg("t"),
             runner_doc_strings::report_every)
        .def("report", &Congruence::report, runner_doc_strings::report)
        .def("report_why_we_stopped",
             &Congruence::report_why_we_stopped,
             runner_doc_strings::report_why_we_stopped)
        .def("kill", &Congruence::kill, runner_doc_strings::kill)
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
             runner_doc_strings::run_for)
        .def("run_until",
             (void (Congruence::*)(std::function<bool()>&)) & Runner::run_until,
             py::arg("func"),
             runner_doc_strings::run_until)
        .def("less",
             &Congruence::less,
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::less)
        .def("const_contains",
             &Congruence::const_contains,
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::const_contains)
        .def("contains",
             &Congruence::contains,
             py::arg("u"),
             py::arg("v"),
             cong_intf_doc_strings::contains)
        .def("number_of_classes",
             &Congruence::number_of_classes,
             cong_intf_doc_strings::number_of_classes)
        .def("number_of_non_trivial_classes",
             &Congruence::number_of_non_trivial_classes,
             cong_intf_doc_strings::number_of_non_trivial_classes)
        .def(
            "non_trivial_classes",
            [](Congruence& C, size_t i) {
              return C.non_trivial_classes()->at(i);
            },
            py::arg("i"),
            cong_intf_doc_strings::non_trivial_classes)
        .def("quotient_froidure_pin",
             &Congruence::quotient_froidure_pin,
             cong_intf_doc_strings::quotient_froidure_pin)
        .def("has_quotient_froidure_pin",
             &Congruence::has_quotient_froidure_pin,
             cong_intf_doc_strings::has_quotient_froidure_pin)
        .def("parent_froidure_pin",
             &Congruence::parent_froidure_pin,
             cong_intf_doc_strings::parent_froidure_pin)
        .def("has_parent_froidure_pin",
             &Congruence::has_parent_froidure_pin,
             cong_intf_doc_strings::has_parent_froidure_pin)
        .def("is_quotient_obviously_finite",
             &Congruence::is_quotient_obviously_finite,
             cong_intf_doc_strings::is_quotient_obviously_finite)
        .def("is_quotient_obviously_infinite",
             &Congruence::is_quotient_obviously_infinite,
             cong_intf_doc_strings::is_quotient_obviously_infinite)
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
             cong_intf_doc_strings::word_to_class_index)
        .def("class_index_to_word",
             &Congruence::class_index_to_word,
             py::arg("i"),
             cong_intf_doc_strings::class_index_to_word)
        .def("kind", &Congruence::kind, cong_intf_doc_strings::kind)
        .def("dead", &Congruence::dead, runner_doc_strings::dead)
        .def("finished", &Congruence::finished, runner_doc_strings::finished)
        .def("timed_out", &Congruence::timed_out, runner_doc_strings::timed_out)
        .def("stopped_by_predicate",
             &Congruence::stopped_by_predicate,
             runner_doc_strings::stopped_by_predicate)
        .def(
            "generating_pairs",
            [](Congruence const& c) {
              return py::make_iterator(c.cbegin_generating_pairs(),
                                       c.cend_generating_pairs());
            },
            cong_intf_doc_strings::generating_pairs);
  }
}  // namespace libsemigroups
