//
// libsemigroups - C++ library for semigroups and monoids
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

// TODO(later):
// 1) RowViews

namespace py = pybind11;
namespace libsemigroups {
  void init_matrix(py::module &m) {
    using Row         = typename BMat<>::Row;
    using scalar_type = typename BMat<>::scalar_type;
    py::class_<BMat<>>(m,
                       "BMat",
                       R"pbdoc(
                         This page describes the functionality for :math:`m
                         \times n` boolean matrices for arbitrary dimensions
                         :math:`m` and :math:`n`.
                       )pbdoc")
        .def(py::init<size_t, size_t>(), R"pbdoc(TODO)pbdoc")
        .def(py::init<BMat<> const &>())
        .def(py::init<std::vector<std::vector<scalar_type>> const &>(),
             R"pbdoc(TODO)pbdoc")
        .def_static("make_identity",
                    py::overload_cast<size_t>(&BMat<>::identity),
                    py::arg("n"),
                    R"pbdoc(
                      Construct the :math:`n \times n` identity matrix.

                      :param n: the dimension

                      :returns: The :math:`n \times n` identity matrix.
                    )pbdoc")
        .def("__repr__",
             [](BMat<> const &x) {
               std::string result = detail::to_string(x);
               std::replace(result.begin(), result.end(), '{', '[');
               std::replace(result.begin(), result.end(), '}', ']');
               return std::string("BMat(") + result + ")";
             })
        .def(pybind11::self == pybind11::self,
             py::arg("that"),
             R"pbdoc(
               Equality comparison.

               Returns ``True`` if ``self`` equals ``that`` by comparing their
               image values.

               :param that: the ``BMat`` for comparison.
               :type that: BMat

               :returns: A ``bool``.
             )pbdoc")
        .def(pybind11::self < pybind11::self,
             py::arg("that"),
             R"pbdoc(
               Less than comparison.

               Returns ``True`` if ``self`` is less than ``that``.

               :param that: the ``BMat`` for comparison.
               :type that: BMat

               :returns: A ``bool``.
             )pbdoc")
        .def(pybind11::self > pybind11::self)
        .def(pybind11::self != pybind11::self)
        .def("__imul__",
             [](BMat<> &mat, scalar_type a) {
               mat *= a;
               return mat;
             })
        .def("__iadd__",
             [](BMat<> &mat, BMat<> const &that) {
               mat += that;
               return mat;
             })
        // TODO(later) no implemented in libsemigroups
        // .def("__iadd__",
        //      [](BMat<> &mat, scalar_type a) {
        //        mat += a;
        //        return mat;
        //      })
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self * pybind11::self)
        .def("product_inplace",
             &BMat<>::product_inplace,
             py::arg("x"),
             py::arg("y"),
             R"pbdoc(
            Multiply two ``BMat`` objects and store the product in ``self``.

            :param x: a ``BMat``.
            :type x: BMat
            :param y: a ``BMat``.
            :type y: BMat

            :return: (None)
          )pbdoc")
        .def(
            "transpose",
            [](BMat<> &x) { x.transpose(); },
            R"pbdoc(
               Transposes the matrix in-place.

               :parameters: (None)
               :returns: (None)

               .. warning::
                 This only works for square matrices.
             )pbdoc")
        .def(
            "number_of_rows",
            [](BMat<> const &x) { return x.number_of_rows(); },
            R"pbdoc(
              Returns the number of rows in ``self``.

              .. code-block:: python

                 from libsemigroups_pybind11 import BMat
                 x = BMat([[0, 1], [1, 0]])
                 x.number_of_rows() # returns 2
            )pbdoc")
        .def(
            "__getitem__",
            [](const BMat<> &mat, py::tuple xy) {
              return mat(xy[0].cast<size_t>(), xy[1].cast<size_t>());
            },
            py::arg("tuple"),
            py::is_operator(),
            R"pbdoc(
              Returns value in a given position.

              :param i: a tuple consisting of the row and column indices.
              :type i: tuple

              :return: An ``int``.
            )pbdoc")
        .def(
            "number_of_cols",
            [](BMat<> const &x) { return x.number_of_cols(); },
            R"pbdoc(
              Returns the number of columns in ``self``.

              .. code-block:: python

                 from libsemigroups_pybind11 import BMat
                 x = BMat([[0, 1], [1, 0]])
                 x.number_of_rows() # returns 2
            )pbdoc")
        .def("swap",
             &BMat<>::swap,
             py::arg("that"),
             R"pbdoc(
              Swaps the contents of ``this`` with the contents of ``that``.

              :param that: the matrix to swap contents with

              :returns: (None)
            )pbdoc")
        .def(
            "row",
            [](BMat<> const &x, size_t i) { return Row(x.row(i)); },
            R"pbdoc(
               Returns the specified row.

               :param i: the index of the row

               :returns: A ``BMat``.
             )pbdoc")
        .def(
            "rows",
            [](BMat<> const &x) {
              std::vector<Row> rs;
              for (size_t i = 0; i < x.number_of_rows(); ++i) {
                rs.push_back(Row(x.row(i)));
              }
              return rs;
            },
            R"pbdoc(
               Returns the specified row.

               :param i: the index of the row

               :returns: A ``BMat``.
             )pbdoc");
  }
}  // namespace libsemigroups
