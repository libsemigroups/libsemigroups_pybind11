//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2022 James D. Mitchell
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

// libsemigroups....
#include <libsemigroups/schreier-sims.hpp>  // for SchreierSims

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, init, make_iterator, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_schreier_sims

namespace py = pybind11;

namespace libsemigroups {

  void init_schreier_sims(py::module& m) {
    py::class_<SchreierSims<256>>(m, "SchreierSims").def(py::init<>());
    /*
        .def(py::init<SchreierSims&&>())
        .def("add_base_point",
             &SchreierSims::add_base_point,
             py::arg("pt"),
             R"pbdoc(
               Add a base point to the stabiliser chain.

               :param pt: the base point to add.
               :type pt: ??

               :return: (None)
               )pbdoc")
        .def("add_generator",
             &SchreierSims::add_generator,
             py::arg("x"),
             R"pbdoc(
               Add a generator.

               :param x: a const reference to the generator to add.
               :type x: ??

               :return: (None)
               )pbdoc")
        .def("clear",
             &SchreierSims::clear,
             R"pbdoc(
               Reset to the trivial group.

               :return: (None)
               )pbdoc")
        .def("run",
             &SchreierSims::run,
             R"pbdoc(
               Run the Schreier-Sims algorithm.

               :return: (None)
               )pbdoc")
        .def("base",
             &SchreierSims::base,
             py::arg("index"),
             R"pbdoc(
               Get a base point.

               :param index: the index of the base point.
               :type index: ??

               :return: the base point with index index.
               )pbdoc")
        .def("base_size",
             &SchreierSims::base_size,
             R"pbdoc(
               Get the size of the current base.

               :return: A size_t.
               )pbdoc")
        .def("empty",
             &SchreierSims::empty,
             R"pbdoc(
               Check if any generators have been added so far.

               :return: true if number_of_generators() == 0 and false otherwise.
               )pbdoc")
        .def("finished",
             &SchreierSims::finished,
             R"pbdoc(
               Check if the stabiliser chain is fully enumerated.

               :return: true if the stabiliser chain is fully enumerated and
       false otherwise. )pbdoc") .def("number_of_generators",
             &SchreierSims::number_of_generators,
             R"pbdoc(
               The number of generators.

               :return: The number of generators, a value of size_t.
               )pbdoc")
        .def("number_of_strong_generators",
             &SchreierSims::number_of_strong_generators,
             py::arg("depth"),
             R"pbdoc(
               The number of strong generators at a given depth.

               :param depth: the depth.
               :type depth: ??

               :return: The number of strong generators, a value of size_t, at
       depth depth of the stabiliser chain. )pbdoc") .def("strong_generator",
             &SchreierSims::strong_generator,
             py::arg("depth"),
             py::arg("index"),
             R"pbdoc(
               Get a strong generator.

               :param depth: the depth.
               :type depth: ??
               :param index: the index of the generator we want.
               :type index: ??

               :return: A const reference to the strong generator of this at
       depth depth and with index index. )pbdoc") .def("size",
             &SchreierSims::size,
             R"pbdoc(
               Returns the size of the group represented by this.

               :return: the size, a value of uint64_t.
               )pbdoc")
        .def("identity",
             &SchreierSims::identity,
             R"pbdoc(
               Returns a const reference to the identity.

               :return: A bool.
               )pbdoc")
        .def("generator",
             &SchreierSims::generator,
             py::arg("index"),
             R"pbdoc(
               Get a generator.

               :param index: the index of the generator we want.
               :type index: ??

               :return: A const reference to the generator of this with index
       index. )pbdoc") .def("transversal_element",
             &SchreierSims::transversal_element,
             py::arg("depth"),
             py::arg("pt"),
             R"pbdoc(
               Get a transversal element.

               :param depth: the depth.
               :type depth: ??
               :param pt: the image of the base point under the traversal.
               :type pt: ??

               :return: A const reference to the transversal element of this at
       depth depth moving the corresponding basepoint to the point pt. )pbdoc")
        .def("inversal_element",
             &SchreierSims::inversal_element,
             py::arg("depth"),
             py::arg("pt"),
             R"pbdoc(
               Get an inversal element.

               :param depth: the depth.
               :type depth: ??
               :param pt: the point to map to the base point under the inversal.
               :type pt: ??

               :return: A const reference to the insversal element of this at
       depth depth moving the corresponding point pt to the basepoint. )pbdoc")
        .def("orbits_lookup",
             &SchreierSims::orbits_lookup,
             py::arg("depth"),
             py::arg("pt"),
             R"pbdoc(
               Check if a point is in the orbit of a basepoint.

               :param depth: the depth.
               :type depth: ??
               :param pt: the point.
               :type pt: ??

               :return: A boolean indicating if the point pt is in the orbit of
       the basepoint of this at depth depth. )pbdoc") .def("contains",
             &SchreierSims::contains,
             py::arg("x"),
             R"pbdoc(
               Test membership of an element.

               :param x: a const reference to the possible element.
               :type x: ??

               :return: A bool.
               )pbdoc")
        .def("sift",
             &SchreierSims::sift,
             py::arg("x"),
             R"pbdoc(
               Sift an element through the stabiliser chain.

               :param x: a const reference to a group element.
               :type x: ??

               :return: A value of type element_type.
               )pbdoc");*/
  }
}  // namespace libsemigroups
