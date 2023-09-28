//
// libsemigroups_pybind11
// Copyright (C) 2021 James D. Mitchell
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
#include <libsemigroups/gabow.hpp>             // for Gabow
#include <libsemigroups/word-graph.hpp>        // for WordGraph, word_graph

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_word_graph

namespace py = pybind11;

namespace libsemigroups {
  using node_type = uint32_t;
  void init_gabow(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Gabow
    ////////////////////////////////////////////////////////////////////////

    py::class_<Gabow<node_type>> g(m, "Gabow");

    g.def(py::init<>())
        .def(py::init<WordGraph<node_type> const&>())
        .def("__repr__",
             [](Gabow<node_type> const& d) {
               std::string result = "<Gabow with repr tbd>";
               return result;
             })
        .def("id",
             &Gabow<node_type>::id,
             py::arg("nd"),
             R"pbdoc(
              Returns the id-number of the strongly connected component
              of a node.

              :param nd: the node.
              :type nd: int

              :Parameters: None
              :return: An ``int``.
            )pbdoc")
        .def("number_of_components",
             &Gabow<node_type>::number_of_components,
             R"pbdoc(
              Returns the number of strongly connected components.

              :Parameters: None
              :return: An ``int``.
            )pbdoc")
        .def("root_of",
             &Gabow<node_type>::root_of,
             py::arg("nd"),
             R"pbdoc(
              Returns the root of a strongly connected components
              containing a given node.

              :param nd: a node.
              :type nd: int

              :return: An ``int``.
            )pbdoc");
    // TODO: Investigate what this should be
    //
    //  .def("spanning_forest",
    //       &Gabow<node_type>::spanning_forest,
    //       py::return_value_policy::copy,  // to ensure the Forest lives
    //       R"pbdoc(
    //         Returns a :py:class:`Forest` comprised of spanning trees
    //         for each scc of this, rooted at the minimum node of that
    //         component, with edges oriented away from the root.

    //         :return: A :py:class:`Forest`.
    //       )pbdoc");
    // .def("reverse_spanning_forest",
    //      &ActionDigraph<node_type>::reverse_spanning_forest,
    //      R"pbdoc(
    //        Returns a :py:class:`Forest` comprised of spanning trees
    //        for each scc of this, rooted at the minimum node of that
    //        component, with edges oriented towards the root.

    //        :return: A :py:class:`Forest`.
    //      )pbdoc")
    //
    //      TODO: I'm not sure how these iterators correspond to the
    //      refactor version iterators
    //      .def(
    //          "sccs_iterator",
    //          [](WordGraph<node_type> const& wg) {
    //            return py::make_iterator(wg.cbegin_sccs(),
    //            wg.cend_sccs());
    //          },
    //          R"pbdoc(
    //               Returns an iterator for the nodes in the scc.
    //             )pbdoc");

    //      .def(
    //          "scc_iterator",
    //          [](WordGraph<node_type> const& ad, node_type const i) {
    //    return py::make_iterator(ad.cbegin_scc(i), ad.cend_scc(i));
    //          },
    //          R"pbdoc(
    //                Returns an iterator pointing to the first node in the
    //                scc with the specified id-number.
    //              )pbdoc")
    //      .def(
    //          "scc_roots_iterator",
    //          [](WordGraph<node_type> const& ad) {
    //    return py::make_iterator(ad.cbegin_scc_roots(),
    //    ad.cend_scc_roots());
    //          },
    //          R"pbdoc(
    //                Returns an iterator pointing to the root of the first
    //                scc.
    //              )pbdoc")
    //             .def("scc_id",
    //           &ActionDigraph<node_type>::scc_id,
    //           py::arg("nd"),
    //           R"pbdoc(
    //                 Returns the id-number of the strongly connected
    //                 component of a node.

    //                 :param nd: the node.
    //                 :type nd: int

    //                 :Parameters: None
    //                 :return: An ``int``.
    //               )pbdoc")
  }
}  // namespace libsemigroups
