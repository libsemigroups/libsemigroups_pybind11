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

.. seealso:: :any:`Runner`.

.. |name| replace:: :any:`KnuthBendix`

.. include:: ../../_static/runner_non_inherit.rst

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

      def_construct_default(
          thing,
          "KnuthBendix",
          doc{.extra_kwargs     = ", rewriting_system: str, order: Order",
              .extra_kwargs_doc = R"pbdoc(
    * **rewriting_system** (*str*) -- the type of rewriting system to use, must be either ``"Trie"`` or ``"Set"``.
    * **order** (*Order*) -- the reduction ordering to use, must be either :any:`Order.shortlex` or :any:`Order.recursive`.)pbdoc"sv});

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

      thing.def(
          "active_rules",
          [](KnuthBendix_& kb) {
            auto rules = kb.active_rules();
            return py::make_iterator(rx::begin(rules), rx::end(rules));
          },
          R"pbdoc(
:sig=(self: KnuthBendix) -> collections.abc.Iterator[tuple[str, str]]:

Return a copy of the active rules.

This member function returns an iterator yielding the pairs of strings
which represent the rewriting rules. The first entry in every such pair is
greater than the second according to the reduction ordering of the
:py:class:`KnuthBendix` instance. The rules are sorted
according to the reduction ordering used by the rewriting system, on the first
entry.

:returns: An iterator yielding the currently active rules.
:rtype: collections.abc.Iterator[tuple[str, str]]
)pbdoc");

      thing.def("gilman_graph_node_labels",
                &KnuthBendix_::gilman_graph_node_labels,
                R"pbdoc(
:sig=(self: KnuthBendix) -> list[str]:

Return the node labels of the Gilman :py:class:`WordGraph`.

Return the node labels of the Gilman :py:class:`WordGraph`, corresponding to the
unique prefixes of the left-hand sides of the rules of the rewriting system.

:returns: The node labels of the Gilman :py:class:`WordGraph`.
:rtype: list[str]

.. seealso:: :any:`gilman_graph`.
)pbdoc",
                py::return_value_policy::reference_internal);

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

      // This does not use def_normal_forms in cong-common.hpp because it
      // does not return an Iterator object, and hence has its own
      // implementation and doc.
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
    >>> list(knuth_bendix.normal_forms(kb).min(1).max(2))
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
:sig=(kb1: KnuthBendix, kb2: KnuthBendix) -> list[list[list[int]]] | list[list[str]]:
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

:param kb1: the instance to compute the partition with.
:type kb1: KnuthBendix
:param kb2: the instance whose normal forms will be partitioned.
:type kb2: KnuthBendix

:returns: The non-trivial classes of *kb1* in *kb2*.
:rtype: list[list[list[int]]] | list[list[str]]

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

Check if all rules are reduced with respect to each other.

:param kb:
  the :any:`KnuthBendix` instance defining the rules that are
  to be checked for being reduced.
:type kb: KnuthBendix

:returns:
  ``True`` if for each pair :math:`(A, B)` and :math:`(C, D)` of rules stored
  within the :any:`KnuthBendix` instance, :math:`C` is neither a
  subword of :math:`A` nor of :math:`B`. Returns ``False`` otherwise.
:rtype: bool
)pbdoc");
    }  // bind_knuth_bendix

    template <typename Word, typename Rewriter>
    void bind_normal_form_range(py::module& m, char const* name) {
      using NormalFormRange = detail::KnuthBendixNormalFormRange<
          Word,
          Rewriter,
          typename Rewriter::reduction_order>;
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

    template <typename Word>
    void bind_redundant_rule(py::module& m) {
      m.def(
          "knuth_bendix_redundant_rule",
          [](Presentation<Word> const& p, std::chrono::milliseconds t)
              -> std::optional<std::pair<Word, Word>> {
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
    }

    template <typename Word, typename RewritingSystem>
    void bind_tietze_explorer(py::module& m, std::string const& name) {
      using TietzeExplorer_
          = knuth_bendix::TietzeExplorer<Word, RewritingSystem>;

      py::class_<TietzeExplorer_> thing(m,
                                        name.c_str(),
                                        R"pbdoc(
Search for a finite complete rewriting system using Tietze transformations.

This class searches for a presentation for which the Knuth-Bendix algorithm
terminates, by introducing new generators for subwords of the rules and trying
all different orders on the resulting alphabet.

More precisely, an instance of this class starts with the presentation of the
:any:`KnuthBendix` instance used to construct it. It then forms presentations
obtained by repeatedly replacing a non-empty subword of length at least 2 by a
new generator. The number of such replacements is controlled by
:any:`TietzeExplorer.depth_min` and :any:`TietzeExplorer.depth_max`. For every
presentation in this search, the alphabet is permuted in every possible way,
and the Knuth-Bendix algorithm is run for :any:`TietzeExplorer.run_each_for`.
The search succeeds if one of these runs produces a confluent rewriting system.

Since this class derives from :any:`Runner`, the search can be run to
completion using :any:`Runner.run`, for a bounded amount of time using
:any:`Runner.run_for`, or until a predicate holds using :any:`Runner.run_until`.
The member function :any:`TietzeExplorer.result` is the usual way to run the
search and obtain the successful :any:`KnuthBendix` instance, if one was found.

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb)
   >>> explorer.depth_min(), explorer.depth_max()
   (0, 3)
)pbdoc");
      // thing.def("__repr__", [](TietzeExplorer_ const& thing) {
      //   return to_human_readable_repr(thing);
      // });
      thing.def(py::init<KnuthBendix<Word, RewritingSystem>&>(), R"pbdoc(
Construct from a KnuthBendix instance.

Constructs a :any:`TietzeExplorer` using the kind and presentation of ``kb``.
The default settings are:

* :any:`TietzeExplorer.depth_min` is ``0``;
* :any:`TietzeExplorer.depth_max` is ``3``;
* :any:`TietzeExplorer.run_each_for` is ``datetime.timedelta``;
* :any:`TietzeExplorer.number_of_threads` is ``1``.

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb)
   >>> explorer.number_of_threads()
   1

:param kb: the KnuthBendix instance whose presentation is to be explored.
:type kb: KnuthBendix
)pbdoc");
      //       thing.def(py::init<TietzeExplorer_ const&>(), R"pbdoc(
      // Copy constructor.
      // )pbdoc");

      thing.def(
          "depth_max",
          [](TietzeExplorer_ const& self) { return self.depth_max(); },
          R"pbdoc(
:sig=(self: TietzeExplorer) -> int:
Get the maximum search depth.

Returns the maximum number of subword replacements by new generators to perform
when constructing presentations for the search. The default value is ``3``.

:returns: The maximum number of new generators introduced.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).depth_max()
   3
)pbdoc");

      thing.def(
          "depth_max",
          [](TietzeExplorer_& self, size_t val) -> TietzeExplorer_& {
            return self.depth_max(val);
          },
          py::arg("val"),
          R"pbdoc(
:sig=(self: TietzeExplorer, val: SupportsInt | SupportsIndex) -> TietzeExplorer:
Set the maximum search depth.

This function sets the maximum number of subword replacements by new generators
to perform when constructing presentations for the search. The default value is
``3``.

:param val: the maximum search depth.
:type val: SupportsInt | SupportsIndex

:returns: ``self``.
:rtype: TietzeExplorer

:raises LibsemigroupsError: if the internal search queue has already been populated.

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).depth_max(1).depth_max()
   1
)pbdoc");

      thing.def(
          "depth_min",
          [](TietzeExplorer_ const& self) { return self.depth_min(); },
          R"pbdoc(
:sig=(self: TietzeExplorer) -> int:
Get the minimum search depth.

Returns the minimum number of subword replacements by new generators required
for a presentation to be tried. The default value is ``0``.

:returns: The minimum number of new generators to introduce.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).depth_min()
   0
)pbdoc");

      thing.def(
          "depth_min",
          [](TietzeExplorer_& self, size_t val) -> TietzeExplorer_& {
            return self.depth_min(val);
          },
          py::arg("val"),
          R"pbdoc(
:sig=(self: TietzeExplorer, val: SupportsInt | SupportsIndex) -> TietzeExplorer:

Set the minimum search depth.

This function sets the minimum number of subword replacements by new generators
required for a presentation to be tried. The default value is ``0``.

:param val: the minimum search depth.
:type val: SupportsInt | SupportsIndex

:returns: ``self``.
:rtype: TietzeExplorer

:raises LibsemigroupsError: if the internal search queue has already been populated.

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).depth_min(1).depth_min()
   1
)pbdoc");

      thing.def("estimated_run_time",
                &TietzeExplorer_::estimated_run_time,
                R"pbdoc(
:sig=(self: TietzeExplorer) -> datetime.timedelta:

Estimate the running time of the search.

Returns :any:`TietzeExplorer.number_of_runs` multiplied by
:any:`TietzeExplorer.run_each_for` and divided by
:any:`TietzeExplorer.number_of_threads`.

:returns: The estimate total run time.
:rtype: datetime.timedelta

.. doctest::

   >>> from datetime import timedelta
   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb)
   >>> isinstance(explorer.estimated_run_time(), timedelta)
   True
)pbdoc");

      thing.def("init",
                &TietzeExplorer_::init,
                py::arg("kb"),
                R"pbdoc(
:sig=(self: TietzeExplorer, kb: KnuthBendix) -> TietzeExplorer:

Reinitialize an existing TietzeExplorer.

This function puts a :any:`TietzeExplorer` object back into the same state as if
it had been newly constructed from ``kb``.

:param kb: the KnuthBendix instance whose presentation is to be explored.
:type kb: KnuthBendix

:returns: ``self``.
:rtype: TietzeExplorer

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb).depth_max(1)
   >>> explorer.init(kb) is explorer
   True
   >>> explorer.depth_max()
   3
)pbdoc");

      thing.def("knuth_bendix",
                &TietzeExplorer_::knuth_bendix,
                R"pbdoc(
:sig=(self: TietzeExplorer) -> KnuthBendix:

Return the initial :any:`KnuthBendix` instance.

Returns the :any:`KnuthBendix` instance supplied at construction or most recent
initialization.

:returns: A :any:`KnuthBendix` instance.
:rtype: KnuthBendix

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb)
   >>> isinstance(explorer.knuth_bendix(), KnuthBendix)
   True
)pbdoc");

      thing.def("number_of_runs",
                &TietzeExplorer_::number_of_runs,
                R"pbdoc(
:sig=(self: TietzeExplorer) -> int:

Return the number of Knuth-Bendix runs to try.

Returns the number of presentations and alphabet orders that will be tried by
the search, subject to the current values of :any:`TietzeExplorer.depth_min`
and :any:`TietzeExplorer.depth_max`.

:returns: An ``int``.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb).depth_max(0)
   >>> explorer.number_of_runs()
   2
)pbdoc");

      thing.def(
          "number_of_threads",
          [](TietzeExplorer_ const& self) { return self.number_of_threads(); },
          R"pbdoc(
:sig=(self: TietzeExplorer) -> int:

Get the number of threads.

Returns the number of threads used to run the search. The default value is
``1``.

:returns: An ``int``.
:rtype: int

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).number_of_threads()
   1
)pbdoc");

      thing.def(
          "number_of_threads",
          [](TietzeExplorer_& self, size_t val) -> TietzeExplorer_& {
            return self.number_of_threads(val);
          },
          py::arg("val"),
          R"pbdoc(
:sig=(self: TietzeExplorer, val: SupportsInt | SupportsIndex) -> TietzeExplorer:

Set the number of threads.

This function sets the number of threads used to run the search. The default
value is ``1``.

:param val: the number of threads to use.
:type val: SupportsInt | SupportsIndex

:returns: ``self``.
:rtype: TietzeExplorer

:raises LibsemigroupsError: if ``val`` is ``0``.

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).number_of_threads(2).number_of_threads()
   2
)pbdoc");

      thing.def("result",
                &TietzeExplorer_::result,
                R"pbdoc(
:sig=(self: TietzeExplorer) -> KnuthBendix | None:

Run the search and return a successful Knuth-Bendix instance.

This function runs the search, if it has not already finished, and returns the
first :any:`KnuthBendix` instance found whose rewriting system is confluent.

:returns: A :any:`KnuthBendix` instance if the search succeeds, and ``None`` otherwise.
:rtype: KnuthBendix | None

:raises LibsemigroupsError: if the initial alphabet size plus
  :any:`TietzeExplorer.depth_max` is greater than 20.

.. seealso:: :any:`Runner.run`

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> result = knuth_bendix.TietzeExplorer(kb).depth_max(0).result()
   >>> isinstance(result, KnuthBendix)
   True
)pbdoc");

      thing.def(
          "run_each_for",
          [](TietzeExplorer_ const& self) { return self.run_each_for(); },
          R"pbdoc(
:sig=(self: TietzeExplorer) -> datetime.timedelta:

Get the time allowed for each Knuth-Bendix run.

Returns the amount of time for which Knuth-Bendix is run for each presentation
and alphabet order tried by the search. The default value is
``datetime.timedelta``.

:returns: A value of type ``datetime.timedelta``.
:rtype: datetime.timedelta

.. doctest::

   >>> from datetime import timedelta
   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb)
   >>> isinstance(explorer.run_each_for(), timedelta)
   True
)pbdoc");

      thing.def(
          "run_each_for",
          [](TietzeExplorer_& self, std::chrono::nanoseconds val)
              -> TietzeExplorer_& { return self.run_each_for(val); },
          py::arg("val"),
          R"pbdoc(
:sig=(self: TietzeExplorer, val: datetime.timedelta) -> TietzeExplorer:

Set the time allowed for each Knuth-Bendix run.

This function sets the amount of time for which Knuth-Bendix is run for each
presentation and alphabet order tried by the search. The default value is
``datetime.timedelta``.

:param val: the amount of time to run each Knuth-Bendix instance for.
:type val: datetime.timedelta

:returns: ``self``.
:rtype: TietzeExplorer

.. doctest::

   >>> from datetime import timedelta
   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> explorer = knuth_bendix.TietzeExplorer(kb)
   >>> explorer.run_each_for(timedelta(milliseconds=1)) is explorer
   True
   >>> explorer.run_each_for() == timedelta(milliseconds=1)
   True
)pbdoc");

      thing.def("success",
                &TietzeExplorer_::success,
                R"pbdoc(
:sig=(self: TietzeExplorer) -> bool:

Check whether the search finished successfully.

Returns ``True`` if the search has finished and found a successful
:any:`KnuthBendix` instance, and ``False`` otherwise.

:returns: Whether or not the search was successful.
:rtype: bool

.. doctest::

   >>> from libsemigroups_pybind11 import (KnuthBendix, Presentation,
   ... congruence_kind, knuth_bendix)
   >>> p = Presentation("ab")
   >>> kb = KnuthBendix(congruence_kind.twosided, p)
   >>> knuth_bendix.TietzeExplorer(kb).success()
   False

)pbdoc");
    }  // bind_tietze_explorer
  }    // namespace

  void init_knuth_bendix(py::module& m) {
    using LenLexTrie = detail::RewritingSystemTrie<LenLexCmp>;
    using LenLexSet  = detail::RewritingSystemSet<LenLexCmp>;
    // TODO rename RevRPOTrie etc
    using RPOTrie = detail::RewritingSystemTrie<RevRPOCmp>;
    using RPOSet  = detail::RewritingSystemSet<RevRPOCmp>;

    bind_knuth_bendix<word_type, LenLexTrie>(m, "KnuthBendixWordLenLexTrie");
    bind_knuth_bendix<word_type, LenLexSet>(m, "KnuthBendixWordLenLexSet");
    bind_knuth_bendix<std::string, LenLexTrie>(m,
                                               "KnuthBendixStringLenLexTrie");
    bind_knuth_bendix<std::string, LenLexSet>(m, "KnuthBendixStringLenLexSet");

    bind_knuth_bendix<word_type, RPOTrie>(m, "KnuthBendixWordRPOTrie");
    bind_knuth_bendix<word_type, RPOSet>(m, "KnuthBendixWordRPOSet");
    bind_knuth_bendix<std::string, RPOTrie>(m, "KnuthBendixStringRPOTrie");
    bind_knuth_bendix<std::string, RPOSet>(m, "KnuthBendixStringRPOSet");

    bind_normal_form_range<word_type, LenLexTrie>(
        m, "KnuthBendixNormalFormRangeWordLenLexTrie");
    bind_normal_form_range<word_type, LenLexSet>(
        m, "KnuthBendixNormalFormRangeWordLenLexSet");
    bind_normal_form_range<std::string, LenLexTrie>(
        m, "KnuthBendixNormalFormRangeStringLenLexTrie");
    bind_normal_form_range<std::string, LenLexSet>(
        m, "KnuthBendixNormalFormRangeStringLenLexSet");

    bind_normal_form_range<word_type, RPOTrie>(
        m, "KnuthBendixNormalFormRangeWordRPOTrie");
    bind_normal_form_range<word_type, RPOSet>(
        m, "KnuthBendixNormalFormRangeWordRPOSet");
    bind_normal_form_range<std::string, RPOTrie>(
        m, "KnuthBendixNormalFormRangeStringRPOTrie");
    bind_normal_form_range<std::string, RPOSet>(
        m, "KnuthBendixNormalFormRangeStringRPOSet");

    bind_redundant_rule<std::string>(m);
    bind_redundant_rule<word_type>(m);

    bind_tietze_explorer<word_type, LenLexSet>(m,
                                               "TietzeExplorerWordLenLexSet");
    bind_tietze_explorer<word_type, LenLexTrie>(m,
                                                "TietzeExplorerWordLenLexTrie");
    bind_tietze_explorer<word_type, RPOSet>(m, "TietzeExplorerWordRPOSet");
    bind_tietze_explorer<word_type, RPOTrie>(m, "TietzeExplorerWordRPOTrie");

    bind_tietze_explorer<std::string, LenLexSet>(
        m, "TietzeExplorerStringLenLexSet");
    bind_tietze_explorer<std::string, LenLexTrie>(
        m, "TietzeExplorerStringLenLexTrie");
    bind_tietze_explorer<std::string, RPOSet>(m, "TietzeExplorerStringRPOSet");
    bind_tietze_explorer<std::string, RPOTrie>(m,
                                               "TietzeExplorerStringRPOTrie");
  }

}  // namespace libsemigroups
