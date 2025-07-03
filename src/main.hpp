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

#ifndef SRC_MAIN_HPP_
#define SRC_MAIN_HPP_

#include <type_traits>
#include <variant>  // for variant

#include <pybind11/pybind11.h>

#include <libsemigroups/constants.hpp>

namespace libsemigroups {

  namespace py = pybind11;

  void init_detail_cong_common(py::module&);
  void init_detail_knuth_bendix_impl(py::module&);
  void init_detail_todd_coxeter_impl(py::module&);
  void init_detail_report(py::module&);

  void init_action(py::module&);
  void init_aho_corasick(py::module&);
  void init_bipart(py::module&);
  void init_blocks(py::module&);
  void init_bmat8(py::module&);
  void init_cong(py::module&);
  void init_constants(py::module&);
  void init_dot(py::module&);
  void init_error(py::module&);
  void init_forest(py::module&);
  void init_freeband(py::module&);
  void init_froidure_pin(py::module&);
  void init_froidure_pin_base(py::module&);
  void init_gabow(py::module&);
  void init_imagerightaction(py::module&);
  void init_inverse_present(py::module&);
  void init_kambites(py::module&);
  void init_kbe(py::module&);
  void init_knuth_bendix(py::module&);
  void init_konieczny(py::module&);
  void init_matrix(py::module&);
  void init_obvinf(py::module&);
  void init_order(py::module&);
  void init_paths(py::module&);
  void init_pbr(py::module&);
  void init_present(py::module&);
  void init_presentation_examples(py::module&);
  void init_ranges(py::module&);
  void init_reporter(py::module&);
  void init_runner(py::module&);
  void init_schreier_sims(py::module&);
  void init_sims(py::module&);
  void init_stephen(py::module&);
  void init_to_congruence(py::module&);
  void init_to_froidure_pin(py::module&);
  void init_to_knuth_bendix(py::module&);
  void init_to_present(py::module&);
  void init_to_present(py::module&);
  void init_to_todd_coxeter(py::module&);
  void init_todd_coxeter(py::module&);
  void init_transf(py::module&);
  void init_types(py::module&);
  void init_ukkonen(py::module&);
  void init_word_graph(py::module&);
  void init_words(py::module&);

  template <typename Int>
  using int_or_unsigned_constant
      = std::variant<Int, Undefined, PositiveInfinity, LimitMax>;

  template <typename Int>
  using int_or_signed_constant = std::
      variant<Int, Undefined, PositiveInfinity, LimitMax, NegativeInfinity>;

  template <typename Int>
  using int_or_constant = std::conditional_t<std::is_signed_v<Int>,
                                             int_or_signed_constant<Int>,
                                             int_or_unsigned_constant<Int>>;

  template <typename Int>
  Int to_int(int_or_constant<Int> val) {
    if (std::holds_alternative<Int>(val)) {
      return std::get<0>(val);
    } else if (std::holds_alternative<Undefined>(val)) {
      return static_cast<Int>(std::get<1>(val));
    } else if (std::holds_alternative<PositiveInfinity>(val)) {
      return static_cast<Int>(std::get<2>(val));
    } else if (std::holds_alternative<LimitMax>(val)) {
      return static_cast<Int>(std::get<3>(val));
    }
    if constexpr (std::is_signed_v<Int>) {
      if (std::holds_alternative<NegativeInfinity>(val)) {
        return static_cast<Int>(std::get<4>(val));
      }
    }
    return 0;  // To keep the compiler happy
  }

  template <typename Int>
  std::vector<Int> to_ints(std::vector<int_or_constant<Int>> const& vec) {
    std::vector<Int> vec_as_ints;
    for (auto const& val : vec) {
      vec_as_ints.push_back(to_int<Int>(val));
    }
    return vec_as_ints;
  }

  template <typename Int>
  std::vector<std::vector<Int>>
  to_ints(std::vector<std::vector<int_or_constant<Int>>> const& vec) {
    std::vector<std::vector<Int>> vec_as_ints;
    for (auto const& val : vec) {
      vec_as_ints.push_back(to_ints<Int>(val));
    }
    return vec_as_ints;
  }

  template <typename Int>
  int_or_constant<Int> from_int(int_or_constant<Int> const& val) {
    if (std::holds_alternative<Int>(val)) {
      if constexpr (std::is_signed_v<Int>) {
        if (std::get<0>(val) == static_cast<Int>(NEGATIVE_INFINITY)) {
          return {NEGATIVE_INFINITY};
        }
      }
      if (std::get<0>(val) == static_cast<Int>(UNDEFINED)) {
        return {UNDEFINED};
      } else if (std::get<0>(val) == static_cast<Int>(POSITIVE_INFINITY)) {
        return {POSITIVE_INFINITY};
      } else if (std::get<0>(val) == static_cast<Int>(LIMIT_MAX)) {
        return {LIMIT_MAX};
      }
    }
    return val;
  }

  template <typename Int>
  int_or_constant<Int> from_int(Int val) {
    if constexpr (std::is_signed_v<Int>) {
      if (val == static_cast<Int>(NEGATIVE_INFINITY)) {
        return {NEGATIVE_INFINITY};
      }
    }
    if (val == static_cast<Int>(UNDEFINED)) {
      return {UNDEFINED};
    } else if (val == static_cast<Int>(POSITIVE_INFINITY)) {
      return {POSITIVE_INFINITY};
    } else if (val == static_cast<Int>(LIMIT_MAX)) {
      return {LIMIT_MAX};
    }
    return {val};
  }

  template <typename Int>
  void from_ints(std::vector<int_or_constant<Int>>& vec) {
    for (auto& val : vec) {
      if (std::holds_alternative<Int>(val)) {
        val = from_int<Int>(val);
      }
    }
  }

  template <typename Int>
  std::pair<int_or_constant<Int>, int_or_constant<Int>>
  from_ints(std::pair<Int, Int> const& pair) {
    return {from_int<Int>(pair.first), from_int<Int>(pair.second)};
  }

  template <typename Int>
  std::tuple<int_or_constant<Int>, int_or_constant<Int>>
  from_ints(std::tuple<Int, Int> const& tup) {
    return {from_int<Int>(std::get<0>(tup)), from_int<Int>(std::get<1>(tup))};
  }
}  // namespace libsemigroups

#endif  // SRC_MAIN_HPP_
