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

#include <variant>

#include <pybind11/pybind11.h>

#include <libsemigroups/constants.hpp>

namespace libsemigroups {

  namespace py = pybind11;

  void init_detail_cong_common(py::module&);
  void init_detail_knuth_bendix_impl(py::module&);
  void init_detail_todd_coxeter_impl(py::module&);

  void init_action(py::module&);
  void init_aho_corasick(py::module&);
  void init_bipart(py::module&);
  void init_blocks(py::module&);
  void init_bmat8(py::module&);
  void init_cong(py::module&);
  void init_dot(py::module&);
  void init_forest(py::module&);
  void init_freeband(py::module&);
  void init_froidure_pin(py::module&);
  void init_froidure_pin_base(py::module&);
  void init_gabow(py::module&);
  void init_imagerightaction(py::module&);
  void init_inverse_present(py::module&);
  void init_kambites(py::module&);
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
  void init_ukkonen(py::module&);
  void init_word_graph(py::module&);
  void init_words(py::module&);

  template <typename Int>
  std::vector<Int>
  to_ints(std::vector<std::variant<Int, Undefined>> const& vec) {
    std::vector<Int> vec_as_ints;
    for (auto const& val : vec) {
      if (std::holds_alternative<Int>(val)) {
        vec_as_ints.push_back(std::get<0>(val));
      } else {
        vec_as_ints.push_back(static_cast<Int>(std::get<1>(val)));
      }
    }
    return vec_as_ints;
  }

  template <typename Int>
  std::vector<std::vector<Int>>
  to_ints(std::vector<std::vector<std::variant<Int, Undefined>>> const& vec) {
    std::vector<std::vector<Int>> vec_as_ints;
    for (auto const& val : vec) {
      vec_as_ints.push_back(to_ints(val));
    }
    return vec_as_ints;
  }

}  // namespace libsemigroups

#endif  // SRC_MAIN_HPP_
