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
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/pbr.hpp>  // for PBR, PBR::vector_type, operator*

// pybind11....
#include <pybind11/operators.h>  // for self, operator*, operator<, oper...
#include <pybind11/pybind11.h>   // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_pbr

namespace py = pybind11;

namespace libsemigroups {
  template <typename T>
  using vector_type = typename PBR::vector_type<T>;

  void init_pbr(py::module& m) {
    py::class_<PBR>(m, "PBR")
        .def(py::init<PBR const&>(),
             py::arg("that"),
             R"pbdoc(
               Copy constructor.

               :param that: the ``PBR`` to copy.
               :type that: PBR
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
        .def_static("make",
                    &PBR::make<vector_type<int32_t>, vector_type<int32_t>>,
                    py::arg("left"),
                    py::arg("right"),
                    R"pbdoc(
                      Construct and validate.

                      :Parameters: - **left** (List[List[int]]) - the 1st argument to forward to the constructor.
                                   - **right** (List[List[int]]) - the 2nd argument to forward to the constructor.

                      :Returns: A PBR constructed from left and right, and validated.
                    )pbdoc")
        .def_static("make",
                    &PBR::make<vector_type<uint32_t>>,
                    py::arg("adj"),
                    R"pbdoc(
                      Construct and validate.

                      :Parameters: - **adj** (List[List[int]]) - the list of adjacencies.

                      :Returns: A new ``PBR``.
                    )pbdoc")
        .def(
            "__getitem__",
            [](const PBR& a, size_t b) { return a[b]; },
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
