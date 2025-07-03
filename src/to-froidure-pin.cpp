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
#include <cstdint>  // for uint32_t

// C++ std headers
#include <string>  // for string

// libsemigroups headers
#include <libsemigroups/cong-class.hpp>          // for Congruence
#include <libsemigroups/froidure-pin.hpp>        // for FroidurePin
#include <libsemigroups/kambites-class.hpp>      // for Kambites
#include <libsemigroups/knuth-bendix-class.hpp>  // for KnuthBendix
#include <libsemigroups/to-froidure-pin.hpp>     // for to<FroidurePin>
#include <libsemigroups/todd-coxeter-class.hpp>  // for ToddCoxeter
#include <libsemigroups/transf.hpp>              // for Transf
#include <libsemigroups/types.hpp>               // for word_type
#include <libsemigroups/word-graph.hpp>          // for WordGraph

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_froidure_pin

namespace libsemigroups {
  namespace py = pybind11;
  namespace {
    template <typename InputType>
    void bind_to_froidure_pin(py::module& m) {
      m.def("to_froidure_pin",
            [](InputType& input) { return to<FroidurePin>(input); });
    }
  }  // namespace

  void init_to_froidure_pin(py::module& m) {
    // Congruence
    bind_to_froidure_pin<Congruence<std::string>>(m);
    bind_to_froidure_pin<Congruence<word_type>>(m);

    // Kambites
    bind_to_froidure_pin<Kambites<std::string>>(m);
    bind_to_froidure_pin<Kambites<detail::MultiView<std::string>>>(m);
    bind_to_froidure_pin<Kambites<word_type>>(m);

    // KnuthBendix
    bind_to_froidure_pin<KnuthBendix<std::string, detail::RewriteFromLeft>>(m);
    bind_to_froidure_pin<KnuthBendix<std::string, detail::RewriteTrie>>(m);
    bind_to_froidure_pin<KnuthBendix<word_type, detail::RewriteFromLeft>>(m);
    bind_to_froidure_pin<KnuthBendix<word_type, detail::RewriteTrie>>(m);

    // ToddCoxeter
    bind_to_froidure_pin<ToddCoxeter<std::string>>(m);
    bind_to_froidure_pin<ToddCoxeter<word_type>>(m);

    // WordGraph
    m.def("to_froidure_pin", [](WordGraph<uint32_t> const& input) {
      return to<FroidurePin<Transf<0, uint32_t>>>(input);
    });
    m.def("to_froidure_pin",
          [](WordGraph<uint32_t> const& input, size_t first, size_t last) {
            return to<FroidurePin<Transf<0, uint32_t>>>(input, first, last);
          });
  }

}  // namespace libsemigroups
