/*   .def("scc_id",
        &ActionDigraph<node_type>::scc_id,
        py::arg("nd"),
        R"pbdoc(
              Returns the id-number of the strongly connected component
              of a node.

              :param nd: the node.
              :type nd: int

              :Parameters: None
              :return: An ``int``.
            )pbdoc")
   .def("number_of_scc",
        &ActionDigraph<node_type>::number_of_scc,
        R"pbdoc(
              Returns the number of strongly connected components.

              :Parameters: None
              :return: An ``int``.
            )pbdoc")
   .def("root_of_scc",
        &ActionDigraph<node_type>::root_of_scc,
        py::arg("nd"),
        R"pbdoc(
              Returns the root of a strongly connected components
              containing a given node.

              :param nd: a node.
              :type nd: int

              :return: An ``int``.
            )pbdoc")
   .def("spanning_forest",
        &ActionDigraph<node_type>::spanning_forest,
        py::return_value_policy::copy,  // to ensure the Forest
        lives !R"pbdoc(
              Returns a :py:class:`Forest` comprised of spanning trees
              for each scc of this, rooted at the minimum node of that
              component, with edges oriented away from the root.

              :return: A :py:class:`Forest`.
            )pbdoc")
   .def("reverse_spanning_forest",
        &ActionDigraph<node_type>::reverse_spanning_forest,
        R"pbdoc(
              Returns a :py:class:`Forest` comprised of spanning trees
              for each scc of this, rooted at the minimum node of that
              component, with edges oriented towards the root.

              :return: A :py:class:`Forest`.
            )pbdoc")
.def(
    "sccs_iterator",
    [](WordGraph<node_type> const& ad) {
      return py::make_iterator(ad.cbegin_sccs(), ad.cend_sccs());
    },
    R"pbdoc(
              Returns an iterator for the nodes in the scc.
            )pbdoc")
    .def(
        "scc_iterator",
        [](WordGraph<node_type> const& ad, node_type const i) {
          return py::make_iterator(ad.cbegin_scc(i), ad.cend_scc(i));
        },
        R"pbdoc(
              Returns an iterator pointing to the first node in the scc
              with the specified id-number.
            )pbdoc")
    .def(
        "scc_roots_iterator",
        [](WordGraph<node_type> const& ad) {
          return py::make_iterator(ad.cbegin_scc_roots(), ad.cend_scc_roots());
        },
        R"pbdoc(
              Returns an iterator pointing to the root of the first scc.
            )pbdoc")
           .def("scc_id",
         &ActionDigraph<node_type>::scc_id,
         py::arg("nd"),
         R"pbdoc(
               Returns the id-number of the strongly connected component
               of a node.

               :param nd: the node.
               :type nd: int

               :Parameters: None
               :return: An ``int``.
             )pbdoc")
    .def("number_of_scc",
         &ActionDigraph<node_type>::number_of_scc,
         R"pbdoc(
               Returns the number of strongly connected components.

               :Parameters: None
               :return: An ``int``.
             )pbdoc")
    .def("root_of_scc",
         &ActionDigraph<node_type>::root_of_scc,
         py::arg("nd"),
         R"pbdoc(
               Returns the root of a strongly connected components
               containing a given node.

               :param nd: a node.
               :type nd: int

               :return: An ``int``.
             )pbdoc")
    .def("spanning_forest",
         &ActionDigraph<node_type>::spanning_forest,
         py::return_value_policy::copy,  // to ensure the Forest
         lives !R"pbdoc(
               Returns a :py:class:`Forest` comprised of spanning trees
               for each scc of this, rooted at the minimum node of that
               component, with edges oriented away from the root.

               :return: A :py:class:`Forest`.
             )pbdoc")
    .def("reverse_spanning_forest",
         &ActionDigraph<node_type>::reverse_spanning_forest,
         R"pbdoc(
               Returns a :py:class:`Forest` comprised of spanning trees
               for each scc of this, rooted at the minimum node of that
               component, with edges oriented towards the root.

               :return: A :py:class:`Forest`.
             )pbdoc")
       :Parameters: - **source** (int) the first node.
                           - **min** (int) the minimum length of a path to
                             enumerate (defaults to ``0``)
                           - **max** (Union[int, PositiveInfinity]) the
                           maximum
                             length of a path to enumerate (defaults to
                             :py:obj:`POSITIVE_INFINITY`).

              :Returns: An iterator.
            )pbdoc")
*/
