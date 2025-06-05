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
#include <libsemigroups/types.hpp>

// pybind11....
#include <pybind11/pybind11.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_types

namespace libsemigroups {

  void init_types(py::module& m) {
    {
      py::options options;
      options.disable_enum_members_docstring();
      py::enum_<tril>(m, "tril", R"pbdoc(
The values in this enum can be used to indicate a result is true, false, or not
currently known.

The valid values are:

.. py:attribute:: tril.false
  :value: <tril.false: 0>

  Value representing false.

.. py:attribute:: tril.true
  :value: <tril.true: 1>

  Value representing true.

.. py:attribute:: tril.unknown
  :value: <tril.unknown: 2>

  Value representing unknown (either true or false).
)pbdoc")
          .value("true", tril::TRUE)
          .value("false", tril::FALSE)
          .value("unknown", tril::unknown);
    }

    py::enum_<congruence_kind>(m, "congruence_kind", R"pbdoc(
    The values in this class can be used to indicate that a congruence should
    be 1-sided or 2-sided.
  )pbdoc")
        .value("onesided",
               congruence_kind::onesided,
               R"pbdoc(Type for 1-sided congruences (right).)pbdoc")
        .value("twosided",
               congruence_kind::twosided,
               R"pbdoc(Type for 2-sided congruences.)pbdoc");
  }

}  // namespace libsemigroups
