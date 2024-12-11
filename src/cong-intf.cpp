//
// libsemigroups_pybind11
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

// libsemigroups headers
#include <libsemigroups/cong-intf.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_cong_intf

namespace py = pybind11;

namespace libsemigroups {

  void init_cong_intf(py::module& m) {
    py::class_<CongruenceInterface, Runner> thing(m,
                                                  "CongruenceInterface",
                                                  R"pbdoc(
Class collecting common aspects of classes representing congruences.

Every class for representing a congruence in ``libsemigroups_pybind11`` is
derived from :any:`CongruenceInterface`, which holds the member functions and
data that are common to all its derived classes. These classes are:

*  :any:`Congruence`
*  :any:`Kambites`
*  :py:class:`KnuthBendixRewriteTrie`
*  :any:`ToddCoxeter`
)pbdoc");
    thing.def(
        "kind",
        [](CongruenceInterface const& self) { return self.kind(); },
        R"pbdoc(
:sig=(self: CongruenceInterface) -> congruence_kind:
The kind of the congruence (1- or 2-sided).

This function returns the kind of the congruence represented by a derived class
of :any:`CongruenceInterface`. See :any:`congruence_kind` for details.

:complexity:
   Constant.

:returns:
   The kind of the congruence (1- or 2-sided).
:rtype:
   congruence_kind
)pbdoc");
    thing.def("generating_pairs",
              &CongruenceInterface::generating_pairs,
              R"pbdoc(
:sig=(self: CongruenceInterface) -> List[List[int]]:

Returns the generating pairs of the congruence.

This function returns the generating pairs of the congruence represented by any
derived class of a :any:`CongruenceInterface`. This is always a
``List[List[int]]``, regardless of the type of presentation used to construct
the derived class.

:complexity:
   Constant.

:returns:
   The generating pairs.

:rtype:
   List[List[int]]
)pbdoc");
    thing.def("number_of_generating_pairs",
              &CongruenceInterface::number_of_generating_pairs,
              R"pbdoc(
Returns the number of generating pairs.

This function returns the number of generating pairs, which is the size of
:any:`generating_pairs` divided by :math:`2`.

:complexity:
   Constant.

:returns:
   The number of generating pairs.
:rtype:
   int
)pbdoc");
  }  // init_cong_intf

}  // namespace libsemigroups
