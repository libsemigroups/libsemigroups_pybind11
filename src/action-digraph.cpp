//
//// libsemigroups - C++ library for semigroups and monoids
//// Copyright (C) 2021 James D. Mitchell
////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU General Public License as published by
//// the Free Software Foundation, either version 3 of the License, or
//// (at your option) any later version.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU General Public License for more details.
////
//// You should have received a copy of the GNU General Public License
//// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {
	using node_type = ActionDigraph<size_t>::node_type;
	using algorithm = ActionDigraph<size_t>::algorithm;
	void init_action_digraph(py::module &m) {
		py::class_<libsemigroups::ActionDigraph<size_t>> ad(m, "ActionDigraph");
		ad.def(py::init<size_t, size_t>());
		ad.def(py::init<ActionDigraph<size_t> const &>())

			.def("nr_nodes",
					&libsemigroups::ActionDigraph<size_t>::nr_nodes,
					R"pbdoc(
             Returns the number of nodes of this.

             :return: The number of nodes, a value of type T.
              )pbdoc")
			.def("nr_edges",
					py::overload_cast<>(&libsemigroups::ActionDigraph<size_t>::nr_edges,
						py::const_), R"pbdoc( Returns the number of edges of this.
                     :Returns: The total number of edges, a value of type size_t.
           )pbdoc")
						.def("nr_edges",
								py::overload_cast<node_type const>(&libsemigroups::ActionDigraph<size_t>::nr_edges, py::const_), py::arg("n"),
								R"pbdoc(
         Returns the number of edges leaving a node.
                 :Parameters: **n** (??) - the node.
                 :Returns: A value of type size_t.
           )pbdoc")
						.def("out_degree",
								&libsemigroups::ActionDigraph<size_t>::out_degree,
								R"pbdoc(
            Returns the out-degree of this.

            :return: The number of out-edges of every node, a value of type T.
          )pbdoc")
						.def("validate",
								&libsemigroups::ActionDigraph<size_t>::validate,
								R"pbdoc(
               Check every node has exactly out_degree() out-edges.

                 :return: A bool.
               )pbdoc")
						.def("add_edge",
								&libsemigroups::ActionDigraph<size_t>::add_edge,
								py::arg("i"), py::arg("j"), py::arg("lbl"),
								R"pbdoc(
                   Add an edge from i to j labelled lbl.

                   :param i: the source node
                   :type i: ??
                   :param j: the range node
                   :type j: ??
                   :param lbl: the label of the edge from i to j
                   :type lbl: ??

                   :return: (None)
                   )pbdoc")
						.def("add_nodes",
								&libsemigroups::ActionDigraph<size_t>::add_nodes,
								py::arg("nr"),
								R"pbdoc(
                   Adds nr nodes to this.

                   :param nr: the number of nodes to add.
                   :type nr: ??

                   :return: (None)
                   )pbdoc")
						.def("add_to_out_degree",
								&libsemigroups::ActionDigraph<size_t>::add_to_out_degree,
								py::arg("nr"),
								R"pbdoc(
                   Adds nr to the out-degree of this.

                   :param nr: the number of new out-edges for every node.
                   :type nr: ??

                   :return: (None)
                   )pbdoc")
						.def("neighbor",
								&libsemigroups::ActionDigraph<size_t>::neighbor,
								py::arg("v"), py::arg("lbl"),
								R"pbdoc(
            Get the range of the edge with source node v and edge-label lbl.

                   :param v: the node
                   :type v: ??
                   :param lbl: the label
                   :type lbl: ??

                   :return: Returns the node adjacent to v via the edge labelled
    lbl, or libsemigroups::UNDEFINED; both are values of type
    ActionDigraph::node_type. )pbdoc")
						.def("reserve",
								&libsemigroups::ActionDigraph<size_t>::reserve,
								py::arg("m"), py::arg("n"),
								R"pbdoc(
                   Ensures that this has capacity for m nodes each with n
    out-edges, but does not modify nr_nodes() or out_degree().

                   :param m: the number of nodes
                   :type m: ??
                   :param n: the out-degree
                   :type n: ??

                   :return: (None)
                   )pbdoc")
						.def("unsafe_neighbor",
								&libsemigroups::ActionDigraph<size_t>::unsafe_neighbor,
								py::arg("v"), py::arg("lbl"),
								R"pbdoc(
                   Get the range of the edge with source node v and edge-label
    lbl.

                   :param v: the node
                   :type v: ??
                   :param lbl: the label
                   :type lbl: ??

                   :return: Returns the node adjacent to v via the edge labelled
    lbl, or libsemigroups::UNDEFINED; both are values of type
    ActionDigraph::node_type. )pbdoc")

						.def("next_neighbor",
								&libsemigroups::ActionDigraph<size_t>::next_neighbor,
								py::arg("v"), py::arg("i"),
								R"pbdoc(
                   Get the next neighbor of a node that doesn't equal
    libsemigroups::UNDEFINED.

                   :param v: the node
                   :type v: ??
                   :param i: the label
                   :type i: ??

                   :return: Returns a std::pair x where:
    x.second is the minimum value in the range $[i, out_degree())$ such that
    neighbor(v, x.second) is not equal to libsemigroups::UNDEFINED; and x.first
    is adjacent to v via an edge labelled x.second; If neighbor(v, i) equals
    libsemigroups::UNDEFINED for every value in the range $[i, out_degree())$,
    then x.first and x.second equal libsemigroups::UNDEFINED.
                   )pbdoc")
						.def("unsafe_next_neighbor",
								&libsemigroups::ActionDigraph<size_t>::unsafe_next_neighbor,
								py::arg("v"), py::arg("i"),
								R"pbdoc(
                   Get the next neighbor of a node that doesn't equal
    libsemigroups::UNDEFINED.

                   :param v: the node
                   :type v: ??
                   :param i: the label
                   :type i: ??

                   :return: Returns a std::pair x where:
    x.second is the minimum value in the range $[i, out_degree())$ such that
    neighbor(v, x.second) is not equal to libsemigroups::UNDEFINED; and x.first
    is adjacent to v via an edge labelled x.second; If neighbor(v, i) equals
    libsemigroups::UNDEFINED for every value in the range $[i, out_degree())$,
    then x.first and x.second equal libsemigroups::UNDEFINED.
                   )pbdoc")

						.def("scc_id",
								&libsemigroups::ActionDigraph<size_t>::scc_id,
								py::arg("nd"),
								R"pbdoc(
          Returns the id-number of the strongly connected component of a node.

                   :param nd: the node.
                   :type nd: ??

                   :return: The index of the node nd, a value of type
    scc_index_type. )pbdoc")
						.def("nr_scc",
								&libsemigroups::ActionDigraph<size_t>::nr_scc,
								R"pbdoc(
                   Returns the number of strongly connected components in this.

                   :return: A size_t.
                   )pbdoc")
						.def("root_of_scc",
								&libsemigroups::ActionDigraph<size_t>::root_of_scc,
								py::arg("nd"),
								R"pbdoc(
              Returns the root of a strongly connected components containing a given node.

                   :param nd: a node.
                   :type nd: ??

                   :return: The root of the scc containing the node nd, a value
    of ActionDigraph::node_type. )pbdoc")
						.def("spanning_forest",                                                                                                                 &libsemigroups::ActionDigraph<size_t>::spanning_forest,
								R"pbdoc(
                   Returns a libsemigroups::Forest comprised of spanning trees
    for each scc of this, rooted on the minimum node of that component, with
    edges oriented away from the root.

                   :return: A const reference to a libsemigroups::Forest.
                   )pbdoc")
						.def("reverse_spanning_forest",
								&libsemigroups::ActionDigraph<size_t>::reverse_spanning_forest,
								R"pbdoc(
                   Returns a libsemigroups::Forest comprised of spanning trees
    for each scc of this, rooted on the minimum node of that component, with
    edges oriented towards the root.

                   :return: A const reference to a libsemigroups::Forest.
                   )pbdoc")
						.def("number_of_paths_algorithm",
								py::overload_cast<node_type
								const>(&libsemigroups::ActionDigraph<size_t>::number_of_paths_algorithm,
									py::const_), py::arg("source"), R"pbdoc( Returns the algorithm used by
    number_of_paths to compute the number of paths originating at the given
    source node.

                   :Parameters: **source** (??) - the source node.

                   :Returns: A value of type ActionDigraph::algorithm.
                   )pbdoc")
								.def("number_of_paths",
										py::overload_cast<node_type
										const>(&libsemigroups::ActionDigraph<size_t>::number_of_paths, py::const_),
										py::arg("source"),
										R"pbdoc(
                   Returns the number of paths originating at the given source node.

                   :Parameters: **source** (??) - the source node.

                   :Returns: A value of type uint64_t.
                   )pbdoc")
								.def("number_of_paths_algorithm",
										py::overload_cast<node_type const,size_t const,size_t
										const>(&libsemigroups::ActionDigraph<size_t>::number_of_paths_algorithm,
											py::const_), py::arg("source"), py::arg("min"), py::arg("max"),
										R"pbdoc(
          Returns the algorithm used by number_of_paths to compute the
    number of paths originating at the given source node with length in the
    range $[min, max)$.

                   :Parameters: - **source** (??) - the source node
                                - **min** (??) - the minimum length of paths to
    count
                                - **max** (??) - the maximum length of paths to
    count

                   :Returns: A value of type ActionDigraph::algorithm.
                   )pbdoc")
								.def("number_of_paths",
										py::overload_cast<node_type const,size_t const,size_t
										const,algorithm const>(&libsemigroups::ActionDigraph<size_t>::number_of_paths,
											py::const_), py::arg("source"), py::arg("min"), py::arg("max"),
										py::arg("lgrthm"), R"pbdoc( Returns the number of paths starting at a given
    node with length in a given range.

                   :Parameters: - **source** (??) - the first node
                                - **min** (??) - the minimum length of a path
                                - **max** (??) - the maximum length of a path
                                - **lgrthm** (??) - the algorithm to use
    (defaults to: algorithm::automatic).

                   :Returns: A value of type uint64_t.
                   )pbdoc")                                                                                                                                                             .def("number_of_paths_algorithm",
										py::overload_cast<node_type const,node_type const,size_t
										const,size_t
										const>(&libsemigroups::ActionDigraph<size_t>::number_of_paths_algorithm,
											py::const_), py::arg("source"), py::arg("target"), py::arg("min"),
										py::arg("max"), R"pbdoc( Returns the algorithm used by number_of_paths to
    compute the number of paths originating at the given source node and ending
    at the given target node with length in the range $[min, max)$.
                                                                                                                                           :Parameters: - **source** (??) - the source node
                                - **target** (??) - the target node
                                - **min** (??) - the minimum length of paths to
    count
                                - **max** (??) - the maximum length of paths to
    count
                   :Returns: A value of type ActionDigraph::algorithm.
                   )pbdoc")
								.def("number_of_paths",
										py::overload_cast<node_type const,node_type const,size_t
										const,size_t const,algorithm
										const>(&libsemigroups::ActionDigraph<size_t>::number_of_paths, py::const_),
										py::arg("source"), py::arg("target"), py::arg("min"),
										py::arg("max"), py::arg("lgrthm"), R"pbdoc( Returns the number of paths
    between a pair of nodes with length in a given range.
                   :Parameters: - **source** (??) - the first node
                                - **target** (??) - the last node
                                - **min** (??) - the minimum length of a path
                                - **max** (??) - the maximum length of a path
                                - **lgrthm** (??) - the algorithm to use
    (defaults to: algorithm::automatic).
                   :Returns: A value of type uint64_t.
                   )pbdoc")

								.def("nodes_iterator",
										[](ActionDigraph<size_t> const &ad){
										return py::make_iterator(ad.cbegin_nodes(),
												ad.cend_nodes());
										},
										R"pbdoc(
            Returns an iterator to the nodes of the digraph.
            )pbdoc")                                                                                                                                                                    .def("reverse_nodes_iterator",
										[](ActionDigraph<size_t> const &ad){
										return py::make_iterator(ad.crbegin_nodes(),
												ad.crend_nodes());                                                                                      },
										R"pbdoc(
            Returns a reversed iterator to the nodes of the digraph.
            )pbdoc")
								.def("edges_iterator",
										[](ActionDigraph<size_t> const &ad, size_t const i){
										return py::make_iterator(ad.cbegin_edges(i),
												ad.cend_edges(i));
										},
										R"pbdoc(
            Returns an iterator to the edges of the digraph.
            )pbdoc")
								.def("sccs_iterator",                                                                                                                   [](ActionDigraph<size_t> const &ad){
										return py::make_iterator(ad.cbegin_sccs(),                                                                                                                                                                                                                      ad.cend_sccs());
										},                                                                                                                      R"pbdoc(
Returns an iterator for the vectors of nodes in the scc.
)pbdoc")
		.def("scc_iterator",
				[](ActionDigraph<size_t> const &ad, size_t const i){
				return py::make_iterator(ad.cbegin_scc(i),
						ad.cend_scc(i));
				},
				R"pbdoc(
Returns an iterator pointing to the first node in the scc with the specified id-number.
)pbdoc")
			.def("scc_roots_iterator",
					[](ActionDigraph<size_t> const &ad){
					return py::make_iterator(ad.cbegin_scc_roots(),
							ad.cend_scc_roots());
					},
					R"pbdoc(
Returns an iterator pointing to the root of the first scc.
)pbdoc")
			.def("panilo_iterator",
					[](ActionDigraph<size_t> const &ad, node_type const &source, size_t const &mn, size_t const&mx){                                                                                                                                                return py::make_iterator(ad.cbegin_panilo(source, mn, mx),
						ad.cend_panilo());
					},
					R"pbdoc(
 Returns a forward iterator pointing to a pair consisting of
    the edge labels of the first path (in lexicographical order) starting at
    source with length in the range $[min, max)$ and the last node of that path.                                        
                   :param source: the source node
                   :type source: ??
                   :param min: the minimum length of a path to enumerate
    (defaults to 0) :type min: ?? :param max: the maximum length of a path to
    enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??

                   :return: An iterator it of type const_panilo_iterator
    pointing to a std::pair where: it->first is a libsemigroups::word_type
    consisting of the edge labels of the first path (in lexicographical order)
    from source of length in the range $[min, max)$; and it->second is the last
    node on the path from source labelled by it->first, a value of
    ActionDigraph::node_type.
)pbdoc");
	}
}
// Compiles but doesn't work because there's no conversion from python to
// std::mt19937 (so it seems).
///*ad.def_static("random",
// py::overload_cast<size_t const, size_t const, std::mt19937>(
// &libsemigroups::ActionDigraph<size_t>::random),
// py::arg("nr_nodes"),
// py::arg("out_degree"),
//  py::arg("mt"),
//  R"pbdoc(
//  Constructs a random ActionDigraph from mt with the specified
// number of nodes and out-degree.
//
//  :Parameters: - **nr_nodes** (??) - the number of nodes
// - **out_degree** (??) - the out-degree of every
//node
//  - **mt** (??) - a std::mt19937 used as a random
//  source (defaults to: std::mt19937(std::random_device()()))
// 	)pbdoc");*/
//  	}
// 	}  // namespace libsemigroups
// /* .def("random",
//   py::overload_cast<T const,T
//    const,std::mt19937>(&libsemigroups::ActionDigraph::random),
//  py::arg("nr_nodes"), py::arg("out_degree"), py::arg("mt"),
//  R"pbdoc(
//  Constructs a random ActionDigraph from mt with the specified
// 
//   :Parameters: - **nr_nodes** (??) - the number of nodes
// 	           - **out_degree** (??) - the out-degree of every node
//                 - **mt** (??) - a std::mt19937 used as a random source (defaults to: std::mt19937(std::random_device()()))
//
//   )pbdoc")
//  .def("random_acyclic",
//   &libsemigroups::ActionDigraph::random_acyclic,
//   py::arg("nr_nodes"), py::arg("out_degree"), py::arg("nr_edges"),
//   py::arg("mt"), R"pbdoc( Constructs a random acyclic ActionDigraph from mt
//   with the specified number of nodes and edges, and out-degree.
//   :param nr_nodes: the number of nodes
//    :type nr_nodes: ??
// 	:param out_degree: the out-degree of every node
//   :type out_degree: ??
//   :param nr_edges: the out-degree of every node
//   :type nr_edges: ??
//  :param mt: a std::mt19937 used as a random source (defaults
//  to: std::mt19937(std::random_device()())) :type mt: ?? )pbdoc")
//
//
// Iterators
//   .def("cbegin_nodes",
//     &libsemigroups::ActionDigraph::cbegin_nodes,
//     R"pbdoc(
//    Returns a ActionDigraph::const_iterator_nodes (random access
//    iterator) pointing at the first node of the digraph.
//
//:return: An ActionDigraph::const_iterator_nodes.
//                            )pbdoc")
//    .def("cend_nodes",
//    &libsemigroups::ActionDigraph::cend_nodes,
//    R"pbdoc(
//   Returns a ActionDigraph::const_iterator_nodes (random access
//   iterator) pointing one-past-the-last node of the digraph.
//
//  :return: An ActionDigraph::const_iterator_nodes.
//                         )pbdoc")
//
//   .def("crbegin_nodes",
//   &libsemigroups::ActionDigraph::crbegin_nodes,
//  R"pbdoc(
// Returns a ActionDigraph::const_reverse_iterator_nodes (random
//  access iterator) pointing at the last node of the digraph.
//
// :return: An ActionDigraph::const_reverse_iterator_nodes.
//                                )pbdoc")
//.def("crend_nodes",
// &libsemigroups::ActionDigraph::crend_nodes,
// R"pbdoc(
//   Returns a ActionDigraph::const_reverse_iterator_nodes (random
//   access iterator) pointing one-past-the-first node of the digraph.
//
//   :return: An ActionDigraph::const_reverse_iterator_nodes.
//                          )pbdoc")
//
//
//  .def("cbegin_edges",
// 	&libsemigroups::ActionDigraph::cbegin_edges,
// py::arg("i"),
//     	R"pbdoc(
//      Returns a ActionDigraph::const_iterator_edges (random access
//       iterator) pointing at the first neighbor of the node i.
//
// 	 :param i: a node in the digraph.
// 	 :type i: ??
//
// 	 :return: An ActionDigraph::const_iterator_edges.
// 	  )pbdoc")
// 	  .def("cend_edges",
// 	&libsemigroups::ActionDigraph::cend_edges,
// 	 py::arg("i"),
// 	R"pbdoc(
// 		Returns a ActionDigraph::const_iterator_edges (random access
// 			 iterator) pointing one-past-the-last neighbor of the node i.
//
// 			 :param i: a node in the digraph.
// 			 :type i: ??
//
// 			 :return: An ActionDigraph::const_iterator_edges.
// 			  )pbdoc")
//
//
//.def("cbegin_sccs",
// 			   		&libsemigroups::ActionDigraph::cbegin_sccs,
// 			R"pbdoc(
// 			       Returns an iterator pointing to the vector of nodes in the
// 			         first scc.
//
// 			 :return: A ActionDigraph::const_iterator_sccs.
//                              )pbdoc")
//                .def("cend_sccs",
//               		&libsemigroups::ActionDigraph::cend_sccs,
//        				R"pbdoc(
//            Returns an iterator pointing one past the last vector of nodes in the final scc.
//
// 	      :return: A ActionDigraph::const_iterator_sccs.
// 	       )pbdoc")
//
//
// 	      .def("cbegin_scc",
// 	    		&libsemigroups::ActionDigraph::cbegin_scc,
// 	py::arg("i"),
// 	R"pbdoc(
// Returns an iterator pointing to the first node in the scc
//       with the specified id-number.
//
//      :param i: the id-number of the scc.
//      :type i: ??
//
//      :return: A ActionDigraph::const_iterator_scc.
//       )pbdoc")
//             .def("cend_scc",
// 	&libsemigroups::ActionDigraph::cend_scc,
// py::arg("i"),
// R"pbdoc(
//                 Returns an iterator pointing one past the last node in the scc with the specified id-number.
//                 :param i: the id-number of the scc.
//                 :type i: ??
//
// :return: A ActionDigraph::const_iterator_scc.
// )pbdoc")
//    .def("cbegin_scc_roots",
//	&libsemigroups::ActionDigraph::cbegin_scc_roots,
//  R"pbdoc(
//          Returns an iterator pointing to the root of the first scc.
//
// 	                                                                                                   				                                      :return: A ActionDigraph::const_iterator_scc_roots.
//            				                                                         )pbdoc")
//  .def("cend_scc_roots",
//  		&libsemigroups::ActionDigraph::cend_scc_roots,
//                                                                                                    				                                                         				R"pbdoc(
//          Returns an iterator pointing one past the root of the last
//      scc.
//
//       :return: A ActionDigraph::const_iterator_scc_roots.
//                                                                  )pbdoc")
//                                  .def("cbegin_panilo",
//                                          		&libsemigroups::ActionDigraph::cbegin_panilo,
//                                                                  				py::arg("source"), py::arg("min"), py::arg("max"),
//                                                                  						R"pbdoc(
//   Returns a forward iterator pointing to a pair consisting of the edge labels of the first path (in lexicographical order) starting at
//    source with length in the range $[min, max)$ and the last node of that path.
//
//                         :param source: the source node
//                                            :type source: ??
//                                                               :param min: the minimum length of a path to enumerate
//     (defaults to 0) :type min: ?? :param max: the maximum length of a path to
//         enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??
//
//    :return: An iterator it of type const_panilo_iterator
//        pointing to a std::pair where: it->first is a libsemigroups::word_type
//            consisting of the edge labels of the first path (in lexicographical order)
//                           from source of length in the range $[min, max)$; and it->second is the last
//                                    node on the path from source labelled by it->first, a value of
//                                        ActionDigraph::node_type.
//
//
//                      )pbdoc")
//                      .def("cend_panilo",
//                      		&libsemigroups::ActionDigraph::cend_panilo,
// 		R"pbdoc(
// 		                   Returns a forward iterator pointing to one after the last
// 		                       path from any node in the digraph.
//
//              )pbdoc")
//              .def("cbegin_panislo",
//              		&libsemigroups::ActionDigraph::cbegin_panislo,
//        				py::arg("source"), py::arg("min"), py::arg("max"),
//        						R"pbdoc(
//        						                   Returns a forward iterator pointing to a pair consisting of
//                        						                       the edge labels of the first path (in short-lex order) starting at source
//                        						                           with length in the range $[min, max)$ and the last node of that path.
//
//       :param source: the source node
//                          :type source: ??
//                                             :param min: the minimum length of a path to enumerate
//                                                 (defaults to 0) :type min: ?? :param max: the maximum length of a path to
//                        enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??
//
//                              :return: An iterator it of type const_panislo_iterator
//                                  pointing to a std::pair where: it->first is a libsemigroups::word_type
//                                      consisting of the edge labels of the first path (in short-lex order) from
//                                          source of length in the range $[min, max)$; and it->second is the last node
//                                              on the path from source labelled by it->first, a value of
//                                                  ActionDigraph::node_type.
//
//
//      )pbdoc")
//      .def("cend_panislo",
//      		&libsemigroups::ActionDigraph::cend_panislo,
//      				R"pbdoc(
//                 Returns a forward iterator pointing to one after the last
//                     path from any node in the digraph.
//
//          )pbdoc")
//          .def("cbegin_pilo",
//          		&libsemigroups::ActionDigraph::cbegin_pilo,
//          				py::arg("source"), py::arg("min"), py::arg("max"),
//       					R"pbdoc(
//       					                   Returns a forward iterator pointing to the edge labels of the
//       					                       first path (in lexicographical order) starting at source with length in the
//       					                           range $[min, max)$.
//
// 	      :param source: the source node
// 	                         :type source: ??
// 	 	                                                                                    :param min: the minimum length of a path to enumerate
// 	                  (defaults to 0) :type min: ?? :param max: the maximum length of a path to
// 	                      enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??
//
// 	                                                                               :return: An iterator it of type const_pilo_iterator pointing
// 	                                                                                   to a libsemigroups::word_type consisting of the edge labels of the first
// 	                                                                                       path (in lexicographical order) from source of length in the range $[min,
// 	      max)$. )pbdoc") .def("cend_pilo", &libsemigroups::ActionDigraph::cend_pilo,
// 	                                		R"pbdoc(
// 	                                		                   Returns a forward iterator pointing to one after the last
// 	 path from any node in the digraph.
//
// 	                   )pbdoc")
// 	                   .def("cbegin_pislo",
// 	                   		&libsemigroups::ActionDigraph::cbegin_pislo,
// 	                   				py::arg("source"), py::arg("min"), py::arg("max"),
// 	    				R"pbdoc(
// 	    				                   Returns a forward iterator pointing to the edge labels of the
// 	     first path (in short-lex order) starting at source with length in the range
// 	            $[min, max)$.
//
// 	                :param source: the source node
// 	                                   :type source: ??
// 	                                                      :param min: the minimum length of a path to enumerate
// 	                                                          (defaults to 0) :type min: ?? :param max: the maximum length of a path to
// 	                        enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??
//
// 	                                                     :return: An iterator it of type const_pislo_iterator pointing
// 	                                                         to a libsemigroups::word_type consisting of the edge labels of the first
// 	                                                             path (in short-lex order) from source of length in the range $[min, max)$.
// 	                   )pbdoc")
// 	        .def("cend_pislo",
// 	         		&libsemigroups::ActionDigraph::cend_pislo,
// 	                     				R"pbdoc(
// 	                     				                   Returns a forward iterator pointing to one after the last
// 	                     				                       path from any node in the digraph.
//
// 	                     )pbdoc")
// 	                     .def("cbegin_pstilo",
// 	                     		&libsemigroups::ActionDigraph::cbegin_pstilo,
// 	                     				py::arg("source"), py::arg("target"), py::arg("min"),
// 	                     						py::arg("max"), R"pbdoc( Returns a forward iterator pointing to the edge
// 	                     						    labels of the first path (in lexicographical order) starting at the node
// 	                     						        source and ending at the node target with length in the range $[min, max)$.
//
//                   						                           :param source: the first node
//                   						                                              :type source: ??
//                   						                                                                 :param target: the last node
//                   						                                                                                    :type target: ??
//                   						                                                                                                       :param min: the minimum length of a path to enumerate
//             (defaults to 0) :type min: ?? :param max: the maximum length of a path to
//                 enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??
//
//                                                                   :return: An iterator it of type const_pstilo_iterator
//                                                                       pointing to a libsemigroups::word_type consisting of the edge labels of the
//                                                                           first path (in lexicographical order) from the node source to the node
//                                                                               target with length in the range $[min, max)$ (if any). )pbdoc")
//                                                                               .def("cend_pstilo",
//                                                                              		&libsemigroups::ActionDigraph::cend_pstilo,
// 		R"pbdoc(
// 		                   Returns a forward iterator pointing to one after the last
// 		                       path from any node in the digraph.
//
//         )pbdoc")
//         .def("cbegin_pstislo",
//         		&libsemigroups::ActionDigraph::cbegin_pstislo,
//         				py::arg("source"), py::arg("target"), py::arg("min"),
//         						py::arg("max"), R"pbdoc( Returns a forward iterator pointing to the edge
//         						    labels of the first path (in short-lex order) starting at the node source
//         						        and ending at the node target with length in the range $[min, max)$.
//
// :param source: the first node
//                    :type source: ??
// :param target: the last node
//  :type target: ??
// param min: the minimum length of a path to enumerate
//    (defaults to 0) :type min: ?? :param max: the maximum length of a path to
//  enumerate (defaults to libsemigroups::POSITIVE_INFINITY). :type max: ??
//
// :return: An iterator it of type const_pstislo_iterator
// pointing to a libsemigroups::word_type consisting of the edge labels of the
// first path (in short-lex order) from the node source to the node target with
//      length in the range $[min, max)$ (if any). )pbdoc") .def("cend_pstislo",
// &libsemigroups::ActionDigraph::cend_pstislo,
// 	R"pbdoc(
//      Returns a forward iterator pointing to one after the last
//        path from any node in the digraph.
//    )pbdoc")
// ;*/
//
