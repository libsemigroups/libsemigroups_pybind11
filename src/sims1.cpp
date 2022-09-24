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

// TODO:
// * iwyu
// * Sims1::find_if
// * Sims1::for_each
// * Sims1Stats
// * RepOrc
// * MinimalRepOrc

// C std headers....
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, size_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/present.hpp>  // for Presentation
#include <libsemigroups/sims1.hpp>    // for Sims1

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_sims1

namespace py = pybind11;

namespace libsemigroups {
  void init_sims1(py::module& m) {
    py::class_<Sims1Stats>(m, "Sims1Stats")
        .def_readonly("max_pending", &Sims1Stats::max_pending)
        .def_readonly("total_pending", &Sims1Stats::total_pending);

    py::class_<Sims1<size_t>>(m, "Sims1")
        .def(py::init<congruence_kind>())
        .def(py::init<Sims1<size_t> const&>())
        .def("number_of_threads",
             py::overload_cast<size_t>(&Sims1<size_t>::number_of_threads),
             py::arg("val"),
             R"pbdoc(
               Set the number of threads.

               :Parameters: **val** (int) - the maximum number of threads to use.

               :Returns: ``self``.
               )pbdoc")
        .def("number_of_threads",
             py::overload_cast<>(&Sims1<size_t>::number_of_threads, py::const_),
             R"pbdoc(
               Returns the current number of threads.

               :Parameters: None

               :Returns: An ``int``.
               )pbdoc")
        .def("report_interval",
             py::overload_cast<>(&Sims1<size_t>::report_interval, py::const_),
             R"pbdoc(
               Returns the current report interval.

               :Parameters: None

               :Returns: A ``int``.
               )pbdoc")
        .def("report_interval",
             py::overload_cast<size_t>(&Sims1<size_t>::report_interval),
             py::arg("val"),
             R"pbdoc(
               Set the report interval.

               :Parameters: **val** (int) - the new value for the report interval.

               :Returns: ``self``.
               )pbdoc")
        .def(
            "short_rules",
            [](Sims1<size_t> const& s) { return s.short_rules(); },
            R"pbdoc(
               Returns the current short rules.

               :Parameters: None

               :Returns: A ``Presentation``.
             )pbdoc")
        .def("short_rules",
             &Sims1<size_t>::short_rules<Presentation<word_type>>,
             py::arg("p"),
             R"pbdoc(
               Set the short rules.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.
               )pbdoc")
        .def("short_rules",
             &Sims1<size_t>::short_rules<Presentation<std::string>>,
             py::arg("p"),
             R"pbdoc(
               Set the short rules.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.
               )pbdoc")
        .def(
            "long_rules",
            [](Sims1<size_t> const& s) { return s.long_rules(); },
            R"pbdoc(
               Returns the current long rules.

               :Parameters: None

               :Returns: A ``Presentation``.
            )pbdoc")
        .def("long_rules",
             &Sims1<size_t>::long_rules<Presentation<word_type>>,
             py::arg("p"),
             R"pbdoc(
               Set the long rules.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.
               )pbdoc")
        .def("long_rules",
             &Sims1<size_t>::long_rules<Presentation<std::string>>,
             py::arg("p"),
             R"pbdoc(
               Set the long rules.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.
               )pbdoc")
        .def("stats",
             py::overload_cast<>(&Sims1<size_t>::stats, py::const_),
             R"pbdoc(
               Returns the current stats object.

               :Parameters: None

               :Returns: A ``Sims1Stats`` object.
               )pbdoc")
        .def("split_at",
             &Sims1<size_t>::split_at,
             py::arg("val"),
             R"pbdoc(
               Split the rules in short_rules and long_rules.

               :param val: the relation to split at.
               :type val: int

               :return: (None)
               )pbdoc")
        .def("long_rule_length",
             &Sims1<size_t>::long_rule_length,
             py::arg("val"),
             R"pbdoc(
               Define the long rule length.

               :param val: the value of the long rule length.
               :type val: int

               :return: ``self``.
               )pbdoc")
        .def(
            "extra",
            [](Sims1<size_t> const& s) { return s.extra(); },
            R"pbdoc(
               Returns the additional defining pairs.

               :Parameters: None

               :Returns: A ``Presentation``.
             )pbdoc")
        .def("extra",
             &Sims1<size_t>::extra<Presentation<word_type>>,
             py::arg("p"),
             R"pbdoc(
               Set the extra rules.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.
               )pbdoc")
        .def("extra",
             &Sims1<size_t>::extra<Presentation<std::string>>,
             py::arg("p"),
             R"pbdoc(
               Set the extra rules.

               :Parameters: **p** (Presentation) - the presentation.

               :Returns: ``self``.
               )pbdoc")
        .def(
            "iterator",
            [](Sims1<size_t> const& s, size_t n) {
              return py::make_iterator(s.cbegin(n), s.cend(n));
            },
            R"pbdoc(
               Returns an iterator pointing at the first congruence.

               :param n: the maximum number of classes in a congruence.
               :type n: int

               :return: An iterator pointing to an ActionDigraph with at most n nodes.
               )pbdoc")
        .def("number_of_congruences",
             &Sims1<size_t>::number_of_congruences,
             py::arg("n"),
             R"pbdoc(
               Returns the number of one-sided congruences with up to a given number of classes.

               :param n: the maximum number of congruence classes.
               :type n: int

               :return: A value of type uint64_t.
               )pbdoc");
  }
}  // namespace libsemigroups
