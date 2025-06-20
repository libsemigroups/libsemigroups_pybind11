//
// libsemigroups_pybind11
// Copyright (C) 2025 Reinis Cirpons
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
#include <libsemigroups/constants.hpp>  // for POSITIVE_INFINITY
#include <libsemigroups/stephen.hpp>    // for Stephen
#include <libsemigroups/types.hpp>      // for word_type

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_stephen

namespace libsemigroups {
  namespace py = pybind11;

  namespace {
    template <typename PresentationType>
    void bind_stephen(py::module& m, std::string const& name) {
      using Stephen_ = Stephen<PresentationType>;

      py::class_<Stephen_, Runner> thing(m,
                                         name.c_str(),
                                         R"pbdoc(
Class for constructing a word graph of left factors a word in a finitely
presented semigroup.

This page describes the class :any:`Stephen` which implements Stephen's
procedure for constructing the :any:`WordGraph` corresponding to the left
factors of a word in a finitely presented semigroup or a finitely presented
inverse semigroup. The algorithm implemented in this class is closely related
to the Todd-Coxeter algorithm (as implemented in :any:`ToddCoxeter`) and
originates in :cite:`Stephen1987aa`.

.. seealso:: :any:`Runner`.

.. |name| replace:: :any:`Stephen`

.. include:: ../../_static/runner_non_inherit.rst

.. doctest::

   >>> from libsemigroups_pybind11 import (Stephen, stephen, Presentation,
   ... presentation)
   >>> p = Presentation([0, 1])
   >>> presentation.add_rule(p, [0, 0, 0], [0])
   >>> presentation.add_rule(p, [1, 1, 1], [1])
   >>> presentation.add_rule(p, [0, 1, 0, 1], [0, 0])
   >>> s = Stephen(p)
   >>> s.set_word([1, 1, 0, 1]).run()
   >>> stephen.accepts(s, [1, 1, 0, 0, 1, 0])
   True
   >>> stephen.accepts(s, [])
   False
)pbdoc");
      thing.def("__repr__", [](Stephen_ const& stephen) {
        return to_human_readable_repr(stephen);
      });
      thing.def("__copy__",
                [](Stephen_ const& self) { return Stephen_(self); });
      // Not directly usable so not included
      //       thing.def(py::init<>(), R"pbdoc(
      // This function default constructs an empty instance of :any:`Stephen`.
      // Use :py:meth:`~Stephen.init` and :py:meth:`~Stephen.set_word` to
      // specify the presentation and the word, respectively. )pbdoc");
      thing.def(py::init<PresentationType const&>(),
                py::arg("p"),
                R"pbdoc(
:sig=(self: Stephen, p: Presentation) -> None:

This function constructs :any:`Stephen` from a presentation.

:param p: the presentation.
:type p: Presentation
)pbdoc");
      thing.def(
          "copy",
          [](Stephen_ const& self) { return Stephen_(self); },
          R"pbdoc(
:sig=(self: Stephen) -> Stephen:

This function returns a copy of a :any:`Stephen` object.

:returns: A copy.
:rtype: Stephen
)pbdoc");
      thing.def("accept_state",
                &Stephen_::accept_state,
                R"pbdoc(
:sig=(self: Stephen) -> int:

This function gets the accept state of the word graph. Running this function
triggers the algorithm implemented in this class (if it hasn't been triggered
already), and then returns the accept state of the produced word graph.

:returns: the node.
:rtype: int

:raises LibsemigroupsError:
  if no presentation was set at the construction or with
  :py:meth:`~Stephen.init` or if no word was set with
  :py:meth:`~Stephen.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");
      // The next one is complicated to use/doc so omitted.
      //       thing.def(
      //           "init",
      //           [](Stephen_& self) -> Stephen_& { return self.init(); },
      //           R"pbdoc(
      // :sig=(self: Stephen) -> Stephen:
      //
      // Reinitialize an existing Stephen object.
      //
      // This function puts a :any:`Stephen` object back into the same state as
      // if it had been newly constructed. If there are
      //
      // :returns: *self*.
      // :rtype: Stephen
      // )pbdoc");
      thing.def(
          "init",
          [](Stephen_& self, PresentationType const& p) -> Stephen_& {
            return self.init(p);
          },
          py::arg("p"),
          R"pbdoc(
:sig=(self: Stephen, p: Presentation | InversePresentation) -> Stephen:
Initialize from a presentation.

This function puts a :any:`Stephen` object back into the same state as if it
had been newly constructed from the presentation *p*.

:param p: the presentation.
:type p: Presentation | InversePresentation

:returns: *self*.
:rtype: Stephen
)pbdoc");
      thing.def("is_word_set",
                &Stephen_::is_word_set,
                R"pbdoc(
:sig=(self: Stephen) -> bool:
Check if the initial word is set.

Returns ``True`` if a word has been set with :py:meth:`~Stephen.set_word` since
the last presentation change and ``False`` otherwise.

:returns: A bool.
:rtype: bool
)pbdoc");
      thing.def(
          "__imul__",
          [](Stephen_& self, Stephen_& other) -> Stephen_& {
            self *= other;
            return self;
          },
          py::arg("other"),
          R"pbdoc(
Append a Stephen object.

This function appends the :any:`Stephen` object *other* to *self* . This
modifies the current :any:`Stephen` instance in-place. The result is a
:any:`Stephen` instance with underlying word equal to the concatenation of
``this.word()`` and ``other.word()``.The advantage of this is other if either
*self* or *other* have already been (partially) run, then we can reuse the
underlying word graphs instead of having to recompute them completely from
scratch.

:param other: the Stephen instance to append.
:type other: Stephen

:returns: *self*.
:rtype: Stephen

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  *self* or *other* or with :py:meth:`~Stephen.init` or if no word was set with
  :py:meth:`~Stephen.set_word`.

:raises LibsemigroupsError:
  if the presentations for *self* and *other* differ.
)pbdoc");
      thing.def(
          "__eq__",
          [](Stephen_& self, Stephen_& other) { return self == other; },
          py::arg("other"));
      thing.def("presentation",
                &Stephen_::presentation,
                R"pbdoc(
:sig=(self: Stephen) -> Presentation | InversePresentation:
Get the input presentation.

:returns: A presentation.
:rtype: Presentation | InversePresentation
)pbdoc");
      // TODO(2): Change to support std::string once we have that implemented
      // in libsemigroups itself
      thing.def(
          "set_word",
          [](Stephen_& self, word_type const& word) -> Stephen_& {
            return stephen::set_word(self, word);
          },
          py::arg("word"),
          R"pbdoc(
:sig=(self: Stephen, word: list[int]) -> Stephen:

Set the initial word.

This function sets the word whose left factors, or equivalent words, are sought.

:param word: the word to be set.
:type word: list[int]

:returns: *self*.
:rtype: Stephen

:raises LibsemigroupsError:
  if any of the values in *word* are out of range, i.e. they do not belong to
  the alphabet of :any:`Stephen.presentation`.
)pbdoc");
      thing.def("word",
                &Stephen_::word,
                R"pbdoc(
:sig=(self: Stephen) -> list[int]:

Get the initial word.

Returns the word set by :py:meth:`~Stephen.set_word`.

:returns: A  word.
:rtype: list[int]

:raises LibsemigroupsError:
  if no presentation was set at the construction or
  with :py:meth:`~Stephen.init` or if no word was set with
  :py:meth:`~Stephen.set_word`.
)pbdoc");
      thing.def("word_graph",
                &Stephen_::word_graph,
                py::return_value_policy::reference_internal,
                R"pbdoc(
:sig=(self: Stephen) -> WordGraph:

Get the word graph.

This function returns the word graph in its present state. The algorithm
implemented in this class is not triggered by calls to this function.

:returns: A :any:`WordGraph`.
:rtype: WordGraph

:raises LibsemigroupsError:
  if no presentation was set at the construction or
  with :py:meth:`~Stephen.init` or if no word was set with
  :py:meth:`~Stephen.set_word`.
)pbdoc");

      thing.def_static("initial_state",
                       &Stephen_::initial_state,
                       R"pbdoc(
:sig=(self: Stephen) -> int:

Get the initial state of the word graph.

:returns: the node.
:rtype: int
)pbdoc");

      // Helpers
      m.def("stephen_accepts",
            &stephen::accepts<PresentationType>,
            py::arg("s"),
            py::arg("w"),
            R"pbdoc(
:sig=(s: Stephen, w: list[int]) -> bool:
:only-document-once:
Check if a word is accepted by a :any:`Stephen` instance.

This function triggers the algorithm implemented in this class (if it hasn't
been triggered already), and then returns ``True`` if the input word *w*
labels a path in :any:`Stephen.word_graph` with source ``0`` and target
:any:`Stephen.accept_state`.

For a :any:`Stephen` instance constructed from a :any:`Presentation`, a word
*w* is accepted if and only if *w* is equivalent to :any:`Stephen.word` in
the semigroup defined by :any:`Stephen.presentation`.

For a :any:`Stephen` instance constructed from a :any:`InversePresentation`, a
word *w* is accepted if and only if :math:`uu^{-1}w` is equivalent to
:math:`u` in the semigroup defined by :any:`Stephen.presentation`, where
:math:`u` is the value of :any:`Stephen.word`.

:param s: the Stephen instance
:type s: Stephen

:param w: the input word.
:type w: list[int]

:returns: A ``bool``.
:rtype: bool

:raises LibsemigroupsError:
  if no presentation was set at the construction of *s* or with
  :any:`Stephen.init` or if no word was set with :any:`Stephen.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");

      m.def("stephen_dot",
            &stephen::dot<PresentationType>,
            py::arg("s"),
            R"pbdoc(
:sig=(s: Stephen) -> Dot:
:only-document-once:

Return a :any:`Dot` object representing the underlying word graph of the
:any:`Stephen` object *s*.

:param s: the Stephen object.
:type s: Stephen

:returns: A :any:`Dot` object.
:rtype: Dot
)pbdoc");

      m.def("stephen_is_left_factor",
            &stephen::is_left_factor<PresentationType>,
            py::arg("s"),
            py::arg("w"),
            R"pbdoc(
:sig=(s: Stephen, w: list[int]) -> bool:
:only-document-once:
Check if a word is a left factor of :any:`Stephen.word`.

This function triggers the algorithm implemented in this class (if it hasn't
been triggered already), and then returns ``True`` if the input word *w* is a
left factor of :any:`Stephen.word` in the semigroup defined by
:any:`Stephen.presentation` . A word is a left factor of :any:`Stephen.word` if
it labels a path in :any:`Stephen.word_graph` with source ``0``.

:param s: the Stephen instance.
:type s: Stephen

:param w: the input word.
:type w: list[int]

:returns: A ``bool``.
:rtype: bool

:raises LibsemigroupsError:
  if no presentation was set at the construction of *s* or with
  :any:`Stephen.init` or if no word was set with
  :any:`Stephen.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");

      m.def("stephen_left_factors",
            &stephen::left_factors<PresentationType>,
            py::arg("s"),
            R"pbdoc(
:sig=(s: Stephen) -> Paths:
:only-document-once:
Returns a :any:`Paths` object containing all the words (in short-lex order) that are left factors of :any:`Stephen.word`.

This function triggers the algorithm implemented in this class (if it hasn't been triggered already).

:param s: the Stephen instance
:type s: Stephen

:returns: A :any:`Paths` object containing all the words (in short-lex order) that are left factors of :any:`Stephen.word`.
:rtype: Paths

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  *s* or with :any:`Stephen.init` or if no word
  was set with :any:`Stephen.set_word`.

.. warning::
    Termination of the Stephen algorithm is undecidable in general, and
    this function may never terminate.
)pbdoc");

      m.def(
          "stephen_number_of_left_factors",
          [](Stephen_& s, size_t min, size_t max) {
            return from_int(stephen::number_of_left_factors(s, min, max));
          },
          py::arg("s"),
          py::arg("min") = 0,
          py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
          R"pbdoc(
:sig=(s: Stephen, min: int = 0, max: int | PositiveInfinity = POSITIVE_INFINITY) -> int | PositiveInfinity:
:only-document-once:
Returns the number of left factors with length in a given range.

This function returns the number of left factors of the :any:`Stephen.word` in
the instance *s* with length between *min* and *max*. This is the same as
the number of paths in :any:`Stephen.word_graph` (if the inherited
:any:`Runner.run` method of *s* has been called) with source ``0`` and length
in the range *min* to *max*.

:param s: the Stephen instance.
:type s: Stephen

:param min: the minimum length of a word (default: 0).
:type min: int

:param max:
    one more than the maximum length of a word (default:
    :any:`POSITIVE_INFINITY`).
:type max: int | PositiveInfinity

:returns: An ``int`` or :any:`POSITIVE_INFINITY`.
:rtype: int | PositiveInfinity

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  *s* or with :any:`Stephen.init` or if no word
  was set with :any:`Stephen.set_word`.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
)pbdoc");

      m.def(
          "stephen_number_of_words_accepted",
          [](Stephen_& s, size_t min, size_t max) {
            return from_int(stephen::number_of_words_accepted(s, min, max));
          },
          py::arg("s"),
          py::arg("min") = 0,
          py::arg("max") = static_cast<size_t>(POSITIVE_INFINITY),
          R"pbdoc(
:sig=(s: Stephen, min: int = 0, max: int | PositiveInfinity = POSITIVE_INFINITY) -> int | PositiveInfinity:
:only-document-once:

Returns the number of words accepted with length in a given range.

This function triggers the algorithm implemented in this class (if it hasn't
been triggered already) and then returns the the number of paths in
:any:`Stephen.word_graph` with source ``0``, target
:any:`Stephen.accept_state`, and length in the range *min* to *max*.

For a :any:`Stephen` instance constructed from a :any:`Presentation`
this is the same as the number of words that are equivalent to
:any:`Stephen.word` with length between *min* and *max*.

For a :any:`Stephen` instance constructed from a :any:`InversePresentation`,
this is the same as the number of words *w* such that :math:`uu^{-1}w` is
equivalent to :math:`u` in the semigroup defined by
:any:`Stephen.presentation`, where :math:`u` is the value of
:any:`Stephen.word`.

:param s: the Stephen instance.
:type s: Stephen

:param min: the minimum length of a word (default: ``0``).
:type min: int

:param max:
  one more than the maximum length of a word (default:
  :any:`POSITIVE_INFINITY`).
:type max: int | PositiveInfinity

:returns: The number of words accepted.
:rtype: int | PositiveInfinity

:raises LibsemigroupsError:
  if no presentation was set at the construction of *s* or with
  :any:`Stephen.init` or if no word was set with
  :any:`Stephen.set_word`.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
)pbdoc");

      m.def("stephen_words_accepted",
            &stephen::words_accepted<PresentationType>,
            py::arg("s"),
            R"pbdoc(
:sig=(s: Stephen) -> Paths:
:only-document-once:

Returns a :any:`Paths` object containing all words accepted by a Stephen instance in short-lex order.

This function triggers the algorithm implemented in this class (if it hasn't been triggered already).

:param s: the Stephen instance
:type s: Stephen

:returns: A :any:`Paths` object containing all words equivalent to :any:`Stephen.word` in short-lex order.
:rtype: Paths

:raises LibsemigroupsError:
  if no presentation was set at the construction of
  *s* or with :any:`Stephen.init` or if no word
  was set with :any:`Stephen.set_word`.

.. warning::
  Termination of the Stephen algorithm is undecidable in general, and
  this function may never terminate.
)pbdoc");
    }

  }  // namespace
  void init_stephen(py::module& m) {
    // TODO(2): figure out how to handle std::string Stephens once that's
    // supported
    bind_stephen<Presentation<word_type>>(m, "StephenPresentationWord");
    bind_stephen<InversePresentation<word_type>>(
        m, "StephenInversePresentationWord");
  }

}  // namespace libsemigroups
