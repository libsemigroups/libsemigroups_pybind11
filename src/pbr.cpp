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

#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <libsemigroups/libsemigroups.hpp>

#include "main.hpp"

namespace py = pybind11;

namespace libsemigroups {

  void init_pbr(py::module &m) {
    py::class_<PBR>(m,
                    "PBR",
                    R"pbdoc(
                     Class for partitioned binary relations (PBR).

                     Partitioned binary relations (PBRs) are a generalisation
                     of bipartitions, which were introduced by
                     `Martin and Mazorchuk <https://arxiv.org/abs/1102.0862>`_.
                   )pbdoc")
        .def("identity",
             py::overload_cast<>(&PBR::identity, py::const_),
             R"pbdoc(
               Returns the identity ``PBR`` with degree :py:meth:`degree`.

               :Parameters: None.
               :Returns: A ``PBR``.
             )pbdoc")
        .def_static("make_identity",
                    py::overload_cast<size_t>(&PBR::identity),
                    py::arg("n"),
                    R"pbdoc(
                      Returns the identity ``PBR`` with specified degree.

                      :Parameters: **n** (int) - the degree.

                      :Returns: A ``PBR``.
                    )pbdoc")
        // TODO(later) implement this in libsemigroups
        // .def_static("make",
        //             &PBR::make<std::vector<uint32_t>,
        //                                       std::vector<uint32_t>>,
        //             py::arg("left"),
        //             py::arg("right"),
        //             R"pbdoc(
        //        Construct and validate.

        //        :Parameters: - **left** (??) - the 1st argument to forward to
        //        the constructor.
        //                     - **right** (??) - the 2nd argument to forward to
        //                     the constructor.

        //        :Returns: A PBR constructed from args and validated.
        //        )pbdoc")
        .def_static("make",
                    &PBR::make<typename PBR::vector_type>,
                    py::arg("adj"),
                    R"pbdoc(
                      Construct and validate.

                      :Parameters: - **adj** (List[List[int]]) - the list of adjacencies.

                      :Returns: A new ``PBR``.
                    )pbdoc")
        .def(
            "__getitem__",
            [](const PBR &a, size_t b) { return a[b]; },
            py::arg("i"),
            py::is_operator(),
            R"pbdoc(
              Returns the nodes adjacent to the given node.

              :param i: an integer
              :type i: int

              :return: A ``int``.
            )pbdoc")
        .def(pybind11::self == pybind11::self,
             py::arg("that"),
             R"pbdoc(
               Equality comparison.

               Returns ``True`` if ``self`` equals ``that`` by comparing their
               image values.

               :param that: the ``PBR`` for comparison.
               :type that: PBR

               :returns: A ``bool``.
             )pbdoc")
        .def(pybind11::self < pybind11::self,
             py::arg("that"),
             R"pbdoc(
               Less than comparison.

               Returns ``True`` if ``self`` is less than ``that``.

               :param that: the ``PBR`` for comparison.
               :type that: PBR

               :returns: A ``bool``.
             )pbdoc")
        .def(py::self * py::self,
             py::arg("that"),
             R"pbdoc(
               Right multiply ``self`` by ``that``.

               :param that: the ``PBR`` to multiply with.
               :type that: PBR

               :returns: A ``PBR``.
             )pbdoc")
        .def("degree",
             &PBR::degree,
             R"pbdoc(
               Returns the degree of a PBR.

               :Parameters: None.
               :return: An ``int``.
             )pbdoc")
        .def("product_inplace",
             &PBR::product_inplace,
             py::arg("x"),
             py::arg("y"),
             py::arg("thread_id"),
             R"pbdoc(
               Multiply two ``PBR`` objects and store the product in ``self``.

               :param x: a ``PBR``.
               :type x: PBR
               :param y: a ``PBR``.
               :type y: PBR
               :param thread_id: the index of the calling thread (defaults to ``0``).
               :type thread_id: int

               :return: (None)
             )pbdoc")
        .def("__hash__", &PBR::hash_value);
  }
}  // namespace libsemigroups
