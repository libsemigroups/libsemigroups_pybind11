//
// libsemigroups_pybind11
// Copyright (C) 2024 James Mitchell
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
#include <libsemigroups/cong.hpp>
#include <libsemigroups/kambites.hpp>
#include <libsemigroups/obvinf.hpp>
#include <libsemigroups/todd-coxeter.hpp>

// pybind11....
#include <pybind11/pybind11.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_obvinf

namespace libsemigroups {
  namespace py = pybind11;

  void init_obvinf(py::module& m) {
    m.def(
        "is_obviously_infinite",
        [](Presentation<word_type> const& p) {
          return is_obviously_infinite(p);
        },
        R"pbdoc(
:sig=(p: Presentation) -> bool:

Function for checking if the finitely presented semigroup or monoid
defined by a :any:`Presentation` object is obviously infinite or not.

This function returns ``True`` if the finitely presented semigroup or
monoid defined by the :any:`Presentation` object *p* is obviously infinite.

:param p: the presentation.
:type p: Presentation

:returns:
    Whether or not the presentation defines an obviously infinite semigroup or
    monoid.
:rtype:
    bool

:raises LibsemigroupsError: If the presentation *p* is not valid.

.. note::
   If this function returns ``False``, it is still possible that semigroup or
   monoid defined by *p* is infinite.
)pbdoc");

    m.def(
        "is_obviously_infinite",
        [](Presentation<std::string> const& p) {
          return is_obviously_infinite(p);
        },
        R"pbdoc(
:sig=(p: Presentation) -> bool:
:only-document-once:

Function for checking if the finitely presented semigroup or monoid
defined by a :any:`Presentation` object is obviously infinite or not.

This function returns ``True`` if the finitely presented semigroup or
monoid defined by the :any:`Presentation` object *p* is obviously infinite.

:param p: the presentation.
:type p: Presentation

:returns:
    Whether or not the presentation defines an obviously infinite semigroup or
    monoid.
:rtype:
    bool

:raises LibsemigroupsError: If the presentation *p* is not valid.

.. note::
   If this function returns ``False``, it is still possible that semigroup or
   monoid defined by *p* is infinite.
)pbdoc");

    m.def(
        "is_obviously_infinite",
        [](detail::ToddCoxeterImpl const& tc) {
          return is_obviously_infinite(tc);
        },
        py::arg("tc"),
        R"pbdoc(
:sig=(tc: ToddCoxeter) -> bool:

Function for checking if the quotient of a finitely presented semigroup or
monoid defined by a :any:`ToddCoxeter` object is obviously infinite or not.

This function returns ``True`` if the quotient of the finitely presented
semigroup or monoid defined by the :any:`ToddCoxeter` object *tc* is obviously
infinite; ``False`` is returned if it is not.

:param tc: the :any:`ToddCoxeter` instance.
:type tc: :any:`ToddCoxeter`

:returns:
    Whether or not the quotient defined by a :any:`ToddCoxeter` instance is
    obviously infinite.
:rtype:
    bool

.. note::
    If this function returns ``False``, it is still possible that the quotient
    defined by the :any:`ToddCoxeter` object *tc* is infinite.
)pbdoc");
  }  // init_obvinf

}  // namespace libsemigroups
