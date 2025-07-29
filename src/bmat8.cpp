
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

// TODO
// * iwyu

// libsemigroups headers
#include <libsemigroups/bmat8.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_bmat8

namespace libsemigroups {
  namespace py = pybind11;

  void init_bmat8(py::module& m) {
    py::class_<BMat8> thing(m,
                            "BMat8",
                            R"pbdoc(
Fast boolean matrices of dimension up to 8 x 8.

Instance of this class represent 8 x 8 matrices over the boolean semiring. The
functions for these small matrices are more optimised than the generic
functions for boolean matrices. Note that all :any:`BMat8` are represented
internally as an 8 x 8 matrix; any entries not defined by the user are taken to
be ``0``. This does not affect the results of any calculations.

There are numerous functions for computing things about :any:`BMat8` objects in
the submodule ``bmat8``.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> x[1, 1] = 1
   >>> x
   BMat8([[0, 1],
          [1, 1]])
   >>> x[0, 1]
   True
   >>> x[1, 1]
   True
   >>> x * x
   BMat8([[1, 1],
          [1, 1]])
   >>> x < x * x
   True
   >>> x *= x
   >>> x
   BMat8([[1, 1],
          [1, 1]])
   >>> x.to_int()
   13889101250810609664
   >>> bin(x.to_int())
   '0b1100000011000000000000000000000000000000000000000000000000000000'
   >>> x == BMat8([[1, 1, 0], [1, 1, 0], [0, 0, 0]]) # All BMat8's are really 8x8!
   True
   >>> y = BMat8([[1, 0, 1], [0, 1, 0], [0, 0, 0]])
   >>> y[0]  # The first row
   [True, False, True, False, False, False, False, False]
   >>> x + y
   BMat8([[1, 1, 1],
          [1, 1, 0],
          [0, 0, 0]])
   >>> x += y
   >>> x
   BMat8([[1, 1, 1],
          [1, 1, 0],
          [0, 0, 0]])
   >>> 1 * x == x
   True
   >>> x * 0
   BMat8(0)

:any:`BMat8` objects can be used with the following algorithms in
``libsemigroups_pybind11``

* :any:`FroidurePin`
* :any:`Konieczny`
* :any:`Action`
)pbdoc");

    // The next function __len__ is not really required, but without this the
    // error messages from, e.g. FroidurePin.current_position gives misleading
    // error messages in:
    // >>> from libsemigroups_pybind11 import FroidurePin, Perm, BMat8
    // >>> S = FroidurePin(Perm([1, 0, 2, 3, 4, 5, 6]),
    // ...                 Perm([1, 2, 3, 4, 5, 0, 6]))
    // >>> S.current_position(BMat8(0))
    // TypeError: object of type '_libsemigroups_pybind11.BMat8' has no len()
    //
    // when it should be:
    //
    // TypeError: current_position(): incompatible function arguments. The
    // following argument types are supported:
    //  1. (self: _libsemigroups_pybind11.FroidurePinPerm16, x:
    //  _libsemigroups_pybind11.StaticPerm16) -> int
    //  2. (self: _libsemigroups_pybind11.FroidurePinBase, w: list[int]) -> int
    //  3. (self: _libsemigroups_pybind11.FroidurePinBase, i: int) -> int
    thing.def("__len__", [](BMat8 const& x) { return 8; });
    thing.def("__repr__",
              [](BMat8 const& x) { return to_human_readable_repr(x, "[]"); });
    thing.def(
        "__setitem__",
        [](BMat8& x, std::pair<size_t, size_t> tup, bool val) {
          x.at(tup.first, tup.second) = val;
        },
        py::is_operator());
    thing.def(
        "__getitem__",
        [](BMat8 const& x, std::pair<size_t, size_t> tup) {
          return x.at(tup.first, tup.second);
        },
        py::is_operator());
    thing.def(
        "__getitem__",
        [](BMat8 const& x, size_t r) { return bmat8::to_vector(x.at(r)); },
        py::is_operator());
    thing.def(
        "__hash__",
        [](BMat8 const& x) { return Hash<BMat8>()(x); },
        py::is_operator());
    thing.def(py::self == py::self);
    thing.def(py::self != py::self);
    thing.def(py::self <= py::self);
    thing.def(py::self >= py::self);

    thing.def(py::self += py::self);
    thing.def(py::self + py::self);
    thing.def(py::self * bool());
    thing.def(bool() * py::self);
    thing.def(py::self < py::self);
    thing.def(py::self > py::self);
    thing.def(py::self * py::self);
    thing.def(py::self *= py::self);

    thing.def(py::init<>(), R"pbdoc(
Default constructor.

There is no guarantee about the contents of the matrix constructed.

:complexity:
   Constant.
)pbdoc");
    thing.def(py::init<uint64_t>(),
              py::arg("val"),
              R"pbdoc(
:sig=(self: BMat8, val: int) -> None:

Construct from ``int``.

This constructor initializes a :any:`BMat8` to have rows equal to the 8 chunks,
of 8 bits each, of the binary representation of ``mat``.

:param val: the integer representation of the matrix being constructed.
:type val: int

:complexity: Constant.)pbdoc");
    thing.def(py::init<std::vector<std::vector<bool>> const&>(),
              py::arg("rows"),
              R"pbdoc(
:sig=(self: BMat8, rows: list[list[bool]]) -> None:

Construct from list of rows.

This constructor initializes a matrix where the rows of the matrix are the
lists in *rows*.

:param rows: the list of rows of the matrix being constructed.
:type rows: list[list[bool]]

:raises LibsemigroupsError:  if *rows* has 0 rows.
:raises LibsemigroupsError:  if *rows* has more than 8 rows.
:raises LibsemigroupsError:  if the rows of *rows* are not all of the same length.

:complexity: Constant.)pbdoc");
    thing.def(
        "degree", [](BMat8 const& self) { return 8; }, R"pbdoc(
Returns the degree of *self*.

This function always returns ``8``.

:returns: The degree of the matrisx, ``8``.
:rtype: int
)pbdoc");

    thing.def(
        "copy",
        [](BMat8 const& self) { return BMat8(self); },
        R"pbdoc(
Copy a BMat8.

:returns: A copy of the argument.
:rtype: BMat8
)pbdoc");
    thing.def("__copy__", [](BMat8 const& self) { return BMat8(self); });

    thing.def("to_int",
              &BMat8::to_int,
              R"pbdoc(
Returns the integer representation of a :any:`BMat8`.

Returns a non-negative integer obtained by interpreting an 8 x 8 :any:`BMat8`
as a sequence of 64 bits (reading rows left to right, from top to bottom) and
then realising this sequence as an unsigned int.

:returns:
   The integer value of the matrix.

:rtype:
   int

:complexity:
   Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> x.to_int()
   4647714815446351872
   >>> bin(x.to_int())
   '0b100000010000000000000000000000000000000000000000000000000000000'
)pbdoc");

    thing.def("swap",
              &BMat8::swap,
              py::arg("that"),
              R"pbdoc(
Swaps *self* with *that*.

This function swaps the values of *self* and *that*.

:param that: the :any:`BMat8` to swap this with.
:type that: BMat8

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> y = BMat8([[1, 1], [0, 0]])
   >>> BMat8.swap(x,y)
   >>> x
   BMat8([[1, 1],
          [0, 0]])
   >>> y
   BMat8([[0, 1],
          [1, 0]])

)pbdoc");

    ////////////////////////////////////////////////////////////////////////
    // Helper functions from libsemigroups::bmat8
    ////////////////////////////////////////////////////////////////////////

    m.def("bmat8_one",
          &bmat8::one<BMat8>,
          py::arg("dim") = 8,
          R"pbdoc(
:sig=(dim: int) -> BMat8:
Returns the identity :any:`BMat8` of a given dimension.

This function returns the :any:`BMat8` with the first *dim* entries in the
main diagonal equal to ``1`` and every other value equal to ``0``.

:param dim: the dimension of the identity (default: ``8``)
:type dim: int

:returns: A :any:`BMat8`.
:rtype: BMat8

:complexity: Constant.

.. doctest::

 >>> from libsemigroups_pybind11 import bmat8
 >>> bmat8.one(4)
 BMat8([[1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]])
)pbdoc");

    m.def(
        "bmat8_random",
        [](size_t dim) { return bmat8::random(dim); },
        py::arg("dim") = 8,
        R"pbdoc(
:sig=(dim: int) -> BMat8:
Construct a random :any:`BMat8` of dimension at most *dim*.

This function returns a :any:`BMat8` chosen at random, where only the top-left
*dim* by *dim* entries can be non-zero.

:param dim: the dimension.
:type dim: int

:returns: A :any:`BMat8`.
:rtype: BMat8
)pbdoc");

    m.def("bmat8_transpose",
          &bmat8::transpose,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> BMat8:
Returns the transpose of a :any:`BMat8`.

This function returns the transpose of its argument *x*, which is computed
using the technique found in :cite:`Knuth2009aa`.

:param x: the matrix to transpose.
:type x: BMat8

:returns: A :any:`BMat8`.
:rtype: BMat8

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[1, 0, 1], [0, 1, 0], [0, 0, 0]])
   >>> bmat8.transpose(x)
   BMat8([[1, 0, 0],
          [0, 1, 0],
          [1, 0, 0]])
)pbdoc");

    m.def("bmat8_row_space_basis",
          &bmat8::row_space_basis,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> BMat8:
Find a basis for the row space of a :any:`BMat8`.

This function returns a :any:`BMat8` whose non-zero rows form a basis for the
row space of *x*.

:param x: the matrix.
:type x: BMat8

:returns: A :any:`BMat8`.
:rtype: BMat8

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[1, 0, 1], [0, 1, 0], [0, 0, 0]])
   >>> bmat8.row_space_basis(x)
   BMat8([[1, 0, 1],
          [0, 1, 0],
          [0, 0, 0]])
)pbdoc");
    m.def("bmat8_col_space_basis",
          &bmat8::col_space_basis,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> BMat8:
Find a basis for the column space of a :any:`BMat8`.

This function returns a :any:`BMat8` whose non-zero columns form a basis for
the column space of *x*.

:param x: the matrix.
:type x: BMat8

:returns: A :any:`BMat8`.
:rtype: BMat8

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[1, 0, 1], [0, 1, 0], [0, 0, 0]])
   >>> bmat8.col_space_basis(x)
   BMat8([[1, 0],
          [0, 1]])
)pbdoc");

    m.def("bmat8_number_of_rows",
          &bmat8::number_of_rows,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> int:
Returns the number of non-zero rows in a :any:`BMat8`.

:any:`BMat8` objects do not know their "dimension" - in effect they are all of
dimension 8. However, this function can be used to obtain the number of
non-zero rows of a :any:`BMat8`.

:param x: the matrix.
:type x: BMat8
:returns: The number of non-zero rows.
:rtype: int

:complexity: Constant.

.. seealso::  :any:`number_of_cols` and :any:`minimum_dim`.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[1, 0, 1], [0, 1, 0], [0, 0, 0]])
   >>> bmat8.number_of_rows(x)
   2
)pbdoc");

    m.def("bmat8_number_of_cols",
          &bmat8::number_of_cols,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> int:
Returns the number of non-zero columns in a :any:`BMat8`.

:any:`BMat8` objects do not know their "dimension" - in effect they are all of
dimension 8. However, this function can be used to obtain the number of
non-zero rows of a :any:`BMat8`.

:param x: the matrix.
:type x: BMat8

:returns: The number of non-zero columns.
:rtype: int

:complexity: Constant.

.. seealso::  :any:`number_of_rows` and :any:`minimum_dim`.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[1, 0, 1], [0, 1, 0], [0, 0, 0]])
   >>> bmat8.number_of_cols(x)
   3
)pbdoc");

    m.def("bmat8_row_space_size",
          &bmat8::row_space_size,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> int:
Returns the size of the row space of a :any:`BMat8`.

:param x: the matrix.
:type x: BMat8

:returns: The size of the row space of *x*.
:rtype: int

:complexity:  :math:`O(n)` where :math:`n` is the return value of this function.

.. seealso::  :any:`col_space_size`.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[1, 0, 0], [0, 1, 1], [0, 1, 0]])
   >>> bmat8.row_space_size(x)
   6
)pbdoc");

    m.def("bmat8_col_space_size",
          &bmat8::col_space_size,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> int:
Returns the size of the column space of a :any:`BMat8`.

:param x: the matrix.
:type x: BMat8

:returns: The size of the column space of *x*.
:rtype: int

:complexity: :math:`O(n)` where :math:`n` is the return value of this function.

.. seealso:: :any:`row_space_size`.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> bmat8.col_space_size(x)
   4
)pbdoc");
    m.def("bmat8_minimum_dim",
          &bmat8::minimum_dim,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> int:
Returns the minimum dimension of a :any:`BMat8`.

This function returns the maximal *n* such that row *n* or column *n* in
the boolean matrix *x* contains a ``1``. Equivalent to the maximum of
:any:`number_of_rows` and :any:`number_of_cols`.

:param x: the matrix.
:type x: BMat8

:returns: The minimum dimension of *x*.
:rtype: int

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> bmat8.minimum_dim(x)
   2
)pbdoc");

    m.def(
        "bmat8_rows",
        [](BMat8 const& x) {
          std::vector<std::vector<bool>> result;
          for (auto row : bmat8::rows(x)) {
            result.push_back(bmat8::to_vector(row));
          }
          return result;
        },
        py::arg("x"),
        R"pbdoc(
:sig=(x: BMat8) -> list[list[bool]]:
Returns a list of the rows of a :any:`BMat8`.

This function returns the rows of *x*. The returned list always has length 8,
even if *x* was constructed with fewer rows.

:param x: the matrix.
:type x: BMat8


:returns: The list of rows of the boolean matrix *x*.
:rtype: list[list[bool]]

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> bmat8.rows(x)  # doctest: +NORMALIZE_WHITESPACE
   [[False, True, False, False, False, False, False, False],
    [True, False, False, False, False, False, False, False],
    [False, False, False, False, False, False, False, False],
    [False, False, False, False, False, False, False, False],
    [False, False, False, False, False, False, False, False],
    [False, False, False, False, False, False, False, False],
    [False, False, False, False, False, False, False, False],
    [False, False, False, False, False, False, False, False]]
)pbdoc");

    m.def("bmat8_is_regular_element",
          &bmat8::is_regular_element,
          py::arg("x"),
          R"pbdoc(
:sig=(x: BMat8) -> bool:
Check whether *x* is a regular element of the full boolean matrix monoid of
appropriate dimension.

:param x: the matrix.
:type x: BMat8

:returns:
  ``True`` if there exists a boolean matrix ``y`` such that ``x * y * x = x``
  where *x*, and ``False`` otherwise.
:rtype: bool

:complexity: Constant.

.. doctest::

   >>> from libsemigroups_pybind11 import BMat8, bmat8
   >>> x = BMat8([[0, 1], [1, 0]])
   >>> bmat8.is_regular_element(x)
   True
   >>> sum(1 for x in range(100000) if bmat8.is_regular_element(BMat8(x)))
   97996
)pbdoc");
  }  // init_bmat8
}  // namespace libsemigroups
