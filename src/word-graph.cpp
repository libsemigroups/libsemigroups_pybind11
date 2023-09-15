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
#include <libsemigroups/word-graph.hpp>

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for class_, make_iterator, init, enum_
#include <pybind11/stl.h>        // for conversion of C++ to py types

// libsemigroups_pybind11....
#include "main.hpp"  // for init_word_graph

namespace py = pybind11;

namespace libsemigroups {
  using node_type = uint32_t;
  void init_word_digraph(py::module& m) {
    ////////////////////////////////////////////////////////////////////////
    // WordGraph
    ////////////////////////////////////////////////////////////////////////

    py::class_<WordGraph<node_type>> wg(m, "WordGraph");

    wg.def(py::init<>())
        .def(py::init<node_type>())
        .def(py::init<node_type, node_type>(),
             py::arg("m"),
             py::arg("n"),
             R"pbdoc(
                   Construct from number of nodes and out degree.

                   :Parameters: - **m** (int) the number of nodes in the digraph
                                  (default: ``0``).
                                - **n** (int) the out-degree of every node
                                  (default: ``0``).
                 )pbdoc")
        .def(py::init<WordGraph<node_type> const&>(),
             R"pbdoc(
                   Construct a copy.
                 )pbdoc")
        .def("__repr__",
             [](WordGraph<node_type> const& d) {
               std::string result = "<action digraph with ";
               result += std::to_string(d.number_of_nodes());
               result += " nodes, ";
               result += std::to_string(d.number_of_edges());
               result += " edges, ";
               result += std::to_string(d.out_degree());
               result += " out-degree>";
               return result;
             })
        .def("__str__", &detail::to_string<WordGraph<node_type>>)
        .def(pybind11::self == pybind11::self)
        .def("number_of_nodes",
             &WordGraph<node_type>::number_of_nodes,
             R"pbdoc(
                   Returns the number of nodes of this.

                   :Parameters: None
                   :return: An ``int``.
                 )pbdoc")
        .def("number_of_edges",
             py::overload_cast<>(&WordGraph<node_type>::number_of_edges,
                                 py::const_),
             R"pbdoc(
                   Returns the total number of edges.

                   :Parameters: None
                   :Returns: An ``int``.
                 )pbdoc")
        .def("number_of_edges",
             py::overload_cast<node_type const>(
                 &WordGraph<node_type>::number_of_edges, py::const_),
             py::arg("n"),
             R"pbdoc(
                   Returns the number of edges incident to a node.

                   :Parameters: - **n** the node.
                   :Returns: An ``int``.
                 )pbdoc")
        .def("out_degree",
             &WordGraph<node_type>::out_degree,
             R"pbdoc(
                   Returns the maximum out-degree of any node.

                   :Parameters: None
                   :return: An ``int``.
                 )pbdoc")
        .def("set_target",
             &WordGraph<node_type>::set_target,
             py::arg("i"),
             py::arg("lbl"),
             py::arg("j"),
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
             &WordGraph<node_type>::add_nodes,
             py::arg("nr"),
             R"pbdoc(
               Adds ``nr`` nodes to this.

               :param nr: the number of nodes to add.
               :type nr: int

               :return: (None)
             )pbdoc")
        .def("add_to_out_degree",
             &WordGraph<node_type>::add_to_out_degree,
             py::arg("nr"),
             R"pbdoc(
               Adds ``nr`` to the out-degree of this.

               :param nr: the number of new out-edges for every node.
               :type nr: int

               :return: (None)
             )pbdoc")
        .def("target",
             &WordGraph<node_type>::target,
             py::arg("v"),
             py::arg("lbl"),
             R"pbdoc(
               Get the range of the edge with source node ``v`` and
               edge-label
               ``lbl``.

               :param v: the node
               :type v: int
               :param lbl: the label
               :type lbl: int

               :return:
                 An ``int`` or :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("reserve",
             &WordGraph<node_type>::reserve,
             py::arg("m"),
             py::arg("n"),
             R"pbdoc(
               Ensures that this has capacity for m nodes each with n
               out-edges, but does not modify :py:meth:`number_of_nodes`
               or :py:meth:`out_degree`.

               :param m: the number of nodes
               :type m: int
               :param n: the out-degree
               :type n: int

               :return: (None)
             )pbdoc")
        .def("target_no_checks",
             &WordGraph<node_type>::target_no_checks,
             py::arg("v"),
             py::arg("lbl"),
             R"pbdoc(
               Get the target of the edge with source node ``v`` and
               edge-label
               ``lbl``.

               :param v: the node
               :type v: int
               :param lbl: the label
               :type lbl: int

               :return: An ``int`` or :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("next_target",
             &WordGraph<node_type>::next_target,
             py::arg("v"),
             py::arg("i"),
             R"pbdoc(
               Get the next target of a node that doesn't equal
               :py:obj:`UNDEFINED`.

               :param v: the node
               :type v: int
               :param i: the label
               :type i: int

               :return: A tuple.

               Specifically, a tuple ``x`` is returned where

                 * ``x[0]`` is adjacent to ``v`` via an edge labelled
                 ``x[1]``;
                 * ``x[1]`` is the minimum value in the range
                   :math:`[i, N)` where ``N`` is :py:meth:`degree` such
                   that
                   ``neighbor(v, x[1])`` is not equal to
                   :py:obj:`UNDEFINED`.

               If ``neighbor(v, i)`` is undefined for every value of
               ``i``, then ``x[0]`` and ``x[1]`` equal
               :py:obj:`UNDEFINED`.
             )pbdoc")
        .def("next_target_no_checks",
             &WordGraph<node_type>::next_target_no_checks,
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

                 * ``x[0]`` is adjacent to ``v`` via an edge labelled
                 ``x[1]``;
                 * ``x[1]`` is the minimum value in the range
                   :math:`[i, N)` where ``N`` is :py:meth:`degree` such
                   that
                   ``neighbor(v, x[1])`` is not equal to
                   :py:obj:`UNDEFINED`.

               If ``neighbor(v, i)`` is undefined for every value of
               ``i``, then ``x[0]`` and ``x[1]`` equal
               :py:obj:`UNDEFINED`.
             )pbdoc")
        .def(
            "nodes_iterator",
            [](WordGraph<node_type> const& wg) {
              return py::make_iterator(wg.cbegin_nodes(), wg.cend_nodes());
            },
            R"pbdoc(
              Returns an iterator to the nodes of the digraph.
            )pbdoc")
        .def(
            "reverse_nodes_iterator",
            [](WordGraph<node_type> const& wg) {
              return py::make_iterator(wg.crbegin_nodes(), wg.crend_nodes());
            },
            R"pbdoc(
                Returns a reversed iterator to the nodes of the digraph.
            )pbdoc")
        .def(
            "edges_iterator",
            [](WordGraph<node_type> const& wg, node_type const i) {
              return py::make_iterator(wg.cbegin_targets(i),
                                       wg.cend_targets(i));
            },
            py::arg("i"),
            R"pbdoc(
              Returns an iterator to the edges of a node in the digraph.

              :param i: the node.
              :type i: int

              :return: An iterator.
            )pbdoc")
        .def_static(
            "random",
            [](node_type nr_nodes, node_type out_degree) {
              return WordGraph<node_type>::random(nr_nodes, out_degree);
            },
            py::arg("nr_nodes"),
            py::arg("out_degree"),
            R"pbdoc(
              Constructs a random :py:class:`WordGraph` with the
              specified number of nodes and out-degree.

              :param nr_nodes:  the number of nodes
              :type nr_nodes: int
              :param out_degree: the maximum out-degree of every node
              :type out_degree: int
              :returns: An ``WordGraph``.
            )pbdoc")
        .def_static(
            "random_acyclic",
            [](node_type nr_nodes, node_type out_degree, node_type nr_edges) {
              return WordGraph<node_type>::random_acyclic(
                  nr_nodes, out_degree, nr_edges);
            },
            py::arg("nr_nodes"),
            py::arg("out_degree"),
            py::arg("nr_edges"),
            R"pbdoc(
              Constructs a random acyclic :py:class:`WordGraph` with
              the specified number of nodes and edges, and out-degree.

              :param nr_nodes: the number of nodes
              :type nr_nodes: int
              :param out_degree: the out-degree of every node
              :type out_degree: int
              :param nr_edges: the out-degree of every node
              :type nr_edges: int
              :returns: An ``WordGraph``.
            )pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // word_graph
    ////////////////////////////////////////////////////////////////////////

    // TODO(later) there are several more functions in
    // word_graph
    // that can be included here!

    m.def("is_complete",
          &word_graph::is_complete<node_type>,
          R"pbdoc(
               Check every node has exactly :py:meth:`out_degree`
               out-edges.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc");
    m.def("add_cycle",
          py::overload_cast<WordGraph<node_type>&, size_t>(
              &word_graph::add_cycle<node_type>),
          py::arg("wg"),
          py::arg("N"),
          R"pbdoc(
        Adds a cycle consisting of ``N`` new nodes.

        :param wg: the action digraph.
        :type wg: WordGraph
        :param N: the length of the cycle.
        :type N: int

        :return: None.
      )pbdoc");
    m.def("is_acyclic",
          py::overload_cast<WordGraph<node_type> const&>(
              &word_graph::is_acyclic<node_type>),
          py::arg("wg"),
          R"pbdoc(
        Check if a digraph is acyclic.

        A digraph is acyclic if every directed cycle is trivial.

        :param wg: the digraph.
        :type wg: WordGraph

        :return: A ``bool``.
      )pbdoc");
    m.def("topological_sort",
          py::overload_cast<WordGraph<node_type> const&>(
              &word_graph::topological_sort<node_type>),
          py::arg("wg"),
          R"pbdoc(
        Returns the nodes of the digraph in topological order (see
        below) if possible.

        If it is not empty, the returned list has the property that if
        an edge from a node ``n`` points to a node ``m``, then ``m``
        occurs before ``n`` in the list.

        :Parameters: - **wg** (WordGraph) the digraph.
        :Returns: A list of ``int``.
      )pbdoc");
    m.def("topological_sort",
          py::overload_cast<WordGraph<node_type> const&, node_type>(
              &word_graph::topological_sort<node_type, node_type>),
          py::arg("wg"),
          py::arg("source"),
          R"pbdoc(
        Returns the nodes of the digraph reachable from a given node
        in topological order (see below) if possible.

        If it is not empty, the returned list has the property that if
        an edge from a node ``n`` points to a node ``m``, then ``m``
        occurs before ``n`` in the list, and the last item in the list
        is source.

        :Parameters: - **wg** (WordGraph) the digraph.
                     - **source** (int) the source node.
        :Returns: A list of ``int``.
      )pbdoc");
    m.def("follow_path",
          &word_graph::follow_path<node_type, node_type>,
          py::arg("wg"),
          py::arg("source"),
          py::arg("path"),
          R"pbdoc(
        Find the node that a path starting at a given node leads to.

        :param wg: the ``WordGraph`` object to check.
        :type wg: WordGraph
        :param first: the starting node.
        :type first: int
        :param path: the path to follow.
        :type path: List[int]

        :return:
          An ``int`` corresponding to the node at the end of the path
          or :py:class:`UNDEFINED` otherwise.

        :raises RuntimeError:
          if ``source`` is not a node in the digraph or ``path``
          contains a value that is not an edge-label.
      )pbdoc");
    m.def("to_word_graph",
          // TODO: Document this
          &to_word_graph<node_type>,
          py::arg("num_nodes"),
          py::arg("l"));
  }
}  // namespace libsemigroups
