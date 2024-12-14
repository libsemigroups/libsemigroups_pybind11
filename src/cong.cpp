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
#include <libsemigroups/cong.hpp>

// pybind11....
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "cong-common.hpp"  // for contains etc

namespace libsemigroups {
  namespace py = pybind11;

  namespace {

    template <typename Word>
    void bind_cong(py::module& m, char const* name) {
      py::class_<Congruence<Word>, detail::CongruenceCommon> thing(m,
                                                              name,
                                                              R"pbdoc(
Class for running :any:`KambitesMultiStringView`,
:any:`KnuthBendixStringRewriteTrie`, and :any:`ToddCoxeterWord` in parallel.

On this page we describe the functionality relating to the class
:any:`CongruenceWord` in ``libsemigroups``. This class can be used for computing a
congruence over a semigroup or monoid by running every applicable algorithm
from ``libsemigroups`` (and some variants of the same algorithm) in parallel.
This class is provided for convenience, at present it is not very customisable,
and lacks some of the fine grained control offered by the classes implementing
individual algorithms, such as :any:`KambitesMultiStringView`,
:any:`KnuthBendixStringRewriteTrie`, and :any:`ToddCoxeterWord`.

.. seealso::  :any:`congruence_kind` and :any:`tril`.

.. doctest::

    >>> from libsemigroups_pybind11 import (Presentation, presentation, Congruence,
    ... congruence_kind, is_obviously_infinite)
    >>> p = Presentation([0, 1])
    >>> p.contains_empty_word(True)
    <monoid presentation with 2 letters, 0 rules, and length 0>
    >>> presentation.add_rule(p, [0, 1], [])
    >>> cong = Congruence(congruence_kind.twosided, p)
    >>> is_obviously_infinite(cong)
    True
    >>> cong.add_generating_pair([1, 1, 1], [])
    <Congruence over <monoid presentation with 2 letters, 1 rule, and length 2> with 4 runners>
    >>> cong.number_of_classes()
    3
    >>> is_obviously_infinite(cong)
    False
 )pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Methods from cong-common.hpp . . .
      ////////////////////////////////////////////////////////////////////////

      def_construct_default(thing, name);
      def_init_default(thing, name);

      def_construct_kind_presentation(thing, name);
      def_init_kind_presentation(thing, name);

      def_copy(thing, name);

      def_generating_pairs(thing, name);
      def_presentation(thing, name);

      def_number_of_classes(thing, name);

      def_add_generating_pair(thing, name);

      def_currently_contains(thing, name);
      def_contains(thing, name);

      def_reduce_no_run(thing, name);
      def_reduce(thing, name);

      ////////////////////////////////////////////////////////////////////////
      // Congruence specific stuff
      ////////////////////////////////////////////////////////////////////////

      thing.def("__repr__", [](Congruence<Word> const& thing) {
        return to_human_readable_repr(thing);
      });

      thing.def(py::init<congruence_kind,
                         FroidurePinBase&,
                         WordGraph<uint32_t> const&>(),
                py::arg("knd"),
                py::arg("S"),
                py::arg("wg"),
                R"pbdoc(
:sig=(knd: congruence_kind, S: FroidurePinBase, wg: WordGraph) -> None:

Construct from :any:`congruence_kind`, :any:`FroidurePinPBR`, and :any:`WordGraph`.

Constructs a :any:`CongruenceWord` over the :any:`FroidurePinPBR` instance *S*
representing a 1- or 2-sided congruence according to *knd*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param S:
   a reference to the :any:`FroidurePinBase` over which the congruence
   is being defined.
:type S: FroidurePinBase

:param wg: the left or right Cayley graph of S.
:type wg: WordGraph

:returns:  ``self``.
:rtype: CongruenceWord
)pbdoc");

      thing.def(
          "init",
          [](Congruence<Word>&          self,
             congruence_kind            knd,
             FroidurePinBase&           S,
             WordGraph<uint32_t> const& wg) -> Congruence<Word>& {
            return self.init(knd, S, wg);
          },
          py::arg("knd"),
          py::arg("S"),
          py::arg("wg"),
          R"pbdoc(
:sig=(self: CongruenceWord, knd: congruence_kind, S: FroidurePinBase, wg: WordGraph) -> CongruenceWord:

Re-initialize from :any:`congruence_kind`, :any:`FroidurePinPBR`, and
:any:`WordGraph`.

This function re-initializes a :any:`CongruenceWord` instance as if it had been
newly constructed over the :any:`FroidurePinPBR` instance *S* representing a 1-
or 2-sided congruence according to *knd*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param S:
  a reference to the FroidurePin over which the congruence is being
  defined.
:type S: FroidurePinBase

:param wg: the left or right Cayley graph of S.
:type wg: WordGraph

:returns:  ``self``.
:rtype: Congruence
)pbdoc");

      thing.def(
          "max_threads",
          [](Congruence<Word>& self, size_t val) -> Congruence<Word>& {
            return self.max_threads(val);
          },
          py::arg("val"),
          R"pbdoc(
Set the maximum number of threads.

:param val: the number of threads.
:type val: int

:returns: ``self``.
:rtype: Congruence
)pbdoc");

      thing.def(
          "max_threads",
          [](Congruence<Word> const& self) { return self.max_threads(); },
          R"pbdoc(
Get the current maximum number of threads.

:returns: The current maximum number of threads.
:rtype: int
    )pbdoc");

      thing.def("number_of_runners",
                &Congruence<Word>::number_of_runners,
                R"pbdoc(
Get the number of runners. This function returns the number of distinct
:any:`detail::CongruenceCommon` instances that are contained in a
:any:`CongruenceWord` object.

:returns:
   The number of runners.
:rtype:
   int
)pbdoc");

      thing.def("presentation",
                &Congruence<Word>::presentation,
                R"pbdoc(
:sig=(self: Congruence) -> PresentationStrings:

Get the presentation defining the parent semigroup of the congruence.
This function returns the presentation used to construct a
:any:`CongruenceWord` object. This is not always possible.

:returns:
   The presentation.
:rtype:
   PresentationStrings

:raises LibsemigroupsError:
   if :any:`Runner.finished` returns ``True`` and ``has(KnuthBendix)`` returns
   ``True``.

:raises LibsemigroupsError:
   if no :any:`PresentationStrings` was used to construct or initialise ``self``.
)pbdoc");

      // Return by value on purpose, to avoid complications with "get" being
      // called before "run" and the obtained object being deleted.
      // Might be better to return a shared_ptr but then KnuthBendix must be
      // stored as shared_ptr, and then everything above KnuthBendix in the
      // class hierarchy, and then possibly everything below Reporter too which
      // is then basically everything.
      thing.def(
          "_get_knuth_bendix",
          [](Congruence<Word> const& self) -> KnuthBendix<Word> {
            return *self.template get<KnuthBendix<Word>>();
          },
          py::return_value_policy::copy);

      // Return by value on purpose, to avoid complications with "get" being
      // called before "run" and the obtained object being deleted.
      // Might be better to return a shared_ptr but then KnuthBendix must be
      // stored as shared_ptr, and then everything above KnuthBendix in the
      // class hierarchy, and then possibly everything below Reporter too which
      // is then basically everything.
      thing.def("_get_todd_coxeter", [](Congruence<Word> const& self) {
        return *self.template get<ToddCoxeter<Word>>();
      });

      // Return by value on purpose, to avoid complications with "get" being
      // called before "run" and the obtained object being deleted.
      // Might be better to return a shared_ptr but then KnuthBendix must be
      // stored as shared_ptr, and then everything above KnuthBendix in the
      // class hierarchy, and then possibly everything below Reporter too which
      // is then basically everything.
      thing.def("_get_kambites", [](Congruence<Word> const& self) {
        return *self.template get<Kambites<Word>>();
      });

      thing.def("_has_knuth_bendix", [](Congruence<Word> const& self) {
        return self.template has<KnuthBendix<Word>>();
      });

      thing.def("_has_todd_coxeter", [](Congruence<Word> const& self) {
        return self.template has<ToddCoxeter<Word>>();
      });

      thing.def("_has_kambites", [](Congruence<Word> const& self) {
        return self.template has<Kambites<Word>>();
      });

      ////////////////////////////////////////////////////////////////////////
      // Helpers from cong-common.hpp . . .
      ////////////////////////////////////////////////////////////////////////

      def_partition<Congruence<Word>>(
          m,
          "CongruenceWord",
          "congruence",
          doc{.only_document_once = ":only-document-once:", .var = "c"});

      def_non_trivial_classes<Congruence<Word>>(
          m,
          "CongruenceWord",
          "congruence",
          doc{.only_document_once = ":only-document-once:", .var = "c"});

      // Not yet implemented in libsemigroups
      // def_normal_forms<Congruence<Word>>(
      //     m,
      //     "CongruenceWord",
      //     "congruence",
      //     doc{.only_document_once = ":only-document-once:", .var =
      //     "c"});

    }  // bind_cong
  }  // namespace

  void init_cong(py::module& m) {
    bind_cong<word_type>(m, "CongruenceWord");
    bind_cong<std::string>(m, "CongruenceString");
  }
}  // namespace libsemigroups
