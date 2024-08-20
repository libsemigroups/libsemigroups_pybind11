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
#include <libsemigroups/constants.hpp>  // for MaxPlusTruncMat, MinPlusTruncMat
#include <libsemigroups/matrix.hpp>     // for MaxPlusTruncMat, MinPlusTruncMat

#include <libsemigroups/detail/string.hpp>  // for string_format, to_string

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for init, class_, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_matrix

namespace py = pybind11;

namespace libsemigroups {
  namespace {

    template <typename Mat>
    Mat const* semiring(size_t threshold) {
      static std::unordered_map<size_t, std::unique_ptr<Mat const>> cache;
      auto it = cache.find(threshold);
      if (it == cache.end()) {
        it = cache.emplace(threshold, std::make_unique<Mat const>(threshold))
                 .first;
      }
      return it->second.get();
    }

    template <typename Mat>
    Mat const* semiring(size_t threshold, size_t period) {
      static std::unordered_map<std::pair<size_t, size_t>,
                                std::unique_ptr<Mat const>,
                                Hash<std::pair<size_t, size_t>>>
           cache;
      auto tp = std::make_pair(threshold, period);
      auto it = cache.find(tp);
      if (it == cache.end()) {
        it = cache.emplace(tp, std::make_unique<Mat const>(threshold, period))
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
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", 80);
        };
        py_type = "MinPlusMat";
      } else if constexpr (IsProjMaxPlusMat<Mat>) {
        repr_prefix += "ProjMaxPlus, ";
        repr_short = "proj. max-plus matrix";
        repr       = [repr_prefix, repr_short](Mat const& x) {
          return to_human_readable_repr(x, repr_prefix, repr_short, "[]", 80);
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
                                        80);
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
                                        80);
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
                                        80);
        };
        py_type = "NTPMat";
      }

      py::class_<Mat> thing(m, py_type.c_str());

      thing.def("__repr__", repr);
      thing.def("__hash__", &Mat::hash_value);
      thing.def("__copy__", [](Mat const& x) { return Mat(x); });
      thing.def(
          "_at",
          [](const Mat& mat, py::tuple xy) {
            return mat.at(xy[0].cast<size_t>(), xy[1].cast<size_t>());
          },
          py::is_operator());
      thing.def(
          "_at",
          [](Mat const& thing, size_t i) {
            try {
              auto r = thing.row(i);
              return std::vector<typename Mat::scalar_type>(r.begin(), r.end());
            } catch (LibsemigroupsException const& e) {
              // This is done so that "list" works as expected for a
              // matrix
              throw std::out_of_range(e.what());
            }
          },
          py::is_operator());
      thing.def(
          "__setitem__",
          [](Mat& mat, py::tuple xy, typename Mat::scalar_type val) {
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
          [](Mat&                                          mat,
             size_t                                        r,
             std::vector<typename Mat::scalar_type> const& row) {
            auto rv = mat.row(r);
            if (row.size() != rv.size()) {
              LIBSEMIGROUPS_EXCEPTION(
                  "invalid row length, expected {}, but found {}",
                  rv.size(),
                  row.size());
            }
            for (auto item : row) {
              matrix::throw_if_bad_entry(mat, item);
            }
            std::copy(row.cbegin(), row.cend(), rv.begin());
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

      thing.def("copy", [](Mat const& x) { return Mat(x); });
      thing.def("product_inplace", [](Mat& xy, Mat const& thing, Mat const& y) {
        matrix::throw_if_bad_dim(thing, y);
        matrix::throw_if_bad_dim(xy, thing);
        xy.product_inplace_no_checks(thing, y);
      });
      thing.def("transpose", [](Mat& thing) { thing.transpose(); });
      thing.def("swap", &Mat::swap);
      thing.def("_scalar_zero",
                [](Mat const& thing) { return thing.scalar_zero(); });
      thing.def("scalar_one",
                [](Mat const& thing) { return thing.scalar_one(); });
      thing.def("number_of_rows",
                [](Mat const& thing) { return thing.number_of_rows(); });
      thing.def("number_of_cols",
                [](Mat const& thing) { return thing.number_of_cols(); });
      thing.def("row",
                [](Mat const& thing, size_t i) { return Row(thing.row(i)); });
      thing.def("rows", [](Mat const& thing) {
        std::vector<Row> rows;
        for (size_t i = 0; i < thing.number_of_rows(); ++i) {
          rows.push_back(Row(thing.row(i)));
        }
        return rows;
      });
      return thing;
    }

    template <typename Mat>
    auto bind_matrix_no_semiring(py::module& m) {
      using scalar_type = typename Mat::scalar_type;
      auto thing        = bind_matrix_common<Mat>(m);

      thing.def(py::init([](std::vector<std::vector<scalar_type>> const& rows) {
        return to_matrix<Mat>(rows);
      }));
      thing.def(py::init<size_t, size_t>());
      thing.def("one", [](Mat const& self, size_t n) { return Mat::one(n); });
      thing.def("one", py::overload_cast<>(&Mat::one, py::const_));
    }

    template <typename Mat>
    auto bind_matrix_trunc_semiring(py::module& m) {
      using semiring_type = typename Mat::semiring_type;
      using scalar_type   = typename Mat::scalar_type;
      auto thing          = bind_matrix_common<Mat>(m);

      thing.def(py::init([](size_t threshold, size_t r, size_t c) {
        return Mat(semiring<semiring_type>(threshold), r, c);
      }));
      thing.def(
          py::init([](size_t                                       threshold,
                      std::vector<std::vector<scalar_type>> const& entries) {
            return to_matrix<Mat>(semiring<semiring_type>(threshold), entries);
          }));
      thing.def("one", [](Mat const& self, size_t n) {
        return Mat::one(semiring<semiring_type>(matrix::threshold(self)), n);
      });
      thing.def("one", [](Mat const& self) { return self.one(); });

      m.def(
          "threshold",
          [](Mat const& x) { return matrix::threshold(x); },
          py::arg("x"),
          R"pbdoc(
:sig=(x:Matrix)->int:
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
      auto thing          = bind_matrix_common<Mat>(m);

      thing.def(
          py::init([](size_t                                       threshold,
                      size_t                                       period,
                      std::vector<std::vector<scalar_type>> const& entries) {
            return to_matrix<Mat>(semiring<semiring_type>(threshold, period),
                                  entries);
          }));
      thing.def(
          py::init([](size_t threshold, size_t period, size_t r, size_t c) {
            return Mat(semiring<semiring_type>(threshold, period), r, c);
          }));
      thing.def("one", [](Mat const& self, size_t n) {
        return Mat::one(semiring<semiring_type>(matrix::threshold(self),
                                                matrix::period(self)),
                        n);
      });
      thing.def("one", [](Mat const& self) { return self.one(); });

      m.def(
          "period",
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
          "threshold",
          [](Mat const& x) { return matrix::threshold(x); },
          py::arg("x"),
          R"pbdoc(
:sig=(x:Matrix)->int:
:only-document-once:
Returns the threshold of a matrix over a truncated semiring.

This function returns the threshold of a matrix over a truncated semiring,
that is a matrix whose kind is any of:

* :any:`MatrixKind.MaxPlusTrunc`
* :any:`MatrixKind.MinPlusTrunc`
* :any:`MatrixKind.NTP`

:param x: the matrix.
:type x: Mat

:returns: The threshold of *x*.
:rtype: int
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
        "row_basis",
        [](BMat<> const& x) {
          std::vector<std::vector<int64_t>> result;
          for (auto rv : matrix::row_basis(x)) {
            result.emplace_back(rv.begin(), rv.end());
          }
          return result;
        },
        py::arg("x"),
        R"pbdoc(
:sig=(x:Matrix)->List[List[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]:
Returns a row space basis of a matrix as a list of lists. The matrix *x* which
must be one of:

* :any:`MatrixKind.Boolean`
* :any:`MatrixKind.MaxPlusTrunc`

This function returns a row space basis of the matrix *x* as a list of lists
of rows.

:param x: the matrix.
:type x: Matrix

:complexity:
  :math:`O(r ^ 2 c)` where :math:`r` is the number of rows in ``x``
  and :math:`c` is the number of columns in ``x``.

:returns: A basis for the row space of *x*.
:rtype: List[List[int | POSITIVE_INFINITY | NEGATIVE_INFINITY]]
)pbdoc");
    m.def("row_basis", [](MaxPlusTruncMat<0, 0, 0, int64_t> const& x) {
      std::vector<std::vector<int64_t>> result;
      for (auto rv : matrix::row_basis(x)) {
        result.emplace_back(rv.begin(), rv.end());
      }
      return result;
    });
  }

}  // namespace libsemigroups
