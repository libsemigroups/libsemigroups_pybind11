//
// libsemigroups_pybind11
// Copyright (C) 2025 James Mitchell
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
#include <libsemigroups/knuth-bendix-helpers.hpp>
#include <libsemigroups/knuth-bendix.hpp>

// pybind11....
#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "cong-common.hpp"  // for def_construct_default
#include "main.hpp"         // for init_knuth_bendix

namespace libsemigroups {
  namespace py = pybind11;
  using std::literals::operator""sv;

  namespace {
    template <typename Word, typename Rewriter>
    void bind_knuth_bendix(py::module& m, std::string const& name) {
      using KnuthBendix_     = KnuthBendix<Word, Rewriter>;
      using KnuthBendixImpl_ = detail::KnuthBendixImpl<Rewriter>;

      py::class_<KnuthBendix_, KnuthBendixImpl_> thing(m,
                                                       name.c_str(),
                                                       R"pbdoc(
Class containing an implementation of the Knuth-Bendix Algorithm.

On this page we describe the functionality relating to the Knuth-Bendix
algorithm for semigroups and monoids in ``libsemigroups_pybind11``. This page
contains details of the member functions of the class
:any:`KnuthBendix`. This class is used to represent a `string rewriting system <https://w.wiki/9Re>`_
defining a 1- or 2-sided congruence on a finitely presented monoid or
semigroup.

:any:`KnuthBendix` inherits from :any:`Runner` and has the
nested class :any:`KnuthBendix.options`.

 .. doctest::

    >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
    ... presentation, congruence_kind)
    >>> p = Presentation("abc")
    >>> presentation.add_rule(p, "aaaa", "a")
    >>> presentation.add_rule(p, "bbbb", "b")
    >>> presentation.add_rule(p, "cccc", "c")
    >>> presentation.add_rule(p, "abab", "aaa")
    >>> presentation.add_rule(p, "bcbc", "bbb")
    >>> kb = KnuthBendix(congruence_kind.twosided, p)
    >>> not kb.confluent()
    True
    >>> kb.run()
    >>> kb.number_of_active_rules()
    31
    >>> kb.confluent()
    True
)pbdoc");

      // __repr__ is implemented in KnuthBendixImpl_

      ////////////////////////////////////////////////////////////////////////

      def_construct_default(thing, "KnuthBendix");
      def_init_default(thing, "KnuthBendix");
      def_construct_kind_presentation(thing, "KnuthBendix");
      def_init_kind_presentation(thing, "KnuthBendix");
      def_copy(thing, "KnuthBendix");
      // number_of_classes is implemented in KnuthBendixImpl_
      def_generating_pairs(thing, "KnuthBendix");
      def_presentation(thing, "KnuthBendix");
      def_add_generating_pair(thing, "KnuthBendix");
      def_currently_contains(thing, "KnuthBendix");
      def_contains(thing, "KnuthBendix");

      auto extra_detail = R"pbdoc(
Otherwise, this function returns the word *w* rewritten according to
the current rules in the :py:class:`KnuthBendix` instance.
)pbdoc"sv;

      def_reduce_no_run(thing, "KnuthBendix", doc{.detail = extra_detail});
      def_reduce(thing, "KnuthBendix");

      ////////////////////////////////////////////////////////////////////////
      // Helpers from cong-common.hpp . . .
      ////////////////////////////////////////////////////////////////////////

      def_partition<KnuthBendix_>(m,
                                  "KnuthBendix",
                                  "knuth_bendix",
                                  doc{.only_document_once = true, .var = "kb"});

      def_non_trivial_classes<KnuthBendix_>(
          m,
          "KnuthBendix",
          "knuth_bendix",
          doc{.only_document_once = true, .var = "kb"});

      ////////////////////////////////////////////////////////////////////////
      // Helper functions - specific to KnuthBendix
      ////////////////////////////////////////////////////////////////////////

      // This does not use def_normal_forms in cong-common.hpp because it does
      // not return an Iterator object, and hence has its own implementation
      // and doc.
      m.def(
          "knuth_bendix_normal_forms",
          [](KnuthBendix_& ci) { return congruence_common::normal_forms(ci); },
          py::arg("kb"),
          R"pbdoc(
:sig=(kb: KnuthBendix) -> Range:
:only-document-once:

This function returns a range object (with the same methods as :any:`Paths`)
containing normal forms of the classes of the congruence represented by an
instance of :any:`KnuthBendix`. The order of the classes, and
the normal form that is returned, are controlled by the reduction order used to
construct *kb*. This function triggers a full enumeration of *kb*.

:param kb: the :any:`KnuthBendix` instance.
:type kb: KnuthBendix

:returns: A range object.
:rtype: Range

.. warning::
    Termination of the Knuth-Bendix algorithm is undecidable in general, and
    this function may never terminate.

.. doctest::

    >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
    ... presentation, congruence_kind, knuth_bendix)
    >>> p = Presentation("abc")
    >>> presentation.add_rule(p, "aaaa", "a")
    >>> presentation.add_rule(p, "bbbb", "b")
    >>> presentation.add_rule(p, "cccc", "c")
    >>> presentation.add_rule(p, "abab", "aaa")
    >>> presentation.add_rule(p, "bcbc", "bbb")
    >>> kb = KnuthBendix(congruence_kind.twosided, p)
    >>> kb.number_of_classes()
    +∞
    >>> list(knuth_bendix.normal_forms(kb).min(1).max(3))
    ['a', 'b', 'c', 'aa', 'ab', 'ac', 'ba', 'bb', 'bc', 'ca', 'cb', 'cc']
)pbdoc");

      m.def(
          "knuth_bendix_by_overlap_length",
          [](KnuthBendix_& kb) { knuth_bendix::by_overlap_length(kb); },
          py::arg("kb"),
          R"pbdoc(
:sig=(kb: KnuthBendix) -> None:
:only-document-once:

Run the Knuth-Bendix algorithm by considering all overlaps of a given
length.

This function runs the Knuth-Bendix algorithm on the rewriting system
represented by a :any:`KnuthBendix` instance by considering all overlaps of a
given length :math:`n` (according to the :any:`KnuthBendix.options.overlap`)
before those overlaps of length :math:`n + 1`.

:param kb: the :any:`KnuthBendix` instance.
:type kb: KnuthBendix

.. seealso:: :any:`KnuthBendix.overlap_policy`
)pbdoc");

      m.def(
          "knuth_bendix_non_trivial_classes",
          [](KnuthBendix_& kb1, KnuthBendix_& kb2) {
            return knuth_bendix::non_trivial_classes(kb1, kb2);
          },
          py::arg("kb1"),
          py::arg("kb2"),
          R"pbdoc(
:sig=(kb1: KnuthBendix, kb2: KnuthBendix) -> list[list[list[int]] | list[str]]:
:only-document-once:

Find the non-trivial classes of the quotient of one
:any:`KnuthBendix` instance in another.

This function returns the classes with size at least :math:`2` in the
normal forms of *kb2* in *kb1* (the greater congruence, with fewer
classes). This function triggers a full enumeration of both *kb2* and
*kb1*.

Note that this function does **not** compute the normal forms of *kb2* and try
to compute the partition of these induced by *kb1*, before filtering out the
classes of size :math:`1`. In particular, it is possible to compute the
non-trivial classes of *kb1* in *kb2* if there are only finitely many finite
such classes, regardless of whether or not *kb2* or *kb1* has infinitely many
classes.

:param kb1: the instance to compute the partition.
:type kb1: KnuthBendix
:param kb2: the instance whose normal forms will be partitioned.
:type kb2: KnuthBendix

:returns: The non-trivial classes of *kb1* in *kb2*.
:rtype: list[list[list[int]] | list[str]]

:raises LibsemigroupsError:
  if *kb1* has infinitely many classes and *kb2* has finitely many classes (so
  that there is at least one infinite non-trivial class).

:raises LibsemigroupsError:
  if the alphabets of the presentations of *kb1* and *kb2* are not equal.

:raises LibsemigroupsError:
  if the :any:`KnuthBendix.gilman_graph` of *kb1* has fewer
  nodes than that of *kb2*.

.. warning::
    Termination of the Knuth-Bendix algorithm is undecidable in general, and
    this function may never terminate.

.. doctest::

    >>> from libsemigroups_pybind11 import (knuth_bendix, presentation,
    ... Presentation, congruence_kind, KnuthBendix)
    >>> p = Presentation("abc")
    >>> p.rules = ["ab", "ba", "ac", "ca", "aa", "a", "ac", "a", "ca",
    ... "a", "bc", "cb", "bbb", "b", "bc", "b", "cb", "b"]
    >>> kb1 = KnuthBendix(congruence_kind.twosided, p)
    >>> presentation.add_rule(p, "a", "b")
    >>> kb2 = KnuthBendix(congruence_kind.twosided, p)
    >>> knuth_bendix.non_trivial_classes(kb1, kb2)
    [['b', 'ab', 'bb', 'abb', 'a']]
    >>> p = Presentation([0, 1, 2])
    >>> p.rules = [[0, 1], [1, 0], [0, 2], [2, 0], [0, 0], [0], [0, 2], [0], [2, 0],
    ... [0], [1, 2], [2, 1], [1, 1, 1], [1], [1, 2], [1], [2, 1], [1]]
    >>> kb1 = KnuthBendix(congruence_kind.twosided, p)
    >>> presentation.add_rule(p, [0], [1])
    >>> kb2 = KnuthBendix(congruence_kind.twosided, p)
    >>> knuth_bendix.non_trivial_classes(kb1, kb2)
    [[[1], [0, 1], [1, 1], [0, 1, 1], [0]]]
)pbdoc");

      m.def(
          "knuth_bendix_is_reduced",
          [](KnuthBendix_& kb) { return knuth_bendix::is_reduced(kb); },
          py::arg("kb"),
          R"pbdoc(
:sig=(kb: KnuthBendix) -> bool:
:only-document-once:

Check if the all rules are reduced with respect to each other.

:param kb:
  the :any:`KnuthBendix` instance defining the rules that are
  to be checked for being reduced.
:type kb: KnuthBendix

:returns:
  ``True`` if for each pair :math:`(A, B)` and :math:`(C, D)` of rules stored
  within the :any:`KnuthBendix` instance, :math:`C` is neither a
  subword of :math:`A` nor :math:`B`. Returns ``False`` otherwise.
:rtype: bool
)pbdoc");

      m.def(
          "knuth_bendix_redundant_rule",
          [](Presentation<word_type> const& p, std::chrono::milliseconds t)
              -> std::optional<std::pair<word_type, word_type>> {
            auto it = knuth_bendix::redundant_rule(p, t);
            if (it != p.rules.cend()) {
              return std::make_pair(*it, *(it + 1));
            }
            return {};
          },
          R"pbdoc(
:sig=(p: Presentation, t: datetime.timedelta) -> tuple[list[int], list[int]] | tuple[str, str] | None:
:only-document-once:

Return a redundant rule or ``None``.

Starting with the last rule in the presentation, this function attempts
to run the Knuth-Bendix algorithm on the rules of the presentation
except for a given omitted rule. For every such omitted rule,
Knuth-Bendix is run for the length of time indicated by the second
parameter *t*, and then it is checked if the omitted rule can be shown
to be redundant. If the omitted rule can be shown to be redundant in
this way, then this rule is returned. If no rule can be shown to be
redundant in this way, then ``None`` is returned.

:param p: the presentation.
:type p: Presentation

:param t: time to run Knuth-Bendix for every omitted rule.
:type t: datetime.timedelta

:returns: A redundant rule or ``None``.
:rtype: tuple[list[int], list[int]] | tuple[str, str] | None

.. warning::
  The progress of the Knuth-Bendix algorithm may differ between
  different calls to this function even if the parameters are identical.
  As such this is non-deterministic, and may produce different results
  with the same input.

.. doctest::

  >>> from libsemigroups_pybind11 import (knuth_bendix, presentation,
  ... Presentation)
  >>> from datetime import timedelta
  >>> p = Presentation("ab")
  >>> presentation.add_rule(p, "ab", "ba")
  >>> presentation.add_rule(p, "bab", "abb")
  >>> t = timedelta(seconds = 1)
  >>> p.rules
  ['ab', 'ba', 'bab', 'abb']
  >>> knuth_bendix.redundant_rule(p, t)
  ('bab', 'abb')
)pbdoc");

      // Documented above
      m.def(
          "knuth_bendix_redundant_rule",
          [](Presentation<std::string> const& p, std::chrono::milliseconds t)
              -> std::optional<std::pair<std::string, std::string>> {
            auto it = knuth_bendix::redundant_rule(p, t);
            if (it != p.rules.cend()) {
              return std::make_pair(*it, *(it + 1));
            }
            return {};
          },
          // Signature required to avoid generating any doc
          R"pbdoc(
:sig=(p: Presentation, t: datetime.timedelta) -> tuple[list[int], list[int]] | tuple[str, str] | None:
:only-document-once:
)pbdoc");
    }  // bind_knuth_bendix

    template <typename Word, typename Rewriter>
    void bind_normal_form_range(py::module& m, char const* name) {
      using NormalFormRange
          = detail::KnuthBendixNormalFormRange<Word, Rewriter, ShortLexCompare>;
      py::class_<NormalFormRange> thing(m, name);

      thing.def("__repr__", [](NormalFormRange const& nfr) {
        return to_human_readable_repr(nfr);
      });

      thing.def("__copy__", [](NormalFormRange const& self) {
        return NormalFormRange(self);
      });
      thing.def(
          "copy",
          [](NormalFormRange const& self) { return NormalFormRange(self); },
          R"pbdoc(
:sig=(self: NormalFormRange) -> NormalFormRange:

Copy a :any:`NormalFormRange` object.

:returns: A copy.
:rtype: NormalFormRange
)pbdoc");
      // __len__ is not allowed to return anything other than an int, hence
      // __len__ and count don't have the same behaviour.
      thing.def("__len__", [](NormalFormRange const& nfr) {
        auto result = nfr.count();
        if (result == POSITIVE_INFINITY) {
          return py::module_::import("sys").attr("maxsize").cast<uint64_t>();
        }
        return result;
      });

      thing.def("__iter__", [](NormalFormRange const& nfr) {
        return py::make_iterator(rx::begin(nfr), rx::end(nfr));
      });

      thing.def("at_end", [](NormalFormRange& nfr) { return nfr.at_end(); });
      thing.def("count", [](NormalFormRange& nfr) { return nfr.count(); });

      thing.def("get", [](NormalFormRange& nfr) { return nfr.get(); });

      thing.def("max", [](NormalFormRange const& self) { return self.max(); });

      thing.def("max",
                [](NormalFormRange& self, PositiveInfinity const& val)
                    -> NormalFormRange& { return self.max(val); });
      thing.def("max",
                [](NormalFormRange& self, size_t val) -> NormalFormRange& {
                  return self.max(val);
                });
      thing.def("min", [](NormalFormRange const& self) { return self.min(); });
      thing.def("min",
                [](NormalFormRange& self, size_t val) -> NormalFormRange& {
                  return self.min(val);
                });
      thing.def("next", [](NormalFormRange& nfr) { nfr.next(); });
    }  // bind_normal_form_range
  }  // namespace

  void init_knuth_bendix(py::module& m) {
    using RewriteTrie     = detail::RewriteTrie;
    using RewriteFromLeft = detail::RewriteFromLeft;

    bind_knuth_bendix<word_type, RewriteTrie>(m, "KnuthBendixWordRewriteTrie");
    bind_knuth_bendix<word_type, RewriteFromLeft>(
        m, "KnuthBendixWordRewriteFromLeft");
    bind_knuth_bendix<std::string, RewriteTrie>(m,
                                                "KnuthBendixStringRewriteTrie");
    bind_knuth_bendix<std::string, RewriteFromLeft>(
        m, "KnuthBendixStringRewriteFromLeft");

    bind_normal_form_range<word_type, RewriteTrie>(
        m, "KnuthBendixNormalFormRangeWordRewriteTrie");
    bind_normal_form_range<word_type, RewriteFromLeft>(
        m, "KnuthBendixNormalFormRangeWordRewriteFromLeft");
    bind_normal_form_range<std::string, RewriteTrie>(
        m, "KnuthBendixNormalFormRangeStringRewriteTrie");
    bind_normal_form_range<std::string, RewriteFromLeft>(
        m, "KnuthBendixNormalFormRangeStringRewriteFromLeft");
  }

}  // namespace libsemigroups
