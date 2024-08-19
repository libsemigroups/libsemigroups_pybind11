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
#include <libsemigroups/adapters.hpp>       // for Hash
#include <libsemigroups/detail/string.hpp>  // for string_format, to_string
#include <libsemigroups/matrix.hpp>  // for MaxPlusTruncMat, MinPlusTruncMat

// pybind11....
#include <pybind11/operators.h>  // for self, self_t, operator!=, operator*
#include <pybind11/pybind11.h>   // for init, class_, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_matrix

// TODO(later):
// 1) RowViews
// 2) make/constructors from Row (so that the output of, e.g. rows, can be used
//    as input.

namespace py = pybind11;
namespace libsemigroups {
  namespace detail {
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

      // TODO remove py_type
      template <typename Mat>
      auto bind_matrix_common(py::module& m, char const* py_type) {
        using Row         = typename Mat::Row;
        using scalar_type = typename Mat::scalar_type;

        std::string  repr_prefix = "Matrix(MatrixKind.", repr_short;
        size_t const cols        = 80;

        std::function<std::string(Mat const&)> repr;

        // TODO simplify
        // Have to pass repr_prefix and repr_short by value o/w they
        // don't exist after this function is called
        if constexpr (IsIntMat<Mat>) {
          repr_prefix += "Integer, ";
          repr_short = "integer matrix";
          repr       = [repr_prefix, repr_short](Mat const& x) {
            return to_human_readable_repr(
                x, repr_prefix, repr_short, "[]", cols);
          };
        } else if constexpr (IsBMat<Mat>) {
          repr_prefix += "Boolean, ";
          repr_short = "boolean matrix";
          repr       = [repr_prefix, repr_short](Mat const& x) {
            return to_human_readable_repr(
                x, repr_prefix, repr_short, "[]", cols);
          };
        } else if constexpr (IsMaxPlusMat<Mat>) {
          repr_prefix += "MaxPlus, ";
          repr_short = "max-plus matrix";
          repr       = [repr_prefix, repr_short](Mat const& x) {
            return to_human_readable_repr(
                x, repr_prefix, repr_short, "[]", cols);
          };
        } else if constexpr (IsMinPlusMat<Mat>) {
          repr_prefix += "MinPlus, ";
          repr_short = "min-plus matrix";
          repr       = [repr_prefix, repr_short](Mat const& x) {
            return to_human_readable_repr(x, repr_prefix, repr_short, "[]", 80);
          };
        } else if constexpr (IsProjMaxPlusMat<Mat>) {
          repr_prefix += "ProjMaxPlus, ";
          repr_short = "proj. max-plus matrix";
          repr       = [repr_prefix, repr_short](Mat const& x) {
            return to_human_readable_repr(x, repr_prefix, repr_short, "[]", 80);
          };
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
        }

        py::class_<Mat> thing(m, py_type);

        thing.def("__repr__", repr);
        thing.def(py::init<Mat const&>());
        thing.def("__hash__", &Mat::hash_value);
        thing.def(
            "__getitem__",
            [](const Mat& mat, py::tuple xy) {
              return mat.at(xy[0].cast<size_t>(), xy[1].cast<size_t>());
            },
            py::is_operator());
        thing.def(
            "__getitem__",
            [](Mat const& thing, size_t i) {
              try {
                auto r = thing.row(i);
                return std::vector<typename Mat::scalar_type>(r.begin(),
                                                              r.end());
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
              std::copy(row.cbegin(), row.cend(), rv.begin());
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
        thing.def("__len__",
                  [](Mat const& thing) { return thing.number_of_rows(); });
        thing.def("product_inplace",
                  [](Mat& xy, Mat const& thing, Mat const& y) {
                    matrix::throw_if_bad_dim(thing, y);
                    matrix::throw_if_bad_dim(xy, thing);
                    xy.product_inplace_no_checks(thing, y);
                  });
        thing.def("transpose", [](Mat& thing) { thing.transpose(); });
        thing.def("swap", &Mat::swap);
        thing.def("scalar_zero",
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
        thing.def("__pow__", &matrix::pow<Mat>);
        return thing;
      }

      template <typename Mat>
      auto bind_matrix_compile(py::module& m, char const* py_type) {
        using scalar_type = typename Mat::scalar_type;
        auto x            = bind_matrix_common<Mat>(m, py_type);

        // TODO(0) use move constructor
        x.def(py::init([](std::vector<std::vector<scalar_type>> const& rows) {
           return to_matrix<Mat>(rows);
         }))
            .def(py::init<size_t, size_t>())
            .def("one", [](Mat const& self, size_t n) { return Mat::one(n); })
            .def("one", py::overload_cast<>(&Mat::one, py::const_));
      }

      template <typename Mat>
      auto bind_matrix_run(py::module& m, char const* py_type) {
        using semiring_type = typename Mat::semiring_type;
        using scalar_type   = typename Mat::scalar_type;
        auto x              = bind_matrix_common<Mat>(m, py_type);

        x.def(py::init([](size_t threshold, size_t r, size_t c) {
           return Mat(semiring<semiring_type>(threshold), r, c);
         }))
            .def(py::init(
                [](size_t                                       threshold,
                   std::vector<std::vector<scalar_type>> const& entries) {
                  return to_matrix<Mat>(semiring<semiring_type>(threshold),
                                        entries);
                }))
            .def("one",
                 [](Mat const& self, size_t n) {
                   return Mat::one(
                       semiring<semiring_type>(matrix::threshold(self)), n);
                 })
            .def("one", [](Mat const& self) { return self.one(); });
      }

      auto bind_ntp_matrix(py::module& m, char const* py_type) {
        using Mat           = NTPMat<0, 0, 0, 0, int64_t>;
        using semiring_type = typename Mat::semiring_type;
        using scalar_type   = typename Mat::scalar_type;
        auto x              = bind_matrix_common<Mat>(m, py_type);

        x.def(
             py::init([](size_t                                       threshold,
                         size_t                                       period,
                         std::vector<std::vector<scalar_type>> const& entries) {
               return to_matrix<Mat>(semiring<semiring_type>(threshold, period),
                                     entries);
             }))
            .def(py::init(
                [](size_t threshold, size_t period, size_t r, size_t c) {
                  return Mat(semiring<semiring_type>(threshold, period), r, c);
                }))
            .def("one",
                 [](Mat const& self, size_t n) {
                   return Mat::one(
                       semiring<semiring_type>(matrix::threshold(self),
                                               matrix::period(self)),
                       n);
                 })
            .def("one", [](Mat const& self) { return self.one(); });
      }
    }  // namespace
  }  // namespace detail

  void init_matrix(py::module& m) {
    detail::bind_matrix_compile<BMat<>>(m, "BMat");
    detail::bind_matrix_compile<IntMat<0, 0, int64_t>>(m, "IntMat");
    detail::bind_matrix_compile<MaxPlusMat<0, 0, int64_t>>(m, "MaxPlusMat");
    detail::bind_matrix_compile<MinPlusMat<0, 0, int64_t>>(m, "MinPlusMat");
    detail::bind_matrix_compile<ProjMaxPlusMat<0, 0, int64_t>>(
        m, "ProjMaxPlusMat");
    detail::bind_matrix_run<MaxPlusTruncMat<0, 0, 0, int64_t>>(
        m, "MaxPlusTruncMat");
    detail::bind_matrix_run<MinPlusTruncMat<0, 0, 0, int64_t>>(
        m, "MinPlusTruncMat");
    detail::bind_ntp_matrix(m, "NTPMat");
  }
}  // namespace libsemigroups
