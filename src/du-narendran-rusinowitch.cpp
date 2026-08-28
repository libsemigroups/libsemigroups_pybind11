//
// libsemigroups_pybind11
// Copyright (C) 2026 James D. Mitchell
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
#include <string>  // for string

// libsemigroups....
#include <libsemigroups/du-narendran-rusinowitch.hpp>  // for du_narendran_...
#include <libsemigroups/presentation.hpp>              // for Presentation
#include <libsemigroups/types.hpp>                     // for word_type

// pybind11....
#include <pybind11/pybind11.h>  // for arg, module
#include <pybind11/stl.h>       // for automatic conversions

// libsemigroups_pybind11....
#include "main.hpp"  // for init_du_narendran_rusinowitch

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    template <typename Word>
    void bind_du_narendran_rusinowitch(py::module& m) {
      m.def(
          "du_narendran_rusinowitch",
          [](Presentation<Word> const& p) {
            return du_narendran_rusinowitch(p);
          },
          py::arg("p"),
          R"pbdoc(
:sig=(p: Presentation) -> str | list[int]:
:only-document-once:
Return an ordered alphabet such that the rules are oriented with
respect to recursive-path order; see :any:`rpo_cmp`.

This function returns the alphabet of *p* ordered so that the rules of
*p* satisfy :math:`x_i \to y_i` and :math`x_i > y_i` with respect to
recursive-path order and the returned alphabet order. The returned
alphabet is empty if this fails, or if the alphabet was empty to begin
with.

:param p: the presentation whose rules are to be oriented.
:type p: Presentation

:returns: The ordered alphabet, if such an alphabet exists. Otherwise, the
  empty alphabet.
:rtype: str | list[int]

:raises LibsemigroupsError: if the alphabet or rules of *p* are invalid.

.. seealso::
  :any:`rpo_cmp`

.. doctest::

   >>> from libsemigroups_pybind11 import Presentation, du_narendran_rusinowitch
   >>> p = Presentation("abcd")
   >>> p.rules = ["a", "cc", "d", "bcc", "bccb", "c", "cccb", "bccc"]
   >>> du_narendran_rusinowitch(p)
   'bcda'
)pbdoc");
    }
  }  // namespace

  void init_du_narendran_rusinowitch(py::module& m) {
    bind_du_narendran_rusinowitch<std::string>(m);
    bind_du_narendran_rusinowitch<word_type>(m);
  }
}  // namespace libsemigroups
