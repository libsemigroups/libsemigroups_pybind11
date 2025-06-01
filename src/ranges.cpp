//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2024 Joseph Edwards
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

// C++ stl headers....
#include <vector>  // for vector

// libsemigroups....

#include <libsemigroups/paths.hpp>       // for Paths
#include <libsemigroups/word-range.hpp>  // for ToWord, ToString, WordRange...

// pybind11....
#include <pybind11/pybind11.h>  // for make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_words

namespace libsemigroups {
  namespace py = pybind11;

  template <typename T, typename Converter>
  void bind_converter(py::module& m, std::string const& name) {
    py::class_<T> thing(m, name.c_str());
    thing.def(py::init<decltype(T::_input) const&, Converter const&>());
    thing.def("get", &T::get);
    thing.def("__iter__", [](T const& x) {
      return py::make_iterator(rx::begin(x), rx::end(x));
    });
    thing.def("next", &T::next);
    thing.def("at_end", &T::at_end);
    thing.def("__len__", &T::size_hint);
  }

  void init_ranges(py::module& m) {
    using node_type = uint32_t;
    bind_converter<ToString::Range<Paths<node_type>>, ToString>(
        m, "ToStringPathsRange");
    bind_converter<ToString::Range<WordRange>, ToString>(m,
                                                         "ToStringWordRange");
    bind_converter<ToWord::Range<StringRange>, ToWord>(m, "ToWordStringRange");
  }
}  // namespace libsemigroups
