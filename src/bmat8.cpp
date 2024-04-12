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

// C std headers....
// TODO complete or delete

// C++ stl headers....
// TODO complete or delete

// libsemigroups headers
#include <libsemigroups/bmat8.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_bmat8

namespace py = pybind11;

namespace libsemigroups {

  void init_bmat8(py::module& m) {
    py::class_<BMat8> thing(m,
                            "BMat8",
                            R"pbdoc(
Defined in ``bmat8.hpp``.Class for fast boolean matrices of dimension up to 8 x 8The member functions for these small matrices over the boolean semiring are more optimised than the generic member functions for boolean matrices. Note that all :any:`BMat8` are represented internally as an 8 x 8 matrix; any entries not defined by the user are taken to be 0. This does not affect the results of any calculations. :any:`BMat8` is a trivial class.)pbdoc");
    thing.def("__repr__", &detail::to_string<BMat8 const&>);
    thing.def(py::init<>(), R"pbdoc(
Default constructor.There is no guarantee about the contents of the
matrix constructed.

:parameters:
   (None)

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.
)pbdoc");
    thing.def(py::init<uint64_t>(), R"pbdoc(

:param mat: the integer representation of the matrix being constructed.
:type mat: int
Construct from uint64_t.This constructor initializes a :any:`BMat8` to have rows equal to the 8 chunks, of 8 bits each, of the binary representation of ``mat``.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.)pbdoc");
    thing.def(py::init<std::vector<std::vector<bool>> const&>(), R"pbdoc(

:param mat: the vector of vectors representation of the matrix being constructed.
:type mat: std::vector >
A constructor.This constructor initializes a matrix where the rows of the matrix are the vectors in ``mat``.

:raises LibsemigroupsError:  if ``mat`` has 0 rows.

:raises LibsemigroupsError:  if ``mat`` has more than 8 rows.

:raises LibsemigroupsError:  if the rows of ``mat`` are not all of the same length.

:complexity: Constant.)pbdoc");
    thing.def(py::init<BMat8 const&>(), R"pbdoc(
Default copy constructor.

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.
)pbdoc");
    thing.def(py::self == py::self,
              py::arg("that"),
              R"pbdoc(

:param that: the BMat8 for comparison.
:type that: BMat8
Returns ``True`` if ``self`` equals ``that``.This member function checks the mathematical equality of two :any:`BMat8` objects.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.)pbdoc");
    thing.def(py::self != py::self,
              py::arg("that"),
              R"pbdoc(

:param that: the BMat8 for comparison.
:type that: BMat8
Returns ``True`` if ``self`` does not equal ``that``This member function checks the mathematical inequality of two :any:`BMat8` objects.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.)pbdoc");
    thing.def(py::self < py::self,
              py::arg("that"),
              R"pbdoc(

:param that: the BMat8 for comparison.
:type that: BMat8
Returns ``True`` if ``self`` is less than ``that``.This member function checks whether a :any:`BMat8` objects is less than another. We order by the results of :any:`to_int()` for each matrix.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.)pbdoc");
    thing.def(py::self > py::self,
              py::arg("that"),
              R"pbdoc(

:param that: the BMat8 for comparison.
:type that: BMat8
Returns ``True`` if ``self`` is greater than ``that``.This member function checks whether a :any:`BMat8` objects is greater than another. We order by the results of :any:`to_int()` for each matrix.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.)pbdoc");
    thing.def("get",
              &BMat8::get,
              py::arg("i"),
              py::arg("j"),
              R"pbdoc(

:param i: the row of the entry sought.
:type i: int

:param j: the column of the entry sought.
:type j: int
Returns the entry in the ( ``i`` , ``j`` )th position.This member function returns the entry in the ( ``i`` , ``j`` )th position.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.


:returns: A ``bool``.

:rtype: bool
)pbdoc");
    thing.def("set",
              &BMat8::set,
              py::arg("i"),
              py::arg("j"),
              py::arg("val"),
              R"pbdoc(

:param i: the row
:type i: int

:param j: the column
:type j: int

:param val: the value to set in position (i, j)th
:type val: bool
Sets the ( ``i`` , ``j`` )th position to ``val``.This member function sets the ( ``i`` , ``j`` )th entry of ``self`` to ``val`` . Uses the bit twiddle for setting bits found`here <http://graphics.stanford.edu/~seander/bithacks>`_.

:raises LibsemigroupsError:  if ``i`` or ``j`` is out of bounds.

:complexity: Constant.


:returns:  (None)

:rtype: None
)pbdoc");
    thing.def("to_int",
              &BMat8::to_int,
              R"pbdoc(
Returns the integer representation of ``self``.Returns an unsigned
integer obtained by interpreting an 8 x 8 :any:`BMat8` as a sequence of
64 bits (reading rows left to right, from top to bottom) and then
realising this sequence as an unsigned int.

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.

:parameters:
   (None)

:returns:
   A ``int``.

:rtype:
   int
)pbdoc");
    thing.def("transpose",
              &BMat8::transpose,
              R"pbdoc(
Returns the transpose of ``self``.Uses the technique found in`Knu09
<../biblio.html#knuth2009aa>`_.

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.

:parameters:
   (None)

:returns:
   A :any:`BMat8`.

:rtype:
   BMat8
)pbdoc");
    thing.def(py::self * py::self,
              py::arg("that"),
              R"pbdoc(

:param that: the matrix we want to multiply by this.
:type that: BMat8
Returns the matrix product of ``self`` and ``that``This member function returns the standard matrix product (over the boolean semiring) of two :any:`BMat8` objects. Uses the technique given`here <https://stackoverflow.com/a/18448513>`_.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.


:returns: A :any:`BMat8`.

:rtype: BMat8
)pbdoc");
    thing.def("swap",
              &BMat8::swap,
              py::arg("that"),
              R"pbdoc(

:param that: the BMat8 to swap this with.
:type that: BMat8
Swaps ``self`` with ``that``.This member function swaps the values of ``self`` and ``that``.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.


:returns:  (None)

:rtype: None
)pbdoc");
    thing.def("row_space_basis",
              &BMat8::row_space_basis,
              R"pbdoc(
Find a basis for the row space of ``self``.This member function returns
a :any:`BMat8` whose non-zero rows form a basis for the row space of
``self``.

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.

:parameters:
   (None)

:returns:
   A :any:`BMat8`.

:rtype:
   BMat8
)pbdoc");
    thing.def("col_space_basis",
              &BMat8::col_space_basis,
              R"pbdoc(
Find a basis for the column space of ``self``.This member function
returns a :any:`BMat8` whose non-zero columns form a basis for the
column space of ``self``.

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.

:parameters:
   (None)

:returns:
   A :any:`BMat8`.

:rtype:
   BMat8
)pbdoc");
    thing.def("rows",
              &BMat8::rows,
              R"pbdoc(
Returns a vector containing the rows of ``self``This member function
returns a :any:`list` of uint8_ts representing the rows of ``self`` .
The vector will always be of length 8, even if ``self`` was constructed
with fewer rows.

:exceptions:
   This function guarantees not to throw a ``LibsemigroupsError``.

:complexity:
   Constant.

:parameters:
   (None)

:returns:
   A std::vector<uint8_t>.

:rtype:
   list
)pbdoc");
    thing.def("row_space_size",
              &BMat8::row_space_size,
              R"pbdoc(
Find the size of the row space of ``self``.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.

:complexity:  :math:`O(n)` where :math:`n` is the return value of this function.

:parameters:  (None)

.. seealso::  :any:`bmat8::col_space_size`.

:returns: A ``int``.

:rtype: int
)pbdoc");
    thing.def("number_of_rows",
              &BMat8::number_of_rows,
              R"pbdoc(
Returns the number of non-zero rows in ``self``.BMat8s do not know their "dimension" - in effect they are all of dimension 8. However, this member function can be used to obtain the number of non-zero rows of ``self``.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:parameters:  (None)

.. seealso::  :any:`bmat8::number_of_cols` and :any:`bmat8::minimum_dim`.

:returns: A ``int``.

:rtype: int
)pbdoc");
    thing.def("is_regular_element",
              &BMat8::is_regular_element,
              R"pbdoc(
Check whether ``self`` is a regular element of the full boolean matrix
monoid of appropriate dimension.

:exceptions:
   This function is ``noexcept`` and is guaranteed never to throw.

:complexity:
   Constant.

:parameters:
   (None)

:returns:
   A ``True`` if there exists a boolean matrix ``y`` such that ``x * y *
   x = x`` where ``x`` is ``self``.

:rtype:
   bool
)pbdoc");
    thing.def_static(
        "random",
        [](size_t dim) { return BMat8::random(dim); },
        py::arg("dim") = 8,
        R"pbdoc(
Construct a random :any:`BMat8` of dimension at most ``dim``.This static
member function returns a :any:`BMat8` chosen at random, where only the
top-left ``dim`` x ``dim`` entries can be non-zero.

:exceptions:
   This function guarantees not to throw a ``LibsemigroupsError``.

:parameters:
   (None)

:returns:
   A :any:`BMat8`.

:rtype:
   BMat8
)pbdoc");
    thing.def_static("one",
                     &BMat8::one,
                     py::arg("dim"),
                     R"pbdoc(

:param dim: the dimension of the identity (default: 8)
:type dim: int
Returns the identity :any:`BMat8`.This member function returns the :any:`BMat8` with the first ``dim`` entries in the main diagonal equal to ``1`` and every other value equal to ``0``.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.


:returns: A :any:`BMat8`.

:rtype: BMat8
)pbdoc");

    m.def("number_of_cols",
          &bmat8::number_of_cols,
          py::arg("x"),
          R"pbdoc(

:param x: the BMat8 whose number of columns we want.
:type x: BMat8
Returns the number of non-zero columns in ``x``.BMat8s do not know their "dimension" - in effect they are all of dimension 8. However, this member function can be used to obtain the number of non-zero rows of ``self``.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

.. seealso::  :any:`BMat8::number_of_rows` and :any:`bmat8::minimum_dim`.


:returns: A ``int``.

:rtype: int
)pbdoc");
    m.def("col_space_size",
          &bmat8::col_space_size,
          py::arg("x"),
          R"pbdoc(

:param x: a BMat8.
:type x: BMat8
Find the size of the column space of ``x``.

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.

:complexity:  :math:`O(n)` where :math:`n` is the return value of this function.

.. seealso::  :any:`BMat8::row_space_size`.


:returns: A ``int``.

:rtype: int
)pbdoc");
    m.def("minimum_dim",
          &bmat8::minimum_dim,
          py::arg("x"),
          R"pbdoc(

:param x: a BMat8.
:type x: BMat8
Find the minimum dimension of ``x``.This member function returns the maximal ``i`` such that row ``i`` or column ``i`` contains a ``1``.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.)pbdoc");
  }
}  // namespace libsemigroups
