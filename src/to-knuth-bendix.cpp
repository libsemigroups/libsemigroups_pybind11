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

// C++ std headers
#include <string>  // for string

// libsemigroups headers
#include <libsemigroups/froidure-pin-base.hpp>   // for FroidurePinBase
#include <libsemigroups/knuth-bendix-class.hpp>  // for KnuthBendix
#include <libsemigroups/to-knuth-bendix.hpp>     // for to<KnuthBendix>
#include <libsemigroups/todd-coxeter-class.hpp>  // for ToddCoxeter
#include <libsemigroups/types.hpp>               // for word_type

#include <libsemigroups/detail/rewriting-system.hpp>  // for detail::LenLexTrie

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_to_knuth_bendix

namespace libsemigroups {
  namespace py = pybind11;
  namespace {
    template <typename Word, typename Rewriter>
    void bind_froidure_pin_to_knuth_bendix(py::module&        m,
                                           std::string const& name) {
      std::string fn_name = std::string("to_knuth_bendix_") + name;
      m.def(fn_name.c_str(), [](congruence_kind knd, FroidurePinBase& fp) {
        return to<KnuthBendix<Word, Rewriter>>(knd, fp);
      });
    }

    template <typename Word, typename Rewriter>
    void bind_todd_coxeter_to_knuth_bendix(py::module&        m,
                                           std::string const& name) {
      std::string fn_name = std::string("to_knuth_bendix_") + name;
      m.def(fn_name.c_str(), [](congruence_kind knd, ToddCoxeter<Word>& tc) {
        return to<KnuthBendix<Word, Rewriter>>(knd, tc);
      });
    }

    template <typename Word>
    void bind_todd_coxeter_to_knuth_bendix_default(py::module& m) {
      m.def("to_knuth_bendix", [](congruence_kind knd, ToddCoxeter<Word>& tc) {
        return to<KnuthBendix>(knd, tc);
      });
    }
  }  // namespace

  void init_to_knuth_bendix(py::module& m) {
    using LenLexTrie = detail::RewritingSystemTrie<ShortLexCompare>;
    using LenLexSet  = detail::RewritingSystemSet<ShortLexCompare>;
    using RPOTrie    = detail::RewritingSystemTrie<RecursivePathCompare>;
    using RPOSet     = detail::RewritingSystemSet<RecursivePathCompare>;

    // FroidurePin
    bind_froidure_pin_to_knuth_bendix<std::string, LenLexSet>(
        m, "string_LenLexSet");
    bind_froidure_pin_to_knuth_bendix<std::string, LenLexTrie>(
        m, "string_LenLexTrie");
    bind_froidure_pin_to_knuth_bendix<word_type, LenLexSet>(m,
                                                            "word_LenLexSet");
    bind_froidure_pin_to_knuth_bendix<word_type, LenLexTrie>(m,
                                                             "word_LenLexTrie");

    // ToddCoxeter + rewriter
    bind_todd_coxeter_to_knuth_bendix<std::string, LenLexSet>(m, "LenLexSet");
    bind_todd_coxeter_to_knuth_bendix<word_type, LenLexSet>(m, "LenLexSet");
    bind_todd_coxeter_to_knuth_bendix<std::string, LenLexTrie>(m, "LenLexTrie");
    bind_todd_coxeter_to_knuth_bendix<word_type, LenLexTrie>(m, "LenLexTrie");

    // ToddCoxeter
    bind_todd_coxeter_to_knuth_bendix_default<std::string>(m);
    bind_todd_coxeter_to_knuth_bendix_default<word_type>(m);
  }
}  // namespace libsemigroups
