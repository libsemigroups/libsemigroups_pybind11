//
// libsemigroups_pybind11
// Copyright (C) 2023 Murray T. Whyte
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

// C std headers....
#include <stdint.h>  // for uint64_t

// C++ stl headers....
#include <array>             // for array
#include <cstddef>           // for uint32_t
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <string>            // for to_string, basic_string
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/constants.hpp>  // for operator!=, operator==

#include <libsemigroups/detail/int-range.hpp>  // for IntegralRange<>::value_type
#include <libsemigroups/order.hpp>             // for order
#include <libsemigroups/paths.hpp>             // for Paths
#include <libsemigroups/word-graph.hpp>        // for WordGraph, word_graph

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_paths

namespace py = pybind11;

namespace libsemigroups {
  using node_type = uint32_t;
  using size_type = typename WordGraph<node_type>::size_type;
  void init_paths(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Paths
    ////////////////////////////////////////////////////////////////////////

    py::enum_<paths::algorithm>(m, "algorithm")
        .value("dfs", paths::algorithm::dfs, R"pbdoc(Use a
             depth-first-search.)pbdoc")
        .value("matrix",
               paths::algorithm::matrix,
               R"pbdoc(Use the adjacency matrix and matrix
                 multiplication.)pbdoc")
        .value("acyclic",
               paths::algorithm::acyclic,
               R"pbdoc(
                      Use a dynamic programming approach for acyclic digraphs.
                    )pbdoc")
        .value("trivial",
               paths::algorithm::trivial,
               R"pbdoc(Try to utilise some corner cases.)pbdoc")
        .value("automatic",
               paths::algorithm::automatic,
               R"pbdoc(
                      The function :py:meth:`number_of_paths` tries to decide
                      which algorithm is best.
                    )pbdoc");

    py::class_<Paths<node_type>> p(m, "Paths");

    p.def(py::init<>())
        .def(py::init<Paths<node_type> const&>())
        .def(py::init<WordGraph<node_type> const&>())
        .def("__repr__",
             [](Paths<node_type> const& d) {
               std::string result = "<Paths with repr tbd>";
               return result;
             })
        .def("get",
             &Paths<node_type>::get,
             R"pbdoc(
                     TODO
                    )pbdoc")
        .def("next",
             &Paths<node_type>::next,
             R"pbdoc(
                     TODO
                    )pbdoc")

        .def("at_end",
             &Paths<node_type>::at_end,
             R"pbdoc(
                     TODO
                    )pbdoc")

        .def("count",
             &Paths<node_type>::count,
             R"pbdoc(
                     TODO
                    )pbdoc")
        .def("source", [](Paths<node_type> const& p) { return p.from(); })
        .def("source",
             [](Paths<node_type>& p, node_type n) { return p.from(n); })
        .def("target", [](Paths<node_type> const& p) { return p.to(); })
        .def("target", [](Paths<node_type>& p, node_type n) { return p.to(n); })
        .def("min_length", [](Paths<node_type> const& p) { return p.min(); })
        .def("min_length",
             [](Paths<node_type>& p, size_type n) { return p.min(n); })
        .def("max_length", [](Paths<node_type> const& p) { return p.max(); })
        .def("max_length",
             [](Paths<node_type>& p, size_type n) { return p.max(n); })
        .def("order",
             [](Paths<node_type>& p, Order val) { return p.order(val); })
        .def("order", [](Paths<node_type> const& p) { return p.order(); })
        .def("__or__", [](Paths<node_type> const& p, ToStrings const& to_str) {
          using rx::operator|;
          return p | to_str;
        });
  }
}  // namespace libsemigroups
