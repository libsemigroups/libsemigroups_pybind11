//
// libsemigroups_pybind11
// Copyright (C) 2022 James D. Mitchell
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

// TODO:
// * iwyu

// C std headers....
#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t

// C++ stl headers....
#include <initializer_list>  // for initializer_list
#include <vector>            // for vector

// libsemigroups....
#include <libsemigroups/froidure-pin-base.hpp>  // for FroidurePinBase
#include <libsemigroups/presentation.hpp>       // for Presentation
#include <libsemigroups/to-presentation.hpp>    // for make

// pybind11....
#include <pybind11/pybind11.h>  // for class_, init, module
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_present

namespace py = pybind11;

namespace libsemigroups {

  namespace {

    template <typename T>
    std::string presentation_repr(Presentation<T> const& p) {
      std::ostringstream out;

      out << "<" << (p.contains_empty_word() ? "monoid" : "semigroup")
          << " presentation of length "
          << presentation::length(p) + p.alphabet().size() << ", with "
          << p.alphabet().size() << " generators and " << p.rules.size()
          << " relations>";
      return out.str();
    }

    template <typename T>
    void bind_present(py::module& m, std::string const& name) {
      using size_type = typename Presentation<T>::size_type;

      py::class_<Presentation<T>>(m, name.c_str())
          .def(py::init<>())
          .def(py::init<Presentation<T> const&>())
          .def("alphabet",
               py::overload_cast<>(&Presentation<T>::alphabet, py::const_))
          .def("alphabet",
               py::overload_cast<size_type>(&Presentation<T>::alphabet))
          .def("alphabet",
               py::overload_cast<T const&>(&Presentation<T>::alphabet))
          .def("alphabet_from_rules", &Presentation<T>::alphabet_from_rules)
          .def("letter", &Presentation<T>::letter)
          .def("index", &Presentation<T>::index)
          .def("contains_empty_word",
               py::overload_cast<>(&Presentation<T>::contains_empty_word,
                                   py::const_))
          .def("contains_empty_word",
               py::overload_cast<bool>(&Presentation<T>::contains_empty_word))
          .def_readwrite("rules", &Presentation<T>::rules)
          .def("validate_alphabet",
               py::overload_cast<>(&Presentation<T>::validate_alphabet,
                                   py::const_))
          .def("validate_letter", &Presentation<T>::validate_letter)
          .def("validate_rules", &Presentation<T>::validate_rules)
          .def("validate", &Presentation<T>::validate)
          .def("__repr__", &presentation_repr<T>);

      m.def("add_rule", &presentation::add_rule<T>);
      m.def("add_rule_no_checks", &presentation::add_rule_no_checks<T>);
      m.def("add_rules",
            py::overload_cast<Presentation<T>&, Presentation<T> const&>(
                &presentation::add_rules_no_checks<T>));
      m.def("add_identity_rules", &presentation::add_identity_rules<T>);
      m.def("add_zero_rules", &presentation::add_zero_rules<T>);
      // There are two definitions here to handle the default parameters
      m.def("add_inverse_rules",
            [](Presentation<T>&                      p,
               T const&                              vals,
               typename Presentation<T>::letter_type e) {
              presentation::add_inverse_rules(p, vals, e);
            });
      m.def("add_inverse_rules", [](Presentation<T>& p, T const& vals) {
        presentation::add_inverse_rules(p, vals);
      });
      m.def("remove_duplicate_rules", &presentation::remove_duplicate_rules<T>);
      m.def("remove_trivial_rules", &presentation::remove_trivial_rules<T>);
      m.def("reduce_complements", &presentation::reduce_complements<T>);
      m.def("sort_each_rule", &presentation::sort_each_rule<T>);
      m.def("sort_rules", &presentation::sort_rules<T>);
      m.def("longest_subword_reducing_length",
            &presentation::longest_subword_reducing_length<T>);
      m.def("replace_word_with_new_generator",
            [](Presentation<T>& p, T const& w) -> void {
              presentation::replace_word_with_new_generator(p, w);
            });
      m.def(
          "replace_subword",
          [](Presentation<T>& p, T const& existing, T const& replace) -> void {
            presentation::replace_subword(p, existing, replace);
          });
      m.def("replace_word", &presentation::replace_word<T>);
      m.def("length", [](Presentation<T> const& p) -> size_t {
        return presentation::length<T>(p);
      });
      m.def("reverse", &presentation::reverse<T>);
      m.def("normalize_alphabet", &presentation::normalize_alphabet<T>);
      m.def("remove_redundant_generators",
            &presentation::remove_redundant_generators<T>);
      m.def("add_zero_rules", &presentation::add_zero_rules<T>);
      m.def("are_rules_sorted", &presentation::are_rules_sorted<T>);
      m.def("change_alphabet", &presentation::change_alphabet<T>);
      m.def("first_unused_letter", &presentation::first_unused_letter<T>);
      m.def("greedy_reduce_length", &presentation::greedy_reduce_length<T>);
      m.def("longest_rule", [](Presentation<T> const& p) {
        return std::distance(p.rules.cbegin(), presentation::longest_rule(p));
      });
      m.def("longest_rule_length", [](Presentation<T> const& p) {
        return presentation::longest_rule_length(p);
      });
      m.def("shortest_rule", [](Presentation<T> const& p) {
        return std::distance(p.rules.cbegin(), presentation::shortest_rule(p));
      });
      m.def("shortest_rule_length", [](Presentation<T> const& p) {
        return presentation::shortest_rule_length(p);
      });
      m.def("make_semigroup", presentation::make_semigroup<T>);

      m.def("to_presentation",
            py::overload_cast<FroidurePinBase&>(to_presentation<word_type>));

      m.def("to_presentation",
            [](Presentation<std::string> const& p) -> Presentation<word_type> {
              return to_presentation<word_type>(p);
            });

      m.def("to_presentation",
            [](Presentation<word_type> const& p) -> Presentation<std::string> {
              return to_presentation<std::string>(p);
            });

      // m.def("to_presentation",
      //       [](Presentation<word_type> const& p,
      //          std::string const&             s) -> Presentation<std::string>
      //          {
      //         return to_presentation<std::string>(p, s);
      //       });
      m.def("is_strongly_compressible",
            &presentation::is_strongly_compressible<T>);
      m.def("strongly_compress", &presentation::strongly_compress<T>);
      m.def("reduce_to_2_generators",
            &presentation::reduce_to_2_generators<T>,
            py::arg("p"),
            py::arg("index") = 0);
    }
  }  // namespace

  void init_present(py::module& m) {
    bind_present<word_type>(m, "PresentationWords");
    bind_present<std::string>(m, "PresentationStrings");
  }
}  // namespace libsemigroups
