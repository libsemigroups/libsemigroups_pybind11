//
// libsemigroups_pybind11
// Copyright (C) 2024 Joseph Edwards
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

// C++ std headers
#include <functional>  // for function
#include <string>      // for string, basic_string, oper...

#include <libsemigroups/froidure-pin-base.hpp>   // for FroidurePinBase
#include <libsemigroups/knuth-bendix-class.hpp>  // for KnuthBendix
#include <libsemigroups/presentation.hpp>        // for Presentation
#include <libsemigroups/to-presentation.hpp>     // for to<Presentation>
#include <libsemigroups/types.hpp>               // for word_type

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, init, module

#include "main.hpp"  // for init_present

namespace libsemigroups {
  namespace py = pybind11;
  namespace {
    template <typename InputWord, typename OutputWord>
    void bind_pres_to_pres(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_presentation_") + name;
      m.def(fn_name.c_str(), [](Presentation<InputWord> const& p) {
        return to<Presentation<OutputWord>>(p);
      });
    }

    template <typename InputWord, typename OutputWord>
    void bind_pres_func_to_pres(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_presentation_") + name;
      m.def(fn_name.c_str(),
            [](Presentation<InputWord> const&                        p,
               std::function<typename Presentation<OutputWord>::letter_type(
                   typename Presentation<InputWord>::letter_type)>&& f) {
              return to<Presentation<OutputWord>>(p, f);
            });
    }

    template <typename Word, typename Rewriter>
    void bind_kb_to_pres(py::module& m) {
      m.def("to_presentation", [](KnuthBendix<Word, Rewriter>& kb) {
        return to<Presentation>(kb);
      });
    }

    template <typename WordIn, typename WordOut, typename Rewriter>
    void bind_kb_to_pres_with_word(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_presentation_") + name;
      m.def(fn_name.c_str(), [](KnuthBendix<WordIn, Rewriter>& kb) {
        return to<Presentation<WordOut>>(kb);
      });
    }

    template <typename Word>
    void bind_fp_to_pres(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_presentation_") + name;
      m.def(fn_name.c_str(),
            [](FroidurePinBase& fp) { return to<Presentation<Word>>(fp); });
    }

    template <typename Word>
    void bind_pres_to_inv_pres(py::module& m) {
      m.def("to_inverse_presentation", [](Presentation<Word> const& p) {
        return to<InversePresentation>(p);
      });
    }

    template <typename InputWord, typename OutputWord>
    void bind_inv_pres_to_inv_pres(py::module& m, std::string const& name) {
      std::string fn_name = std::string("to_inverse_presentation_") + name;
      m.def(fn_name.c_str(), [](InversePresentation<InputWord> const& ip) {
        return to<InversePresentation<OutputWord>>(ip);
      });
    }

    template <typename InputWord, typename OutputWord>
    void bind_inv_pres_func_to_inv_pres(py::module&        m,
                                        std::string const& name) {
      std::string fn_name = std::string("to_inverse_presentation_") + name;
      m.def(fn_name.c_str(),
            [](InversePresentation<InputWord> const& ip,
               std::function<
                   typename InversePresentation<OutputWord>::letter_type(
                       typename InversePresentation<InputWord>::letter_type)>&&
                   f) { return to<InversePresentation<OutputWord>>(ip, f); });
    }
  }  // namespace

  void init_to_present(py::module& m) {
    ////////////////////////////////////////////////////////////////////////////
    // to<Presentation>
    ////////////////////////////////////////////////////////////////////////////

    // From Presentation
    bind_pres_to_pres<std::string, word_type>(m, "word");
    bind_pres_to_pres<word_type, word_type>(m, "word");
    bind_pres_to_pres<word_type, std::string>(m, "string");
    bind_pres_to_pres<std::string, std::string>(m, "string");

    // From Presentation + function
    bind_pres_func_to_pres<std::string, word_type>(m, "word");
    bind_pres_func_to_pres<word_type, word_type>(m, "word");
    bind_pres_func_to_pres<word_type, std::string>(m, "string");
    bind_pres_func_to_pres<std::string, std::string>(m, "string");

    // From KnuthBendix
    bind_kb_to_pres<std::string, detail::RewriteFromLeft>(m);
    bind_kb_to_pres<std::string, detail::RewriteTrie>(m);
    bind_kb_to_pres<word_type, detail::RewriteFromLeft>(m);
    bind_kb_to_pres<word_type, detail::RewriteTrie>(m);

    bind_kb_to_pres_with_word<std::string,
                              std::string,
                              detail::RewriteFromLeft>(m, "string");
    bind_kb_to_pres_with_word<std::string, std::string, detail::RewriteTrie>(
        m, "string");
    bind_kb_to_pres_with_word<word_type, std::string, detail::RewriteFromLeft>(
        m, "string");
    bind_kb_to_pres_with_word<word_type, std::string, detail::RewriteTrie>(
        m, "string");
    bind_kb_to_pres_with_word<std::string, word_type, detail::RewriteFromLeft>(
        m, "word");
    bind_kb_to_pres_with_word<std::string, word_type, detail::RewriteTrie>(
        m, "word");
    bind_kb_to_pres_with_word<word_type, word_type, detail::RewriteFromLeft>(
        m, "word");
    bind_kb_to_pres_with_word<word_type, word_type, detail::RewriteTrie>(
        m, "word");

    // From FroidurePin
    bind_fp_to_pres<std::string>(m, "string");
    bind_fp_to_pres<word_type>(m, "word");

    ////////////////////////////////////////////////////////////////////////////
    // to<InversePresentation>
    ////////////////////////////////////////////////////////////////////////////

    // From Presentation
    bind_pres_to_inv_pres<std::string>(m);
    bind_pres_to_inv_pres<word_type>(m);

    // From InversePresentation
    bind_inv_pres_to_inv_pres<std::string, word_type>(m, "word");
    bind_inv_pres_to_inv_pres<word_type, word_type>(m, "word");
    bind_inv_pres_to_inv_pres<word_type, std::string>(m, "string");
    bind_inv_pres_to_inv_pres<std::string, std::string>(m, "string");

    // From InversePresentation + function
    bind_inv_pres_func_to_inv_pres<std::string, word_type>(m, "word");
    bind_inv_pres_func_to_inv_pres<word_type, word_type>(m, "word");
    bind_inv_pres_func_to_inv_pres<word_type, std::string>(m, "string");
    bind_inv_pres_func_to_inv_pres<std::string, std::string>(m, "string");
  }

}  // namespace libsemigroups
