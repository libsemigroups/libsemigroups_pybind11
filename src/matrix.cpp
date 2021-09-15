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
// 2) make/constructors from Row (so that the output of, e.g. rows, can be used
//    as input.

namespace py = pybind11;
namespace libsemigroups {
  namespace detail {
    namespace {

      template <typename T>
      std::string matrix_repr(T const &x) {
        std::string result = detail::to_string(x);
        std::replace(result.begin(), result.end(), '{', '[');
        std::replace(result.begin(), result.end(), '}', ']');
        return result;
      }

      template <typename T>
      auto bind_matrix_common(py::module &m,
                              char const *type_name,
                              char const *long_name) {
        using Row         = typename T::Row;
        using scalar_type = typename T::scalar_type;

        std::string doc = string_format(
            R"pbdoc(
              This class implements :math:`m \times n` %s matrices for
              arbitrary dimensions :math:`m` and :math:`n`.
             )pbdoc",
            long_name);

        py::class_<T> x(m, type_name, doc.c_str());

        doc = string_format(
            R"pbdoc(
              Copy a matrix.

              :Parameters: - **x** (%s) the matrix to copy.
            )pbdoc",
            type_name);
        // This one has to come before the vector vector one.
        x.def(py::init<T const &>(), doc.c_str())
            .def(pybind11::self > pybind11::self)   // no doc
            .def(pybind11::self != pybind11::self)  // no doc
            .def(
                "transpose",
                [](T &x) { x.transpose(); },
                R"pbdoc(
                  Transposes the matrix in-place.

                  :parameters: (None)
                  :returns: (None)

                  .. warning::
                    This only works for square matrices.
                )pbdoc")
            .def(
                "__getitem__",
                [](const T &mat, py::tuple xy) {
                  return mat(xy[0].cast<size_t>(), xy[1].cast<size_t>());
                },
                py::arg("tup"),
                py::is_operator(),
                R"pbdoc(
                  Returns value in a given position.

                  :param tup: a tuple consisting of the row and column indices.
                  :type tup: tuple

                  :return: An ``int``.
                )pbdoc")
            .def("swap",
                 &T::swap,
                 py::arg("that"),
                 R"pbdoc(
                   Swaps the contents of ``self`` with the contents of ``that``.

                   :param that: the matrix to swap contents with

                   :returns: (None)
                 )pbdoc")
            .def(
                "zero",
                [](T const &x) { return x.zero(); },
                R"pbdoc(
                  Returns the zero of the underlying semiring.

                  :parameters: None
                  :returns: An ``int``.
                )pbdoc")
            .def(
                "one",
                [](T const &x) { return x.one(); },
                R"pbdoc(
                  Returns the one of the underlying semiring.

                  :parameters: None
                  :returns: An ``int``.
                )pbdoc");

        doc = string_format(
            R"pbdoc(
               Equality comparison.

               Returns ``True`` if ``self`` equals ``that``.

               :param that: the matrix for comparison.
               :type that: %s

               :returns: A ``bool``.
             )pbdoc",
            type_name);
        x.def(pybind11::self == pybind11::self, py::arg("that"), doc.c_str());

        doc = string_format(
            R"pbdoc(
              Less than comparison.

              Returns ``True`` if ``self`` is less than ``that``.

              :param that: the matrix for comparison.
              :type that: %s

              :returns: A ``bool``.
            )pbdoc",
            type_name);
        x.def(pybind11::self < pybind11::self, py::arg("that"), doc.c_str());

        x.def(
            "__imul__",
            [](T &mat, scalar_type a) {
              mat *= a;
              return mat;
            },
            R"pbdoc(TODO)pbdoc");
        x.def(
            "__iadd__",
            [](T &mat, T const &that) {
              mat += that;
              return mat;
            },
            R"pbdoc(TODO)pbdoc");
        x.def(pybind11::self + pybind11::self, R"pbdoc(TODO)pbdoc");
        x.def(pybind11::self * pybind11::self, R"pbdoc(TODO)pbdoc");

        doc = string_format(
            R"pbdoc(
              Multiply two matrices and stores the product in ``self``.

              :param x: matrix to multiply.
              :type x: %s
              :param y: matrix to multiply.
              :type y: %s

              :return: (None)
            )pbdoc",
            type_name,
            type_name);
        x.def("product_inplace",
              &T::product_inplace,
              py::arg("x"),
              py::arg("y"),
              doc.c_str());

        doc = string_format(R"pbdoc(
                              Returns the number of rows.

                              .. code-block:: python

                                 from libsemigroups_pybind11 import %s
                                 x = %s([[0, 1], [1, 0]])
                                 x.number_of_rows() # returns 2
                            )pbdoc",
                            type_name,
                            type_name);
        x.def(
            "number_of_rows",
            [](T const &x) { return x.number_of_rows(); },
            doc.c_str());

        doc = string_format(R"pbdoc(
                              Returns the number of columns.

                              .. code-block:: python

                                 from libsemigroups_pybind11 import %s
                                 x = %s([[0, 1], [1, 0]])
                                 x.number_of_cols() # returns 2
                            )pbdoc",
                            type_name,
                            type_name);
        x.def(
            "number_of_cols",
            [](T const &x) { return x.number_of_cols(); },
            doc.c_str());

        doc = string_format(R"pbdoc(
                              Returns the specified row.

                              :param i: the index of the row

                              :returns: A ``%s``.
                            )pbdoc",
                            type_name);
        x.def(
            "row",
            [](T const &x, size_t i) { return Row(x.row(i)); },
            doc.c_str());

        doc = string_format(R"pbdoc(
                              Returns the rows.

                              :parameters: (None)
                              :returns: A ``List[%s]``.
                            )pbdoc",
                            type_name);

        x.def(
            "rows",
            [](T const &x) {
              std::vector<Row> rs;
              for (size_t i = 0; i < x.number_of_rows(); ++i) {
                rs.push_back(Row(x.row(i)));
              }
              return rs;
            },
            doc.c_str());

        // TODO(later) no implemented in libsemigroups
        // x.def("__iadd__",
        //      [](T &mat, scalar_type a) {
        //        mat += a;
        //        return mat;
        //      })
        return x;
      }

      template <typename T>
      auto bind_matrix_compile(py::module &m,
                               char const *type_name,
                               char const *long_name) {
        using scalar_type = typename T::scalar_type;
        auto x            = bind_matrix_common<T>(m, type_name, long_name);

        x.def(py::init<std::vector<std::vector<scalar_type>> const &>(),
              py::arg("x"),
              R"pbdoc(
                Construct a matrix.

                :Parameters: - **x** (List[List[int]]) the values to be copied into the matrix.
              )pbdoc")
            .def("__repr__",
                 [type_name](T const &x) -> std::string {
                   return std::string(type_name) + "(" + matrix_repr(x) + ")";
                 })
            .def_static("make_identity",
                        py::overload_cast<size_t>(&T::identity),
                        py::arg("n"),
                        R"pbdoc(
                       Construct the :math:`n \times n` identity matrix.

                       :param n: the dimension

                       :returns: The :math:`n \times n` identity matrix.
                     )pbdoc");

        std::string doc = string_format(
            R"pbdoc(
              Construct a matrix.

              :param r: the number of rows in the matrix
              :type r: int
              :param c: the number of columns in the matrix
              :type c: int

              **Example**

              .. code-block:: cpp

                 m = %s(2, 3); // construct a 2 x 3 matrix
            )pbdoc",
            type_name);
        x.def(py::init<size_t, size_t>(),
              py::arg("r"),
              py::arg("c"),
              doc.c_str());
      }

      template <typename T>
      T const *semiring(size_t threshold) {
        static std::unordered_map<size_t, std::unique_ptr<T const>> cache;
        auto it = cache.find(threshold);
        if (it == cache.end()) {
          it = cache.emplace(threshold, std::make_unique<T const>(threshold))
                   .first;
        }
        return it->second.get();
      }

      template <typename T>
      T const *semiring(size_t threshold, size_t period) {
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
      auto bind_matrix_run(py::module &m,
                           char const *type_name,
                           char const *long_name) {
        using semiring_type = typename T::semiring_type;
        using scalar_type   = typename T::scalar_type;
        auto x              = bind_matrix_common<T>(m, type_name, long_name);

        std::string doc = R"pbdoc(
                             Construct a matrix.

                             :Parameters:
                                - **threshold** (int) the threshold
                                - **x** (List[List[int]]) the values to be
                                  copied into the matrix.
                           )pbdoc";

        x.def_static(
            "make",
            [](size_t                                       threshold,
               std::vector<std::vector<scalar_type>> const &entries) {
              // TODO(now) should be T::make but there's no make for
              // dynamic runtime matrices and vectors!
              auto result = T(semiring<semiring_type>(threshold), entries);
              validate(result);
              return result;
            },
            py::arg("threshold"),
            py::arg("entries"),
            doc.c_str());

        doc = string_format(
            R"pbdoc(
               Construct a matrix.

               :Parameters: - **threshold** (int) the threshold.
                            - **r** (int) the number of rows.
                            - **c** (int) the number of columns.

               **Example**

               .. code-block:: cpp

                  m = %s(11, 2, 3); // construct a 2 x 3 matrix with threshold 11
             )pbdoc",
            type_name);
        x.def_static(
             "make",
             [](size_t threshold, size_t r, size_t c) {
               return T(semiring<semiring_type>(threshold), r, c);
             },
             py::arg("threshold"),
             py::arg("r"),
             py::arg("c"),
             doc.c_str())
            .def_static(
                "make_identity",
                [](size_t threshold, size_t n) {
                  return T::identity(semiring<semiring_type>(threshold), n);
                },
                py::arg("threshold"),
                py::arg("n"),
                R"pbdoc(
                       Construct the :math:`n \times n` identity matrix.
                       TODO
                       :param n: the dimension

                       :returns: The :math:`n \times n` identity matrix.
                     )pbdoc");

        x.def("__repr__", [type_name](T const &x) -> std::string {
          return std::string(type_name) + "(" + to_string(matrix_threshold(x))
                 + ", " + matrix_repr(x) + ")";
        });
      }

      auto bind_ntp_matrix(py::module &m,
                           char const *type_name,
                           char const *long_name) {
        using T             = NTPMat<>;
        using semiring_type = typename T::semiring_type;
        using scalar_type   = typename T::scalar_type;
        auto x              = bind_matrix_common<T>(m, type_name, long_name);

        std::string doc = R"pbdoc(
                             Construct a matrix.

                             :Parameters:
                                - **threshold** (int) the threshold
                                - **period** (int) the period
                                - **x** (List[List[int]]) the values to be
                                  copied into the matrix.
                           )pbdoc";

        x.def_static(
            "make",
            [](size_t                                       threshold,
               size_t                                       period,
               std::vector<std::vector<scalar_type>> const &entries) {
              // TODO(now) should be T::make but there's no make for
              // dynamic runtime matrices and vectors!
              auto result
                  = T(semiring<semiring_type>(threshold, period), entries);
              validate(result);
              return result;
            },
            py::arg("threshold"),
            py::arg("period"),
            py::arg("entries"),
            doc.c_str());

        doc = string_format(
            R"pbdoc(
               Construct a matrix.

               :Parameters: - **threshold** (int) the threshold.
                            - **period** (int) the period
                            - **r** (int) the number of rows.
                            - **c** (int) the number of columns.

               **Example**

               .. code-block:: cpp

                  m = %s(7, 5, 2, 3); // construct a 2 x 3 matrix with threshold 11
             )pbdoc",
            type_name);
        x.def_static(
             "make",
             [](size_t threshold, size_t period, size_t r, size_t c) {
               return T(semiring<semiring_type>(threshold, period), r, c);
             },
             py::arg("threshold"),
             py::arg("period"),
             py::arg("r"),
             py::arg("c"),
             doc.c_str())
            .def_static(
                "make_identity",
                [](size_t threshold, size_t period, size_t n) {
                  return T::identity(semiring<semiring_type>(threshold, period),
                                     n);
                },
                py::arg("threshold"),
                py::arg("period"),
                py::arg("n"),
                R"pbdoc(
                       Construct the :math:`n \times n` identity matrix.
                       TODO
                       :param n: the dimension

                       :returns: The :math:`n \times n` identity matrix.
                     )pbdoc");

        x.def("__repr__", [type_name](T const &x) -> std::string {
          return string_format("%s(%llu, %llu, %s)",
                               type_name,
                               static_cast<uint64_t>(matrix_threshold(x)),
                               static_cast<uint64_t>(matrix_period(x)),
                               matrix_repr(x).c_str());
        });
      }

    }  // namespace
  }    // namespace detail

  void init_matrix(py::module &m) {
    detail::bind_matrix_compile<BMat<>>(m, "BMat", "boolean");
    detail::bind_matrix_compile<IntMat<>>(m, "IntMat", "integer");
    detail::bind_matrix_compile<MaxPlusMat<>>(m, "MaxPlusMat", "max-plus");
    detail::bind_matrix_compile<MinPlusMat<>>(m, "MinPlusMat", "min-plus");
    detail::bind_matrix_compile<ProjMaxPlusMat<>>(
        m, "ProjMaxPlusMat", "projective max-plus");

    detail::bind_matrix_run<MaxPlusTruncMat<>>(
        m, "MaxPlusTruncMat", "max-plus trunc");
    detail::bind_matrix_run<MinPlusTruncMat<>>(
        m, "MinPlusTruncMat", "min-plus trunc");

    detail::bind_ntp_matrix(m, "NTPMat", "ntp");
  }
}  // namespace libsemigroups
