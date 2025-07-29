

//
// libsemigroups_pybind11
// Copyright (C) 2024 James D. Mitchell
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
#include <libsemigroups/dot.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_dot

namespace libsemigroups {
  namespace py = pybind11;

  void init_dot(py::module& m) {
    ////////////////////////////////////////////////////////////////////////////
    // Dot class declaration
    ////////////////////////////////////////////////////////////////////////////
    py::class_<Dot> dot(m,
                        "Dot",
                        R"pbdoc(A representation of a graph in the DOT_ language
of Graphviz_.

This class facilitates the creation and rendering of graph descriptions in the
DOT_ language of the Graphviz_ graph drawing software. This class is fairly
rudimentary, and is not intended to implement every feature of the DOT_
language. You can create a :any:`Dot` object, assemble the graph by adding nodes
and edges, attributes, labels, colours, subgraphs, and clusters, and retrieve
its DOT_ source code string (:any:`Dot.to_string`). Write the source code to a
file and render it with the Graphviz_ installation on your system.
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // Node nested class
    ////////////////////////////////////////////////////////////////////////////
    py::class_<Dot::Node> n(dot,
                            "Node",
                            R"pbdoc(
This nested class represents a node in the represented graph.
)pbdoc");
    n.def("__repr__",
          py::overload_cast<Dot::Node const&>(&to_human_readable_repr));
    n.def_property_readonly(
        "attrs",
        [](Dot::Node& self) { return self.attrs; },
        R"pbdoc(
     Read-only dictionary containing the attributes of the node.
     )pbdoc");
    n.def_readonly("name",
                   &Dot::Node::name,
                   R"pbdoc(
The name of the node.
)pbdoc");
    n.def("add_attr",
          &Dot::Node::add_attr<std::string const&, std::string const&>,
          py::arg("key"),
          py::arg("val"),
          R"pbdoc(Add an attribute to an node.

This function adds a new attribute, or replaces the value of an existing
attribute of a :any:`Dot.Node`.

:param key: the name of the attribute.
:type key: str
:param val: the value of the attribute.
:type val: str

:returns: *self*
:rtype: Dot.Node
)pbdoc",
          py::return_value_policy::reference);

    ////////////////////////////////////////////////////////////////////////////
    // Edge nested class
    ////////////////////////////////////////////////////////////////////////////
    py::class_<Dot::Edge> e(dot,
                            "Edge",
                            R"pbdoc(
Instances of this nested class represents an edge in the represented graph.

:any:`Edge` objects can only be constructed by calling :any:`Dot.add_edge`.
)pbdoc");
    e.def("__repr__",
          py::overload_cast<Dot::Edge const&>(&to_human_readable_repr));
    e.def_property_readonly(
        "attrs",
        [](Dot::Edge& self) { return self.attrs; },
        R"pbdoc(
Read-only dictionary containing containing the attributes of the :any:`Edge`.
     )pbdoc");
    e.def_readonly("head",
                   &Dot::Edge::head,
                   R"pbdoc(
The name (read-only `str`) of the head of the edge.
)pbdoc");
    e.def_readonly("tail",
                   &Dot::Edge::tail,
                   R"pbdoc(
The name (read-only `str`) of the tail of the edge.
)pbdoc");
    e.def("add_attr",
          &Dot::Edge::add_attr<std::string const&, std::string const&>,
          py::arg("key"),
          py::arg("val"),
          R"pbdoc(Add an attribute to an edge.

This function adds a new attribute, or replaces the value of an existing
attribute of an :any:`Edge`.

:param key: the name of the attribute.
:type key: str
:param val: the value of the attribute.
:type val: str

:returns: *self*
:rtype: Dot.Edge
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // Kind enum
    ////////////////////////////////////////////////////////////////////////////
    py::options options;
    options.disable_enum_members_docstring();
    py::enum_<Dot::Kind> kind(dot, "Kind", R"pbdoc(
The values in this enum can be used to indicate the type of a graph.

The valid values are:

.. py:attribute:: Kind.digraph
  :value: <Kind.digraph: 0>

  Value indicating that the represented graph has directed edges ->.

.. py:attribute:: Kind.graph
  :value: <Kind.graph: 1>

  Value indicating that the represented graph has undirected edges --.

.. py:attribute:: Kind.subgraph
  :value: <Kind.subgraph: 2>

  Value indicating that the represented graph is a subgraph of another Dot object.
)pbdoc");
    kind.value("digraph", Dot::Kind::digraph)
        .value("graph", Dot::Kind::graph)
        .value("subgraph", Dot::Kind::subgraph);
    kind.def("__repr__", [](Dot::Kind const& knd) {
      return to_human_readable_repr(knd, ".");
    });

    ////////////////////////////////////////////////////////////////////////////
    // Dot members
    ////////////////////////////////////////////////////////////////////////////
    dot.def("__repr__", py::overload_cast<Dot const&>(&to_human_readable_repr));

    // This does not return by reference, not sure how to get it to, given that
    // it returns a list of strings.
    dot.def_property_readonly(
        "colors",
        [](Dot const& self) { return Dot::colors; },
        R"pbdoc(
A list of default HTML/hex colours.
)pbdoc");

    dot.def(py::init<>(), R"pbdoc(
Default constructor that constructs an empty :any:`Dot` object with no nodes,
edges, attributes, or subgraphs.
)pbdoc");
    dot.def("__copy__", [](Dot const& d) { return Dot(d); });
    dot.def(
        "copy",
        [](Dot const& d) { return Dot(d); },
        R"pbdoc(
:sig=(self: Dot) -> Dot:

Copy a :any:`Dot` object.

:returns: A copy.
:rtype: Dot
)pbdoc");

    dot.def("add_node",
            &Dot::add_node<std::string const&>,
            py::return_value_policy::reference_internal,
            // The return_value_policy ensures that the value is returned by
            // reference and that the Dot instance is kept alive until the
            // last node reference is collected.
            py::arg("name"),
            R"pbdoc(
:sig=(self:Dot,name:str)->Dot.Node:

This function adds a node with name *name*.

:param name: the name of the node to add.
:type name: str

:returns: a newly created :any:`Dot.Node` object with name *name*.
:rtype: Dot.Node

:raises LibsemigroupsError: if there is already a node with name *name*.
)pbdoc");
    dot.def("add_edge",
            &Dot::add_edge<std::string const&, std::string const&>,
            py::return_value_policy::reference_internal,
            // The return_value_policy ensures that the value is returned by
            // reference and that the Dot instance is kept alive until the
            // last edge reference is collected.
            py::arg("head"),
            py::arg("tail"),
            R"pbdoc(
:sig=(self:Dot,head:str,tail:str)->Dot.Edge:
This function adds an edge from the node named *head* to the node named *tail*.

:param head: the name of the node at the head of the edge.
:type head: str
:param tail: the name of the node at the tail of the edge.
:type tail: str

:returns: a newly created :any:`Dot.Edge` object.
:rtype: Dot.Edge

:raises LibsemigroupsError: if there is no node named *head* and/or *tail*.
)pbdoc");

    dot.def(
        "add_subgraph",
        [](Dot& self, Dot const& subgraph) -> Dot& {
          return self.add_subgraph(subgraph);
        },
        py::return_value_policy::reference_internal,
        // The return_value_policy ensures that the value is returned by
        // reference and that the Dot instance is kept alive until the
        // last edge reference is collected.
        py::arg("subgraph"),
        R"pbdoc(
This functions adds the :any:`Dot` object *subgraph* as a subgraph of *self*.
The following transformations are performed

* the ``label`` attribute of the added subgraph is the :any:`Dot.name` of
  *subgraph*;
* the :any:`Dot.name` of the added subgraph has the prefix ``"cluster_"``
  added;
* every node in the added subgraph has the prefix ``"name_"`` prepended
  (where ``"name"`` is the return value of :any:`Dot.name`);
* the ``label`` attribute of every node in the added subgraph is its
  original name in *subgraph*;
* every edge is modified so that its head and tail use the new node
  names;
* every edge has the attribute ``constraint`` set to ``false``.

:param subgraph: the :any:`Dot` object to use as a subgraph.
:type subgraph: Dot

:returns: *self*.
:rtype: Dot
)pbdoc");

    dot.def("edges",
            py::overload_cast<>(&Dot::edges, py::const_),
            R"pbdoc(
:sig=(self:Dot)->list[Dot.Edge]:
Returns a copy of the list of the current edges (:any:`Edge` objects) in the
represented graph.

:returns:
  The list of current edges.
:rtype:
   list[Dot.Edge]
)pbdoc");

    dot.def("subgraphs",
            py::overload_cast<>(&Dot::subgraphs, py::const_),
            R"pbdoc(
Returns a copy of the list of the current subgraphs (:any:`Dot` objects) in the
represented graph.

:returns:
  The list of current subgraphs.
:rtype:
   list[Dot]
)pbdoc");

    dot.def("attrs",
            &Dot::attrs,
            R"pbdoc(
Returns a read-only dictionary of the current attributes of the
represented graph.

:returns:
  A dictionary.
:rtype:
   dict[str, str]
)pbdoc");

    dot.def(
        "is_node",
        [](Dot const& self, std::string const& name) {
          return self.is_node(name);
        },
        py::arg("name"),
        R"pbdoc(
Check if there is a node with a given name.

This function returns ``True`` if *name* is currently the name of a node in
the represented graph and ``False`` otherwise.

:param name: the name of the node.
:type name: str

:returns: whether or not *name* is the name of a node.
:rtype: bool
)pbdoc");
    dot.def("kind",
            py::overload_cast<Dot::Kind>(&Dot::kind),
            py::arg("val"),
            R"pbdoc(
:sig=(self: Dot, val: Dot.Kind)->Dot:

Set the kind of the represented graph.

:param val:
   the kind.
:type val:
   Dot.Kind

:returns:
   *self*.
:rtype:
   Dot
)pbdoc");

    dot.def("kind",
            py::overload_cast<>(&Dot::kind, py::const_),
            R"pbdoc(
:sig=(self: Dot)->Dot.Kind:

Get the kind of the represented graph.

:returns:
   The kind of the graph represented.
:rtype:
   Dot.Kind
)pbdoc");

    dot.def(
        "name",
        [](Dot const& self) { return self.name(); },
        R"pbdoc(
Get the current name of the represented graph.

:returns:
   The current name of the represented graph.
:rtype:
   str
)pbdoc");

    dot.def(
        "name",
        [](Dot& self, std::string const& val) { return self.name(val); },
        py::arg("val"),
        R"pbdoc(
Set the name of the represented graph.

:param val:
   the name.
:type val:
   str

:returns:
   *self*.
:rtype:
   Dot
)pbdoc");

    dot.def(
        "nodes",
        [](Dot const& d) -> std::vector<Dot::Node> {
          return d.nodes() | rx::to_vector();
        },
        R"pbdoc(
:sig=(self: Dot) -> list[Dot.Node]:
Returns a copy of the list of the current nodes in the represented
graph.

:returns:
  The list of current nodes.
:rtype:
   list[Dot.Node]
)pbdoc");

    dot.def(
        "node",
        [](Dot& d, std::string const& name) -> Dot::Node& {
          return d.node(name);
        },
        py::return_value_policy::reference_internal,
        py::arg("name"),
        R"pbdoc(
:sig=(self: Dot, name: str) -> Dot.Node:
Returns the node object with name *name*.

:param name: the name of the node.
:type name: str

:returns:
  The node named *name*.
:rtype:
   Dot.Node

:raises LibsemigroupsError: if there is no node named *name*.
)pbdoc");

    dot.def(
        "edge",
        [](Dot& d, std::string const& head, std::string const& tail)
            -> Dot::Edge& { return d.edge(head, tail); },
        py::return_value_policy::reference_internal,
        py::arg("head"),
        py::arg("tail"),
        R"pbdoc(
:sig=(self: Dot, head: str, tail: str) -> Dot.Edge:

Returns the edge object with head named *head* and tail named *tail*.

:param head: the name of the head node of the edge (node where the edge starts).
:type head: str

:param tail: the name of the tail node of the edge (node where the edge ends).
:type tail: str

:returns:
  The edge from the node named *head* to the node named *tail*.
:rtype:
   Dot.Edge

:raises LibsemigroupsError: if there is no node named *head* or no node named *tail*.
:raises LibsemigroupsError: if there is no edge from *head* to *tail*.
)pbdoc");

    dot.def("__str__", &Dot::to_string);
    dot.def("to_string",
            &Dot::to_string,
            R"pbdoc(
Convert a :any:`Dot` object to a string. This function returns the string
representation of the :any:`Dot` object. This string contains a
representation of the graph in the DOT_ language for Graphviz_.

:returns:
   The string representation of *self*.
:rtype:
   str
)pbdoc");

    dot.def("add_attr",
            &Dot::add_attr<std::string const&, std::string const&>,
            py::arg("key"),
            py::arg("val"),
            R"pbdoc(Add an attribute to a :any:`Dot` object.

This function adds a new attribute, or replaces the value of an existing
attribute of an :any:`Dot`.

:param key: the name of the attribute.
:type key: str
:param val: the value of the attribute.
:type val: str

:returns: *self*
:rtype: Dot
)pbdoc",
            py::return_value_policy::reference);

    dot.def("add_attr",
            &Dot::add_attr<std::string const&>,
            py::arg("key"),
            R"pbdoc(Add an attribute to a :any:`Dot` object.

This function adds a new attribute, or replaces the value of an existing
attribute of an :any:`Dot`.

:param key: the name of the attribute.
:type key: str

:returns: *self*
:rtype: Dot
)pbdoc",
            py::return_value_policy::reference);
  }  // init_dot

}  // namespace libsemigroups
