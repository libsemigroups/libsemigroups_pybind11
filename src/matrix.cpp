//
// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2021-2024 James D. Mitchell
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

// C++ stl headers....
#include <cstddef>        // for size_t
#include <cstdint>        // for int64_t
#include <memory>         // for allocator, make_unique, unique_ptr
#include <string>         // for char_traits, operator==, operator+
#include <unordered_map>  // for operator==, unordered_map
#include <utility>        // for make_pair, pair
#include <vector>         // for vector

// libsemigroups....
#include <libsemigroups/adapters.hpp>   // for Hash
#include <libsemigroups/constants.hpp>  // for PositiveInfinity, NegativeInf...
#include <libsemigroups/exception.hpp>  // for LIBSEMIGROUPS_EXCEPTION
#include <libsemigroups/matrix.hpp>     // for MaxPlusTruncMat, MinPlusTruncMat

#include <libsemigroups/detail/string.hpp>  // for string_format, to_string

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for init, class_, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_matrix

namespace libsemigroups {
  namespace py = pybind11;
  namespace {

    template <typename Semiring>
    Semiring const* semiring(size_t threshold) {
      static std::unordered_map<size_t, std::unique_ptr<Semiring const>> cache;
      auto it = cache.find(threshold);
      if (it == cache.end()) {
        it = cache
                 .emplace(threshold,
                          std::make_unique<Semiring const>(threshold))
                 .first;
      }
      return it->second.get();
    }

    template <typename Semiring>
    Semiring const* semiring(size_t threshold, size_t period) {
      static std::unordered_map<std::pair<size_t, size_t>,
                                std::unique_ptr<Semiring const>,
                                Hash<std::pair<size_t, size_t>>>
           cache;
      auto tp = std::make_pair(threshold, period);
      auto it = cache.find(tp);
      if (it == cache.end()) {
        it = cache
                 .emplace(tp,
                          std::make_unique<Semiring const>(threshold, period))
                 .first;
      }
      return it->second.get();
    }

    template <typename Mat>
    auto bind_matrix_common(py::module& m) {
      using Row         = typename Mat::Row;
      using scalar_type = typename Mat::scalar_type;

      std::string  repr_prefix = "Matrix(MatrixKind.", repr_short, py_type;
      size_t const cols        = 80;

      std::function<std::string(Mat const&)> repr;

      // Have to pass repr_prefix and repr_short by value o/w they
      // don't exist after this function is called
      if constexpr (IsIntMat<Mat>) {
        repr_prefix += "Integer, ";
        repr_short = "integer matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", cols);
        };
        py_type = "IntMat";
      } else if constexpr (IsBMat<Mat>) {
        repr_prefix += "Boolean, ";
        repr_short = "boolean matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", cols);
        };
        py_type = "BMat";
      } else if constexpr (IsMaxPlusMat<Mat>) {
        repr_prefix += "MaxPlus, ";
        repr_short = "max-plus matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", cols);
        };
        py_type = "MaxPlusMat";
      } else if constexpr (IsMinPlusMat<Mat>) {
        repr_prefix += "MinPlus, ";
        repr_short = "min-plus matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", cols);
        };
        py_type = "MinPlusMat";
      } else if constexpr (IsProjMaxPlusMat<Mat>) {
        repr_prefix += "ProjMaxPlus, ";
        repr_short = "proj. max-plus matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", cols);
        };
        py_type = "ProjMaxPlusMat";
      } else if constexpr (IsMaxPlusTruncMat<Mat>) {
        repr_prefix += "MaxPlusTrunc, {}, ";
        repr_short = "max-plus {}-trunc. matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          auto t = matrix::threshold(x);
          return to_human_readable_repr(x,
                                        fmt::format(repr_prefix, t),
                                        fmt::format(repr_short, t),
                                        "[]",
                                        cols);
        };
        py_type = "MaxPlusTruncMat";
      } else if constexpr (IsMinPlusTruncMat<Mat>) {
        repr_prefix += "MinPlusTrunc, {}, ";
        repr_short = "min-plus {}-trunc. matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          auto t = matrix::threshold(x);
          return to_human_readable_repr(x,
                                        fmt::format(repr_prefix, t),
                                        fmt::format(repr_short, t),
                                        "[]",
                                        cols);
        };
        py_type = "MinPlusTruncMat";
      } else {
        static_assert(IsNTPMat<Mat>);
        repr_prefix += "NTP, {}, {}, ";
        repr_short = "({}, {})-ntp matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          auto t = matrix::threshold(x);
          auto p = matrix::period(x);
          return to_human_readable_repr(x,
                                        fmt::format(repr_prefix, t, p),
                                        fmt::format(repr_short, t, p),
                                        "[]",
                                        cols);
        };
        py_type = "NTPMat";
      }

      using scalar_type = typename Mat::scalar_type;

      py::class_<Mat> thing(m,
                            py_type.c_str(),
                            R"pbdoc(
This page contains the documentation for functionality in
``libsemigroups_pybind11`` for matrices.

Matrices over various semirings can be constructed using the class
:py:class:`Matrix`. These internal types are optimised in
various ways so that the underlying semiring operations are as fast as possible.

Some helper functions for :py:class:`Matrix` objects are documented in the
submodule :any:`libsemigroups_pybind11.matrix`.

.. warning::

    The entries in a ``libsemigroups_pybind11`` matrix are stored internally as
    64-bit signed integers, and there are no checks that the multiplication does
    not overflow.

.. seealso::

    :any:`MatrixKind`.

.. doctest::

    >>> from libsemigroups_pybind11 import Matrix, MatrixKind
    >>> x = Matrix(MatrixKind.Integer, [[2]])
    >>> x ** 64
    Matrix(MatrixKind.Integer, [[0]])
    >>> x = Matrix(MatrixKind.Integer, [[0, 1, 1], [1, 2, 3], [-1, 0, -1]])
    >>> x[0, 0]
    0
    >>> x[0, 1]
    1
    >>> x[1]
    [1, 2, 3]
    >>> x[0, 0] = 666
    >>> x
    Matrix(MatrixKind.Integer, [[666,   1,   1],
                                [  1,   2,   3],
                                [ -1,   0,  -1]])

    >>> x[0] = [0, 1, 1]
    >>> x
    Matrix(MatrixKind.Integer, [[ 0,  1,  1],
                                [ 1,  2,  3],
                                [-1,  0, -1]])
    >>> x += 1
    >>> x
    Matrix(MatrixKind.Integer, [[1, 2, 2],
                                [2, 3, 4],
                                [0, 1, 0]])
    >>> x *= 2
    >>> x
    Matrix(MatrixKind.Integer, [[2, 4, 4],
                                [4, 6, 8],
                                [0, 2, 0]])
    >>> x += x
    >>> x
    Matrix(MatrixKind.Integer, [[ 4,  8,  8],
                                [ 8, 12, 16],
                                [ 0,  4,  0]])
    >>> x + x
    Matrix(MatrixKind.Integer, [[ 8, 16, 16],
                                [16, 24, 32],
                                [ 0,  8,  0]])
    >>> x * x
    Matrix(MatrixKind.Integer, [[ 80, 160, 160],
                                [128, 272, 256],
                                [ 32,  48,  64]])
    >>> y = x.one()
    >>> y
    Matrix(MatrixKind.Integer, [[1, 0, 0],
                                [0, 1, 0],
                                [0, 0, 1]])
    >>> x.one(2)
    Matrix(MatrixKind.Integer, [[1, 0],
                                [0, 1]])
    >>> x.swap(y)
    >>> x
    Matrix(MatrixKind.Integer, [[1, 0, 0],
                                [0, 1, 0],
                                [0, 0, 1]])
    >>> y
    Matrix(MatrixKind.Integer, [[ 4,  8,  8],
                                [ 8, 12, 16],
                                [ 0,  4,  0]])
    >>> x.number_of_rows()
    3
    >>> x.number_of_cols()
    3
    >>> y = x.copy()
    >>> x is not y
    True
    >>> x == y
    True
    >>> x != y
    False
    >>> x < y
    False
    >>> x != y
    False
    >>> x > y
    False
    >>> x >= y
    True
    >>> x <= y
    True
    >>> x ** 10 == y
    True
    >>> len(x)
    3
    >>> list(x)
    [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
    >>> x + 2 == 2 + x
    True
    >>> x * 2 == 2 * x
    True
    >>> import copy
    >>> z = copy.copy(y)
    >>> z *= 0
    >>> z
    Matrix(MatrixKind.Integer, [[0, 0, 0],
                                [0, 0, 0],
                                [0, 0, 0]])
    >>> z = Matrix(MatrixKind.Integer, 4, 4)
    >>> z
    Matrix(MatrixKind.Integer, [[0, 0, 0, 0],
                                [0, 0, 0, 0],
                                [0, 0, 0, 0],
                                [0, 0, 0, 0]])
    >>> d = {z: True}
    >>> z in d
    True

)pbdoc");

      thing.def("__repr__", repr);
      thing.def("__hash__", &Mat::hash_value);
      thing.def("__copy__", [](Mat const& x) { return Mat(x); });
      thing.def(
          "__getitem__",
          [](Mat const& mat, py::tuple xy) {
            return from_int(mat.at(xy[0].cast<size_t>(), xy[1].cast<size_t>()));
          },
          py::is_operator());
      thing.def(
          "__getitem__",
          [](Mat const& thing, size_t i) {
            try {
              auto                                      r = thing.row(i);
              std::vector<int_or_constant<scalar_type>> result(r.begin(),
                                                               r.end());
              from_ints<scalar_type>(result);
              return result;
            } catch (LibsemigroupsException const& e) {
              // This is done so that "list" works as expected for a
              // matrix
              throw std::out_of_range(e.what());
            }
          },
          py::is_operator());
      thing.def(
          "__setitem__",
          [](Mat& mat, py::tuple xy, scalar_type val) {
            matrix::throw_if_bad_entry(mat, val);
            auto r       = xy[0].cast<size_t>();
            auto c       = xy[1].cast<size_t>();
            mat.at(r, c) = val;
          },
          py::is_operator());
      thing.def(
          "__setitem__",
          [](Mat& mat, py::tuple xy, PositiveInfinity const& val) {
            matrix::throw_if_bad_entry(mat, val);
            mat.at(xy[0].cast<size_t>(), xy[1].cast<size_t>()) = val;
          },
          py::is_operator());
      thing.def(
          "__setitem__",
          [](Mat& mat, py::tuple xy, NegativeInfinity const& val) {
            matrix::throw_if_bad_entry(mat, val);
            mat.at(xy[0].cast<size_t>(), xy[1].cast<size_t>()) = val;
          },
          py::is_operator());
      thing.def(
          "__setitem__",
          [](Mat&                                             mat,
             size_t                                           r,
             std::vector<int_or_constant<scalar_type>> const& row) {
            auto rv = mat.row(r);
            if (row.size() != rv.size()) {
              LIBSEMIGROUPS_EXCEPTION(
                  "invalid row length, expected {}, but found {}",
                  rv.size(),
                  row.size());
            }
            for (auto& item : row) {
              matrix::throw_if_bad_entry(mat, to_int<scalar_type>(item));
            }
            auto dit = rv.begin();
            for (auto it = row.cbegin(); it != row.cend(); ++it, ++dit) {
              *dit = to_int<scalar_type>(*it);
            }
          },
          py::is_operator());
      thing.def(
          "__setitem__",
          [](Mat& mat, size_t r, py::list row) {
            if (row.size() != mat.number_of_rows()) {
              LIBSEMIGROUPS_EXCEPTION(
                  "invalid row length, expected {}, but found {}",
                  mat.number_of_rows(),
                  row.size());
            }
            auto py_int_type = py::globals()["__builtins__"].attr("int");
            for (auto item : row) {
              if (py::isinstance(item, py_int_type)) {
                matrix::throw_if_bad_entry(mat, item.cast<scalar_type>());
              } else if (py::isinstance<PositiveInfinity>(item)) {
                matrix::throw_if_bad_entry(mat, item.cast<PositiveInfinity>());
              } else if (py::isinstance<NegativeInfinity>(item)) {
                matrix::throw_if_bad_entry(mat, item.cast<NegativeInfinity>());
              } else {
                // TODO(later) get the name of the type and say that too
                throw pybind11::type_error(
                    fmt::format("invalid entry, expected entries to be "
                                "integers, -{0}, and/or +{0}",
                                u8"\u221E"));
              }
            }
            for (size_t c = 0; c < row.size(); ++c) {
              if (py::isinstance(row[c], py_int_type)) {
                mat(r, c) = row[c].cast<scalar_type>();
              } else if (py::isinstance<PositiveInfinity>(row[c])) {
                mat(r, c) = row[c].cast<PositiveInfinity>();
              } else if (py::isinstance<NegativeInfinity>(row[c])) {
                mat(r, c) = row[c].cast<NegativeInfinity>();
              }
            }
          },
          py::is_operator());
      thing.def(
          "__imul__",
          [](Mat& mat, scalar_type a) {
            mat *= a;
            return mat;
          },
          py::is_operator());
      thing.def(
          "__iadd__",
          [](Mat& mat, Mat const& that) {
            matrix::throw_if_bad_dim(mat, that);
            mat += that;
            return mat;
          },
          py::is_operator());
      thing.def(
          "__iadd__",
          [](Mat& mat, scalar_type a) {
            mat += a;
            return mat;
          },
          py::is_operator());
      thing.def(
          "__gt__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self > other;
          },
          py::is_operator());
      thing.def(
          "__ge__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self >= other;
          },
          py::is_operator());
      thing.def(
          "__ne__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self != other;
          },
          py::is_operator());
      thing.def(
          "__eq__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self == other;
          },
          py::is_operator());
      thing.def(
          "__lt__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self < other;
          },
          py::is_operator());
      thing.def(
          "__le__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self <= other;
          },
          py::is_operator());
      thing.def(
          "__add__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self + other;
          },
          py::is_operator());
      thing.def(
          "__mul__",
          [](Mat const& self, Mat const& other) {
            matrix::throw_if_bad_dim(self, other);
            return self * other;
          },
          py::is_operator());
      thing.def(py::self + scalar_type());
      thing.def(scalar_type() + py::self);
      thing.def(py::self * scalar_type());
      thing.def(scalar_type() * py::self);

      thing.def("__len__",
                [](Mat const& thing) { return thing.number_of_rows(); });
      thing.def("__pow__", &matrix::pow<Mat>);

      thing.def(
          "copy", [](Mat const& x) { return Mat(x); }, R"pbdoc(
:sig=(self: Matrix) -> Matrix:
Copy a :any:`Matrix` object.

:returns: A copy.
:rtype: Matrix
      )pbdoc");
      thing.def(
          "product_inplace",
          [](Mat& xy, Mat const& thing, Mat const& y) {
            matrix::throw_if_bad_dim(thing, y);
            matrix::throw_if_bad_dim(xy, thing);
            xy.product_inplace_no_checks(thing, y);
          },
          R"pbdoc(
:sig=(self: Matrix, x: Matrix, y: Matrix) -> None:

Multiply two matrices and stores the product in *self*.

:param x: first matrix to multiply.
:type x: Matrix
:param y: second matrix to multiply.
:type y: Matrix

:raises LibsemigroupsError:
  if *x* and *y* are not square, or do not have the same number of rows.

:raises TypeError:
  if *x* and *y* are not defined over the same semiring.)pbdoc");
      thing.def(
          "transpose",
          [](Mat& thing) { thing.transpose(); },
          R"pbdoc(
:sig=(self: Matrix) -> None:

Transposes the matrix in-place.

:raises LibsemigroupsError:
  if *self* is not a square matrix.)pbdoc");
      thing.def("swap", &Mat::swap, R"pbdoc(
:sig=(self: Matrix, that: Matrix) -> None:

Swaps the contents of *self* with the contents of *that*.

:param that: the matrix to swap contents with
:type that: Matrix)pbdoc");
      thing.def(
          "scalar_zero",
          [](Mat const& thing) { return from_int(thing.scalar_zero()); },
          R"pbdoc(
:sig=(self: Matrix) -> int | PositiveInfinity | NegativeInfinity:

Returns the additive identity of the underlying semiring of a
matrix.

:returns: The additive identity of the underlying semiring.
:rtype: int | PositiveInfinity | NegativeInfinity

.. doctest::

    >>> from libsemigroups_pybind11 import Matrix, MatrixKind, POSITIVE_INFINITY
    >>> x = Matrix(MatrixKind.MinPlusTrunc, 11 ,[[0, 1, 1], [0] * 3, [1] * 3])
    >>> x.scalar_zero() == POSITIVE_INFINITY
    True)pbdoc");
      thing.def(
          "scalar_one",
          [](Mat const& thing) { return thing.scalar_one(); },
          R"pbdoc(
:sig=(self: Matrix) -> int:

Returns the multiplicative identity of the underlying semiring of a
matrix.

:returns: The multiplicative identity of the underlying semiring.
:rtype: int

.. doctest::

    >>> from libsemigroups_pybind11 import Matrix, MatrixKind
    >>> x = Matrix(MatrixKind.MinPlusTrunc, 11 ,[[0, 1, 1], [0] * 3, [1] * 3])
    >>> x.scalar_one()
    0)pbdoc");

      thing.def(
          "number_of_rows",
          [](Mat const& thing) { return thing.number_of_rows(); },
          R"pbdoc(
:sig=(self: Matrix) -> int:

Returns the number of rows.

:returns: The number of rows in the matrix.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11 import Matrix, MatrixKind
  >>> x = Matrix(MatrixKind.Integer, [[0, 1], [1, 0]])
  >>> x.number_of_rows()
  2)pbdoc");

      thing.def(
          "degree",
          [](Mat const& thing) { return thing.number_of_rows(); },
          R"pbdoc(
:sig=(self: Matrix) -> int:

Returns the degree of a :any:`Matrix`.

Returns the degree of a :any:`Matrix`, where the *degree* of a :any:`Matrix` is
just the number of rows.

:returns: The degree.
:rtype: int

:complexity: Constant.
)pbdoc");
      thing.def(
          "number_of_cols",
          [](Mat const& thing) { return thing.number_of_cols(); },
          R"pbdoc(
:sig=(self: Matrix) -> int:

Returns the number of columns.

:returns: The number of columns in the matrix.
:rtype: int

.. doctest::

  >>> from libsemigroups_pybind11 import Matrix, MatrixKind
  >>> x = Matrix(MatrixKind.Integer, [[0, 1], [1, 0]])
  >>> x.number_of_cols()
  2)pbdoc");
      thing.def(
          "row",
          [](Mat const& thing, size_t i) { return Row(thing.row(i)); },
          R"pbdoc(
:sig=(self: Matrix, i: int) -> Matrix:

Returns the specified row.

:param i: the index of the row.
:type i: int

:returns: The row.
:rtype: Matrix

:raises LibsemigroupsError:
  if *i* is greater than or equal to :any:`number_of_rows`.)pbdoc");
      thing.def(
          "rows",
          [](Mat const& thing) {
            std::vector<Row> rows;
            for (size_t i = 0; i < thing.number_of_rows(); ++i) {
              rows.push_back(Row(thing.row(i)));
            }
            return rows;
          },
          R"pbdoc(
:sig=(self: Matrix) -> list[Matrix]:

Returns a list of all rows of a matrix.

:returns: A list of the rows.
:rtype: list[Matrix])pbdoc");
      return thing;
    }

    template <typename Mat>
    auto bind_matrix_no_semiring(py::module& m) {
      using scalar_type = typename Mat::scalar_type;

      auto thing = bind_matrix_common<Mat>(m);

      thing.def(
          py::init(
              [](std::vector<std::vector<int_or_constant<scalar_type>>> const&
                     rows) { return make<Mat>(to_ints<scalar_type>(rows)); }),
          py::arg("rows"),
          R"pbdoc(
:sig=(self: Matrix, kind: MatrixKind, rows: list[list[int | PositiveInfinity | NegativeInfinity]]) -> None:

Construct a matrix from rows.

:param kind: specifies the underlying semiring.
:type kind: MatrixKind

:param rows: the rows of the matrix.
:type rows: list[list[int | PositiveInfinity | NegativeInfinity]]

:raise TypeError: if *kind* is
    :any:`MatrixKind.MaxPlusTrunc`,
    :any:`MatrixKind.MinPlusTrunc`, or
    :any:`MatrixKind.NTP`.

:raise LibsemigroupsError:
  if the entries in *rows* are not of equal length.

:raise LibsemigroupsError:
  if any of the entries of the lists in *rows* do not belong to
  the underlying semiring.
)pbdoc");
      thing.def(py::init<size_t, size_t>(),
                R"pbdoc(
:sig=(self: Matrix, kind: MatrixKind, r: int, c: int) -> None:

Construct an uninitialized *r* by *c* matrix.

:param kind: specifies the underlying semiring.
:type kind: MatrixKind

:param r: the number of rows in the matrix.
:type r: int

:param c: the number of columns in the matrix.
:type c: int

:raise TypeError: if *kind* is
    :any:`MatrixKind.MaxPlusTrunc`,
    :any:`MatrixKind.MinPlusTrunc`,
    or :any:`MatrixKind.NTP`.

.. doctest::

  >>> from libsemigroups_pybind11 import Matrix, MatrixKind
  >>> # construct a 2 x 3 boolean matrix
  >>> Matrix(MatrixKind.Boolean, 2, 3)
  Matrix(MatrixKind.Boolean, [[0, 0, 0],
                              [0, 0, 0]]))pbdoc");
      thing.def("one", [](Mat const& self, size_t n) { return Mat::one(n); });
      thing.def("one", py::overload_cast<>(&Mat::one, py::const_));
    }

    template <typename Mat>
    auto bind_matrix_trunc_semiring(py::module& m) {
      using semiring_type = typename Mat::semiring_type;
      using scalar_type   = typename Mat::scalar_type;

      auto thing = bind_matrix_common<Mat>(m);

      thing.def(py::init([](size_t threshold, size_t r, size_t c) {
                  return Mat(semiring<semiring_type>(threshold), r, c);
                }),
                R"pbdoc(
:sig=(self: Matrix, kind: MatrixKind, threshold: int, r: int, c: int) -> None:

Construct an uninitialized `r` by `c` matrix.

:param kind: specifies the underlying semiring.
:type kind: MatrixKind

:param threshold: the threshold of the underlying semiring.
:type threshold: int

:param r: the number of rows in the matrix
:type r: int

:param c: the number of columns in the matrix
:type c: int

:raise TypeError:
  if *kind* is not :any:`MatrixKind.MaxPlusTrunc` or
  :any:`MatrixKind.MinPlusTrunc`.

.. doctest::

  >>> from libsemigroups_pybind11 import Matrix, MatrixKind
  >>> # construct a 2 x 3 max-plus truncated matrix
  >>> Matrix(MatrixKind.MaxPlusTrunc, 11, 2, 3)
  Matrix(MatrixKind.MaxPlusTrunc, 11, [[0, 0, 0],
                                       [0, 0, 0]]))pbdoc");
      thing.def(
          py::init(
              [](size_t threshold,
                 std::vector<std::vector<int_or_constant<scalar_type>>> const&
                     rows) {
                return make<Mat>(semiring<semiring_type>(threshold),
                                 to_ints<scalar_type>(rows));
              }),
          R"pbdoc(
:sig=(self: Matrix, kind: MatrixKind, threshold: int, rows: list[list[int | PositiveInfinity | NegativeInfinity]]) -> None:

Construct a matrix from threshold and rows.

:param kind: specifies the underlying semiring.
:type kind: MatrixKind

:param threshold: the threshold of the underlying semiring.
:type threshold: int

:param rows: the rows of the matrix.
:type rows: list[list[int | PositiveInfinity | NegativeInfinity]]

:raise TypeError:
    if *kind* is not :any:`MatrixKind.MaxPlusTrunc` or
    :any:`MatrixKind.MinPlusTrunc`.

:raise LibsemigroupsError:
  if the entries in *rows* are not of equal length.

:raise LibsemigroupsError:
  if any of the entries of the lists in *rows* do not belong to
  the underlying semiring.
)pbdoc");

      thing.def("one", [](Mat const& self, size_t n) {
        return Mat::one(semiring<semiring_type>(matrix::threshold(self)), n);
      });
      thing.def("one", [](Mat const& self) { return self.one(); });

      m.def(
          "matrix_threshold",
          [](Mat const& x) { return matrix::threshold(x); },
          py::arg("x"),
          R"pbdoc(
:sig=(x: Matrix) -> int:
:only-document-once:
Returns the threshold of a matrix over a truncated semiring.

This function returns the threshold of a matrix over a truncated semiring,
that is a matrix whose kind is any of:

* :any:`MatrixKind.MaxPlusTrunc`
* :any:`MatrixKind.MinPlusTrunc`
* :any:`MatrixKind.NTP`

:param x: the matrix.
:type x: Matrix

:returns: The threshold of *x*.
:rtype: int
)pbdoc");
    }

    template <typename Mat>
    auto bind_ntp_matrix(py::module& m) {
      using semiring_type = typename Mat::semiring_type;
      using scalar_type   = typename Mat::scalar_type;

      auto thing = bind_matrix_common<Mat>(m);

      thing.def(
          py::init(
              [](size_t threshold,
                 size_t period,
                 std::vector<std::vector<int_or_constant<scalar_type>>> const&
                     entries) {
                return make<Mat>(semiring<semiring_type>(threshold, period),
                                 to_ints<scalar_type>(entries));
              }),
          R"pbdoc(
:sig=(self: Matrix, kind: MatrixKind, threshold: int, period: int, rows: list[list[int]]) -> None:

Construct a matrix from threshold, period, and rows.

:param kind: specifies the underlying semiring.
:type kind: MatrixKind

:param threshold: the threshold of the underlying semiring.
:type threshold: int

:param period: the period of the underlying semiring.
:type period: int

:param rows: the rows of the matrix.
:type rows: list[list[int]]

:raise TypeError: if *kind* is not :any:`MatrixKind.NTP`.

:raise LibsemigroupsError:
  if the entries in *rows* are not of equal length.

:raise LibsemigroupsError:
  if any of the entries of the lists in *rows* do not belong to
  the underlying semiring.)pbdoc");
      thing.def(
          py::init([](size_t threshold, size_t period, size_t r, size_t c) {
            return Mat(semiring<semiring_type>(threshold, period), r, c);
          }),
          R"pbdoc(
:sig=(self: Matrix, kind: MatrixKind, threshold: int, period: int, r: int, c: int) -> None:

Construct an uninitialized `r` by `c` matrix with threshold and period.

:param kind: specifies the underlying semiring.
:type kind: MatrixKind

:param threshold: the threshold of the underlying semiring.
:type threshold: int

:param period: the period of the underlying semiring.
:type period: int

:param r: the number of rows in the matrix.
:type r: int

:param c: the number of columns in the matrix.
:type c: int

:raise TypeError: if *kind* is not :any:`MatrixKind.NTP`.

.. doctest::

  >>> from libsemigroups_pybind11 import Matrix, MatrixKind
  >>> # construct a 2 x 3 ntp matrix
  >>> Matrix(MatrixKind.NTP, 5, 7, 2, 3)
  Matrix(MatrixKind.NTP, 5, 7, [[0, 0, 0],
                                [0, 0, 0]])
)pbdoc");
      thing.def(
          "one",
          [](Mat const& self, size_t n) {
            return Mat::one(semiring<semiring_type>(matrix::threshold(self),
                                                    matrix::period(self)),
                            n);
          },
          R"pbdoc(
:sig=(self: Matrix, n: int) -> Matrix:

Construct the :math:`n \times n` identity matrix.

:param n: the dimension of the matrix.
:type n: int

:returns: An identity matrix.
:rtype: Matrix)pbdoc");
      thing.def(
          "one",
          [](Mat const& self) { return self.one(); },
          R"pbdoc(
:sig=(self: Matrix) -> Matrix:

This function returns the identity matrix of the same dimensions as *self*.

:returns: An identity matrix.
:rtype: Matrix)pbdoc");

      m.def(
          "matrix_period",
          [](Mat const& x) { return matrix::period(x); },
          py::arg("x"),
          R"pbdoc(
:sig=(x:Matrix)->int:
Returns the period of an ntp matrix. This function returns the period of
the ntp matrix *x* using its underlying semiring.

:param x: the ntp matrix.
:type x: Matrix

:returns: The period of the matrix *x*.
:rtype: int
)pbdoc");
      m.def(
          "matrix_threshold",
          [](Mat const& x) { return matrix::threshold(x); },
          py::arg("x"),
          R"pbdoc(
:sig=(x: Matrix) -> int:
:only-document-once:
)pbdoc");
    }
  }  // namespace

  void init_matrix(py::module& m) {
    bind_matrix_no_semiring<BMat<>>(m);
    bind_matrix_no_semiring<IntMat<0, 0, int64_t>>(m);
    bind_matrix_no_semiring<MaxPlusMat<0, 0, int64_t>>(m);
    bind_matrix_no_semiring<MinPlusMat<0, 0, int64_t>>(m);
    bind_matrix_no_semiring<ProjMaxPlusMat<0, 0, int64_t>>(m);
    bind_matrix_trunc_semiring<MaxPlusTruncMat<0, 0, 0, int64_t>>(m);
    bind_matrix_trunc_semiring<MinPlusTruncMat<0, 0, 0, int64_t>>(m);
    bind_ntp_matrix<NTPMat<0, 0, 0, 0, int64_t>>(m);

    m.def(
        "matrix_row_space_size",
        [](BMat<> const& x) { return matrix::row_space_size(x); },
        py::arg("x"),
        R"pbdoc(
:sig=(x:Matrix)->int:

Returns the size of the row space of a boolean matrix. This function returns
the size of the row space of the boolean matrix *x*.

:param x: the matrix.
:type x: Matrix

:returns: The size of the row space of the matrix *x*.
:rtype: int

:complexity:
  :math:`O(mn)` where :math:`m` is the size of the row basis of *x* and
  :math:`n` is the size of the row space.

.. doctest::

   >>> from libsemigroups_pybind11 import Matrix, MatrixKind, matrix
   >>> x = Matrix(MatrixKind.Boolean, [[1,  0,  0],  [0,  0,  1],  [0,  1,  0]])
   >>> matrix.row_space_size(x)
   7
)pbdoc");
    m.def(
        "matrix_row_basis",
        [](BMat<> const& x) {
          std::vector<std::vector<int64_t>> result;
          for (auto rv : matrix::row_basis(x)) {
            result.emplace_back(rv.begin(), rv.end());
          }
          return result;
        },
        py::arg("x"),
        R"pbdoc(
:sig=(x: Matrix) -> list[list[int | PositiveInfinity | NegativeInfinity]]:
:only-document-once:
Returns a row space basis of a matrix as a list of lists. The matrix *x* which
must be one of:

* :any:`MatrixKind.Boolean`
* :any:`MatrixKind.MaxPlusTrunc`

This function returns a row space basis of the matrix *x* as a list of lists
of rows.

:param x: the matrix.
:type x: Matrix

:returns: A basis for the row space of *x*.
:rtype: list[list[int | PositiveInfinity | NegativeInfinity]]

:complexity:
  :math:`O(r ^ 2 c)` where :math:`r` is the number of rows in *x*
  and :math:`c` is the number of columns in *x*.
)pbdoc");

    m.def(
        "matrix_row_basis",
        [](MaxPlusTruncMat<0, 0, 0, int64_t> const& x) {
          std::vector<std::vector<int_or_signed_constant<int64_t>>> result;
          for (auto rv : matrix::row_basis(x)) {
            result.emplace_back(rv.begin(), rv.end());
            from_ints<int64_t>(result.back());
          }
          return result;
        },
        R"pbdoc(
:sig=(x: Matrix) -> list[list[int | PositiveInfinity | NegativeInfinity]]:
:only-document-once:
)pbdoc");
  }

}  // namespace libsemigroups
