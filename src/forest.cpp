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
#include <stddef.h>  // for size_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list

// libsemigroups....
#include <libsemigroups/forest.hpp>  // for Forest

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, make_iterator, module

// libsemigroups_pybind11....
#include "main.hpp"  // for init_forest

namespace py = pybind11;

namespace libsemigroups {
  void init_forest(py::module& m) {
    py::class_<Forest>(m, "Forest")
        .def(py::init<size_t>(), R"pbdoc(
          Constructs a forest with 0 nodes.

          The :py:class:`Forest` is initialised so that the :py:meth:`parent`
          and :py:meth:`label` of every node is :py:obj:`UNDEFINED`.

          :Parameters: None.
        )pbdoc")
        .def(py::init<Forest const&>(),
             py::arg("that"),
             R"pbdoc(
               Copy constructor.

               :Parameters: - **that** (:py:class:`Forest`) the :py:class:`Forest` to copy.
        )pbdoc")
        .def("add_nodes",
             &Forest::add_nodes,
             py::arg("n"),
             R"pbdoc(
               Add nodes to the Forest.

               :param n: the number of nodes to add.
               :type n: in

               :return: (None)
               )pbdoc")
        .def("clear",
             &Forest::clear,
             R"pbdoc(
               Removes all nodes from the forest.

               :return: (None)
               )pbdoc")
        .def("set",
             &Forest::set,
             py::arg("node"),
             py::arg("parent"),
             py::arg("gen"),
             R"pbdoc(
               Set the parent and edge label for a node.

               :param node: the node whose parent and label to set.
               :type node: int
               :param parent: the parent node
               :type parent: int
               :param gen: the label of the edge from parent to node.
               :type gen: int

               :return: (None)
             )pbdoc")
        .def("number_of_nodes",
             &Forest::number_of_nodes,
             R"pbdoc(
               Returns the number of nodes in the forest.

               :return: An ``int``.
             )pbdoc")
        .def("parent",
             &Forest::parent,
             py::arg("i"),
             R"pbdoc(
               Returns the parent of a node.

               :param i: the node whose parent is sought.
               :type i: int

               :return: An ``int``.
             )pbdoc")
        .def("label",
             &Forest::label,
             py::arg("i"),
             R"pbdoc(
               Returns the label of the edge from a node to its parent.

               :param i: the node whose label is sought.
               :type i: int

               :return: A ``int``.
               )pbdoc")
        .def(
            "parent_iterator",
            [](Forest const& f) {
              return py::make_iterator(f.cbegin_parent(), f.cend_parent());
            },
            R"pbdoc(
              Returns an iterator pointing to the parent of the first node.

              :Parameters: None.
              :return: An iterator.
            )pbdoc");
  }
}  // namespace libsemigroups
