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
#include <libsemigroups/detail/report.hpp>

// pybind11....
#include <pybind11/pybind11.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_detail_report

namespace libsemigroups {

  void init_detail_report(py::module& m) {
    py::class_<ReportGuard>(m,
                            "ReportGuard",
                            R"pbdoc(
Objects of this type can be used to enable printing of some information
during various of the computation in ``libsemigroups_pybind11``. Reporting
is enabled (or not) at construction time, and disable when the
:any:`ReportGuard` goes out of scope.
      )pbdoc")
        .def(py::init<bool>(),
             py::arg("val") = true,
             R"pbdoc(
Constructs a :any:`ReportGuard` with reporting enabled by
default.

:param val:
   whether to report or not (default: ``True``).

:type val:
   bool
)pbdoc");
  }
}  // namespace libsemigroups
