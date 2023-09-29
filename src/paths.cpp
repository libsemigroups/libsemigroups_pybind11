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
#include <libsemigroups/paths.hpp>             // for Pilo, etc.
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
  void init_pilo(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // Pilo
    ////////////////////////////////////////////////////////////////////////

    //   py::class_<Pilo<node_type>> p(m, "Pilo");

    //   p.def(py::init<>())
    //       .def(py::init<WordGraph<node_type> const&>())
    //       .def("__repr__", [](Pilo<node_type> const& d) {
    //         std::string result = "<Pilo with repr tbd>";
    //         return result;
    //       });
  }

  /*py::enum_<algorithm>(ad, "algorithm")
      .value("dfs", algorithm::dfs, R"pbdoc(Use a
             depth-first-search.)pbdoc")
          .value("matrix",
                 algorithm::matrix,
                 R"pbdoc(Use the adjacency matrix and matrix
                 multiplication.)pbdoc")
          .value("acyclic",
                 algorithm::acyclic,
                 R"pbdoc(
                      Use a dynamic programming approach for acyclic digraphs.
                    )pbdoc")
          .value("trivial",
                 algorithm::trivial,
                 R"pbdoc(Try to utilise some corner cases.)pbdoc")
          .value("automatic",
                 algorithm::automatic,
                 R"pbdoc(
                      The function :py:meth:`number_of_paths` tries to decide
                      which algorithm is best.
                    )pbdoc");

      .def("number_of_paths_algorithm",
           py::overload_cast<node_type const>(
               &WordGraph<node_type>::number_of_paths_algorithm,
               py::const_),
           py::arg("source"),
           R"pbdoc(
                 Returns the algorithm used by :py:meth:`number_of_paths` to
                 compute the number of paths originating at the given source
                 node.

                 :Parameters: - **source** (int) the source node.

                 :Returns: A value of type
                 :py:obj:`WordGraph.algorithm`.
               )pbdoc")
              .def("number_of_paths",
                   py::overload_cast<node_type const>(
                       &WordGraph<node_type>::number_of_paths,
                       py::const_),
                   py::arg("source"),
                   R"pbdoc(
                 Returns the number of paths originating at the given
                 source node.

                 :Parameters: - **source** (int) the source node.

                 :return: An ``int``.
               )pbdoc")
              .def("number_of_paths_algorithm",
                   py::overload_cast<node_type const,
                                     node_type const,
                                     node_type const>(
                       &WordGraph<node_type>::number_of_paths_algorithm,
                       py::const_),
                   py::arg("source"),
                   py::arg("min"),
                   py::arg("max"),
                   R"pbdoc(
                 Returns the algorithm used by number_of_paths to compute
                 the number of paths originating at the given source node
                 with length in the range $[min, max)$.

                 :Parameters: - **source** (int) the source node
                              - **min** (int) the minimum length of paths to
                                count
                              - **max** (int) the maximum length of paths to
                                count
                 :Returns: A value of type
                 :py:obj:`WordGraph.algorithm`.
               )pbdoc")
              .def("number_of_paths_algorithm",
                   [](WordGraph<node_type> const& ad,
                      node_type const             source,
                      node_type const             min,
                      PositiveInfinity const&     max) {
                     return ad.number_of_paths_algorithm(source, min, max);
                   })
              .def("number_of_paths",
                   py::overload_cast<node_type const,
                                     node_type const,
                                     node_type const,
                                     algorithm const>(
                       &WordGraph<node_type>::number_of_paths,
                       py::const_),
                   py::arg("source"),
                   py::arg("min"),
                   py::arg("max"),
                   py::arg("lgrthm"),
                   R"pbdoc(
                 Returns the number of paths starting at a given node with
                 length in a given range.

                 :Parameters: - **source** (int) the source node
                              - **min** (int) the minimum length of paths to
                                count
                              - **max** (int) the maximum length of paths to
                                count
                              - **lgrthm**
                              (:py:obj:`WordGraph.algorithm`)
                                the algorithm to use (defaults to:
                                :py:obj:`WordGraph.algorithm.automatic`).

                 :Returns: An ``int``.
               )pbdoc")
              .def("number_of_paths_algorithm",
                   py::overload_cast<node_type const,
                                     node_type const,
                                     node_type const,
                                     node_type const>(
                       &WordGraph<node_type>::number_of_paths_algorithm,
                       py::const_),
                   py::arg("source"),
                   py::arg("target"),
                   py::arg("min"),
                   py::arg("max"),
                   R"pbdoc(
                 Returns the algorithm used by number_of_paths to compute
                 the number of paths originating at the given source node
                 and ending at the given target node with length in the
                 range :math:`[min, max)`.

                 :Parameters: - **source** (int) the source node
                              - **target** (int) the target node
                              - **min**    (int) the minimum length of paths
                              to count
                              - **max**    (int) the maximum length of paths
                              to count

                 :Returns: A value of type
                 :py:obj:`WordGraph.algorithm`.
               )pbdoc")
              .def("number_of_paths",
                   py::overload_cast<node_type const,
                                     node_type const,
                                     node_type const,
                                     node_type const,
                                     algorithm const>(
                       &WordGraph<node_type>::number_of_paths,
                       py::const_),
                   py::arg("source"),
                   py::arg("target"),
                   py::arg("min"),
                   py::arg("max"),
                   py::arg("lgrthm"),
                   R"pbdoc(
                 Returns the number of paths between a pair of nodes with
                 length in a given range.

                 :Parameters: - **source** (int) the source node
                              - **target** (int) the target node
                              - **min**    (int) the minimum length of paths
                              to count
                              - **max**    (int) the maximum length of paths
                              to count
                              - **lgrthm** (WordGraph.algorithm) the
                                algorithm to use (defaults to:
                                :py:obj:`WordGraph.algorithm.automatic`)

                 :Returns: An ``int``.
               )pbdoc")
              .def(
                  "number_of_paths",
                  [](WordGraph<node_type> const& ad,
                     node_type                   source,
                     node_type                   target,
                     node_type                   min,
                     node_type                   max) {
                    return ad.number_of_paths(source, target, min, max);
                  },
                  py::arg("source"),
                  py::arg("target"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns the number of paths between a pair of nodes with
                length in a given range.

                :Parameters: - **source** (int) the source node
                             - **target** (int) the target node
                             - **min**    (int) the minimum length of paths
                             to count
                             - **max**    (int) the maximum length of paths
                             to count

                :Returns: An ``int``.
              )pbdoc")
              .def(
                  "number_of_paths",
                  [](WordGraph<node_type> const& ad,
                     node_type                   source,
                     node_type                   min,
                     node_type                   max) {
                    return ad.number_of_paths(source, min, max);
                  },
                  py::arg("source"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns the number of paths starting at a node with length
                in a given range.

                :Parameters: - **source** (int) the source node
                             - **min**    (int) the minimum length of paths
                             to count
                             - **max**    (int) the maximum length of paths
                             to count

                :Returns: An ``int``.
              )pbdoc")
              .def("number_of_paths",
                   [](WordGraph<node_type> const& ad,
                      node_type                   source,
                      node_type                   min,
                      PositiveInfinity            max) {
                     return ad.number_of_paths(source, min, max);
                   })
              .def(
                  "panilo_iterator",
                  [](WordGraph<node_type> const& ad,
                     node_type const&            source,
                     node_type const&            mn,
                     node_type const&            mx) {
                    return py::make_iterator(ad.cbegin_panilo(source, mn, mx),
                                             ad.cend_panilo());
                  },
                  py::arg("source"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns an iterator to a pair consisting of the edge labels
                of the first path (in lexicographical order) starting at
                ``source`` with length in the range :math:`[min, max)` and
                the last node of that path.

                :Parameters: - **source** (int) the first node.
                             - **min** (int) the minimum length of a path to
                               enumerate (defaults to ``0``)
                             - **max** (Union[int, PositiveInfinity]) the
                             maximum
                               length of a path to enumerate (defaults to
                               :py:obj:`POSITIVE_INFINITY`).

                :Returns: An iterator.
              )pbdoc")
              .def("panilo_iterator",
                   [](WordGraph<node_type> const& ad,
                      node_type const&            source,
                      node_type const&            mn,
                      PositiveInfinity const&     mx) {
                     return py::make_iterator(ad.cbegin_panilo(source, mn, mx),
                                              ad.cend_panilo());
                   })
              .def(
                  "panislo_iterator",
                  [](WordGraph<node_type> const& ad,
                     node_type const&            source,
                     node_type const&            mn,
                     node_type const&            mx) {
                    return py::make_iterator(ad.cbegin_panislo(source, mn, mx),
                                             ad.cend_panislo());
                  },
                  py::arg("source"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns an iterator to a pair consisting of the edge labels
                of the first path (in short-lex order) starting at source
                with length in the range :math:`[min, max)` and the last
                node of that path.

                :Parameters: - **source** (int) the first node.
                             - **min** (int) the minimum length of a path to
                               enumerate (defaults to ``0``)
                             - **max** (Union[int, PositiveInfinity]) the
                             maximum
                               length of a path to enumerate (defaults to
                               :py:obj:`POSITIVE_INFINITY`).

                :Returns: An iterator.
              )pbdoc")
              .def("panislo_iterator",
                   [](WordGraph<node_type> const& ad,
                      node_type const&            source,
                      node_type const&            mn,
                      PositiveInfinity const&     mx) {
                     return py::make_iterator(ad.cbegin_panislo(source, mn, mx),
                                              ad.cend_panislo());
                   })
              .def(
                  "pilo_iterator",
                  [](WordGraph<node_type> const& ad,
                     node_type const&            source,
                     node_type const&            mn,
                     node_type const&            mx) {
                    return py::make_iterator(ad.cbegin_pilo(source, mn, mx),
                                             ad.cend_pilo());
                  },
                  py::arg("source"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns an iterator to the edge labels of the first path (in
                lexicographical order) starting at source with length in the
                range :math:`[min, max)`.
      .def("pilo_iterator",
           [](WordGraph<node_type> const& ad,
              node_type const&             source,
              node_type const&                mn,
              PositiveInfinity const&      mx) {
             return py::make_iterator(ad.cbegin_pilo(source, mn, mx),
                                      ad.cend_pilo());
           })
      .def(
          "pislo_iterator",
          [](WordGraph<node_type> const& ad,
             node_type const&             source,
             node_type const&                mn,
             node_type const&                mx) {
            return py::make_iterator(ad.cbegin_pislo(source, mn, mx),
                                     ad.cend_pislo());
          },
          py::arg("source"),
          py::arg("min"),
          py::arg("max"),
          R"pbdoc(
                Returns an iterator pointing to the edge labels of the first
                path (in short-lex order) starting at source with length in
                the range :math:`[min, max)`.

                :Parameters: - **source** (int) the first node.
                             - **min** (int) the minimum length of a path to
                               enumerate (defaults to ``0``)
                             - **max** (Union[int, PositiveInfinity]) the
                             maximum
                               length of a path to enumerate (defaults to
                               :py:obj:`POSITIVE_INFINITY`).

                :Returns: An iterator.
              )pbdoc")
              .def("pislo_iterator",
                   [](WordGraph<node_type> const& ad,
                      node_type const&            source,
                      node_type const&            mn,
                      PositiveInfinity const&     mx) {
                     return py::make_iterator(ad.cbegin_pislo(source, mn, mx),
                                              ad.cend_pislo());
                   })
              .def(
                  "pstislo_iterator",
                  [](WordGraph<node_type> const& ad,
                     node_type const&            source,
                     node_type const&            target,
                     node_type const&            mn,
                     node_type const&            mx) {
                    return py::make_iterator(
                        ad.cbegin_pstislo(source, target, mn, mx),
                        ad.cend_pstislo());
                  },
                  py::arg("source"),
                  py::arg("target"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns an iterator to the edge labels of the
                first path (in short-lex order) starting at the node
                ``source`` and ending at the node ``target`` with length in
                the range :math:`[min, max)`.

                :Parameters: - **source** (int) the first node.
                             - **min** (int) the minimum length of a path to
                               enumerate (defaults to ``0``)
                             - **max** (Union[int, PositiveInfinity]) the
                             maximum
                               length of a path to enumerate (defaults to
                               :py:obj:`POSITIVE_INFINITY`).

                :Returns: An iterator.
              )pbdoc")
              .def("pstislo_iterator",
                   [](WordGraph<node_type> const& ad,
                      node_type const&            source,
                      node_type const&            target,
                      node_type const&            mn,
                      PositiveInfinity const&     mx) {
                     return py::make_iterator(
                         ad.cbegin_pstislo(source, target, mn, mx),
                         ad.cend_pstislo());
                   })
              .def(
                  "pstilo_iterator",
                  [](WordGraph<node_type> const& ad,
                     node_type const&            source,
                     node_type const&            target,
                     node_type const&            mn,
                     node_type const&            mx) {
                    return py::make_iterator(
                        ad.cbegin_pstilo(source, target, mn, mx),
                        ad.cend_pstilo());
                  },
                  py::arg("source"),
                  py::arg("target"),
                  py::arg("min"),
                  py::arg("max"),
                  R"pbdoc(
                Returns an iterator to the edge labels of the first path (in
                lex order) starting at the node ``source`` and ending at the
                node
                ``target`` with length in the range :math:`[min, max)`.

                :Parameters: - **source** (int) the first node.
                             - **target** (int) the last node.
                             - **min** (int) the minimum length of a path to
                               enumerate (defaults to ``0``)
                             - **max** (Union[int, PositiveInfinity]) the
                             maximum
                               length of a path to enumerate (defaults to
                               :py:obj:`POSITIVE_INFINITY`).

                :Returns: An iterator.
              )pbdoc")
              .def("pstilo_iterator",
                   [](WordGraph<node_type> const& ad,
                      node_type const&            source,
                      node_type const&            target,
                      node_type const&            mn,
                      PositiveInfinity const&     mx) {
                     return py::make_iterator(
                         ad.cbegin_pstilo(source, target, mn, mx),
                         ad.cend_pstilo());
                   })
  */
}  // namespace libsemigroups
