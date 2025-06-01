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
#include <cstdint>  // for unint32_t

// C++ std headers
#include <string>  // for string

// libsemigroups headers
#include <libsemigroups/froidure-pin-base.hpp>   // for FroidurePinBase
#include <libsemigroups/knuth-bendix-class.hpp>  // for KnuthBendix
#include <libsemigroups/to-todd-coxeter.hpp>     // for to-todd-coxeter
#include <libsemigroups/todd-coxeter-class.hpp>  // for ToddCoxeter
#include <libsemigroups/types.hpp>               // for congruence_kind
#include <libsemigroups/word-graph.hpp>          // for WordGraph

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_froidure_pin

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    template <typename Word, typename Rewriter>
    void bind_to_todd_coxeter_kb(py::module& m) {
      m.def("to_todd_coxeter",
            [](congruence_kind knd, KnuthBendix<Word, Rewriter>& kb) {
              return to<ToddCoxeter>(knd, kb);
            });
    }
  }  // namespace

  void init_to_todd_coxeter(py::module& m) {
    // KnuthBendix
    bind_to_todd_coxeter_kb<std::string, detail::RewriteFromLeft>(m);
    bind_to_todd_coxeter_kb<std::string, detail::RewriteTrie>(m);
    bind_to_todd_coxeter_kb<word_type, detail::RewriteFromLeft>(m);
    bind_to_todd_coxeter_kb<word_type, detail::RewriteTrie>(m);

    // WordGraph
    m.def("to_todd_coxeter_word",
          [](congruence_kind            knd,
             FroidurePinBase&           fpb,
             WordGraph<uint32_t> const& wg) {
            return to<ToddCoxeter<word_type>>(knd, fpb, wg);
          });
    m.def("to_todd_coxeter_string",
          [](congruence_kind            knd,
             FroidurePinBase&           fpb,
             WordGraph<uint32_t> const& wg) {
            return to<ToddCoxeter<std::string>>(knd, fpb, wg);
          });
  }

}  // namespace libsemigroups
