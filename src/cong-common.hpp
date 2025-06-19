//
// libsemigroups_pybind11 - python bindings for the C++ library libsemigroups
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

#ifndef SRC_CONG_COMMON_HPP_
#define SRC_CONG_COMMON_HPP_

#include <string_view>

#include <pybind11/pybind11.h>  // for class_, make_iterator, enum_, init

namespace libsemigroups {
  namespace py = pybind11;
  namespace detail {
    class CongruenceCommon;
  }

  // A little helper struct to simulate keyword arguments for the functions
  // declared below.
  struct doc {
    std::string_view detail             = "";
    std::string_view example            = "";
    std::string_view extra_kwargs       = "";
    std::string_view extra_kwargs_doc   = "";
    bool             only_document_once = false;
    std::string_view raises             = "";
    std::string_view var                = "";
  };

  ////////////////////////////////////////////////////////////////////////
  // Member functions/methods
  ////////////////////////////////////////////////////////////////////////

  // Explicit instances of these functions and their implementations are in the
  // cpp file, since we know every type they are used with a priori.

  template <typename Thing, typename ThingBase>
  void def_construct_default(py::class_<Thing, ThingBase>& thing,
                             std::string_view              name,
                             doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_init_default(py::class_<Thing, ThingBase>& thing,
                        std::string_view              name,
                        doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_construct_kind_presentation(py::class_<Thing, ThingBase>& thing,
                                       std::string_view              name,
                                       doc extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_init_kind_presentation(py::class_<Thing, ThingBase>& thing,
                                  std::string_view              name,
                                  doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_number_of_classes(py::class_<Thing, ThingBase>& thing,
                             std::string_view              name,
                             doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_copy(py::class_<Thing, ThingBase>& thing,
                std::string_view              name,
                doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_add_generating_pair(py::class_<Thing, ThingBase>& thing,
                               std::string_view              name,
                               doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_currently_contains(py::class_<Thing, ThingBase>& thing,
                              std::string_view              name,
                              doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_contains(py::class_<Thing, ThingBase>& thing,
                    std::string_view              name,
                    doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_reduce_no_run(py::class_<Thing, ThingBase>& thing,
                         std::string_view              name,
                         doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_reduce(py::class_<Thing, ThingBase>& thing,
                  std::string_view              name,
                  doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_generating_pairs(py::class_<Thing, ThingBase>& thing,
                            std::string_view              name,
                            doc                           extra_doc = {});

  template <typename Thing, typename ThingBase>
  void def_presentation(py::class_<Thing, ThingBase>& thing,
                        std::string_view              name,
                        doc                           extra_doc = {});

  ////////////////////////////////////////////////////////////////////////
  // Helpers
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing>
  void def_non_trivial_classes(py::module&      m,
                               std::string_view class_name,
                               std::string_view func_prefix,
                               doc              extra_doc = {});

  template <typename Thing>
  void def_partition(py::module&      m,
                     std::string_view class_name,
                     std::string_view func_prefix,
                     doc              extra_doc = {});

  template <typename Thing>
  void def_normal_forms(py::module&      m,
                        std::string_view class_name,
                        std::string_view func_prefix,
                        doc              extra_doc = {});

}  // namespace libsemigroups
#endif  // SRC_CONG_COMMON_HPP_
