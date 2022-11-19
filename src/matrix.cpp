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
#include <stdint.h>  // for uint64_t

// C++ stl headers....
#include <algorithm>         // for replace
#include <cstddef>           // for size_t
#include <initializer_list>  // for initializer_list
#include <iosfwd>            // for string
#include <memory>            // for allocator, make_unique, unique_ptr
#include <regex>             // for regex_replace
#include <string>            // for char_traits, operator==, operator+
#include <unordered_map>     // for operator==, unordered_map
#include <utility>           // for make_pair, pair
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/adapters.hpp>  // for Hash
#include <libsemigroups/matrix.hpp>    // for MaxPlusTruncMat, MinPlusTruncMat
#include <libsemigroups/string.hpp>    // for string_format, to_string

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

      template <typename T>
      std::string matrix_repr(T const& x) {
        std::string result = detail::to_string(x);
        std::replace(result.begin(), result.end(), '{', '[');
        std::replace(result.begin(), result.end(), '}', ']');
        result = std::regex_replace(
            result, std::regex(R"(-2147483648\b)"), "NEGATIVE_INFINITY");
        result = std::regex_replace(
            result, std::regex(R"(\b2147483646\b)"), "POSITIVE_INFINITY");
        return result;
      }

      template <typename T>
      T const* semiring(size_t threshold) {
        static std::unordered_map<size_t, std::unique_ptr<T const>> cache;
        auto it = cache.find(threshold);
        if (it == cache.end()) {
          it = cache.emplace(threshold, std::make_unique<T const>(threshold))
                   .first;
        }
        return it->second.get();
      }

      template <typename T>
      T const* semiring(size_t threshold, size_t period) {
        static std::unordered_map<std::pair<size_t, size_t>,
                                  std::unique_ptr<T const>,
                                  Hash<std::pair<size_t, size_t>>>
             cache;
        auto tp = std::make_pair(threshold, period);
        auto it = cache.find(tp);
        if (it == cache.end()) {
          it = cache.emplace(tp, std::make_unique<T const>(threshold, period))
                   .first;
        }
        return it->second.get();
      }

      template <typename T>
      auto bind_matrix_common(py::module& m, char const* type_name) {
        using Row         = typename T::Row;
        using scalar_type = typename T::scalar_type;

        py::class_<T> x(m, type_name);

        // This one has to come before the vector vector one.
        x.def(py::init<T const&>())
            .def(pybind11::self > pybind11::self)
            .def(pybind11::self != pybind11::self)
            .def("transpose", [](T& x) { x.transpose(); })
            .def(
                "__getitem__",
                [](const T& mat, py::tuple xy) {
                  return mat(xy[0].cast<size_t>(), xy[1].cast<size_t>());
                },
                py::is_operator())
            .def("swap", &T::swap)
            .def("zero", [](T const& x) { return x.zero(); })
            .def("one", [](T const& x) { return x.one(); })
            .def(pybind11::self == pybind11::self)
            .def(pybind11::self < pybind11::self)
            .def("__imul__",
                 [](T& mat, scalar_type a) {
                   mat *= a;
                   return mat;
                 })
            .def("__iadd__",
                 [](T& mat, T const& that) {
                   mat += that;
                   return mat;
                 })
            .def(pybind11::self + pybind11::self)
            .def(pybind11::self * pybind11::self)
            .def("product_inplace", &T::product_inplace)
            .def("number_of_rows",
                 [](T const& x) { return x.number_of_rows(); })
            .def("number_of_cols",
                 [](T const& x) { return x.number_of_cols(); })
            .def("row", [](T const& x, size_t i) { return Row(x.row(i)); })
            .def("rows", [](T const& x) {
              std::vector<Row> rs;
              for (size_t i = 0; i < x.number_of_rows(); ++i) {
                rs.push_back(Row(x.row(i)));
              }
              return rs;
            });

        // TODO(later) no implemented in libsemigroups
        // x.def("__iadd__",
        //      [](T &mat, scalar_type a) {
        //        mat += a;
        //        return mat;
        //      })
        return x;
      }

      template <typename T>
      auto bind_matrix_compile(py::module& m, char const* type_name) {
        using scalar_type = typename T::scalar_type;
        auto x            = bind_matrix_common<T>(m, type_name);

        x.def(py::init<std::vector<std::vector<scalar_type>> const&>())
            .def("__repr__",
                 [type_name](T const& x) -> std::string {
                   std::string str(type_name);
                   if (str == "BMat") {
                     str = "Boolean";
                   } else if (str == "IntMat") {
                     str = "Integer";
                   } else {
                     str.erase(str.end() - 3, str.end());
                   }
                   return string_format("Matrix(MatrixKind.%s, %s)",
                                        str.c_str(),
                                        matrix_repr(x).c_str());
                 })
            .def("__pow__", &matrix_helpers::pow<T>)
            .def_static("make_identity",
                        py::overload_cast<size_t>(&T::identity))
            .def(py::init<size_t, size_t>());
      }

      template <typename T>
      auto bind_matrix_run(py::module& m, char const* type_name) {
        using semiring_type = typename T::semiring_type;
        using scalar_type   = typename T::scalar_type;
        auto x              = bind_matrix_common<T>(m, type_name);

        x.def_static("make",
                     [](size_t                                       threshold,
                        std::vector<std::vector<scalar_type>> const& entries) {
                       // TODO(later) should be T::make but there's no make for
                       // dynamic runtime matrices and vectors!
                       auto result
                           = T(semiring<semiring_type>(threshold), entries);
                       validate(result);
                       return result;
                     })
            .def(py::init([](size_t threshold, size_t r, size_t c) {
              return T(semiring<semiring_type>(threshold), r, c);
            }))
            .def(py::init(
                [](size_t                                       threshold,
                   std::vector<std::vector<scalar_type>> const& entries) {
                  return T(semiring<semiring_type>(threshold), entries);
                }))
            .def_static("make_identity",
                        [](size_t threshold, size_t n) {
                          return T::identity(semiring<semiring_type>(threshold),
                                             n);
                        })
            .def("__repr__", [type_name](T const& x) -> std::string {
              auto n = std::string(type_name).size();
              return string_format(
                  "Matrix(MatrixKind.%s, %llu, %s)",
                  std::string(type_name, type_name + n - 3).c_str(),
                  static_cast<uint64_t>(matrix_threshold(x)),
                  matrix_repr(x).c_str());
            });
      }

      auto bind_ntp_matrix(py::module& m, char const* type_name) {
        using T             = NTPMat<>;
        using semiring_type = typename T::semiring_type;
        using scalar_type   = typename T::scalar_type;
        auto x              = bind_matrix_common<T>(m, type_name);

        x.def_static("make",
                     [](size_t                                       threshold,
                        size_t                                       period,
                        std::vector<std::vector<scalar_type>> const& entries) {
                       // TODO(later) should be T::make but there's no make for
                       // dynamic runtime matrices and vectors!
                       auto result = T(
                           semiring<semiring_type>(threshold, period), entries);
                       validate(result);
                       return result;
                     })
            .def(py::init(
                [](size_t                                       threshold,
                   size_t                                       period,
                   std::vector<std::vector<scalar_type>> const& entries) {
                  return T(semiring<semiring_type>(threshold, period), entries);
                }))
            .def(py::init(
                [](size_t threshold, size_t period, size_t r, size_t c) {
                  return T(semiring<semiring_type>(threshold, period), r, c);
                }))
            .def_static("make_identity",
                        [](size_t threshold, size_t period, size_t n) {
                          return T::identity(
                              semiring<semiring_type>(threshold, period), n);
                        })
            .def("__repr__", [](T const& x) -> std::string {
              return string_format("Matrix(MatrixKind.NTP, %llu, %llu, %s)",
                                   static_cast<uint64_t>(matrix_threshold(x)),
                                   static_cast<uint64_t>(matrix_period(x)),
                                   matrix_repr(x).c_str());
            });
      }

    }  // namespace
  }    // namespace detail

  void init_matrix(py::module& m) {
    detail::bind_matrix_compile<BMat<>>(m, "BMat");
    detail::bind_matrix_compile<IntMat<>>(m, "IntMat");
    detail::bind_matrix_compile<MaxPlusMat<>>(m, "MaxPlusMat");
    detail::bind_matrix_compile<MinPlusMat<>>(m, "MinPlusMat");
    detail::bind_matrix_compile<ProjMaxPlusMat<>>(m, "ProjMaxPlusMat");
    detail::bind_matrix_run<MaxPlusTruncMat<>>(m, "MaxPlusTruncMat");
    detail::bind_matrix_run<MinPlusTruncMat<>>(m, "MinPlusTruncMat");
    detail::bind_ntp_matrix(m, "NTPMat");
  }
}  // namespace libsemigroups
