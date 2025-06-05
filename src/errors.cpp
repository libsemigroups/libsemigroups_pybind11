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
    // TODO this doesn't seem to properly catch all LibsemigroupsExceptions,
    // particularly on macOS. This may have been resolved in pybind11 2.12.0
    static py::exception<LibsemigroupsException> exc(
        m, "LibsemigroupsError", PyExc_RuntimeError);
    py::register_exception_translator([](std::exception_ptr p) {
      try {
        if (p) {
          std::rethrow_exception(p);
        }
      } catch (LibsemigroupsException const& e) {
        exc(formatted_error_message(e).c_str());
      } catch (py::stop_iteration const& e) {
        throw e;
      } catch (std::runtime_error const& e) {
        exc(formatted_error_message(e).c_str());
      }
    });
    // TODO: Doc
    m.def("error_message_with_prefix",
          py::overload_cast<>(&error_message_with_prefix));
    m.def("error_message_with_prefix",
          py::overload_cast<bool>(&error_message_with_prefix));
  }
}  // namespace libsemigroups
