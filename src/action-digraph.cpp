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
#include <cstddef>           // for size_t
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <string>            // for to_string, basic_string
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/constants.hpp>       // for operator!=, operator==
#include <libsemigroups/digraph-helper.hpp>  // for topological_sort,...
#include <libsemigroups/digraph.hpp>         // for ActionDigraph
#include <libsemigroups/int-range.hpp>       // for IntegralRange<>::value_type

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_action_digraph

namespace py = pybind11;

namespace libsemigroups {
  namespace libsemigroups_pybind11 {
    // TODO Avoid duplicate code the following is a copy of
    // libsemigroups::action_digraph_helper::make, which can't be used because
    // it has initializer_list's as 2nd arg.
    ActionDigraph<size_t> make(size_t                           num_nodes,
                               std::vector<std::vector<size_t>> il) {
      ActionDigraph<size_t> result(num_nodes, il.begin()->size());
      for (size_t i = 0; i < il.size(); ++i) {
        for (size_t j = 0; j < (il.begin() + i)->size(); ++j) {
          auto val = *((il.begin() + i)->begin() + j);
          if (val != UNDEFINED) {
            result.add_edge(i, *((il.begin() + i)->begin() + j), j);
          }
        }
      }
      return result;
    }
  }  // namespace libsemigroups_pybind11

  using node_type = ActionDigraph<size_t>::node_type;
  using algorithm = ActionDigraph<size_t>::algorithm;
  void init_action_digraph(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // ActionDigraph
    ////////////////////////////////////////////////////////////////////////

    py::class_<ActionDigraph<size_t>> ad(m, "ActionDigraph");

    py::enum_<algorithm>(ad, "algorithm")
        .value("dfs", algorithm::dfs, R"pbdoc(Use a depth-first-search.)pbdoc")
        .value(
            "matrix",
            algorithm::matrix,
            R"pbdoc(Use the adjacency matrix and matrix multiplication.)pbdoc")
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

    ad.def(py::init<>())
        .def(py::init<size_t>())
        .def(py::init<size_t, size_t>(),
             py::arg("m"),
             py::arg("n"),
             R"pbdoc(
               Construct from number of nodes and out degree.

               :Parameters: - **m** (int) the number of nodes in the digraph
                              (default: ``0``).
                            - **n** (int) the out-degree of every node (default:
                              ``0``).
             )pbdoc")
        .def(py::init<ActionDigraph<size_t> const&>(),
             R"pbdoc(
               Construct a copy.
             )pbdoc")
        .def("__repr__",
             [](ActionDigraph<size_t> const& d) {
               std::string result = "<action digraph with ";
               result += std::to_string(d.number_of_nodes());
               result += " nodes, ";
               result += std::to_string(d.number_of_edges());
               result += " edges, ";
               result += std::to_string(d.out_degree());
               result += " out-degree>";
               return result;
             })
        .def("__str__", &detail::to_string<ActionDigraph<size_t>>)
        .def(pybind11::self == pybind11::self)
        .def("number_of_nodes",
             &ActionDigraph<size_t>::number_of_nodes,
             R"pbdoc(
               Returns the number of nodes of this.

               :Parameters: None
               :return: An ``int``.
             )pbdoc")
        .def("number_of_edges",
             py::overload_cast<>(&ActionDigraph<size_t>::number_of_edges,
                                 py::const_),
             R"pbdoc(
               Returns the total number of edges.

               :Parameters: None
               :Returns: An ``int``.
             )pbdoc")
        .def("number_of_edges",
             py::overload_cast<node_type const>(
                 &ActionDigraph<size_t>::number_of_edges, py::const_),
             py::arg("n"),
             R"pbdoc(
               Returns the number of edges incident to a node.

               :Parameters: - **n** the node.
               :Returns: An ``int``.
             )pbdoc")
        .def("out_degree",
             &ActionDigraph<size_t>::out_degree,
             R"pbdoc(
               Returns the maximum out-degree of any node.

               :Parameters: None
               :return: An ``int``.
             )pbdoc")
        .def("validate",
             &ActionDigraph<size_t>::validate,
             R"pbdoc(
               Check every node has exactly :py:meth:`out_degree` out-edges.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc")
        .def("add_edge",
             &ActionDigraph<size_t>::add_edge,
             py::arg("i"),
             py::arg("j"),
             py::arg("lbl"),
             R"pbdoc(
               Add an edge from ``i`` to ``j`` labelled ``lbl``.

               :param i: the source node
               :type i: int
               :param j: the target node
               :type j: int
               :param lbl: the label of the edge from ``i`` to ``j``
               :type lbl: int

               :return: (None)
             )pbdoc")
        .def("add_nodes",
             &ActionDigraph<size_t>::add_nodes,
             py::arg("nr"),
             R"pbdoc(
               Adds ``nr`` nodes to this.

               :param nr: the number of nodes to add.
               :type nr: int

               :return: (None)
             )pbdoc")
        .def("add_to_out_degree",
             &ActionDigraph<size_t>::add_to_out_degree,
             py::arg("nr"),
             R"pbdoc(
               Adds ``nr`` to the out-degree of this.

               :param nr: the number of new out-edges for every node.
               :type nr: int

               :return: (None)
             )pbdoc")
        .def("neighbor",
             &ActionDigraph<size_t>::neighbor,
             py::arg("v"),
             py::arg("lbl"),
             R"pbdoc(
               Get the range of the edge with source node ``v`` and edge-label
               ``lbl``.

               :param v: the node
               :type v: int
               :param lbl: the label
               :type lbl: int

               :return:
                 An ``int`` or :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("reserve",
             &ActionDigraph<size_t>::reserve,
             py::arg("m"),
             py::arg("n"),
             R"pbdoc(
               Ensures that this has capacity for m nodes each with n
               out-edges, but does not modify :py:meth:`number_of_nodes` or
               :py:meth:`out_degree`.

               :param m: the number of nodes
               :type m: int
               :param n: the out-degree
               :type n: int

               :return: (None)
             )pbdoc")
        .def("unsafe_neighbor",
             &ActionDigraph<size_t>::unsafe_neighbor,
             py::arg("v"),
             py::arg("lbl"),
             R"pbdoc(
               Get the range of the edge with source node ``v`` and edge-label
               ``lbl``.

               :param v: the node
               :type v: int
               :param lbl: the label
               :type lbl: int

               :return: An ``int`` or :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("next_neighbor",
             &ActionDigraph<size_t>::next_neighbor,
             py::arg("v"),
             py::arg("i"),
             R"pbdoc(
               Get the next neighbor of a node that doesn't equal
               :py:obj:`UNDEFINED`.

               :param v: the node
               :type v: int
               :param i: the label
               :type i: int

               :return: A tuple.

               Specifically, a tuple ``x`` is returned where

                 * ``x[0]`` is adjacent to ``v`` via an edge labelled ``x[1]``;
                 * ``x[1]`` is the minimum value in the range
                   :math:`[i, N)` where ``N`` is :py:meth:`degree` such that
                   ``neighbor(v, x[1])`` is not equal to :py:obj:`UNDEFINED`.

               If ``neighbor(v, i)`` is undefined for every value of ``i``,
               then ``x[0]`` and ``x[1]`` equal :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("unsafe_next_neighbor",
             &ActionDigraph<size_t>::unsafe_next_neighbor,
             py::arg("v"),
             py::arg("i"),
             R"pbdoc(
               Get the next neighbor of a node that doesn't equal
               :py:obj:`UNDEFINED`.

               :param v: the node
               :type v: int
               :param i: the label
               :type i: int

               :return: A tuple.

               Specifically, a tuple ``x`` is returned where

                 * ``x[0]`` is adjacent to ``v`` via an edge labelled ``x[1]``;
                 * ``x[1]`` is the minimum value in the range
                   :math:`[i, N)` where ``N`` is :py:meth:`degree` such that
                   ``neighbor(v, x[1])`` is not equal to :py:obj:`UNDEFINED`.

               If ``neighbor(v, i)`` is undefined for every value of ``i``,
               then ``x[0]`` and ``x[1]`` equal :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("scc_id",
             &ActionDigraph<size_t>::scc_id,
             py::arg("nd"),
             R"pbdoc(
               Returns the id-number of the strongly connected component of a
               node.

               :param nd: the node.
               :type nd: int

               :Parameters: None
               :return: An ``int``.
             )pbdoc")
        .def("number_of_scc",
             &ActionDigraph<size_t>::number_of_scc,
             R"pbdoc(
               Returns the number of strongly connected components.

               :Parameters: None
               :return: An ``int``.
             )pbdoc")
        .def("root_of_scc",
             &ActionDigraph<size_t>::root_of_scc,
             py::arg("nd"),
             R"pbdoc(
               Returns the root of a strongly connected components containing a
               given node.

               :param nd: a node.
               :type nd: int

               :return: An ``int``.
             )pbdoc")
        .def("spanning_forest",
             &ActionDigraph<size_t>::spanning_forest,
             py::return_value_policy::copy,  // to ensure the Forest lives!
             R"pbdoc(
               Returns a :py:class:`Forest` comprised of spanning trees for
               each scc of this, rooted at the minimum node of that component,
               with edges oriented away from the root.

               :return: A :py:class:`Forest`.
             )pbdoc")
        .def("reverse_spanning_forest",
             &ActionDigraph<size_t>::reverse_spanning_forest,
             R"pbdoc(
               Returns a :py:class:`Forest` comprised of spanning trees for
               each scc of this, rooted at the minimum node of that component,
               with edges oriented towards the root.

               :return: A :py:class:`Forest`.
             )pbdoc")
        .def("number_of_paths_algorithm",
             py::overload_cast<node_type const>(
                 &ActionDigraph<size_t>::number_of_paths_algorithm, py::const_),
             py::arg("source"),
             R"pbdoc(
               Returns the algorithm used by :py:meth:`number_of_paths` to
               compute the number of paths originating at the given source
               node.

               :Parameters: - **source** (int) the source node.

               :Returns: A value of type :py:obj:`ActionDigraph.algorithm`.
             )pbdoc")
        .def("number_of_paths",
             py::overload_cast<node_type const>(
                 &ActionDigraph<size_t>::number_of_paths, py::const_),
             py::arg("source"),
             R"pbdoc(
               Returns the number of paths originating at the given
               source node.

               :Parameters: - **source** (int) the source node.

               :return: An ``int``.
             )pbdoc")
        .def("number_of_paths_algorithm",
             py::overload_cast<node_type const, size_t const, size_t const>(
                 &ActionDigraph<size_t>::number_of_paths_algorithm, py::const_),
             py::arg("source"),
             py::arg("min"),
             py::arg("max"),
             R"pbdoc(
               Returns the algorithm used by number_of_paths to compute the
               number of paths originating at the given source node with length
               in the range $[min, max)$.

               :Parameters: - **source** (int) the source node
                            - **min** (int) the minimum length of paths to
                              count
                            - **max** (int) the maximum length of paths to
                              count
               :Returns: A value of type :py:obj:`ActionDigraph.algorithm`.
             )pbdoc")
        .def("number_of_paths_algorithm",
             [](ActionDigraph<size_t> const& ad,
                node_type const              source,
                size_t const                 min,
                PositiveInfinity const&      max) {
               return ad.number_of_paths_algorithm(source, min, max);
             })
        .def("number_of_paths",
             py::overload_cast<node_type const,
                               size_t const,
                               size_t const,
                               algorithm const>(
                 &ActionDigraph<size_t>::number_of_paths, py::const_),
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
                            - **lgrthm** (:py:obj:`ActionDigraph.algorithm`)
                              the algorithm to use (defaults to:
                              :py:obj:`ActionDigraph.algorithm.automatic`).

               :Returns: An ``int``.
             )pbdoc")
        .def("number_of_paths_algorithm",
             py::overload_cast<node_type const,
                               node_type const,
                               size_t const,
                               size_t const>(
                 &ActionDigraph<size_t>::number_of_paths_algorithm, py::const_),
             py::arg("source"),
             py::arg("target"),
             py::arg("min"),
             py::arg("max"),
             R"pbdoc(
               Returns the algorithm used by number_of_paths to compute the
               number of paths originating at the given source node and ending
               at the given target node with length in the range :math:`[min,
               max)`.

               :Parameters: - **source** (int) the source node
                            - **target** (int) the target node
                            - **min**    (int) the minimum length of paths to count
                            - **max**    (int) the maximum length of paths to count

               :Returns: A value of type :py:obj:`ActionDigraph.algorithm`.
             )pbdoc")
        .def("number_of_paths",
             py::overload_cast<node_type const,
                               node_type const,
                               size_t const,
                               size_t const,
                               algorithm const>(
                 &ActionDigraph<size_t>::number_of_paths, py::const_),
             py::arg("source"),
             py::arg("target"),
             py::arg("min"),
             py::arg("max"),
             py::arg("lgrthm"),
             R"pbdoc(
               Returns the number of paths between a pair of nodes with length
               in a given range.

               :Parameters: - **source** (int) the source node
                            - **target** (int) the target node
                            - **min**    (int) the minimum length of paths to count
                            - **max**    (int) the maximum length of paths to count
                            - **lgrthm** (ActionDigraph.algorithm) the
                              algorithm to use (defaults to:
                              :py:obj:`ActionDigraph.algorithm.automatic`)

               :Returns: An ``int``.
             )pbdoc")
        .def(
            "number_of_paths",
            [](ActionDigraph<size_t> const& ad,
               node_type                    source,
               node_type                    target,
               size_t                       min,
               size_t                       max) {
              return ad.number_of_paths(source, target, min, max);
            },
            py::arg("source"),
            py::arg("target"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns the number of paths between a pair of nodes with length
              in a given range.

              :Parameters: - **source** (int) the source node
                           - **target** (int) the target node
                           - **min**    (int) the minimum length of paths to count
                           - **max**    (int) the maximum length of paths to count

              :Returns: An ``int``.
            )pbdoc")
        .def(
            "number_of_paths",
            [](ActionDigraph<size_t> const& ad,
               node_type                    source,
               size_t                       min,
               size_t max) { return ad.number_of_paths(source, min, max); },
            py::arg("source"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns the number of paths starting at a node with length
              in a given range.

              :Parameters: - **source** (int) the source node
                           - **min**    (int) the minimum length of paths to count
                           - **max**    (int) the maximum length of paths to count

              :Returns: An ``int``.
            )pbdoc")
        .def("number_of_paths",
             [](ActionDigraph<size_t> const& ad,
                node_type                    source,
                size_t                       min,
                PositiveInfinity             max) {
               return ad.number_of_paths(source, min, max);
             })
        .def(
            "nodes_iterator",
            [](ActionDigraph<size_t> const& ad) {
              return py::make_iterator(ad.cbegin_nodes(), ad.cend_nodes());
            },
            R"pbdoc(
              Returns an iterator to the nodes of the digraph.
            )pbdoc")
        .def(
            "reverse_nodes_iterator",
            [](ActionDigraph<size_t> const& ad) {
              return py::make_iterator(ad.crbegin_nodes(), ad.crend_nodes());
            },
            R"pbdoc(
                Returns a reversed iterator to the nodes of the digraph.
            )pbdoc")
        .def(
            "edges_iterator",
            [](ActionDigraph<size_t> const& ad, size_t const i) {
              return py::make_iterator(ad.cbegin_edges(i), ad.cend_edges(i));
            },
            py::arg("i"),
            R"pbdoc(
              Returns an iterator to the edges of a node in the digraph.

              :param i: the node.
              :type i: int

              :return: An iterator.
            )pbdoc")
        .def(
            "sccs_iterator",
            [](ActionDigraph<size_t> const& ad) {
              return py::make_iterator(ad.cbegin_sccs(), ad.cend_sccs());
            },
            R"pbdoc(
              Returns an iterator for the nodes in the scc.
            )pbdoc")
        .def(
            "scc_iterator",
            [](ActionDigraph<size_t> const& ad, size_t const i) {
              return py::make_iterator(ad.cbegin_scc(i), ad.cend_scc(i));
            },
            R"pbdoc(
              Returns an iterator pointing to the first node in the scc with the
              specified id-number.
            )pbdoc")
        .def(
            "scc_roots_iterator",
            [](ActionDigraph<size_t> const& ad) {
              return py::make_iterator(ad.cbegin_scc_roots(),
                                       ad.cend_scc_roots());
            },
            R"pbdoc(
              Returns an iterator pointing to the root of the first scc.
            )pbdoc")
        .def(
            "panilo_iterator",
            [](ActionDigraph<size_t> const& ad,
               node_type const&             source,
               size_t const&                mn,
               size_t const&                mx) {
              return py::make_iterator(ad.cbegin_panilo(source, mn, mx),
                                       ad.cend_panilo());
            },
            py::arg("source"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns an iterator to a pair consisting of the edge labels of
              the first path (in lexicographical order) starting at ``source``
              with length in the range :math:`[min, max)` and the last node of
              that path.

              :Parameters: - **source** (int) the first node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
        .def("panilo_iterator",
             [](ActionDigraph<size_t> const& ad,
                node_type const&             source,
                size_t const&                mn,
                PositiveInfinity const&      mx) {
               return py::make_iterator(ad.cbegin_panilo(source, mn, mx),
                                        ad.cend_panilo());
             })
        .def(
            "panislo_iterator",
            [](ActionDigraph<size_t> const& ad,
               node_type const&             source,
               size_t const&                mn,
               size_t const&                mx) {
              return py::make_iterator(ad.cbegin_panislo(source, mn, mx),
                                       ad.cend_panislo());
            },
            py::arg("source"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns an iterator to a pair consisting of the edge labels of
              the first path (in short-lex order) starting at source with
              length in the range :math:`[min, max)` and the last node of that
              path.

              :Parameters: - **source** (int) the first node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
        .def("panislo_iterator",
             [](ActionDigraph<size_t> const& ad,
                node_type const&             source,
                size_t const&                mn,
                PositiveInfinity const&      mx) {
               return py::make_iterator(ad.cbegin_panislo(source, mn, mx),
                                        ad.cend_panislo());
             })
        .def(
            "pilo_iterator",
            [](ActionDigraph<size_t> const& ad,
               node_type const&             source,
               size_t const&                mn,
               size_t const&                mx) {
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

              :Parameters: - **source** (int) the first node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
        .def("pilo_iterator",
             [](ActionDigraph<size_t> const& ad,
                node_type const&             source,
                size_t const&                mn,
                PositiveInfinity const&      mx) {
               return py::make_iterator(ad.cbegin_pilo(source, mn, mx),
                                        ad.cend_pilo());
             })
        .def(
            "pislo_iterator",
            [](ActionDigraph<size_t> const& ad,
               node_type const&             source,
               size_t const&                mn,
               size_t const&                mx) {
              return py::make_iterator(ad.cbegin_pislo(source, mn, mx),
                                       ad.cend_pislo());
            },
            py::arg("source"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns an iterator pointing to the edge labels of the first path
              (in short-lex order) starting at source with length in the range
              :math:`[min, max)`.

              :Parameters: - **source** (int) the first node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
        .def("pislo_iterator",
             [](ActionDigraph<size_t> const& ad,
                node_type const&             source,
                size_t const&                mn,
                PositiveInfinity const&      mx) {
               return py::make_iterator(ad.cbegin_pislo(source, mn, mx),
                                        ad.cend_pislo());
             })
        .def(
            "pstislo_iterator",
            [](ActionDigraph<size_t> const& ad,
               node_type const&             source,
               node_type const&             target,
               size_t const&                mn,
               size_t const&                mx) {
              return py::make_iterator(
                  ad.cbegin_pstislo(source, target, mn, mx), ad.cend_pstislo());
            },
            py::arg("source"),
            py::arg("target"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns an iterator to the edge labels of the
              first path (in short-lex order) starting at the node ``source`` and
              ending at the node ``target`` with length in the range
              :math:`[min, max)`.

              :Parameters: - **source** (int) the first node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
        .def("pstislo_iterator",
             [](ActionDigraph<size_t> const& ad,
                node_type const&             source,
                node_type const&             target,
                size_t const&                mn,
                PositiveInfinity const&      mx) {
               return py::make_iterator(
                   ad.cbegin_pstislo(source, target, mn, mx),
                   ad.cend_pstislo());
             })
        .def(
            "pstilo_iterator",
            [](ActionDigraph<size_t> const& ad,
               node_type const&             source,
               node_type const&             target,
               size_t const&                mn,
               size_t const&                mx) {
              return py::make_iterator(ad.cbegin_pstilo(source, target, mn, mx),
                                       ad.cend_pstilo());
            },
            py::arg("source"),
            py::arg("target"),
            py::arg("min"),
            py::arg("max"),
            R"pbdoc(
              Returns an iterator to the edge labels of the first path (in lex
              order) starting at the node ``source`` and ending at the node
              ``target`` with length in the range :math:`[min, max)`.

              :Parameters: - **source** (int) the first node.
                           - **target** (int) the last node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
        .def("pstilo_iterator",
             [](ActionDigraph<size_t> const& ad,
                node_type const&             source,
                node_type const&             target,
                size_t const&                mn,
                PositiveInfinity const&      mx) {
               return py::make_iterator(
                   ad.cbegin_pstilo(source, target, mn, mx), ad.cend_pstilo());
             })
        .def_static(
            "random",
            [](size_t nr_nodes, size_t out_degree) {
              return ActionDigraph<size_t>::random(nr_nodes, out_degree);
            },
            py::arg("nr_nodes"),
            py::arg("out_degree"),
            R"pbdoc(
              Constructs a random :py:class:`ActionDigraph` with the specified
              number of nodes and out-degree.

              :param nr_nodes:  the number of nodes
              :type nr_nodes: int
              :param out_degree: the maximum out-degree of every node
              :type out_degree: int
              :returns: An ``ActionDigraph``.
            )pbdoc")
        .def_static(
            "random_acyclic",
            [](size_t nr_nodes, size_t out_degree, size_t nr_edges) {
              return ActionDigraph<size_t>::random_acyclic(
                  nr_nodes, out_degree, nr_edges);
            },
            py::arg("nr_nodes"),
            py::arg("out_degree"),
            py::arg("nr_edges"),
            R"pbdoc(
              Constructs a random acyclic :py:class:`ActionDigraph` with the
              specified number of nodes and edges, and out-degree.

              :param nr_nodes: the number of nodes
              :type nr_nodes: int
              :param out_degree: the out-degree of every node
              :type out_degree: int
              :param nr_edges: the out-degree of every node
              :type nr_edges: int
              :returns: An ``ActionDigraph``.
            )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // action_digraph_helper
    ////////////////////////////////////////////////////////////////////////

    // TODO(later) there are several more functions in action_digraph_helper
    // that can be included here!

    m.def("add_cycle",
          py::overload_cast<ActionDigraph<size_t>&, size_t>(
              &action_digraph_helper::add_cycle<size_t>),
          py::arg("ad"),
          py::arg("N"),
          R"pbdoc(
            Adds a cycle consisting of ``N`` new nodes.

            :param ad: the action digraph.
            :type ad: ActionDigraph
            :param N: the length of the cycle.
            :type N: int

            :return: None.
          )pbdoc");
    m.def("is_acyclic",
          py::overload_cast<ActionDigraph<size_t> const&>(
              &action_digraph_helper::is_acyclic<size_t>),
          py::arg("ad"),
          R"pbdoc(
            Check if a digraph is acyclic.

            A digraph is acyclic if every directed cycle is trivial.

            :param ad: the digraph.
            :type ad: ActionDigraph

            :return: A ``bool``.
          )pbdoc");
    m.def("topological_sort",
          py::overload_cast<ActionDigraph<size_t> const&>(
              &action_digraph_helper::topological_sort<size_t>),
          py::arg("ad"),
          R"pbdoc(
            Returns the nodes of the digraph in topological order (see below)
            if possible.

            If it is not empty, the returned list has the property that if an
            edge from a node ``n`` points to a node ``m``, then ``m`` occurs
            before ``n`` in the list.

            :Parameters: - **ad** (ActionDigraph) the digraph.
            :Returns: A list of ``int``.
          )pbdoc");
    m.def("topological_sort",
          py::overload_cast<ActionDigraph<size_t> const&, size_t>(
              &action_digraph_helper::topological_sort<size_t>),
          py::arg("ad"),
          py::arg("source"),
          R"pbdoc(
            Returns the nodes of the digraph reachable from a given node in
            topological order (see below) if possible.

            If it is not empty, the returned list has the property that if an
            edge from a node ``n`` points to a node ``m``, then ``m`` occurs
            before ``n`` in the list, and the last item in the list is source.

            :Parameters: - **ad** (ActionDigraph) the digraph.
                         - **source** (int) the source node.
            :Returns: A list of ``int``.
          )pbdoc");
    m.def("follow_path",
          &action_digraph_helper::follow_path<size_t>,
          py::arg("ad"),
          py::arg("source"),
          py::arg("path"),
          R"pbdoc(
            Find the node that a path starting at a given node leads to.

            :param ad: the ``ActionDigraph`` object to check.
            :type ad: ActionDigraph
            :param first: the starting node.
            :type first: int
            :param path: the path to follow.
            :type path: List[int]

            :return:
              An ``int`` corresponding to the node at the end of the path or
              :py:class:`UNDEFINED` otherwise.

            :raises RuntimeError:
              if ``source`` is not a node in the digraph or ``path`` contains a
              value that is not an edge-label.
          )pbdoc");
    m.def("make",
          // see docs/source/api/digraph-helper.rst for the doc, it's there
          // because otherwise we are documenting the overloaded function
          // `make`, and the doc here is not picked up.
          &libsemigroups_pybind11::make,
          py::arg("num_nodes"),
          py::arg("l"));
  }
}  // namespace libsemigroups
