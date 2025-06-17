//
// libsemigroups_pybind11
// Copyright (C) 2025 James D. Mitchell
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
#include <libsemigroups/constants.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_constants

namespace libsemigroups {

  void init_constants(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Undefined
    ////////////////////////////////////////////////////////////////////////

    py::class_<Undefined>(m, "Undefined", R"pbdoc(
The type of :any:`UNDEFINED`.

This class is the type of the constant value :any:`UNDEFINED`, and appears as
such in type annotations in ``libsemigroups_pybind11``.
)pbdoc")
        .def("__repr__",
             [](Undefined const& val) -> std::string { return "UNDEFINED"; })
        .def(py::self < Undefined())
        // TODO doesn't yet exist .def(py::self <= Undefined())
        .def(
            "__eq__",
            [](Undefined const& lhop, Undefined const& rhop) -> bool {
              return true;
            },
            py::is_operator())
        .def("__hash__", [](Undefined const& op) {
          return std::hash<uint64_t>{}(static_cast<uint64_t>(op));
        });

    m.attr("UNDEFINED") = UNDEFINED;

    ////////////////////////////////////////////////////////////////////////
    // PositiveInfinity
    ////////////////////////////////////////////////////////////////////////

    py::class_<PositiveInfinity>(m,
                                 "PositiveInfinity",
                                 R"pbdoc(
The type of :any:`POSITIVE_INFINITY`.

This class is the type of the constant value :any:`POSITIVE_INFINITY`, and appears as
such in type annotations in ``libsemigroups_pybind11``.
)pbdoc")
        .def("__repr__",
             [](PositiveInfinity const& val) -> std::string {
               return u8"+\u221E";
             })
        .def(pybind11::self < PositiveInfinity())
        // .def(pybind11::self <= PositiveInfinity())
        .def(pybind11::self < NegativeInfinity())
        // .def(pybind11::self <= NegativeInfinity()) TODO not implemented
        .def(pybind11::self < int())
        // .def(pybind11::self <= int()) TODO not implemented
        .def(int() < pybind11::self)
        // .def(int() <= pybind11::self) TODO not implemented
        .def(
            "__eq__",
            [](PositiveInfinity const& lhop,
               PositiveInfinity const& rhop) -> bool { return true; },
            py::is_operator())
        .def(
            "__eq__",
            [](PositiveInfinity const& lhop,
               NegativeInfinity const& rhop) -> bool { return false; },
            py::is_operator())
        .def("__hash__", [](PositiveInfinity const& op) -> int {
          return std::hash<uint64_t>{}(static_cast<uint64_t>(op));
        });

    m.attr("POSITIVE_INFINITY") = POSITIVE_INFINITY;

    ////////////////////////////////////////////////////////////////////////
    // NegativeInfinity
    ////////////////////////////////////////////////////////////////////////

    py::class_<NegativeInfinity>(m,
                                 "NegativeInfinity",
                                 R"pbdoc(
The type of :any:`NEGATIVE_INFINITY`.

This class is the type of the constant value :any:`NEGATIVE_INFINITY`, and appears as
such in type annotations in ``libsemigroups_pybind11``.
)pbdoc")
        .def("__repr__",
             [](NegativeInfinity const& val) -> std::string {
               return u8"-\u221E";
             })
        .def(pybind11::self < PositiveInfinity())
        .def(pybind11::self < NegativeInfinity())
        .def(pybind11::self < int())
        .def(int() < pybind11::self)
        .def(
            "__eq__",
            [](NegativeInfinity const& lhop,
               PositiveInfinity const& rhop) -> bool { return lhop == rhop; },
            py::is_operator())
        .def("__hash__", [](NegativeInfinity const& op) -> int {
          return std::hash<int64_t>{}(static_cast<int64_t>(op));
        });

    m.attr("NEGATIVE_INFINITY") = NEGATIVE_INFINITY;

    ////////////////////////////////////////////////////////////////////////
    // LimitMax
    ////////////////////////////////////////////////////////////////////////

    py::class_<LimitMax>(m,
                         "LimitMax",
                         R"pbdoc(
The type of :any:`LIMIT_MAX`.

This class is the type of the constant value :any:`LIMIT_MAX`, and appears as
such in type annotations in ``libsemigroups_pybind11``.
)pbdoc")
        .def("__repr__",
             [](LimitMax const& val) -> std::string { return "LIMIT_MAX"; })
        .def(pybind11::self < LimitMax())
        .def("__hash__", [](LimitMax const& op) -> int {
          return std::hash<uint64_t>{}(static_cast<uint64_t>(op));
        });

    m.attr("LIMIT_MAX") = LIMIT_MAX;
  }
}  // namespace libsemigroups
