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

#include <stdexcept>
#include <string>

// libsemigroups headers
#include <libsemigroups/exception.hpp>

// pybind11....
#include <pybind11/pybind11.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_error

namespace libsemigroups {

  bool ERROR_MESSAGE_WITH_PREFIX = false;

  void error_message_with_prefix(bool value) {
    ERROR_MESSAGE_WITH_PREFIX = value;
  }

  bool error_message_with_prefix() {
    return ERROR_MESSAGE_WITH_PREFIX;
  }

  std::string formatted_error_message(std::runtime_error const& e) {
    if (error_message_with_prefix()) {
      return std::string(e.what());
    } else {
      // TODO this doesn't work well if backward is enabled.
      std::string out(e.what());
      size_t      pos = out.find(": ");
      if (pos != std::string::npos) {
        out.erase(0, pos + 2);
      }
      return out;
    }
  }

  void init_error(py::module& m) {
    // Using the GIL safe call below rather than simply having a static
    // py::exception is recommended in the pybind11 doc.
    PYBIND11_CONSTINIT static py::gil_safe_call_once_and_store<py::object>
        exc_storage;
    exc_storage.call_once_and_store_result([&]() {
      return py::exception<LibsemigroupsException>(
          m, "LibsemigroupsError", PyExc_RuntimeError);
    });
    py::register_exception_translator([](std::exception_ptr p) {
      try {
        if (p) {
          std::rethrow_exception(p);
        }
      } catch (LibsemigroupsException const& e) {
        py::set_error(exc_storage.get_stored(),
                      formatted_error_message(e).c_str());
      }
    });

    m.def("error_message_with_prefix",
          py::overload_cast<>(&error_message_with_prefix),
          R"pbdoc(
Return whether :any:`LibsemigroupsError` messages have a C++ prefix.

Since ``libsemigroups_pybind11`` is built on top of the C++ library
libsemigroups_, many of the errors thrown in ``libsemigroups_pybind11``
emanate from C++. This function returns whether :any:`LibsemigroupsError`
messages contain a prefix that indicates which C++ function raised the
exception.

:return: Whether :any:`LibsemigroupsError` messages contain a prefix about the
  C++ function that raised the exception.
:rtype: bool
)pbdoc");
    m.def("error_message_with_prefix",
          py::overload_cast<bool>(&error_message_with_prefix),
          py::arg("val"),
          R"pbdoc(
Specify whether :any:`LibsemigroupsError` messages have a C++ prefix.

Since ``libsemigroups_pybind11`` is built on top of the C++ library
libsemigroups_, many of the errors thrown in ``libsemigroups_pybind11``
emanate from C++. This function specifies whether :any:`LibsemigroupsError`
messages should contain a prefix that indicates which C++ function raised the
exception. By default, this information is not included.

:param val: Whether :any:`LibsemigroupsError` messages should contain a prefix
  about the C++ function that raised the exception.
:type val: bool

)pbdoc");
  }
}  // namespace libsemigroups
