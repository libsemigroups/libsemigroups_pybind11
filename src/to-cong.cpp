//
// libsemigroups_pybind11
// Copyright (C) 2025 Joseph Edwards
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

// C std headers
#include <cstddef>  // for size_t

// C++ std headers
#include <string>  // for string

// libsemigroups headers
#include <libsemigroups/cong-class.hpp>         // for Congruence
#include <libsemigroups/froidure-pin-base.hpp>  // for FroidurePinBase
#include <libsemigroups/to-cong.hpp>            // for to<Congruence>
#include <libsemigroups/types.hpp>              // for word_type
#include <libsemigroups/word-graph.hpp>         // for WordGraph

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_froidure_pin

namespace libsemigroups {

  namespace py = pybind11;
  namespace {
    template <typename Word>
    void bind_to_congruence(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_congruence_") + name;
      m.def(fn_name.c_str(),
            [](congruence_kind            knd,
               FroidurePinBase&           fpb,
               WordGraph<uint32_t> const& wg) {
              return to<Congruence<Word>>(knd, fpb, wg);
            });
    }
  }  // namespace
  void init_to_congruence(py::module& m) {
    // FroidurePin
    bind_to_congruence<std::string>(m, "string");
    bind_to_congruence<word_type>(m, "word");
  }

}  // namespace libsemigroups
