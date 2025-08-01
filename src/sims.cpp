//
// libsemigroups_pybind11
// Copyright (C) 2024 Reinis Cirpons
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
#include <pybind11/detail/common.h>
#include <vector>  // for vector

// libsemigroups....
#include <libsemigroups/presentation.hpp>  // for Presentation
#include <libsemigroups/rx/ranges.hpp>  // for rx::begin, rx::end, rx::transform
#include <libsemigroups/sims.hpp>       // for Sims1, Sims2, ....
#include <libsemigroups/types.hpp>      // for word_type
#include <libsemigroups/word-graph.hpp>  // for WordGraph

// pybind11....
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_sims

namespace libsemigroups {
  namespace py          = pybind11;
  using node_type       = uint32_t;
  using word_graph_type = WordGraph<node_type>;
  using size_type       = typename word_graph_type::size_type;

  //////////////////////////////////////////////////////////////////////////////
  // SimsSettings
  //////////////////////////////////////////////////////////////////////////////

  template <typename Subclass>
  void bind_sims_settings(py::class_<SimsSettings<Subclass>>& ss,
                          std::string_view                    doc_type) {
    using SimsSettings_ = SimsSettings<Subclass>;

    ss.def(
        "number_of_threads",
        // TODO(0): add -> Subclass& here and everywhere else where we return a
        // subclass
        [](SimsSettings_& self, size_t val) -> Subclass& {
          return self.number_of_threads(val);
        },
        py::arg("val"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, val: int) -> {0}:
Set the number of threads.

This function sets the number of threads to be used by :any:`Sims1` or
:any:`Sims2`. The default value is ``1``.

:param val: the maximum number of threads to use.
:type val: int

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError:  if the argument *val* is 0.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "number_of_threads",
        [](SimsSettings_ const& self) { return self.number_of_threads(); },
        R"pbdoc(
Get the number of threads.

:returns: The current maximum number of threads.
:rtype: int
)pbdoc");

    ss.def(
        "presentation",
        [](SimsSettings_& self, Presentation<word_type> const& p) -> Subclass& {
          return self.presentation(p);
        },
        py::arg("p"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, p: Presentation) -> {0}:

Set the presentation over which the congruences produced by an instance are
defined.

This function sets the presentation over which the congruences produced by an
instance are defined. These are the rules used at every node in the depth first
search conducted by objects of this type. The parameter *p* is always first
converted to a :any:`Presentation` of ``list[int]`` and
it is this converted value that is used.

:param p: the presentation.
:type p: Presentation

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError: if :any:`Presentation.throw_if_bad_alphabet_or_rules` throws.

:raises LibsemigroupsError: if *p* has 0-generators and 0-relations.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "presentation",
        [](SimsSettings_ const& self) -> auto const& {
          return self.presentation();
        },
        fmt::format(R"pbdoc(
:sig=(self: {0}) -> Presentation:

Get the presentation over which the congruences produced by an instance are
defined.

This function returns the defining presentation of a :any:`Sims1` or
:any:`Sims2` instance. The congruences computed by :any:`Sims1.iterator` of the
appropriate subclass are defined over the semigroup or monoid defined by this
presentation.

:returns: The presentation.
:rtype: Presentation
)pbdoc",
                    doc_type)
            .c_str(),
        py::return_value_policy::reference_internal);

    ss.def(
        "first_long_rule_position",
        [](SimsSettings_& self, size_t pos) -> Subclass& {
          return self.cbegin_long_rules(pos);
        },
        py::arg("pos"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, pos: int) -> {0}:

Set the beginning of the long rules (position).

This function sets the beginning of the long rules using a position in
``self.presentation().rules``. The "long rules" are the rules used after a
complete deterministic word graph has been found in the search. If such a word
graph is compatible with the long rules specified by this function, then this
word graph is accepted, and if not it is rejected.

The purpose of this is to improve the backtrack search by reducing the time
spent processing "long" rules in each node of the search tree, and to only
check them at the leaves.

:param pos: position of the the left hand side of the first long rule.
:type pos: int

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError:
    if *pos* is not a valid position in ``self.presentation().rules``.

:raises LibsemigroupsError:
    if the rule at position *pos* is not the left hand side of a rule (i.e. if
    *pos* is odd).
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "long_rules",
        [](SimsSettings_ const& self) {
          return py::make_iterator(self.cbegin_long_rules(),
                                   self.presentation().rules.cend());
        },
        fmt::format(R"pbdoc(
:sig=(self: {0}) -> collections.abc.Iterator[tuple[list[int], list[int]]]:

Get the long rules.

Returns an iterator of long rules.

:returns: An iterator.
:rtype: collections.abc.Iterator[tuple[list[int], list[int]]]
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "clear_long_rules",
        [](SimsSettings_& self) -> Subclass& {
          return self.clear_long_rules();
        },
        fmt::format(R"pbdoc(
Clear the set of long rules.

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());

    ss.def("number_of_long_rules",
           &SimsSettings_::number_of_long_rules,
           R"pbdoc(
Returns the number of rules marked as long rules.

:returns: The number of long rules.
:rtype: int
)pbdoc");

    ss.def(
        "long_rule_length",
        [](SimsSettings_& self, size_t val) -> Subclass& {
          return self.long_rule_length(val);
        },
        py::arg("val"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, val: int) -> {0}:

Set the length of a long rule.

Define the length of a "long" rule. This function modifies
:py:meth:`~Sims1.presentation()` so that the rules whose length (sum of the
lengths of both sides) is at least *val* (if any) occur at the end of
``presentation().rules`` and so that :py:meth:`~Sims1.long_rules` returns all
such rules. The relative orders of the rules within
:py:meth:`~Sims1.presentation` may not be preserved.

:param val: the long rule length.
:type val: int

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "pruners",
        [](SimsSettings_ const& s) -> auto const& { return s.pruners(); },
        R"pbdoc(
Get all active pruners of the search tree.

This function returns a copy of the list of pruners. A pruner is any function that takes
as input a word graph and returns a boolean. We require that if a pruner
returns ``False`` for a word graph ``wg``, then it returns ``False`` for all
word graphs that are descended from ``wg`` in the Sims word graph search tree.

The pruners are used to refine the congruence search tree during the execution
of the Sims algorithm. As such, the congruences computed by this instance are
only taken among those whose word graphs are accepted by all pruners returned
by :py:meth:`~Sims1.pruners`.

:returns: A list of boolean functions on word graphs, the set of all pruners.
:rtype: list[collections.abc.Callable[[WordGraph], bool]]
)pbdoc",  // The next line seemingly does nothing
        py::return_value_policy::reference_internal);

    ss.def(
        "add_pruner",
        [](SimsSettings_& self, SimsRefinerIdeals const& func) -> Subclass& {
          return self.add_pruner(func);
        },
        py::arg("pruner"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, pruner: collections.abc.Callable[[WordGraph], bool]) -> {0}:
:only-document-once:
Add a pruner to the search tree.

:param pruner: a pruner function.
:type pruner: collections.abc.Callable[[WordGraph], bool]

:returns: The first argument *self*.
:rtype: {0}

.. warning::
    When running the Sims low-index backtrack with multiple threads, each added
    pruner must be guaranteed thread safe. Failing to do so could cause bad
    things to happen.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "add_pruner",
        [](SimsSettings_& self, SimsRefinerFaithful const& func) -> Subclass& {
          return self.add_pruner(func);
        },
        py::arg("pruner"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, pruner: collections.abc.Callable[[WordGraph], bool]) -> {0}:
:only-document-once:
Add a pruner to the search tree.

:param pruner: a pruner function.
:type pruner: collections.abc.Callable[[WordGraph], bool]

:returns: The first argument *self*.
:rtype: {0}

.. warning::
    When running the Sims low-index backtrack with multiple threads, each added
    pruner must be guaranteed thread safe. Failing to do so could cause bad
    things to happen.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "add_pruner",
        [](SimsSettings_&                              self,
           std::function<bool(word_graph_type const&)> func) -> Subclass& {
          return self.add_pruner(func);
        },
        py::arg("pruner"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, pruner: collections.abc.Callable[[WordGraph], bool]) -> {0}:
 collections.abc.Callable[[WordGraph], bool]) -> Sims1:
:only-document-once:
Add a pruner to the search tree.

:param pruner: a pruner function.
:type pruner: collections.abc.Callable[[WordGraph], bool]

:returns: The first argument *self*.
:rtype@ {0}

.. warning::
    When running the Sims low-index backtrack with multiple threads, each added
    pruner must be guaranteed thread safe. Failing to do so could cause bad
    things to happen.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "clear_pruners",
        [](SimsSettings_& self) -> Subclass& { return self.clear_pruners(); },
        fmt::format(R"pbdoc(
Clear the set of pruners.

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "included_pairs",
        [](SimsSettings_ const& self) -> auto const& {
          return self.included_pairs();
        },
        fmt::format(R"pbdoc(
:sig=(self: {0}) -> list[list[int]]:
Returns the set of pairs that must be excluded from every congruence.

This function returns the list of included pairs. The congruences computed by a
:any:`Sims1` or :any:`Sims2` instance always contain the relations of this
list. In other words, the congruences computed by this instance are only taken
among those that contains the pairs of elements of the underlying semigroup
(defined by the presentation returned by :py:meth:`~Sims1.presentation()` and
:py:meth:`~Sims1.long_rules()`) represented by the relations of the list of
words returned by :py:meth:`~Sims1.included_pairs()`.

:returns:
    A list of words ``result`` such that ``(result[2*i], result[2*i+1])`` is
    the ``i``-th included pair.
:rtype: list[list[int]]
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "add_included_pair",
        [](SimsSettings_& self, word_type const& lhs, word_type const& rhs)
            -> Subclass& { return sims::add_included_pair(self, lhs, rhs); },
        py::arg("lhs"),
        py::arg("rhs"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, lhs: list[int], rhs: list[int]) -> {0}:
Add a pair that should be included in every congruence.

:param lhs: the left hand side of the rule being added.
:type lhs: list[int]

:param rhs: the right hand side of the rule being added.
:type rhs: list[int]

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError:  if :any:`Presentation.throw_if_letter_not_in_alphabet` throws on *lhs* or *rhs*.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "clear_included_pairs",
        [](SimsSettings_& self) -> Subclass& {
          return self.clear_included_pairs();
        },
        fmt::format(R"pbdoc(
Clear the set of included words.

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "excluded_pairs",
        [](SimsSettings_ const& self) { return self.excluded_pairs(); },
        fmt::format(R"pbdoc(
:sig=(self: {0}) -> list[list[int]]:
Returns the set of pairs that must be excluded from every congruence.

This function returns the list of excluded pairs. The congruences computed by a
:any:`Sims1` or :any:`Sims2` instance will never contain the relations of this
presentation. In other words, the congruences computed by this instance are
only taken among those that do not contain any of the pairs of elements of the
underlying semigroup (defined by the presentation returned by
:py:meth:`~Sims1.presentation()` and :py:meth:`~Sims1.long_rules()`)
represented by the relations of the presentation returned by
:py:meth:`~Sims1.excluded_pairs()`.

:returns:
    A list of words ``result`` such that ``(result[2*i], result[2*i+1])`` is
    the ``i``-th excluded pair.
:rtype: list[list[int]]
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "add_excluded_pair",
        [](SimsSettings_& self, word_type const& lhs, word_type const& rhs)
            -> Subclass& { return sims::add_excluded_pair(self, lhs, rhs); },
        py::arg("lhs"),
        py::arg("rhs"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, lhs: list[int], rhs: list[int]) -> {0}:

Add a pair that must be excluded from every congruence.

:param lhs: the left hand side of the rule being added.
:type lhs: list[int]

:param rhs: the right hand side of the rule being added.
:type rhs: list[int]

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError:  if :any:`Presentation.throw_if_letter_not_in_alphabet` throws on *lhs* or *rhs*.
)pbdoc",
                    doc_type)
            .c_str());

    ss.def(
        "clear_excluded_pairs",
        [](SimsSettings_& self) -> Subclass& {
          return self.clear_excluded_pairs();
        },
        fmt::format(R"pbdoc(
Clear the set of excluded words.

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());

    ss.def("stats",
           &SimsSettings_::stats,
           R"pbdoc(
Get the current stats object.

This function returns the current stats object. The value returned by this
function is a :any:`SimsStats` object which contains some statistics related to
the current :any:`Sims1` or :any:`Sims2` instance and any part of the depth
first search already conducted.

:returns: The :any:`SimsStats` object containing the current stats.
:rtype: SimsStats
)pbdoc");

    ss.def(
        "idle_thread_restarts",
        [](SimsSettings_ const& self) { return self.idle_thread_restarts(); },
        R"pbdoc(
Get the idle thread restart attempt count.

This function returns the number of times an idle thread will attempt to
restart before yielding during execution.

:returns: The number of idle thread restarts.
:rtype: int
)pbdoc");

    ss.def(
        "idle_thread_restarts",
        [](SimsSettings_& self, size_t val) -> Subclass& {
          return self.idle_thread_restarts(val);
        },
        py::arg("val"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, val: int) -> {0}:

Set the idle thread restart attempt count.

This function sets the idle thread restart attempt count. The default value is
``64``.

:param val:
    the maximum number of times an idle thread will attempt to restart before
    yielding.
:type val: int

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError:  if the argument *val* is 0.
)pbdoc",
                    doc_type)
            .c_str());
  }

  //////////////////////////////////////////////////////////////////////////////
  // Sims1, Sims2, RepOrc and MinimalRepOrc common functions
  //////////////////////////////////////////////////////////////////////////////

  // Some of these have been moved out of the base SimsSettings class. See
  // https://github.com/libsemigroups/libsemigroups_pybind11/issues/305
  template <typename Thing, typename ThingBase>
  void def_sims_reporc_common(py::class_<Thing, ThingBase>& thing,
                              std::string_view              doc_type) {
    thing.def("__repr__",
              [](Thing const& self) { return to_human_readable_repr(self); });

    thing.def(py::init<>(),
              fmt::format(R"pbdoc(
:sig=(self: {0}, word: type) -> None:

This function returns an uninitialized :any:`{0}` object that uses
words of type specified by *word*.

:Keyword Arguments:
    * **word** (*type*) -- the type of words to use, must be ``list[int]``.
)pbdoc",
                          doc_type)
                  .c_str());

    // TODO(0): Uncomment or remove
    // thing.def(py::init<Thing const&>(),
    //           fmt::format(R"pbdoc(
    // Construct from a {0} object.
    // )pbdoc",
    //                       doc_type)
    //               .c_str());

    thing.def(
        "init",
        [](Thing& self) -> Thing& { return self.init(); },
        fmt::format(R"pbdoc(
Reinitialize an existing :any:`{0}` object.

This function puts a :any:`{0}` object back into the same state as if it had
been newly default constructed.

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());

    thing.def(
        "init",
        [](ThingBase& self, Thing const& that) -> Thing& {
          return self.init(that);
        },
        py::arg("that"),
        fmt ::format(R"pbdoc(
Reinitialize an existing :any:`{0}` object.

This function re-initializes a :any:`{0}` instance to be in the same state as
*that*.

:param that: The instance use for reinitialization.
:type that: {0}

:returns: The re-initialized object.
:rtype: {0}
)pbdoc",
                     doc_type)
            .c_str());
  }

  //////////////////////////////////////////////////////////////////////////////
  // Sims1 and Sims2 common functions
  //////////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_sims_common(py::class_<Thing, ThingBase>& thing,
                       std::string_view              doc_type) {
    def_sims_reporc_common(thing, doc_type);

    thing.def(py::init<Presentation<word_type> const&>(),
              py::arg("p"),
              fmt::format(R"pbdoc(
:sig=(self: {0}, p: Presentation) -> None:

Construct from a presentation.

The rules of the presentation *p* are used at every node in the depth first
search conducted by an object of this type.

:param p: the presentation to construct from.
:type p: Presentation

:raises LibsemigroupsError:  if :any:`Presentation.throw_if_bad_alphabet_or_rules` throws

:raises LibsemigroupsError:  if *p* has 0-generators and 0-relations.

.. seealso:: :any:`{0}.presentation`, :any:`{0}.init`
)pbdoc",
                          doc_type)
                  .c_str());

    thing.def("__copy__", [](Thing const& self) { return Thing(self); });

    thing.def(
        "copy",
        [](Thing const& self) { return Thing(self); },
        fmt::format(R"pbdoc(
Copy a :any:`{0}` object.

:returns: A copy.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());
    thing.def(
        "init",
        [](Thing& self, Presentation<word_type> const& p) -> Thing& {
          return self.init(p);
        },
        py::arg("p"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, p: Presentation) -> {0}:

Reinitialize an existing :any:`{0}` object from a presentation.

This function puts an object back into the same state as if it had been newly
constructed from the presentation *p*.

:param p: the presentation.
:type p: Presentation

:returns: The first argument *self*.
:rtype: {0}

:raises LibsemigroupsError: if :any:`Presentation.throw_if_bad_alphabet_or_rules` throws

:raises LibsemigroupsError: if *p* has 0-generators and 0-relations.
)pbdoc",
                    doc_type)
            .c_str());

    thing.def("number_of_congruences",
              &Thing::number_of_congruences,
              py::arg("n"),
              fmt::format(R"pbdoc(
:sig=(self: {0}, n: int) -> int:

Returns the number of one-sided congruences with up to a given number of
classes.

This function exists to:

* provide some feedback on the progress of the computation if it runs for more
  than 1 second.
* allow for the computation of the number of congruence to be performed using
  :py:meth:`~{0}.number_of_threads` in parallel.

:param n: the maximum number of congruence classes.
:type n: int

:returns:
    the number of one sided congruences with at most *n* congruence classes.
:rtype: int

:raises LibsemigroupsError:  if *n* is ``0``.

:raises LibsemigroupsError:
    if :py:meth:`~{0}.presentation()` has 0-generators and 0-relations (i.e.
    it has not been initialised).
)pbdoc",
                          doc_type)
                  .c_str());

    thing.def("for_each",
              &Thing::for_each,
              py::arg("n"),
              py::arg("pred"),
              fmt::format(R"pbdoc(
:sig=(self: {0}, n: int, pred: collections.abc.Callable[[WordGraph], None]) -> None:

Apply a unary predicate to every one-sided congruence with at most a given
number of classes.

This function applies the function *pred* to every one-sided congruence with at
most *n* classes. This function exists to:

* provide some feedback on the progress of the computation if it runs for more
  than 1 second.
* allow for a function to be applied to all found word graphs using
  :py:meth:`~{0}.number_of_threads` in parallel.

:param n: the maximum number of congruence classes.
:type n: int

:param pred: the predicate applied to every congruence found.
:type pred: collections.abc.Callable[[WordGraph], None]

:raises LibsemigroupsError: if *n* is ``0``.

:raises LibsemigroupsError:
    if :py:meth:`~{0}.presentation()` has 0-generators and 0-relations (i.e.
    it has not been initialised).

.. seealso::  :py:meth:`~{0}.iterator`, :py:meth:`~{0}.find_if`
)pbdoc",
                          doc_type)
                  .c_str());

    thing.def("find_if",
              &Thing::find_if,
              py::arg("n"),
              py::arg("pred"),
              fmt::format(R"pbdoc(
:sig=(self: {0}, n: int, pred: collections.abc.Callable[[WordGraph], bool]) -> WordGraph:

Apply a unary predicate to one-sided congruences with at most a given number of
classes, until it returns ``True``.

This function applies the predicate *pred* to every congruence with at most *n*
classes, until a congruence satisfying the predicate is found. This function
exists to:

* provide some feedback on the progress of the computation if it runs for more
  than 1 second.
* allow for searching for a congruence satisfying certain conditions using
  :py:meth:`~{0}.number_of_threads` in parallel.

:param n: the maximum number of congruence classes.
:type n: int

:param pred: the predicate applied to every congruence found.
:type pred: collections.abc.Callable[[WordGraph], bool]

:returns:
    The first :any:`WordGraph` for which *pred* returns ``True``, or the empty
    word graph if no such word graph exists.
:rtype: WordGraph

:raises LibsemigroupsError: if *n* is ``0``.

:raises LibsemigroupsError:
    if :py:meth:`~{0}.presentation()` has 0-generators and 0-relations (i.e.
    it has not been initialised).

.. seealso::  :py:meth:`~{0}.iterator`, :py:meth:`~{0}.for_each`
)pbdoc",
                          doc_type)
                  .c_str());

    thing.def(
        "iterator",
        [](Thing const& self, size_type n) {
          return py::make_iterator(self.cbegin(n), self.cend(n));
        },
        py::arg("n"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, n: int) -> collections.abc.Iterator[WordGraph]:

Returns an iterator yielding all congruences of index at most *n*.

This function returns an iterator yielding instances of :any:`WordGraph` that
represent the congruences with at most *n* classes. The order in which the
congruences are yielded by the iterator is implementation specific. The meaning
of the :any:`WordGraph` objects yielded by the iterator depends on whether the
input is a monoid presentation (i.e.
:py:meth:`~Presentation.contains_empty_word()` returns ``True`` ) or a
semigroup presentation.

If the input is a monoid presentation for a monoid :math:`M`, then the
:any:`WordGraph` pointed to by an iterator of this type has at most *n* nodes,
and the right action of :math:`M` on the nodes of the word graph is isomorphic
to the action of :math:`M` on the classes of a right congruence.

If the input is a semigroup presentation for a semigroup :math:`S`, then the
:any:`WordGraph` has at most *n* + 1 nodes, and the right action of :math:`S`
on the nodes :math:`\{{1, \ldots, n\}}` of the :any:`WordGraph` is isomorphic to
the action of :math:`S` on the classes of a right congruence. It'd probably be
better in this case if node :math:`0` was not included in the output
:any:`WordGraph`, but it is required in the implementation of the low-index
congruence algorithm, and to avoid unnecessary copies, we've left it in for the
time being.

:param n: the maximum number of classes in a congruence.
:type n: int

:returns:
    An iterator ``it`` yielding :any:`WordGraph` objects with at most *n* or
    *n* + 1 nodes depending on the presentation, see above.
:rtype: collections.abc.Iterator[WordGraph]

:raises LibsemigroupsError: if *n* is ``0``.

:raises LibsemigroupsError:
    if :py:meth:`~{0}.presentation()` has 0-generators and 0-relations (i.e.
    it has not been initialised).
)pbdoc",
                    doc_type)
            .c_str());
  }

  //////////////////////////////////////////////////////////////////////////////
  // RepOrc and MinimalRepOrc common functions
  //////////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_reporc_common(py::class_<Thing, ThingBase>& thing,
                         std::string_view              doc_type) {
    def_sims_reporc_common(thing, doc_type);

    thing.def(
        "target_size",
        [](Thing const& self) { return self.target_size(); },
        fmt::format(R"pbdoc(
Get the current target size.

This function returns the current value for the target size, i.e. the desired
size of the transformation semigroup corresponding to the :any:`WordGraph`
returned by the function :py:meth:`~{0}.word_graph`.

:returns: A value of type ``int``.
:rtype: int
)pbdoc",
                    doc_type)
            .c_str());

    thing.def(
        "target_size",
        [](Thing& self, size_t val) -> Thing& { return self.target_size(val); },
        py::arg("val"),
        fmt::format(R"pbdoc(
:sig=(self: {0}, val: int) -> {0}:

Set the target size.

This function sets the target size, i.e. the desired size of the transformation
semigroup corresponding to the :any:`WordGraph` returned by the function
:py:meth:`~{0}.word_graph`.

:param val: the target size.
:type val: int

:returns: The first argument *self*.
:rtype: {0}
)pbdoc",
                    doc_type)
            .c_str());
  }

  void init_sims(py::module& m) {
    ////////////////////////////////////////////////////////////////////////////
    // SimsStats
    ////////////////////////////////////////////////////////////////////////////

    py::class_<SimsStats> st(m,
                             "SimsStats",
                             R"pbdoc(
For keeping track of various statistics arising during the runtime of the low
index algorithm.

The purpose of this class is to collect some statistics related to :any:`Sims1`
or :any:`Sims2`.

.. seealso:: :any:`Sims1`, :any:`Sims2`
)pbdoc");

    st.def("__repr__",
           [](SimsStats const& st) { return to_human_readable_repr(st); });

    st.def(
        "count_last",
        [](SimsStats const& d) { return d.count_last.load(); },
        R"pbdoc(
Get the number of congruences found at time of last report.

This function returns member holds the number of congruences found by the
:any:`Sims1` or :any:`Sims2` algorithm at the time of the last call to
:py:meth:`~SimsStats.stats_check_point`.

:returns: The number of congruences.
:rtype: int

.. seealso::
    :py:meth:`~SimsStats.stats_check_point`, :py:meth:`~SimsStats.count_now`
)pbdoc");

    st.def(
        "count_now",
        [](SimsStats const& d) { return d.count_now.load(); },
        R"pbdoc(
Get number of congruences found up to this point.

This function returns the total number of congruences found during the running
of the :any:`Sims1` or :any:`Sims2` algorithm.

:returns: The number of congruences.
:rtype: int

.. seealso:: :py:meth:`~SimsStats.count_last`
)pbdoc");

    st.def(
        "max_pending",
        [](SimsStats const& d) { return d.max_pending.load(); },
        R"pbdoc(
Get the maximum number of pending definitions.

A "pending definition" is just an edge that will be defined at some point in
the future in the :any:`WordGraph` represented by a :any:`Sims1` or
:any:`Sims2` instance at any given moment. This function returns the maximum
number of such pending definitions that occur during the running of the
algorithms in :any:`Sims1` or :any:`Sims2`.

:returns: The maximum number of definitions.
:rtype: int
)pbdoc");

    st.def(
        "total_pending_last",
        [](SimsStats const& d) { return d.total_pending_last.load(); },
        R"pbdoc(
Get the total number of pending definitions at time of last report.

A "pending definition" is just an edge that will be defined at some point in
the future in the :any:`WordGraph` represented by a :any:`Sims1` or
:any:`Sims2` instance at any given moment. This function returns the total
number of pending definitions that occur at the time of the last call to
:py:meth:`~SimsStats.stats_check_point`. This is the same as the number of
nodes in the search tree encounter during the running of :any:`Sims1` or
:any:`Sims2`.

:returns: The number of pending definitions.
:rtype: int

.. seealso::
    :py:meth:`~SimsStats.stats_check_point`,
    :py:meth:`~SimsStats.total_pending_now`
)pbdoc");

    st.def(
        "total_pending_now",
        [](SimsStats const& d) { return d.total_pending_now.load(); },
        R"pbdoc(
Get the total number of pending definitions.

A "pending definition" is just an edge that will be defined at some point in
the future in the :any:`WordGraph` represented by a :any:`Sims1` or
:any:`Sims2` instance at any given moment. This function returns the total
number of pending definitions that occur during the running of the algorithms
in :any:`Sims1` or :any:`Sims2`. This is the same as the number of nodes in the
search tree encounter during the running of :any:`Sims1` or :any:`Sims2`.

:returns: The number of pending definitions.
:rtype: int

.. seealso:: :py:meth:`~SimsStats.total_pending_last`
)pbdoc");

    st.def(py::init<>(), R"pbdoc(
Default constructor.

Constructs a :any:`SimsStats` object with all statistics set to zero.
)pbdoc");

    st.def("__copy__", [](SimsStats const& self) { return SimsStats(self); });
    st.def(
        "copy",
        [](SimsStats const& self) { return SimsStats(self); },
        R"pbdoc(
Copy a :any:`SimsStats` object.

This function returns a :any:`SimsStats` object that is a copy of *self*. The
state of the new :any:`SimsStats` object is the same as *self*. This triggers
an atomic load on the member variables of *self*.

:returns: The copy.
:rtype: SimsStats
)pbdoc");

    st.def(
        "init",
        [](SimsStats& self) -> SimsStats& { return self.init(); },
        R"pbdoc(
Reinitialize an existing SimsStats object.

This function puts a :any:`SimsStats` object back into the same state as if it
had been newly default constructed.

:returns: The first argument *self*.
:rtype: SimsStats
)pbdoc");

    st.def(
        "init",
        [](SimsStats& self, SimsStats const& that) -> SimsStats& {
          return self.init(that);
        },
        py::arg("that"),
        R"pbdoc(
This function reinitializes the :any:`SimsStats` object from *that*. The state
of the new :any:`SimsStats` object is the same as *that*. This triggers an
atomic load on the member variables of *that*.

:param that: the :any:`SimsStats` to reinitialize from.
:type that: SimsStats

:Returns: The first argument *self*.
:rtype: SimsStats
)pbdoc");

    st.def("stats_check_point",
           &SimsStats::stats_check_point,
           R"pbdoc(
Store the current statistic values.

This function overwrites the values of :py:meth:`~SimsStats.count_last` and
:py:meth:`~SimsStats.total_pending_last` with :py:meth:`~SimsStats.count_now`
and :py:meth:`~SimsStats.total_pending_now` respectively. Triggers an atomic
load on :py:meth:`~SimsStats.count_now` and
:py:meth:`~SimsStats.total_pending_now`.

:Returns: The first argument *self*.
:rtype: SimsStats
)pbdoc");

    st.def("stats_zero",
           &SimsStats::stats_zero,
           R"pbdoc(
Set all statistics to zero.

:Returns: The first argument *self*.
:rtype: SimsStats
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // SimsRefinerFaithful
    ////////////////////////////////////////////////////////////////////////////

    // The refiners are defined before Sims because the
    // refiners appear as parameters types or return types
    // for later functions.

    py::class_<SimsRefinerFaithful> srf(m,
                                        "SimsRefinerFaithful",
                                        R"pbdoc(
For pruning the search tree when looking for congruences arising from right or
two-sided congruences representing faithful actions.

This class provides a pruner for pruning the search tree when looking for right
congruences representing faithful actions. A right congruence represents a
faithful action if and only if it does not contain any non-trivial two-sided
congruence. Equivalently, a word graph of a right congruence represents a
faithful action if and only if there is no nontrivial pair of words :math:`(u,
v)` such that every vertex of the word graph is compatible with :math:`(u, v)`.

.. seealso:: :py:meth:`~Sims1.pruners`, :py:meth:`~Sims1.add_pruner`
)pbdoc");

    srf.def("__repr__", [](SimsRefinerFaithful const& srf) {
      return to_human_readable_repr(srf);
    });

    srf.def(py::init<>(), R"pbdoc(
:sig=(self: SimsRefinerFaithful, word: type) -> None:

This function returns an uninitialized :any:`SimsRefinerFaithful` object that uses
words of type specified by *word*.

:Keyword Arguments:
    * **word** (*type*) -- the type of words to use, must be ``list[int]``.
)pbdoc");

    srf.def(py::init<std::vector<word_type> const&>(),
            py::arg("forbid"),
            R"pbdoc(
:sig=(self: SimsRefinerFaithful, forbid: list[list[int]]) -> None:
Construct from set of forbidden pairs.

This function constructs a :any:`SimsRefinerFaithful` pruner with respect to the set of
forbidden relations in *forbid*. If *forbid* contains no trivial pairs (i.e.
pairs of words that are equal in the underlying semigroup or monoid), then all
word graphs rejected by :any:`SimsRefinerFaithful` are guaranteed to not be
extendable to a word graph representing a faithful congruence. Otherwise, the
pruner will incorrectly reject all word graphs.

If in addition *forbid* is a set of relations containing all minimal congruence
generating pairs of a given semigroup or monoid, then
:any:`SimsRefinerFaithful` will also correctly determine if a complete word
graph represents a faithful congruence. Otherwise, the complete word graphs
accepted by :any:`SimsRefinerFaithful` are not guaranteed to be faithful and
must be checked by some other means.

:param forbid:
    A list of words such that ``(forbid[2*i], forbid[2*i+1])`` is the ``i``-th
    forbidden pair.
:type forbid: list[list[int]]
)pbdoc");

    // Despite what's written here this does not return by
    // reference for JDM
    srf.def(
        "forbid",
        [](SimsRefinerFaithful const& srf) -> auto const& {
          return srf.forbid();
        },
        R"pbdoc(
:sig=(self: SimsRefinerFaithful) -> list[list[int]]:
Get the forbidden pairs defining the refiner.

This function returns the defining forbidden pairs of a
:any:`SimsRefinerFaithful` instance.

:returns:
    A list of words ``result`` such that ``(result[2*i], result[2*i+1])`` is
    the ``i``-th forbidden pair.
:rtype: list[list[int]]
)pbdoc",
        py::return_value_policy::reference_internal);

    srf.def(
        "init",
        [](SimsRefinerFaithful& self) -> SimsRefinerFaithful& {
          return self.init();
        },
        R"pbdoc(
Reinitialize an existing :any:`SimsRefinerFaithful` object.

This function puts an object back into the same state as if it had been newly
default constructed.

:returns: The first argument *self*.
:rtype: SimsRefinerFaithful
)pbdoc");

    srf.def(
        "init",
        [](SimsRefinerFaithful& self, std::vector<word_type> const& forbid)
            -> SimsRefinerFaithful& { return self.init(forbid); },
        py::arg("forbid"),
        R"pbdoc(
:sig=(self: SimsRefinerFaithful, forbid: list[list[int]]) -> SimsRefinerFaithful:

Reinitialize an existing :any:`SimsRefinerFaithful` object from a set of
forbidden pairs.

This function puts an object back into the same state as if it had been newly
constructed from set of forbidden pairs *forbid*.

:param forbid:
    A list of words such that ``(forbid[2*i], forbid[2*i+1])`` is the ``i``-th
    forbidden pair.
:type forbid: list[list[int]]

:returns: The first argument *self*.
:rtype: SimsRefinerFaithful
)pbdoc");

    srf.def(
        "__call__",
        [](SimsRefinerFaithful& d, word_graph_type const& wg) { return d(wg); },
        py::arg("wg"),
        R"pbdoc(
Check if a word graph can be extended to one defining a faithful congruence.

Returns ``False`` if there is no way of adding edges and nodes to
*wg*  which will result in a word graph defining a faithful congruence.
Otherwise returns ``True``.

:param wg: A word graph.
:type wg: WordGraph

:returns: A boolean.
:rtype: bool
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // SimsRefinerIdeals
    ////////////////////////////////////////////////////////////////////////////

    py::class_<SimsRefinerIdeals> sri(m,
                                      "SimsRefinerIdeals",
                                      R"pbdoc(
For pruning the search tree when looking for congruences arising from right or
two-sided ideals.

This class provides a pruner for pruning the search tree when looking for right
or two-sided congruences arising from ideals (Rees congruences).

.. seealso:: :py:meth:`~Sims1.pruners`, :py:meth:`~Sims1.add_pruner`
)pbdoc");

    sri.def("__repr__", [](SimsRefinerIdeals const& sri) {
      return to_human_readable_repr(sri);
    });

    sri.def(py::init<>(), R"pbdoc(
:sig=(self: SimsRefinerIdeals, word: type) -> None:

This function returns an uninitialized :any:`SimsRefinerIdeals` object that uses
words of type specified by *word*.

:Keyword Arguments:
    * **word** (*type*) -- the type of words to use, must be ``list[int]``.
)pbdoc");
    sri.def(py::init<Presentation<word_type> const&>(),
            R"pbdoc(
:sig=(self: SimsRefinerIdeals, p: Presentation) -> None:

Construct from presentation.

This function constructs a :any:`SimsRefinerIdeals` pruner for the semigroup or
monoid defined by *p*.

:param p: A presentation.
:type p: Presentation
)pbdoc");
    sri.def(
        "init",
        [](SimsRefinerIdeals& self) -> SimsRefinerIdeals& {
          return self.init();
        },
        R"pbdoc(
Reinitialize an existing :any:`SimsRefinerIdeals` object.

This function puts an object back into the same state as if it had been newly
default constructed.

:returns: The first argument *self*.
:rtype: SimsRefinerIdeals
)pbdoc");

    sri.def(
        "init",
        [](SimsRefinerIdeals& self, Presentation<word_type> const& p)
            -> SimsRefinerIdeals& { return self.init(p); },
        py::arg("p"),
        R"pbdoc(
:sig=(self: SimsRefinerIdeals, p: Presentation) -> SimsRefinerIdeals:

Reinitialize an existing :any:`SimsRefinerIdeals` object from a presentation.

This function puts an object back into the same state as if it had been newly
constructed from the presentation *p*.

:param p: A presentation.
:type p: Presentation

:returns: The first argument *self*.
:rtype: SimsRefinerIdeals

:raises LibsemigroupsError:  if *p* is not valid
:raises LibsemigroupsError:  if *p* has 0-generators and 0-relations.

.. seealso:: :py:meth:`~SimsRefinerIdeals.presentation`
)pbdoc");

    sri.def(
        "presentation",
        [](SimsRefinerIdeals const& sri) -> auto const& {
          return sri.presentation();
        },
        R"pbdoc(
:sig=(self: SimsRefinerIdeals) -> Presentation:

Get the presentation over which the refiner is defined.

This function returns the defining presentation of a :any:`SimsRefinerIdeals`
instance.

:returns: The presentation.
:rtype: Presentation
)pbdoc",
        py::return_value_policy::reference_internal);

    sri.def(
        "__call__",
        [](SimsRefinerIdeals& d, word_graph_type const& wg) { return d(wg); },
        py::arg("wg"),
        R"pbdoc(
Check if a word graph can be extended to one defining a Rees congruence.

Returns ``False`` if there is no way of adding edges and nodes to *wg*  which
will result in a word graph defining a Rees congruence. Otherwise returns
``True``.

:param wg: A word graph.
:type wg: WordGraph

:returns: A boolean.
:rtype: bool
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // RepOrc
    ////////////////////////////////////////////////////////////////////////////
    py::class_<SimsSettings<RepOrc>>         ssro(m, "SimsSettingsRepOrc");
    py::class_<RepOrc, SimsSettings<RepOrc>> ro(m, "RepOrc", R"pbdoc(
For computing small degree transformation representations of a finite
semigroup or monoid.

This class is a helper for :any:`Sims1` calling the :any:`word_graph` member
function attempts to find a right congruence, represented as an
:any:`WordGraph`, of the semigroup or monoid defined by the presentation
consisting of its :py:meth:`~RepOrc.presentation` and
:py:meth:`~RepOrc.long_rules` with the following properties:

* the transformation semigroup defined by the :any:`WordGraph` has size
  :py:meth:`~RepOrc.target_size` ;
* the number of nodes in the :any:`WordGraph` is at least
  :py:meth:`~RepOrc.min_nodes` and at most :py:meth:`~RepOrc.max_nodes`.

If no such :any:`WordGraph` can be found, then an empty :any:`WordGraph` is
returned (with ``0`` nodes and ``0`` edges).
)pbdoc");

    bind_sims_settings(ssro, "RepOrc");
    def_reporc_common(ro, "RepOrc");

    ro.def(
        "max_nodes",
        [](RepOrc const& self) { return self.max_nodes(); },
        R"pbdoc(
Get the current maximum number of nodes.

This function returns the current value for the maximum number of nodes in the
:any:`WordGraph` that we are seeking.

:returns: A value of type ``int``.
:rtype: int
)pbdoc");

    ro.def(
        "max_nodes",
        [](RepOrc& self, size_t val) -> RepOrc& { return self.max_nodes(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: RepOrc, val: int) -> RepOrc:

Set the maximum number of nodes.

This function sets the maximum number of nodes in the :any:`WordGraph` that we
are seeking.

:param val: the maximum number of nodes
:type val: int

:returns: The first argument *self*.
:rtype: RepOrc
)pbdoc");

    ro.def(
        "min_nodes",
        [](RepOrc const& self) { return self.min_nodes(); },
        R"pbdoc(
Get the current minimum number of nodes.

This function returns the current value for the minimum number of nodes in the
:any:`WordGraph` that we are seeking.

:returns: A value of type ``int``.
:rtype: int
)pbdoc");

    ro.def(
        "min_nodes",
        [](RepOrc& self, size_t val) -> RepOrc& { return self.min_nodes(val); },
        py::arg("val"),
        R"pbdoc(
:sig=(self: RepOrc, val: int) -> RepOrc:

Set the minimum number of nodes.

This function sets the minimal number of nodes in the :any:`WordGraph` that we
are seeking.

:param val: the minimum number of nodes
:type val: int

:returns: The first argument *self*.
:rtype: RepOrc
)pbdoc");

    // The next function returns by value, so no
    // return_value_policy required here.
    ro.def("word_graph",
           &RepOrc::word_graph,
           R"pbdoc(
Get the word graph.

This function attempts to find a right congruence, represented as an
:any:`WordGraph`, of the semigroup or monoid defined by the presentation
consisting of its :py:meth:`~RepOrc.presentation` and
:py:meth`~RepOrc.long_rules` with the following properties:

* the transformation semigroup defined by the :any:`WordGraph` has size
  :py:meth:`~RepOrc.target_size` ;
* the number of nodes in the :any:`WordGraph` is at least
  :py:meth:`~RepOrc.min_nodes` and at most :py:meth:`~RepOrc.max_nodes`.

If no such :any:`WordGraph` can be found, then an empty :any:`WordGraph` is
returned (with ``0`` nodes and ``0`` edges).

:returns: A value of type :any:`WordGraph`.
:rtype: WordGraph
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // MinimalRepOrc
    ////////////////////////////////////////////////////////////////////////////

    py::class_<SimsSettings<MinimalRepOrc>> ssmro(m,
                                                  "SimsSettingsMinimalRepOrc");

    py::class_<MinimalRepOrc, SimsSettings<MinimalRepOrc>> mro(
        m, "MinimalRepOrc", R"pbdoc(
For computing the minimal degree of a transformation representation arising
from a right congruence of a finite semigroup or monoid.

This class is a helper for :any:`Sims1`, calling the :any:`word_graph` member
function attempts to find a right congruence, represented as an
:any:`WordGraph`, with the minimum possible number of nodes such that the
action of the semigroup or monoid defined by the presentation consisting of its
:py:meth:`~MinimalRepOrc.presentation` on the nodes of the :any:`WordGraph`
corresponds to a semigroup of size :py:meth:`~MinimalRepOrc.target_size`.

If no such ::any:`WordGraph` can be found, then an empty :any:`WordGraph` is
returned (with ``0`` nodes and ``0`` edges).
)pbdoc");

    bind_sims_settings(ssmro, "MinimalRepOrc");
    def_reporc_common(mro, "MinimalRepOrc");

    // The next function returns by value, so no
    // return_value_policy required here.
    mro.def("word_graph",
            &MinimalRepOrc::word_graph,
            R"pbdoc(
Get the word graph.

This function attempts to find a right congruence, represented as an
:any:`WordGraph`, with the minimum possible number of nodes such that the
action of the semigroup or monoid defined by the presentation consisting of its
:py:meth:`~MinimalRepOrc.presentation` and :py:meth:`~MinimalRepOrc.long_rules`
on the nodes of the :any:`WordGraph` corresponds to a semigroup of size
:py:meth:`~MinimalRepOrc.target_size`. If no such :any:`WordGraph` can be
found, then an empty :any:`WordGraph` is returned (with ``0`` nodes and ``0``
edges).

The algorithm implemented by this function repeatedly runs:

.. code-block:: python3

    RepOrc(self)
        .min_nodes(1)
        .max_nodes(best)
        .target_size(self.target_size())
        .word_graph();


where ``best`` is initially :py:meth:`~MinimalRepOrc.target_size`, until the
returned :any:`WordGraph` is empty, and then the penultimate :any:`WordGraph`
is returned (if any).

:returns: A value of type :any:`WordGraph`.
:rtype: WordGraph
)pbdoc");

    ////////////////////////////////////////////////////////////////////////////
    // Sims1
    ////////////////////////////////////////////////////////////////////////////

    py::class_<SimsSettings<Sims1>>        sss1(m, "SimsSettingsSims1");
    py::class_<Sims1, SimsSettings<Sims1>> s1(m, "Sims1", R"pbdoc(
For computing finite index right congruences of a finitely presented semigroup
or monoid.

The algorithm implemented by this class is essentially the low index subgroup
algorithm for finitely presented groups described in Section 5.6 of
:cite:`Sims1994aa`. The low index subgroups algorithm was adapted for
semigroups and monoids by M. Anagnostopoulou-Merkouri, R. Cirpons, J. D.
Mitchell, and M. Tsalakou; see :cite:`Anagnostopoulou-Merkouri2023aa`.

The purpose of this class is to provide the functions
:py:meth:`~Sims1.iterator`, :py:meth:`~Sims1.for_each` and
:py:meth:`~Sims1.find_if`, which permit iterating through the one-sided
congruences of a semigroup or monoid defined by a presentation containing (a
possibly empty) set of pairs and with at most a given number of classes. An
iterator returned by :py:meth:`~Sims1.iterator` yields :any:`WordGraph`
instances representing the action of the semigroup or monoid on the classes of
a congruence.

.. seealso:: :any:`Sims2` for equivalent functionality for 2-sided congruences.
)pbdoc");

    bind_sims_settings(sss1, "Sims1");
    def_sims_common(s1, "Sims1");

    ////////////////////////////////////////////////////////////////////////////
    // Sims2
    ////////////////////////////////////////////////////////////////////////////

    py::class_<SimsSettings<Sims2>>        sss2(m, "SimsSettingsSims2");
    py::class_<Sims2, SimsSettings<Sims2>> s2(m, "Sims2", R"pbdoc(
For computing finite index two-sided congruences of a finitely presented
semigroup or monoid.

The algorithm implemented by this class is described in
:cite:`Anagnostopoulou-Merkouri2023aa`. The purpose of this class is to provide
the functions :py:meth:`~Sims2.iterator`, :py:meth:`~Sims2.for_each` and
:py:meth:`~Sims2.find_if`, which permit iterating through the two-sided
congruences of a semigroup or monoid defined by a presentation containing, or
not containing, (possibly empty) sets of pairs and with at most a given number
of classes. An iterator returned by :py:meth:`~Sims2.iterator` yields
:any:`WordGraph` instances representing the action of the semigroup or monoid
on the classes of a congruence.

.. seealso::
    :any:`Sims1` for equivalent functionality for 1-sided congruences.
)pbdoc");

    bind_sims_settings(sss2, "Sims2");
    def_sims_common(s2, "Sims2");

    ////////////////////////////////////////////////////////////////////////////
    // Helper functions
    ////////////////////////////////////////////////////////////////////////////

    m.def(
        "sims_right_generating_pairs",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          auto gen_pairs
              = sims::right_generating_pairs(p, wg)
                | rx::transform([](auto x) -> relation_type { return x; });

          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
:sig=(p: Presentation, wg: WordGraph) -> collections.abc.Iterator[tuple[list[int], list[int]]]:
Compute the right congruence generating pairs of a word graph on
an f.p. semigroup or monoid.

This function returns the right congruence generating pairs of the right
congruence defined by the word graph *wg* on the semigroup or monoid
defined by *p*.

:param p: A presentation.
:type p: Presentation

:param wg: A word graph.
:type wg: WordGraph

:returns: An iterator of generating pairs.
:rtype: collections.abc.Iterator[tuple[list[int], list[int]]]

:raises LibsemigroupsError:
    if the argument *wg* does not define a right congruence on the
    semigroup or monoid defined by *p*.
)pbdoc");

    m.def(
        "sims_right_generating_pairs",
        [](word_graph_type const& wg) {
          auto gen_pairs
              = sims::right_generating_pairs(wg)
                | rx::transform([](auto x) -> relation_type { return x; });

          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> collections.abc.Iterator[tuple[list[int], list[int]]]:
Compute the right congruence generating pairs of a word graph on
the free monoid.

This function returns the right congruence generating pairs of the right
congruence defined by the word graph *wg* on the free monoid.

:param wg: A word graph.
:type wg: WordGraph

:returns: An iterator of generating pairs.
:rtype: collections.abc.Iterator[tuple[list[int], list[int]]]

:raises LibsemigroupsError:
    if the argument *wg* does not define a right congruence on the free monoid.
)pbdoc");

    m.def(
        "sims_two_sided_generating_pairs",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          auto gen_pairs
              = sims::two_sided_generating_pairs(p, wg)
                | rx::transform([](auto x) -> relation_type { return x; });

          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
:sig=(p: Presentation, wg: WordGraph) -> collections.abc.Iterator[tuple[list[int], list[int]]]:
Compute the two-sided congruence generating pairs of a word graph on
an f.p. semigroup or monoid.

This function returns the two-sided congruence generating pairs of the two-sided
congruence defined by the word graph *wg* on the semigroup or monoid
defined by *p*.

:param p: A presentation.
:type p: Presentation

:param wg: A word graph.
:type wg: WordGraph

:returns: An iterator of generating pairs.
:rtype: collections.abc.Iterator[tuple[list[int], list[int]]]

:raises LibsemigroupsError:
    if the argument *wg* does not define a two-sided congruence on the
    semigroup or monoid defined by *p*.

.. note::
    The generating pairs of a two-sided congruence :math:`\rho` as a two-sided
    congruence differ from the generating pairs of :math:`\rho` as a right
    congruence. This function returns the two-sided congruence generating
    pairs, not the right congruence generating pairs of a two-sided congruence.
)pbdoc");

    m.def(
        "sims_two_sided_generating_pairs",
        [](word_graph_type const& wg) {
          auto gen_pairs
              = sims::two_sided_generating_pairs(wg)
                | rx::transform([](auto x) -> relation_type { return x; });

          return py::make_iterator(rx::begin(gen_pairs), rx::end(gen_pairs));
        },
        py::arg("wg"),
        R"pbdoc(
:sig=(wg: WordGraph) -> collections.abc.Iterator[tuple[list[int], list[int]]]:
Compute the two-sided congruence generating pairs of a word graph on the free
monoid.

This function returns the two-sided congruence generating pairs of the two-sided
congruence defined by the word graph *wg* on the free monoid.

:param wg: A word graph.
:type wg: WordGraph

:returns: An iterator of generating pairs.
:rtype: collections.abc.Iterator[tuple[list[int], list[int]]]

:raises LibsemigroupsError:
    if the argument *wg* does not define a two-sided congruence on the free
    monoid.

.. note::
    The generating pairs of a two-sided congruence :math:`\rho` as a two-sided
    congruence differ from the generating pairs of :math:`\rho` as a right
    congruence. This function returns the two-sided congruence generating
    pairs, not the right congruence generating pairs of a two-sided congruence.
)pbdoc");

    m.def(
        "sims_is_right_congruence",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_right_congruence(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
:sig=(p: Presentation, wg: WordGraph) -> bool:

Check if a word graph defines a right congruence on an f.p. semigroup or
monoid.

Returns ``True`` if the word graph *wg* defines a right congruence on the
semigroup or monoid defined by *p* and ``False`` otherwise.

:param p: A presentation.
:type p: Presentation

:param wg: A word graph.
:type wg: WordGraph

:returns: An boolean.
:rtype: bool
)pbdoc");

    m.def(
        "sims_is_right_congruence_of_dual",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_right_congruence_of_dual(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
:sig=(p: Presentation, wg: WordGraph) -> bool:

Check if a word graph defines a right congruence on the dual of an f.p.
semigroup or monoid.

Returns ``True`` if the word graph *wg* defines a right congruence on the dual
of the semigroup or monoid defined by *p* and ``False`` otherwise. This is
equivalent to checking if the word graph defines a left congruence in the
semigroup or monoid defined by *p*.

:param p: A presentation.
:type p: Presentation

:param wg: A word graph.
:type wg: WordGraph

:returns: An boolean.
:rtype: bool
)pbdoc");

    m.def(
        "sims_is_two_sided_congruence",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_two_sided_congruence(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
:sig=(p: Presentation, wg: WordGraph) -> bool:

Check if a word graph defines a two sided congruence on an f.p. semigroup or
monoid.

Returns ``True`` if the word graph *wg* defines a two-sided congruence on the
semigroup or monoid defined by *p* and ``False`` otherwise.

:param p: A presentation.
:type p: Presentation

:param wg: A word graph.
:type wg: WordGraph

:returns: An boolean.
:rtype: bool
)pbdoc");

    m.def(
        "sims_is_maximal_right_congruence",
        [](Presentation<word_type> const& p, word_graph_type const& wg) {
          return sims::is_maximal_right_congruence(p, wg);
        },
        py::arg("p"),
        py::arg("wg"),
        R"pbdoc(
:sig=(p: Presentation, wg: WordGraph) -> bool:

Check if a word graph defines a maximal right congruence on an f.p. semigroup
or monoid.

Returns ``True`` if the word graph *wg* defines a maximal right congruence on
the semigroup or monoid defined by *p* and ``False`` otherwise.

:param p: A presentation.
:type p: Presentation

:param wg: A word graph.
:type wg: WordGraph

:returns: An boolean.
:rtype: bool
)pbdoc");

    m.def(
        "sims_poset",
        [](Sims1& sims, size_t n) {
          return sims::poset(sims.cbegin(n), sims.cend(n));
        },
        py::arg("sims"),
        py::arg("n"),
        R"pbdoc(
:sig=(sims: Sims1, n: int) -> Matrix:

Compute the inclusion poset of a collection of congruences returned by a
:any:`Sims1` object with at most *n* classes.

This function returns a boolean matrix whose :math:`(i, j)`-th entry is 1 if
and only if the congruence defined by the :math:`i`-th word graph returned by
:any:`Sims1.iterator` with input *n* is a subrelation of the congruence
defined by the :math:`j`-th word graph.

When *n* is large enough, so that :any:`Sims1` computes all right congruences
of a given semigroup or monoid, then this is equivalent to computing the right
congruence lattice of the semigroup or monoid.

:param sims: A :any:`Sims1` object.
:type sims: Sims1

:param n: Maximum number of congruence classes.
:type n: int

:returns: A boolean matrix defining the congruence poset.
:rtype: Matrix
)pbdoc");

    m.def(
        "sims_poset",
        [](Sims2& sims, size_t n) {
          return sims::poset(sims.cbegin(n), sims.cend(n));
        },
        py::arg("sims"),
        py::arg("n"),
        R"pbdoc(
:sig=(sims: Sims2, n: int) -> Matrix:

Compute the inclusion poset of a collection of congruences returned by a
:any:`Sims2` object with at most *n* classes.

This function returns a boolean matrix whose :math:`(i, j)`-th entry is 1 if
and only if the congruence defined by the :math:`i`-th word graph returned by
:any:`Sims2.iterator` with input *n* is a subrelation of the congruence
defined by the :math:`j`-th word graph.

When *n* is large enough, so that :any:`Sims2` computes all two-sided
congruences of a given semigroup or monoid, then this is equivalent to
computing the two-sided congruence lattice of the semigroup or monoid.

:param sims: A :any:`Sims2` object.
:type sims: Sims2

:param n: Maximum number of congruence classes.
:type n: int

:returns: A boolean matrix defining the congruence poset.
:rtype: Matrix
)pbdoc");
  }  // init_sims

}  // namespace libsemigroups
