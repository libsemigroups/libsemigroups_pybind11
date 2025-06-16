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
#include <libsemigroups/froidure-pin.hpp>
#include <libsemigroups/knuth-bendix.hpp>

#include <libsemigroups/detail/kbe.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "kbe.hpp"   // for ElementStateful
#include "main.hpp"  // for init_kbe

namespace libsemigroups {
  namespace py = pybind11;

  template <typename Thing>
  void bind_kbe(py::module& m, std::string_view thing_name) {
    using Result = ElementStateful<FroidurePin<Thing>>;
    using Word   = typename FroidurePin<Thing>::element_type::native_word_type;

    py::class_<Result> thing(m, thing_name.data());

    thing.def("__repr__", [](Result const& self) {
      return fmt::format("\"{}\"", self.element.word());
    });
    thing.def("__str__",
              [](Result const& self) { return self.element.word(); });

    thing.def("word", [](Result const& self) { return self.element.word(); });

    thing.def("__mul__", [](Result const& self, Result const& other) {
      Result result;
      result.state_ptr = self.state_ptr;
      Product<typename Result::element_type>()(
          result.element, self.element, other.element, result.state_ptr, 0);
      return result;
    });

    thing.def("__eq__", [](Result const& self, Result const& other) {
      return self.element == other.element && self.state_ptr == other.state_ptr;
    });

    thing.def("__eq__", [](Result const& self, Word const& other) {
      return self.element.word() == other;
    });
    thing.def("__eq__", [](Word const& other, Result const& self) {
      return self.element.word() == other;
    });

    // This class does not have very many methods, maybe that's a good thing,
    // better to convert to a "word" and use that instead.
  }

  void init_kbe(py::module& m) {
    using KBEStringTrie
        = detail::KBE<KnuthBendix<std::string, detail::RewriteTrie>>;
    using KBEWordTrie
        = detail::KBE<KnuthBendix<word_type, detail::RewriteTrie>>;
    using KBEStringFromLeft
        = detail::KBE<KnuthBendix<std::string, detail::RewriteFromLeft>>;
    using KBEWordFromLeft
        = detail::KBE<KnuthBendix<word_type, detail::RewriteFromLeft>>;

    bind_kbe<KBEStringTrie>(m, "KBEStringTrie");
    bind_kbe<KBEWordTrie>(m, "KBEWordTrie");
    bind_kbe<KBEStringFromLeft>(m, "KBEStringFromLeft");
    bind_kbe<KBEWordFromLeft>(m, "KBEWordFromLeft");
  }
}  // namespace libsemigroups
