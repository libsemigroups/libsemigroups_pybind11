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

#include <string>  // for string, basic_string, oper...

#include <libsemigroups/froidure-pin.hpp>     // for Presentation
#include <libsemigroups/presentation.hpp>     // for Presentation
#include <libsemigroups/to-presentation.hpp>  // for to_presentation
#include <libsemigroups/types.hpp>            // for word_type

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>  // for class_, init, module

#include "main.hpp"  // for init_present

namespace py = pybind11;

namespace libsemigroups {
  void init_to_present(py::module& m) {
    ////////////////////////////////////////////////////////////////////////////
    // to_presentation
    ////////////////////////////////////////////////////////////////////////////
    m.def(
        "to_presentation",
        [](Presentation<std::string>& p) {
          return to_presentation<word_type>(p);
        },
        py::arg("p"),
        R"pbdoc(
:sig=(p: PresentationStrings)->PresentationStrings:
:only-document-once:
Make a presentation from a different type of presentation.

Returns a presentation equivalent to the input presentation but with words of a
different type. Presentations with words of type :any:`str` get converted to
presentations with words of type ``List[int]``, and vice versa. 

If the alphabet of of *p* is :math:`\{a_0, a_1, \dots a_{n-1}\}`, then the
conversion from the old alphabet to the new is either:

      * :math:`a_i \mapsto` :any:`human_readable_char(i)` for converting :any:`int` to :any:`str`; or
      * :math:`a_i \mapsto` ``i`` for converting :any:`str` to :any:`int`.

:param p: the input presentation.
:type p: PresentationStrings

:returns: The converted presentation.
:rtype: PresentationStrings.

:raises LibsemigroupsError: if ``p.validate()`` throws.

)pbdoc");
    m.def(
        "to_presentation",
        [](Presentation<std::string>&                    p,
           std::function<Presentation<word_type>::letter_type(
               Presentation<std::string>::letter_type)>& f) {
          return to_presentation<word_type>(p, f);
        },
        py::arg("p"),
        py::arg("f"),
        R"pbdoc(
:sig=(p: PresentationStrings, f: Callable[[int | str], str | int])->PresentationStrings:
:only-document-once:
Make a presentation from a different type of presentation.

Returns a presentation equivalent to the input presentation but with words of a
different type. Presentations with words of type :any:`str` get converted to
presentations with words of type ``List[int]``, and vice versa. The second
parameter specifies how to map the letters of one presentation to the other.

:param p: the input presentation.
:type p: PresentationStrings.

:param f: a function mapping :any:`int` to :any:`str`, or vice versa.
:type f: collections.abc.Callable[[int | str], str | int]

:returns: The converted presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError: if ``p.validate()`` throws.
)pbdoc");
    m.def(
        "to_presentation",
        [](FroidurePinBase& fp) { return to_presentation<std::string>(fp); },
        py::arg("fp"),
        R"pbdoc(
:sig=(fp: FroidurePinBase)->PresentationStrings:
:only-document-once:
Make a presentation over words of type :any:`str` from a FroidurePin object.

This function constructs and returns a :any:`PresentationStrings` object with
words of type :any:`str` using the rules of a :any:`FroidurePin` object.

No enumeration of the argument *fp* is performed, so it might be the case that
the resulting presentation does not define the same semigroup as *fp*. To ensure
that the resulting presentation defines the same semigroup as *fp*, run
:any:`FroidurePin.run` (or any other function that fully enumerates *fp*) prior
to calling this function.


:param fp: the :any:`FroidurePin` object from which to obtain the rules.
:type fp: FroidurePin

:returns: The presentation with the same rules as *fp*.
:rtype: PresentationStrings

:exceptions: This function guarantees not to throw a ``LibsemigroupsError``.
)pbdoc");
    m.def(
        "to_presentation",
        [](Presentation<word_type>& p) {
          return to_presentation<std::string>(p);
        },
        py::arg("p"),
        R"pbdoc(
:sig=(p: PresentationStrings)->PresentationStrings:
)pbdoc");
    m.def(
        "to_presentation",
        [](Presentation<word_type>&                    p,
           std::function<Presentation<std::string>::letter_type(
               Presentation<word_type>::letter_type)>& f) {
          return to_presentation<std::string>(p, f);
        },
        py::arg("p"),
        py::arg("f"),
        R"pbdoc(
:sig=(p: PresentationStrings, f: Callable[[int | str], str | int])->PresentationStrings:
)pbdoc");
    ////////////////////////////////////////////////////////////////////////////
    // to_inverse_presentation
    ////////////////////////////////////////////////////////////////////////////
    m.def(
        "to_inverse_presentation",
        [](InversePresentation<std::string>& ip) {
          return to_inverse_presentation<word_type>(ip);
        },
        py::arg("ip"),
        R"pbdoc(
:sig=(p: InversePresentationStrings)->InversePresentationStrings:
:only-document-once:
Make an inverse presentation from a different type of inverse presentation.

Returns an inverse presentation equivalent to the input inverse presentation but
with words of a different type. Inverse presentations with words of type
:any:`str` get converted to inverse presentations with words of type
``List[int]``, and vice versa. 

If the alphabet of of *ip* is :math:`\{a_0, a_1, \dots a_{n-1}\}`, then the
conversion from the old alphabet to the new is either:

      * :math:`a_i \mapsto` :any:`human_readable_char(i)` for converting :any:`int` to :any:`str`; or
      * :math:`a_i \mapsto` ``i`` for converting :any:`str` to :any:`int`.

:param ip: the input inverse presentation.
:type p: InversePresentationStrings

:returns: The converted inverse presentation.
:rtype: InversePresentationStrings

:raises LibsemigroupsError: if the alphabet of *ip* contains duplicate letters.

)pbdoc");
    m.def(
        "to_inverse_presentation",
        [](InversePresentation<std::string>&                    ip,
           std::function<InversePresentation<word_type>::letter_type(
               InversePresentation<std::string>::letter_type)>& f) {
          return to_inverse_presentation<word_type>(ip, f);
        },
        py::arg("ip"),
        py::arg("f"),
        R"pbdoc(
:sig=(p: InversePresentationStrings, f: Callable[[int | str], str | int])->InversePresentationStrings:
:only-document-once:
Make an inverse presentation from a different type of inverse presentation.

Returns an inverse presentation equivalent to the input inverse
presentation but of a different type. Inverse presentations with words of type
:any:`str` get converted to inverse presentations with words of type
``List[int]``, and vice versa. The second parameter specifies how
to map the letters of one inverse presentation to the other.

:param ip: the input inverse presentation.
:type ip: InversePresentationStrings

:param f: a function mapping :any:`int` to :any:`str`, or vice versa.
:type f: collections.abc.Callable[[int | str], str | int]

:returns: The converted presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError: if the alphabet of *ip* contains duplicate letters.
)pbdoc");
    m.def(
        "to_inverse_presentation",
        [](Presentation<std::string>& p) { return to_inverse_presentation(p); },
        py::arg("p"),
        R"pbdoc(
:sig=(p: PresentationStrings)->InversePresentationStrings:
:only-document-once:
Make an inverse presentation from a presentation

Returns an inverse presentation with rules equivalent to those of the input
presentation, but over a normalised alphabet. If the alphabet of *p* is 
:math:`\{a_0, a_1, \dots, a_{n-1}\}`, then the alphabet of the
returned inverse presentation will be ``{human_readable_letter(p, 0), 
human_readable_letter(p, 1), ... , human_readable_letter(p, 2n-1)}``,
where the inverse of letter :math:`i` is the letter :math:`i + n\,
(\text{mod }2n)`.

:param p: the input presentation.
:type p: PresentationStrings

:return: The converted inverse presentation.
:rtype: InversePresentationStrings

:raises LibsemigroupsError: if ``p.validate()`` throws.
)pbdoc");

    m.def(
        "to_inverse_presentation",
        [](InversePresentation<word_type>& ip) {
          return to_inverse_presentation<std::string>(ip);
        },
        py::arg("ip"),
        R"pbdoc(
:sig=(p: InversePresentationStrings)->InversePresentationStrings:
:only-document-once:
)pbdoc");
    m.def(
        "to_inverse_presentation",
        [](InversePresentation<word_type>&                    ip,
           std::function<InversePresentation<std::string>::letter_type(
               InversePresentation<word_type>::letter_type)>& f) {
          return to_inverse_presentation<std::string>(ip, f);
        },
        py::arg("ip"),
        py::arg("f"));
    m.def(
        "to_inverse_presentation",
        [](Presentation<word_type>& p) { return to_inverse_presentation(p); },
        py::arg("p"));
  }

}  // namespace libsemigroups