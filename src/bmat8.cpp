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

// C std headers....
#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint64_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/bmat8.hpp>   // for BMat8, col_space_size, minimum_dim
#include <libsemigroups/string.hpp>  // for to_string

// pybind11....
#include <pybind11/operators.h>  // for self, operator*, self_t
#include <pybind11/pybind11.h>   // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_bmat8

namespace py = pybind11;

namespace libsemigroups {
  void init_bmat8(py::module& m) {
    py::class_<BMat8>(m, "BMat8")
        .def(py::init<>(), R"pbdoc(
      Returns an uninitialised BMat8.
      )pbdoc")
        .def(py::init<uint64_t>())
        .def(py::init<BMat8 const&>())
        .def(py::init<std::vector<std::vector<bool>> const&>())
        .def("__eq__", &BMat8::operator==)
        .def("__lt__", &BMat8::operator<)  // NOLINT(whitespace/operators)
        .def("get",
             &BMat8::get,
             py::arg("i"),
             py::arg("j"),
             R"pbdoc(
               Returns the entry in the (i, j)th position.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.get(0, 1)
                  True
                  >>> x.get(1, 1)
                  False
             )pbdoc")

        .def("set",
             &BMat8::set,
             py::arg("i"),
             py::arg("j"),
             py::arg("val"),
             R"pbdoc(
               Sets the (i, j)th entry to ``val``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.set(1,1,1)
                  >>> x
                  01000000
                  11000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
              )pbdoc")

        .def("to_int",
             &BMat8::to_int,
             R"pbdoc(
               Returns the integer representation of the ``BMat8``, that is an
               integer obtained by interpreting an 8 x 8 ``BMat8`` as a
               sequence of 64 bits (reading rows left to right, from top to
               bottom) and then realising this sequence as an integer.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.to_int()
                  4647714815446351872
             )pbdoc")

        .def("transpose",
             &BMat8::transpose,
             R"pbdoc(
               Returns the transpose of ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[1, 0], [1, 0]])
                  >>> x.transpose()
                  11000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
              )pbdoc")

        .def(py::self * py::self)
        .def_static("random", py::overload_cast<>(&BMat8::random))
        .def_static("random", py::overload_cast<size_t>(&BMat8::random))
        .def("swap",
             &BMat8::swap,
             py::arg("other"),
             R"pbdoc(
               Swaps the contents of ``self`` and ``other``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> y = BMat8([[1, 1], [0, 0]])
                  >>> BMat8.swap(x,y)
                  >>> x
                  11000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
                  >>> y
                  01000000
                  10000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
             )pbdoc")
        .def("row_space_basis",
             &BMat8::row_space_basis,
             R"pbdoc(
               This method returns a ``BMat8`` whose non-zero rows form a basis for
               the row space of ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.row_space_basis()
                  10000000
                  01000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
             )pbdoc")
        .def("col_space_basis",
             &BMat8::col_space_basis,
             R"pbdoc(
               This method returns a ``BMat8`` whose non-zero columns form a basis for
               the column space of ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.col_space_basis()
                  10000000
                  01000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
             )pbdoc")

        .def("rows",
             &BMat8::rows,
             R"pbdoc(
               This method returns a list of integers representing the rows of
               ``self``. The list will always be of length 8, even if ``self``
               was constructed with fewer rows.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.rows()
                  [64, 128, 0, 0, 0, 0, 0, 0]
              )pbdoc")

        .def("row_space_size",
             &BMat8::row_space_size,
             R"pbdoc(
               Returns the size of the row space of ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.row_space_size()
                  4

             )pbdoc")
        .def("number_of_rows",
             &BMat8::number_of_rows,
             R"pbdoc(
               Returns the number of non-zero rows in ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.number_of_rows()
                  2
              )pbdoc")
        .def("is_regular_element",
             &BMat8::is_regular_element,
             R"pbdoc(
               Check whether ``self`` is a regular element of the full boolean
               matrix monoid of appropriate dimension.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.is_regular_element()
                  True
             )pbdoc")
        .def("one",
             &BMat8::one,  // py::arg("dim") = 8,
             R"pbdoc(
               This method returns the ``BMat8`` with the first ``dim`` entries
               in the main diagonal equal to ``1`` and every other value equal
               to ``0``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> BMat8.one(4)
                  10000000
                  01000000
                  00100000
                  00010000
                  00000000
                  00000000
                  00000000
                  00000000
                  <BLANKLINE>
             )pbdoc")
        .def("__repr__", &detail::to_string<BMat8>)
        .def("number_of_cols",
             &bmat8_helpers::number_of_cols,
             R"pbdoc(
               Returns the number of non-zero columns in ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.number_of_cols()
                  2
             )pbdoc")
        .def("column_space_size",
             &bmat8_helpers::col_space_size,
             R"pbdoc(
               Returns the size of the row space of ``self``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.column_space_size()
                  4
             )pbdoc")
        .def("minimum_dim",
             &bmat8_helpers::minimum_dim,
             R"pbdoc(
               This method returns the maximal ``i`` such that row ``i``
               or column ``i`` contains a ``1``.

               .. doctest::

                  >>> from libsemigroups_pybind11 import BMat8
                  >>> x = BMat8([[0, 1], [1, 0]])
                  >>> x.minimum_dim()
                  2
             )pbdoc");
  }
}  // namespace libsemigroups
