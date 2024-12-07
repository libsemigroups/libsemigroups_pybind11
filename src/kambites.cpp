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
#include <libsemigroups/kambites.hpp>
#include <libsemigroups/to-froidure-pin.hpp>

// pybind11....
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
#include <pybind11/pybind11.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_kambites

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename OtherWord, typename Word>
    void constructor(py::class_<Kambites<Word>>& thing) {
      thing.def(py::init<congruence_kind, Presentation<OtherWord> const&>(),
                py::arg("knd"),
                py::arg("p"),
                R"pbdoc(
:sig=(knd: congruence_kind, p: PresentationStrings) -> None:
:only-document-once:

Construct from :any:`congruence_kind` and :any:`PresentationStrings`.

This function constructs a :any:`Kambites` instance representing a congruence
of kind *knd* over the semigroup or monoid defined by the presentation *p*.
:any:`Kambites` instances can only be used to compute two-sided congruences,
and so the first parameter *knd* must always be
``congruence_kind.twosided``. The parameter *knd* is included for
uniformity of interface between with :any:`KnuthBendix`, :any:`Kambites`, and
:any:`Congruence`.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param p: the presentation.
:type p: PresentationStrings

:raises LibsemigroupsError: if *p* is not valid.

:raises LibsemigroupsError:
  if *knd* is not ``congruence_kind.twosided``.)pbdoc");
    }

    template <typename OtherWord, typename Word>
    void add_generating_pair(py::class_<Kambites<Word>>& thing) {
      thing.def(
          "add_generating_pair",
          [](Kambites<Word>&  self,
             OtherWord const& u,
             OtherWord const& v) -> Kambites<Word>& {
            return kambites::add_generating_pair(self, u, v);
          },
          py::arg("u"),
          py::arg("v"),
          R"pbdoc(
:sig=(self: Kambites, u: List[int] | str, v: List[int] | str) -> Kambites:
:only-document-once:

Add a generating pair.

This function adds a generating pair to the congruence represented by a :any:`Kambites` instance.

:param u: the first item in the pair.
:type u: List[int] | str

:param v: the second item in the pair.
:type v: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.

:raises LibsemigroupsError:  if :any:`Runner.started` returns ``True``.

:returns: ``self``.
:rtype: Kambites
)pbdoc");
    }

    template <typename OtherWord, typename Word>
    void contains(py::class_<Kambites<Word>>& thing) {
      thing.def(
          "contains",
          [](Kambites<Word>& self, OtherWord const& u, OtherWord const& v) {
            return kambites::contains(self, u, v);
          },
          py::arg("u"),
          py::arg("v"),
          R"pbdoc(
:sig=(self: Kambites, u: List[int] | str, v: List[int] | str) -> bool:
:only-document-once:

Check containment of a pair of words.

This function checks whether or not the words *u* and *v* are contained in the
congruence represented by a :any:`Kambites` instance.

:param u: the first item in the pair.
:type u: List[int] | str

:param v: the second item in the pair.
:type v: List[int] | str

:returns: Whether or not the pair belongs to the congruence.
:rtype: bool

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.

:raises LibsemigroupsError:
    if :any:`small_overlap_class` is not at least :math:`4`.
)pbdoc");
    }

    template <typename OtherWord, typename Word>
    void currently_contains(py::class_<Kambites<Word>>& thing) {
      thing.def(
          "currently_contains",
          [](Kambites<Word>& self, OtherWord const& u, OtherWord const& v) {
            return kambites::currently_contains(self, u, v);
          },
          py::arg("u"),
          py::arg("v"),
          R"pbdoc(
:sig=(self: Kambites, u: List[int] | str, v: List[int] | str) -> bool:
:only-document-once:

Check whether a pair of words is already known to belong to the congruence.

This function checks whether or not the words *u* and *v* are already known to
be contained in the congruence represented by a :any:`Kambites` instance.
This function performs no enumeration, so it is possible for the words to be
contained in the congruence, but that this is not currently known.

:param u: the first item in the pair.
:type u: List[int] | str

:param v: the second item in the pair.
:type v: List[int] | str

:returns:
    *  :any:`tril.true` if the words are known to belong to the congruence;
    *  :any:`tril.false` if the words are known to not belong to the congruence;
    *  :any:`tril.unknown` otherwise.
:rtype: tril

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.

:raises LibsemigroupsError:
    if :any:`small_overlap_class` is not at least :math:`4`.
)pbdoc");
    }

    template <typename OtherWord, typename Word>
    void my_init(py::class_<Kambites<Word>>& thing) {
      thing.def(
          "init",
          [](Kambites<Word>&                self,
             congruence_kind                knd,
             Presentation<OtherWord> const& p) { return self.init(knd, p); },
          py::arg("knd"),
          py::arg("p"),
          // TODO(0) adding only-document-once here means that the other
          // overload of init is suppressed also :(
          // :only-document-once:
          R"pbdoc(
:sig=(self: Kambites, knd: congruence_kind, p: Presentation) -> Kambites:

Re-initialize a :any:`Kambites` instance.

This function re-initializes a :any:`Kambites` instance as if it
had been newly constructed from *knd* and *p*.

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: :any:`congruence_kind`

:param p: the presentation.
:type p: PresentationStrings

:returns:  ``self``.
:rtype: Kambites

:raises LibsemigroupsError: if *p* is not valid.
:raises LibsemigroupsError: if *knd* is ``congruence_kind.onesided``.
)pbdoc");
    }

    template <typename OtherWord, typename Word>
    void reduce(py::class_<Kambites<Word>>& thing) {
      thing.def(
          "reduce",
          [](Kambites<Word>& self, OtherWord const& w) {
            return kambites::reduce(self, w);
          },
          py::arg("w"),
          R"pbdoc(
:sig=(self: Kambites, w: List[int] | str) -> List[int] | str:
:only-document-once:

Reduce a word.

This function computes the small overlap class of the :any:`presentation` and
then returns the lexicographically least word equivalent to the input word *w*.
Note that in a small overlap monoid, every congruence class is finite, and so
this lexicographically least word always exists.

:param w: the input word.
:type w: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word` raises.
)pbdoc");
    }

    template <typename OtherWord, typename Word>
    void reduce_no_run(py::class_<Kambites<Word>>& thing) {
      thing.def(
          "reduce_no_run",
          [](Kambites<Word>& self, word_type const& w) {
            return kambites::reduce_no_run(self, w);
          },
          py::arg("w"),
          R"pbdoc(
:sig=(self: Kambites, w: List[int] | str) -> List[int] | str:
:only-document-once:

Reduce a word.

If :any:`Runner.finished` returns ``True``, then returns the lexicographically
least word equivalent to the input word *w*. If :any:`Runner.finished` returns
``False``, then the input word *w* is returned.

:param w: the input word.
:type w: List[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`PresentationStrings.validate_word`
  raises.)pbdoc");
    }

    template <typename Word>
    void bind_kambites(py::module& m, std::string const& name) {
      using Kambites_        = Kambites<Word>;
      using native_word_type = typename Kambites_::native_word_type;

      py::class_<Kambites_> thing(m,
                                  name.c_str(),
                                  R"pbdoc(
Class template implementing small overlap class, equality, and normal forms for
small overlap monoids.

This page describes the class :any:`Kambites` for determining the
small overlap class of a presentation, and, for small overlap monoids (those
with small overlap class 4 or higher) checking equality of words and for
computing normal forms. Note that a :any:`Kambites` instance represents a
congruence on the free monoid or semigroup containing the rules of a
presentation used to construct the instance, and the :any:`generating_pairs`.
As such generating pairs or rules are interchangeable in the context of
:any:`Kambites` objects.)pbdoc");

      thing.def("__repr__",
                [](Kambites_& thing) { return to_human_readable_repr(thing); });
      thing.def(py::init<>(), R"pbdoc(
Default constructor.

This function default constructs an uninitialised :any:`Kambites` instance.
)pbdoc");

      constructor<word_type>(thing);
      constructor<native_word_type>(thing);

      thing.def(
          "copy",
          [](Kambites_ const& self) { return Kambites_(self); },
          R"pbdoc(
Copy a :any:`Kambites` object.

:returns: A copy.
:rtype: Kambites
    )pbdoc");
      thing.def(
          "__copy__",
          [](Kambites_ const& self) { return Kambites_(self); },
          R"pbdoc(
Copy a :any:`Kambites` object.

:returns: A copy.
:rtype: Kambites
    )pbdoc");

      thing.def(
          "init",
          [](Kambites_& self) { return self.init(); },
          R"pbdoc(
Re-initialize a :any:`Kambites` instance.

This function puts a :any:`Kambites` instance back into the state that it would
have been in if it had just been newly default constructed.

:returns:
  ``self``.
:rtype:
  Kambites
      )pbdoc");

      add_generating_pair<word_type>(thing);
      add_generating_pair<std::string>(thing);

      contains<word_type>(thing);
      contains<std::string>(thing);

      currently_contains<word_type>(thing);
      currently_contains<std::string>(thing);

      my_init<word_type>(thing);
      my_init<std::string>(thing);

      reduce<word_type>(thing);
      reduce<std::string>(thing);

      reduce_no_run<word_type>(thing);
      reduce_no_run<std::string>(thing);

      thing.def("number_of_classes", &Kambites_::number_of_classes, R"pbdoc(
Compute the number of classes in the congruence.

This function computes the number of classes in the congruence represented by a
:any:`Kambites` instance if the :any:`small_overlap_class` is at least
:math:`4`. :any:`Kambites` instances can only compute the number of classes if
the condition of the previous sentence is fulfilled, and in this case the
number of classes is always :any:`POSITIVE_INFINITY`. Otherwise an exception
is raised.

:raises LibsemigroupsError:
    if it is not possible to compute the number of classes of the
    congruence because the small overlap class is too small.

:returns:
    The number of congruences classes of a :any:`Kambites` if
    :any:`small_overlap_class` is at least :math:`4`.

:rtype:
    int
)pbdoc");

      thing.def("presentation",
                &Kambites_::presentation,
                R"pbdoc(
Get the presentation used to define a :any:`Kambites` instance (if any). If a
any:`Kambites` instance is constructed or initialised using a presentation,
then the version of this presentation used by the :any:`Kambites` object is
returned by this function.

:returns:
    The presentation.
:rtype:
    PresentationStrings
      )pbdoc");

      thing.def("small_overlap_class",
                &Kambites_::small_overlap_class,
                R"pbdoc(
:sig=(self: Kambites) -> int | PositiveInfinity:

Get the small overlap class.

If :math:`S` is a finitely presented semigroup or monoid with generating set
:math:`A`, then a word :math:`w` over :math:`A` is a *piece* if
:math:`w` occurs as a factor in at least two of the relations defining
:math:`S` or if it occurs as a factor of one relation in two different
positions (possibly overlapping). A finitely presented semigroup
:math:`S` satisfies the condition :math:`C(n)`, for a positive integer
:math:`n` if the minimum number of pieces in any factorisation of a word
occurring as the left or right hand side of a relation of :math:`S` is
at least :math:`n`.

:complexity:
  The current implementation has complexity no worse than :math:`O(m ^ 3)`
  where :math:`m` is the sum of the lengths of the words occurring in the
  relations of the semigroup.

:returns:
  The greatest positive integer :math:`n` such that the finitely
  semigroup or monoid represented by ``self`` satisfies the condition
  :math:`C(n)` ; or :any:`POSITIVE_INFINITY` if no word occurring in a
  relation can be written as a product of pieces.
:rtype: int | PositiveInfinity
      )pbdoc");

      thing.def("ukkonen",
                &Kambites_::ukkonen,
                R"pbdoc(
Returns the generalised suffix tree used to compute pieces.

This function returns the generalised suffix tree :any:`Ukkonen` object
containing the relation words of a :any:`Kambites` object, that is used to
determine the pieces, and decompositions of the relation words.

:returns:
    The generalised suffix tree containing the relation words.
:rtype:
    Ukkonen
)pbdoc");

      m.def(
          "kambites_normal_forms",
          [](Kambites_& k) {
            auto nf = kambites::normal_forms(k);
            return py::make_iterator(rx::begin(nf), rx::end(nf));
          },
          py::arg("k"),
          R"pbdoc(
:sig=(k: Kambites) -> Iterator[List[int]]:

Returns an iterator yielding normal forms.

This function returns an iterator yielding short-lex normal forms of the
classes of the congruence represented by an instance of :any:`Kambites`.

:param k: the :any:`Kambites` instance.
:type k: Kambites

:returns: An iterator yielding normal forms.
:rtype: Iterator[List[int]]
)pbdoc");

      m.def(
          "kambites_non_trivial_classes",
          [](Kambites_& k, std::vector<word_type> const& words) {
            return kambites::non_trivial_classes(
                k, rx::iterator_range(words.begin(), words.end()));
          },
          py::arg("k"),
          py::arg("words"),
          R"pbdoc(
:sig=(k: Kambites, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
:only-document-once:

Find the non-trivial classes in the partition of a list of words.

This function returns the classes with size at least :math:`2` in the partition
of the words in the list *words* induced by the :any:`Kambites` instance *k*.

:param k: the :any:`Kambites` instance.
:type k: Kambites

:param words: the input list of words.
:type r: List[List[int] | str]

:returns: The partition of the input list.
:rtype: List[List[List[int]] | List[str]]

:raises LibsemigroupsError:
    if ``k.small_overlap_class()`` is not at least :math:`4`.
)pbdoc");

      m.def(
          "kambites_non_trivial_classes",
          [](Kambites_& k, std::vector<std::string> const& words) {
            return kambites::non_trivial_classes(
                k, rx::iterator_range(words.begin(), words.end()));
          },
          py::arg("k"),
          py::arg("words"),
          R"pbdoc(
:sig=(k: Kambites, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
)pbdoc");

      m.def(
          "kambites_partition",
          [](Kambites_& k, std::vector<word_type> const& words) {
            return kambites::partition(
                k, rx::iterator_range(words.begin(), words.end()));
          },
          py::arg("k"),
          py::arg("words"),
          R"pbdoc(
:sig=(k: Kambites, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:
:only-document-once:

Partition a list of words.

This function returns the classes in the partition of the words in the input
list *words* induced by the :any:`Kambites` instance *k* . This function
triggers a full enumeration of *k*.

:param k: the :any:`Kambites` instance.
:type k: Kambites

:param words: the input list of words.
:type words: List[List[int] | str]

:returns: The partitioned list of words.
:rtype: List[List[List[int]] | List[str]]

:raises LibsemigroupsError:
   if ``k.small_overlap_class()`` is not at least :math:`4`.
)pbdoc");

      m.def(
          "kambites_partition",
          [](Kambites_& k, std::vector<std::string> const& words) {
            return kambites::partition(
                k, rx::iterator_range(words.begin(), words.end()));
          },
          py::arg("k"),
          py::arg("words"),
          R"pbdoc(
:sig=(k: Kambites, words: List[List[int] | str]) -> List[List[List[int]] | List[str]]:

)pbdoc");
    }  // bind_kambites
  }  // namespace

  void init_kambites(py::module& m) {
    // One call to bind is required per list of types
    bind_kambites<detail::MultiStringView>(m, "Kambites");
  }

}  // namespace libsemigroups
