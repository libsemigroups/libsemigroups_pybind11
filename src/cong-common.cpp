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

#include "cong-common.hpp"  // for doc

#include <string_view>

// libsemigroups headers
#include <libsemigroups/cong.hpp>
#include <libsemigroups/kambites.hpp>
#include <libsemigroups/knuth-bendix.hpp>
#include <libsemigroups/todd-coxeter.hpp>
#include <libsemigroups/types.hpp>

#include <libsemigroups/detail/cong-common-class.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "constants.hpp"
#include "main.hpp"  // for init_detail_cong_common

namespace libsemigroups {
  namespace py = pybind11;

  using RewriteTrie     = detail::RewriteTrie;
  using RewriteFromLeft = detail::RewriteFromLeft;

  template <typename Thing>
  using MultiView = detail::MultiView<Thing>;

  // Alias required because macros don't play well with commas.
  using KnuthBendixStringRewriteTrie = KnuthBendix<std::string, RewriteTrie>;
  using KnuthBendixStringRewriteFromLeft
      = KnuthBendix<std::string, RewriteFromLeft>;
  using KnuthBendixWordRewriteTrie = KnuthBendix<word_type, RewriteTrie>;
  using KnuthBendixWordRewriteFromLeft
      = KnuthBendix<word_type, RewriteFromLeft>;

  ////////////////////////////////////////////////////////////////////////
  // Implementation helpers
  ////////////////////////////////////////////////////////////////////////

  namespace {
    using std::literals::string_view_literals::operator""sv;

    // 100% thread unsafe!!
    char const* make_doc(char const*      fmt_string,
                         std::string_view name,
                         doc              extra_doc) {
      using fmt::literals::operator""_a;

      static auto const only_doc_once = ":only-document-once:"sv;

      static std::string result;
      result.clear();

      fmt::format_to(std::back_inserter(result),
                     fmt_string,
                     "detail"_a           = extra_doc.detail,
                     "example"_a          = extra_doc.example,
                     "extra_kwargs"_a     = extra_doc.extra_kwargs,
                     "extra_kwargs_doc"_a = extra_doc.extra_kwargs_doc,
                     "name"_a             = name,
                     "only_document_once"_a
                     = extra_doc.only_document_once ? only_doc_once : "",
                     "raises"_a = extra_doc.raises,
                     "var"_a    = extra_doc.var);
      return result.c_str();
    }
  }  // namespace

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_construct_default(py::class_<Thing, ThingBase>& thing,
                             std::string_view              name,
                             doc                           extra_doc) {
    thing.def(py::init<>(),
              make_doc(R"pbdoc(
:sig=(word: type{extra_kwargs}) -> None:
{only_document_once}

Default constructor.

This function default constructs an uninitialised :any:`{name}` instance.

:Keyword Arguments:
    * **word** (*type*) -- the type of words to use, must be either ``str`` or ``list[int]``{extra_kwargs_doc}

{detail}
)pbdoc",
                       name,
                       extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_CONSTRUCT_DEFAULT(A, B)          \
  template void def_construct_default<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  DEF_CONSTRUCT_DEFAULT(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_CONSTRUCT_DEFAULT(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_CONSTRUCT_DEFAULT(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_CONSTRUCT_DEFAULT(Kambites<word_type>, detail::CongruenceCommon);
  DEF_CONSTRUCT_DEFAULT(Kambites<MultiView<std::string>>,
                        detail::CongruenceCommon);
  DEF_CONSTRUCT_DEFAULT(Kambites<std::string>, detail::CongruenceCommon);

  DEF_CONSTRUCT_DEFAULT(detail::KnuthBendixImpl<RewriteTrie>,
                        detail::CongruenceCommon);
  DEF_CONSTRUCT_DEFAULT(detail::KnuthBendixImpl<RewriteFromLeft>,
                        detail::CongruenceCommon);

  DEF_CONSTRUCT_DEFAULT(KnuthBendixStringRewriteTrie,
                        detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CONSTRUCT_DEFAULT(KnuthBendixStringRewriteFromLeft,
                        detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_CONSTRUCT_DEFAULT(KnuthBendixWordRewriteTrie,
                        detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CONSTRUCT_DEFAULT(KnuthBendixWordRewriteFromLeft,
                        detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_CONSTRUCT_DEFAULT(Congruence<word_type>, detail::CongruenceCommon);
  DEF_CONSTRUCT_DEFAULT(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_init_default(py::class_<Thing, ThingBase>& thing,
                        std::string_view              name,
                        doc                           extra_doc) {
    thing.def(
        "init",
        [](Thing& self) -> Thing& { return self.init(); },
        make_doc(R"pbdoc(
:sig=(self: {name}) -> {name}:
{only_document_once}

Re-initialize a :any:`{name}` instance.

This function puts a :any:`{name}` instance back into the state that it would
have been in if it had just been newly default constructed.

{detail}

:returns:
  *self*.
:rtype:
    {name})pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_INIT_DEFAULT(A, B) \
  template void def_init_default<A, B>(py::class_<A, B>&, std::string_view, doc)

  DEF_INIT_DEFAULT(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_INIT_DEFAULT(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_INIT_DEFAULT(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_INIT_DEFAULT(Kambites<word_type>, detail::CongruenceCommon);
  DEF_INIT_DEFAULT(Kambites<MultiView<std::string>>, detail::CongruenceCommon);
  DEF_INIT_DEFAULT(Kambites<std::string>, detail::CongruenceCommon);

  DEF_INIT_DEFAULT(detail::KnuthBendixImpl<RewriteTrie>,
                   detail::CongruenceCommon);
  DEF_INIT_DEFAULT(detail::KnuthBendixImpl<RewriteFromLeft>,
                   detail::CongruenceCommon);

  DEF_INIT_DEFAULT(KnuthBendixStringRewriteTrie,
                   detail::KnuthBendixImpl<RewriteTrie>);
  DEF_INIT_DEFAULT(KnuthBendixStringRewriteFromLeft,
                   detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_INIT_DEFAULT(KnuthBendixWordRewriteTrie,
                   detail::KnuthBendixImpl<RewriteTrie>);
  DEF_INIT_DEFAULT(KnuthBendixWordRewriteFromLeft,
                   detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_INIT_DEFAULT(Congruence<word_type>, detail::CongruenceCommon);
  DEF_INIT_DEFAULT(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_construct_kind_presentation(py::class_<Thing, ThingBase>& thing,
                                       std::string_view              name,
                                       doc extra_doc) {
    using Word = typename Thing::native_word_type;
    thing.def(py::init<congruence_kind, Presentation<Word> const&>(),
              py::arg("knd"),
              py::arg("p"),
              make_doc(R"pbdoc(
:sig=(self: {name}, knd: congruence_kind, p: Presentation) -> None:
{only_document_once}

Construct from :any:`congruence_kind` and :any:`Presentation`.

This function constructs a :any:`{name}` instance representing a congruence
of kind *knd* over the semigroup or monoid defined by the presentation *p*.

{detail}

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: congruence_kind

:param p: the presentation.
:type p: Presentation

{raises}

:raises LibsemigroupsError: if *p* is not valid.
)pbdoc",
                       name,
                       extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_CONSTRUCT_KIND_PRESENTATION(A, B)          \
  template void def_construct_kind_presentation<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  DEF_CONSTRUCT_KIND_PRESENTATION(detail::ToddCoxeterImpl,
                                  detail::CongruenceCommon);

  DEF_CONSTRUCT_KIND_PRESENTATION(ToddCoxeter<word_type>,
                                  detail::ToddCoxeterImpl);
  DEF_CONSTRUCT_KIND_PRESENTATION(ToddCoxeter<std::string>,
                                  detail::ToddCoxeterImpl);

  DEF_CONSTRUCT_KIND_PRESENTATION(Kambites<word_type>,
                                  detail::CongruenceCommon);
  DEF_CONSTRUCT_KIND_PRESENTATION(Kambites<MultiView<std::string>>,
                                  detail::CongruenceCommon);
  DEF_CONSTRUCT_KIND_PRESENTATION(Kambites<std::string>,
                                  detail::CongruenceCommon);

  DEF_CONSTRUCT_KIND_PRESENTATION(detail::KnuthBendixImpl<RewriteTrie>,
                                  detail::CongruenceCommon);
  DEF_CONSTRUCT_KIND_PRESENTATION(detail::KnuthBendixImpl<RewriteFromLeft>,
                                  detail::CongruenceCommon);

  DEF_CONSTRUCT_KIND_PRESENTATION(KnuthBendixStringRewriteTrie,
                                  detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CONSTRUCT_KIND_PRESENTATION(KnuthBendixStringRewriteFromLeft,
                                  detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_CONSTRUCT_KIND_PRESENTATION(KnuthBendixWordRewriteTrie,
                                  detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CONSTRUCT_KIND_PRESENTATION(KnuthBendixWordRewriteFromLeft,
                                  detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_CONSTRUCT_KIND_PRESENTATION(Congruence<word_type>,
                                  detail::CongruenceCommon);
  DEF_CONSTRUCT_KIND_PRESENTATION(Congruence<std::string>,
                                  detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_init_kind_presentation(py::class_<Thing, ThingBase>& thing,
                                  std::string_view              name,
                                  doc                           extra_doc) {
    using Word = typename Thing::native_word_type;
    thing.def(
        "init",
        [](Thing&                    self,
           congruence_kind           knd,
           Presentation<Word> const& p) -> Thing& { return self.init(knd, p); },
        py::arg("knd"),
        py::arg("p"),
        make_doc(R"pbdoc(
:sig=(self: {name}, knd: congruence_kind, p: Presentation) -> {name}:
{only_document_once}

Re-initialize a :any:`{name}` instance.

This function re-initializes a :any:`{name}` instance as if it
had been newly constructed from *knd* and *p*.

{detail}

:param knd: the kind (onesided or twosided) of the congruence.
:type knd: :any:`congruence_kind`

:param p: the presentation.
:type p: Presentation

:returns:  *self*.
:rtype: {name}

:raises LibsemigroupsError: if *p* is not valid.
{raises}
)pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_INIT_KIND_PRESENTATION(A, B)          \
  template void def_init_kind_presentation<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  DEF_INIT_KIND_PRESENTATION(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_INIT_KIND_PRESENTATION(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_INIT_KIND_PRESENTATION(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_INIT_KIND_PRESENTATION(Kambites<word_type>, detail::CongruenceCommon);
  DEF_INIT_KIND_PRESENTATION(Kambites<MultiView<std::string>>,
                             detail::CongruenceCommon);
  DEF_INIT_KIND_PRESENTATION(Kambites<std::string>, detail::CongruenceCommon);

  DEF_INIT_KIND_PRESENTATION(detail::KnuthBendixImpl<RewriteTrie>,
                             detail::CongruenceCommon);
  DEF_INIT_KIND_PRESENTATION(detail::KnuthBendixImpl<RewriteFromLeft>,
                             detail::CongruenceCommon);

  DEF_INIT_KIND_PRESENTATION(KnuthBendixStringRewriteTrie,
                             detail::KnuthBendixImpl<RewriteTrie>);
  DEF_INIT_KIND_PRESENTATION(KnuthBendixStringRewriteFromLeft,
                             detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_INIT_KIND_PRESENTATION(KnuthBendixWordRewriteTrie,
                             detail::KnuthBendixImpl<RewriteTrie>);
  DEF_INIT_KIND_PRESENTATION(KnuthBendixWordRewriteFromLeft,
                             detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_INIT_KIND_PRESENTATION(Congruence<word_type>, detail::CongruenceCommon);
  DEF_INIT_KIND_PRESENTATION(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_copy(py::class_<Thing, ThingBase>& thing,
                std::string_view              name,
                doc                           extra_doc) {
    thing.def(
        "copy",
        [](Thing const& self) { return Thing(self); },
        make_doc(R"pbdoc(
:sig=(self: {name}) -> {name}:
{only_document_once}

Copy a :any:`{name}` object.

{detail}

:returns: A copy.
:rtype: {name})pbdoc",
                 name,
                 extra_doc));

    thing.def("__copy__", [](Thing const& self) { return Thing(self); });
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_COPY(A, B) \
  template void def_copy<A, B>(py::class_<A, B>&, std::string_view, doc)

  DEF_COPY(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_COPY(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_COPY(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_COPY(Kambites<word_type>, detail::CongruenceCommon);
  DEF_COPY(Kambites<MultiView<std::string>>, detail::CongruenceCommon);
  DEF_COPY(Kambites<std::string>, detail::CongruenceCommon);

  DEF_COPY(detail::KnuthBendixImpl<RewriteTrie>, detail::CongruenceCommon);
  DEF_COPY(detail::KnuthBendixImpl<RewriteFromLeft>, detail::CongruenceCommon);

  DEF_COPY(KnuthBendixStringRewriteTrie, detail::KnuthBendixImpl<RewriteTrie>);
  DEF_COPY(KnuthBendixStringRewriteFromLeft,
           detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_COPY(KnuthBendixWordRewriteTrie, detail::KnuthBendixImpl<RewriteTrie>);
  DEF_COPY(KnuthBendixWordRewriteFromLeft,
           detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_COPY(Congruence<word_type>, detail::CongruenceCommon);
  DEF_COPY(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_number_of_classes(py::class_<Thing, ThingBase>& thing,
                             std::string_view              name,
                             doc                           extra_doc) {
    thing.def(
        "number_of_classes",
        [](Thing& self) { return from_int(self.number_of_classes()); },
        make_doc(R"pbdoc(
:sig=(self: {name}) -> int | PositiveInfinity:
{only_document_once}

Compute the number of classes in the congruence. This function computes the
number of classes in the congruence represented by a :any:`{name}` instance.

{detail}

:returns:
   The number of congruence classes of a :any:`{name}` instance if this number
   is finite, or :any:`POSITIVE_INFINITY` in some cases if this number is not
   finite.
:rtype:
   int | PositiveInfinity

{raises}
)pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_NUMBER_OF_CLASSES(A, B)          \
  template void def_number_of_classes<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  // Not required for ToddCoxeter<>
  // Not required for KnuthBendix<>

  DEF_NUMBER_OF_CLASSES(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_NUMBER_OF_CLASSES(Kambites<word_type>, detail::CongruenceCommon);
  DEF_NUMBER_OF_CLASSES(Kambites<MultiView<std::string>>,
                        detail::CongruenceCommon);
  DEF_NUMBER_OF_CLASSES(Kambites<std::string>, detail::CongruenceCommon);

  DEF_NUMBER_OF_CLASSES(detail::KnuthBendixImpl<RewriteTrie>,
                        detail::CongruenceCommon);
  DEF_NUMBER_OF_CLASSES(detail::KnuthBendixImpl<RewriteFromLeft>,
                        detail::CongruenceCommon);

  DEF_NUMBER_OF_CLASSES(Congruence<word_type>, detail::CongruenceCommon);
  DEF_NUMBER_OF_CLASSES(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_add_generating_pair(py::class_<Thing, ThingBase>& thing,
                               std::string_view              name,
                               doc                           extra_doc) {
    using Word = typename Thing::native_word_type;
    thing.def(
        "add_generating_pair",
        [](Thing& self, Word const& u, Word const& v) -> Thing& {
          congruence_common::add_generating_pair(self, u, v);
          return self;
        },
        py::arg("u"),
        py::arg("v"),
        make_doc(
            R"pbdoc(
:sig=(self: {name}, u: list[int] | str, v: list[int] | str) -> {name}:
{only_document_once}

Add a generating pair.

This function adds a generating pair to the congruence represented by a
:any:`{name}` instance.

{detail}

:param u: the first item in the pair.
:type u: list[int] | str

:param v: the second item in the pair.
:type v: list[int] | str

:returns: *self*.
:rtype: {name}

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`Presentation.throw_if_letter_not_in_alphabet`
  raises.

:raises LibsemigroupsError:  if :any:`Runner.started` returns ``True``.

{raises}
)pbdoc",
            name,
            extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_ADD_GENERATING_PAIR(A, B)          \
  template void def_add_generating_pair<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  DEF_ADD_GENERATING_PAIR(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_ADD_GENERATING_PAIR(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_ADD_GENERATING_PAIR(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_ADD_GENERATING_PAIR(Kambites<word_type>, detail::CongruenceCommon);
  DEF_ADD_GENERATING_PAIR(Kambites<MultiView<std::string>>,
                          detail::CongruenceCommon);
  DEF_ADD_GENERATING_PAIR(Kambites<std::string>, detail::CongruenceCommon);

  DEF_ADD_GENERATING_PAIR(detail::KnuthBendixImpl<RewriteTrie>,
                          detail::CongruenceCommon);
  DEF_ADD_GENERATING_PAIR(detail::KnuthBendixImpl<RewriteFromLeft>,
                          detail::CongruenceCommon);

  DEF_ADD_GENERATING_PAIR(KnuthBendixStringRewriteTrie,
                          detail::KnuthBendixImpl<RewriteTrie>);
  DEF_ADD_GENERATING_PAIR(KnuthBendixStringRewriteFromLeft,
                          detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_ADD_GENERATING_PAIR(KnuthBendixWordRewriteTrie,
                          detail::KnuthBendixImpl<RewriteTrie>);
  DEF_ADD_GENERATING_PAIR(KnuthBendixWordRewriteFromLeft,
                          detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_ADD_GENERATING_PAIR(Congruence<word_type>, detail::CongruenceCommon);
  DEF_ADD_GENERATING_PAIR(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_currently_contains(py::class_<Thing, ThingBase>& thing,
                              std::string_view              name,
                              doc                           extra_doc) {
    using Word = typename Thing::native_word_type;
    thing.def(
        "currently_contains",
        [](Thing const& self, Word const& u, Word const& v) {
          return congruence_common::currently_contains(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        make_doc(R"pbdoc(
:sig=(self: {name}, u: list[int] | str, v: list[int] | str) -> tril:
{only_document_once}

Check whether a pair of words is already known to belong to the congruence.

This function checks whether or not the words *u* and *v* are already known to
be contained in the congruence represented by a :py:class:`{name}` instance.
This function performs no enumeration, so it is possible for the words to be
contained in the congruence, but that this is not currently known.

:param u: the first word.
:type u: list[int] | str

:param v: the second word.
:type v: list[int] | str

:returns:
    *  :any:`tril.true` if the words are known to belong to the congruence;
    *  :any:`tril.false` if the words are known to not belong to the
       congruence;
    *  :any:`tril.unknown` otherwise.
:rtype: tril

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`Presentation.throw_if_letter_not_in_alphabet`
  raises.

{raises}
)pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_CURRENTLY_CONTAINS(A, B)          \
  template void def_currently_contains<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  DEF_CURRENTLY_CONTAINS(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_CURRENTLY_CONTAINS(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_CURRENTLY_CONTAINS(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_CURRENTLY_CONTAINS(Kambites<word_type>, detail::CongruenceCommon);
  DEF_CURRENTLY_CONTAINS(Kambites<MultiView<std::string>>,
                         detail::CongruenceCommon);
  DEF_CURRENTLY_CONTAINS(Kambites<std::string>, detail::CongruenceCommon);

  DEF_CURRENTLY_CONTAINS(detail::KnuthBendixImpl<RewriteTrie>,
                         detail::CongruenceCommon);
  DEF_CURRENTLY_CONTAINS(detail::KnuthBendixImpl<RewriteFromLeft>,
                         detail::CongruenceCommon);

  DEF_CURRENTLY_CONTAINS(KnuthBendixStringRewriteTrie,
                         detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CURRENTLY_CONTAINS(KnuthBendixStringRewriteFromLeft,
                         detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_CURRENTLY_CONTAINS(KnuthBendixWordRewriteTrie,
                         detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CURRENTLY_CONTAINS(KnuthBendixWordRewriteFromLeft,
                         detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_CURRENTLY_CONTAINS(Congruence<word_type>, detail::CongruenceCommon);
  DEF_CURRENTLY_CONTAINS(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_contains(py::class_<Thing, ThingBase>& thing,
                    std::string_view              name,
                    doc                           extra_doc) {
    using Word = typename Thing::native_word_type;

    thing.def(
        "contains",
        [](Thing& self, Word const& u, Word const& v) {
          return congruence_common::contains(self, u, v);
        },
        py::arg("u"),
        py::arg("v"),
        make_doc(R"pbdoc(
:sig=(self: {name}, u: list[int] | str, v: list[int] | str) -> bool:
{only_document_once}

Check containment of a pair of words.

This function checks whether or not the words *u* and *v* are contained in the
congruence represented by a :py:class:`{name}` instance.

:param u: the first word.
:type u: list[int] | str

:param v: the second word.
:type v: list[int] | str

:returns: Whether or not the pair belongs to the congruence.
:rtype: bool

:raises LibsemigroupsError:
  if any of the values in *u* or *v* is out of range, i.e. they do not belong
  to ``presentation().alphabet()`` and :any:`Presentation.throw_if_letter_not_in_alphabet`
  raises.

{raises}
)pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_CONTAINS(A, B) \
  template void def_contains<A, B>(py::class_<A, B>&, std::string_view, doc)

  DEF_CONTAINS(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_CONTAINS(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_CONTAINS(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_CONTAINS(Kambites<word_type>, detail::CongruenceCommon);
  DEF_CONTAINS(Kambites<MultiView<std::string>>, detail::CongruenceCommon);
  DEF_CONTAINS(Kambites<std::string>, detail::CongruenceCommon);

  DEF_CONTAINS(detail::KnuthBendixImpl<RewriteTrie>, detail::CongruenceCommon);
  DEF_CONTAINS(detail::KnuthBendixImpl<RewriteFromLeft>,
               detail::CongruenceCommon);

  DEF_CONTAINS(KnuthBendixStringRewriteTrie,
               detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CONTAINS(KnuthBendixStringRewriteFromLeft,
               detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_CONTAINS(KnuthBendixWordRewriteTrie,
               detail::KnuthBendixImpl<RewriteTrie>);
  DEF_CONTAINS(KnuthBendixWordRewriteFromLeft,
               detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_CONTAINS(Congruence<word_type>, detail::CongruenceCommon);
  DEF_CONTAINS(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_reduce_no_run(py::class_<Thing, ThingBase>& thing,
                         std::string_view              name,
                         doc                           extra_doc) {
    using Word = typename Thing::native_word_type;
    thing.def(
        "reduce_no_run",
        [](Thing& self, Word const& w) {
          return congruence_common::reduce_no_run(self, w);
        },
        py::arg("w"),
        make_doc(R"pbdoc(
:sig=(self: {name}, w: list[int] | str) -> list[int] | str:
{only_document_once}

Reduce a word.

If :any:`Runner.finished` returns ``True``, then this function returns a
normal form for the input word *w*.

{detail}

:param w: the input word.
:type w: list[int] | str

:returns: A word equivalent to the input word.
:rtype: list[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`Presentation.throw_if_letter_not_in_alphabet`
  raises.

{raises})pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_REDUCE_NO_RUN(A, B)          \
  template void def_reduce_no_run<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  DEF_REDUCE_NO_RUN(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_REDUCE_NO_RUN(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_REDUCE_NO_RUN(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_REDUCE_NO_RUN(Kambites<word_type>, detail::CongruenceCommon);
  DEF_REDUCE_NO_RUN(Kambites<MultiView<std::string>>, detail::CongruenceCommon);
  DEF_REDUCE_NO_RUN(Kambites<std::string>, detail::CongruenceCommon);

  DEF_REDUCE_NO_RUN(detail::KnuthBendixImpl<RewriteTrie>,
                    detail::CongruenceCommon);
  DEF_REDUCE_NO_RUN(detail::KnuthBendixImpl<RewriteFromLeft>,
                    detail::CongruenceCommon);

  DEF_REDUCE_NO_RUN(KnuthBendixStringRewriteTrie,
                    detail::KnuthBendixImpl<RewriteTrie>);
  DEF_REDUCE_NO_RUN(KnuthBendixStringRewriteFromLeft,
                    detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_REDUCE_NO_RUN(KnuthBendixWordRewriteTrie,
                    detail::KnuthBendixImpl<RewriteTrie>);
  DEF_REDUCE_NO_RUN(KnuthBendixWordRewriteFromLeft,
                    detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_REDUCE_NO_RUN(Congruence<word_type>, detail::CongruenceCommon);
  DEF_REDUCE_NO_RUN(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_reduce(py::class_<Thing, ThingBase>& thing,
                  std::string_view              name,
                  doc                           extra_doc) {
    using Word = typename Thing::native_word_type;
    thing.def(
        "reduce",
        [](Thing& self, Word const& w) {
          return congruence_common::reduce(self, w);
        },
        py::arg("w"),
        make_doc(R"pbdoc(
:sig=(self: {name}, w: list[int] | str) -> list[int] | str:
{only_document_once}

Reduce a word.

This function triggers a full enumeration of an :py:class:`{name}` object and
then reduces the word *w*. As such the returned word is a normal form for the
input word.

{detail}

:param w: the input word.
:type w: list[int] | str

:returns: A normal form for the input word.
:rtype: list[int] | str

:raises LibsemigroupsError:
  if any of the values in *w* is out of range, i.e. they do not belong to
  ``presentation().alphabet()`` and :any:`Presentation.throw_if_letter_not_in_alphabet`
  raises.

{raises}
)pbdoc",
                 name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_REDUCE(A, B) \
  template void def_reduce<A, B>(py::class_<A, B>&, std::string_view, doc)

  DEF_REDUCE(detail::ToddCoxeterImpl, detail::CongruenceCommon);

  DEF_REDUCE(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_REDUCE(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_REDUCE(Kambites<word_type>, detail::CongruenceCommon);
  DEF_REDUCE(Kambites<MultiView<std::string>>, detail::CongruenceCommon);
  DEF_REDUCE(Kambites<std::string>, detail::CongruenceCommon);

  DEF_REDUCE(detail::KnuthBendixImpl<RewriteTrie>, detail::CongruenceCommon);
  DEF_REDUCE(detail::KnuthBendixImpl<RewriteFromLeft>,
             detail::CongruenceCommon);

  DEF_REDUCE(KnuthBendixStringRewriteTrie,
             detail::KnuthBendixImpl<RewriteTrie>);
  DEF_REDUCE(KnuthBendixStringRewriteFromLeft,
             detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_REDUCE(KnuthBendixWordRewriteTrie, detail::KnuthBendixImpl<RewriteTrie>);
  DEF_REDUCE(KnuthBendixWordRewriteFromLeft,
             detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_REDUCE(Congruence<word_type>, detail::CongruenceCommon);
  DEF_REDUCE(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_generating_pairs(py::class_<Thing, ThingBase>& thing,
                            std::string_view              name,
                            doc                           extra_doc) {
    thing.def("generating_pairs",
              &Thing::generating_pairs,
              make_doc(
                  R"pbdoc(
:sig=(self : {name}) -> list[list[int] | str]:
{only_document_once}

Get the generating pairs of the congruence.

This function returns the generating pairs of the congruence as added via
:any:`{name}.add_generating_pair`.

:returns:
   The list of generating pairs.
:rtype:
   list[list[int] | str]
  )pbdoc",
                  name,
                  extra_doc));
  }
  ////////////////////////////////////////////////////////////////////////

#define DEF_GENERATING_PAIRS(A, B)          \
  template void def_generating_pairs<A, B>( \
      py::class_<A, B>&, std::string_view, doc)

  // Not required for detail::ToddCoxeterImpl, it's called
  // internal_generating_pairs
  // Not required for detail::KnuthBendixImpl, it's
  // called internal_generating_pairs

  DEF_GENERATING_PAIRS(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_GENERATING_PAIRS(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_GENERATING_PAIRS(Kambites<word_type>, detail::CongruenceCommon);
  DEF_GENERATING_PAIRS(Kambites<MultiView<std::string>>,
                       detail::CongruenceCommon);
  DEF_GENERATING_PAIRS(Kambites<std::string>, detail::CongruenceCommon);

  DEF_GENERATING_PAIRS(KnuthBendixStringRewriteTrie,
                       detail::KnuthBendixImpl<RewriteTrie>);
  DEF_GENERATING_PAIRS(KnuthBendixStringRewriteFromLeft,
                       detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_GENERATING_PAIRS(KnuthBendixWordRewriteTrie,
                       detail::KnuthBendixImpl<RewriteTrie>);
  DEF_GENERATING_PAIRS(KnuthBendixWordRewriteFromLeft,
                       detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_GENERATING_PAIRS(Congruence<word_type>, detail::CongruenceCommon);
  DEF_GENERATING_PAIRS(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing, typename ThingBase>
  void def_presentation(py::class_<Thing, ThingBase>& thing,
                        std::string_view              name,
                        doc                           extra_doc) {
    thing.def("presentation",
              &Thing::presentation,
              make_doc(R"pbdoc(
:sig=(self: {name}) -> Presentation:
{only_document_once}

Get the presentation used to define a :any:`{name}` instance (if any).
If a :any:`{name}` instance is constructed or initialised using a
presentation, then this presentation is returned by this function.

{detail}

:returns:
   The presentation used to construct or initialise a :any:`{name}`
   instance.
:rtype:
   Presentation

{raises}
)pbdoc",
                       name,
                       extra_doc),
              py::return_value_policy::reference_internal);
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_PRESENTATION(A, B) \
  template void def_presentation<A, B>(py::class_<A, B>&, std::string_view, doc)

  // Not required for detail::ToddCoxeterImpl, it's called internal_presentation
  // Not required for detail::KnuthBendixImpl, it's called internal_presentation

  DEF_PRESENTATION(ToddCoxeter<word_type>, detail::ToddCoxeterImpl);
  DEF_PRESENTATION(ToddCoxeter<std::string>, detail::ToddCoxeterImpl);

  DEF_PRESENTATION(Kambites<word_type>, detail::CongruenceCommon);
  DEF_PRESENTATION(Kambites<MultiView<std::string>>, detail::CongruenceCommon);
  DEF_PRESENTATION(Kambites<std::string>, detail::CongruenceCommon);

  DEF_PRESENTATION(KnuthBendixStringRewriteTrie,
                   detail::KnuthBendixImpl<RewriteTrie>);
  DEF_PRESENTATION(KnuthBendixStringRewriteFromLeft,
                   detail::KnuthBendixImpl<RewriteFromLeft>);
  DEF_PRESENTATION(KnuthBendixWordRewriteTrie,
                   detail::KnuthBendixImpl<RewriteTrie>);
  DEF_PRESENTATION(KnuthBendixWordRewriteFromLeft,
                   detail::KnuthBendixImpl<RewriteFromLeft>);

  DEF_PRESENTATION(Congruence<word_type>, detail::CongruenceCommon);
  DEF_PRESENTATION(Congruence<std::string>, detail::CongruenceCommon);

  ////////////////////////////////////////////////////////////////////////
  // Helpers
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing>
  void def_partition(py::module&      m,
                     std::string_view class_name,
                     std::string_view func_prefix,
                     doc              extra_doc) {
    using Word = typename Thing::native_word_type;

    std::string func_name = fmt::format("{}_partition", func_prefix);
    m.def(
        func_name.c_str(),
        [](Thing& ci, std::vector<Word> const& words) {
          return congruence_common::partition(
              ci, rx::iterator_range(words.begin(), words.end()));
        },
        py::arg(extra_doc.var.data()),
        py::arg("words"),
        make_doc(R"pbdoc(
:sig=({var}: {name}, words: list[list[int] | str]) -> list[list[list[int]] | list[str]]:
{only_document_once}

Partition a list of words.

This function returns the classes in the partition of the words in the input
list *words* induced by the :any:`{name}` instance *{var}*. This function
triggers a full enumeration of *{var}*.

{detail}

:param {var}: the :any:`{name}` instance.
:type {var}: {name}

:param words: the input list of words.
:type words: list[list[int] | str]

:returns: The partitioned list of words.
:rtype: list[list[list[int]] | list[str]]

{raises}
)pbdoc",
                 class_name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_PARTITION(Thing)          \
  template void def_partition<Thing>( \
      py::module&, std::string_view, std::string_view, doc)

  DEF_PARTITION(ToddCoxeter<word_type>);
  DEF_PARTITION(ToddCoxeter<std::string>);

  DEF_PARTITION(Kambites<word_type>);
  DEF_PARTITION(Kambites<MultiView<std::string>>);
  DEF_PARTITION(Kambites<std::string>);

  DEF_PARTITION(KnuthBendixStringRewriteTrie);
  DEF_PARTITION(KnuthBendixStringRewriteFromLeft);
  DEF_PARTITION(KnuthBendixWordRewriteTrie);
  DEF_PARTITION(KnuthBendixWordRewriteFromLeft);

  DEF_PARTITION(Congruence<word_type>);
  DEF_PARTITION(Congruence<std::string>);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing>
  void def_non_trivial_classes(py::module&      m,
                               std::string_view class_name,
                               std::string_view func_prefix,
                               doc              extra_doc) {
    using Word = typename Thing::native_word_type;
    std::string func_name(func_prefix);
    func_name += "_non_trivial_classes";
    m.def(
        func_name.c_str(),
        [](Thing& ci, std::vector<Word> const& words) {
          return congruence_common::non_trivial_classes(
              ci, rx::iterator_range(words.begin(), words.end()));
        },
        py::arg(extra_doc.var.data()),
        py::arg("words"),
        make_doc(
            R"pbdoc(
:sig=({var}: {name}, words: list[list[int] | str]) -> list[list[list[int]] | list[str]]:
{only_document_once}

Find the non-trivial classes in the partition of a list of words.

This function returns the classes with size at least :math:`2` in the
partition of the words in the list *words* induced by the :any:`{name}`
instance *{var}*.

{detail}

:param {var}: the :any:`{name}` instance.
:type {var}: {name}

:param words: the input list of words.
:type words: list[list[int] | str]

:returns: The partition of the input list.
:rtype: list[list[list[int]] | list[str]]

{raises})pbdoc",
            class_name,
            extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_NON_TRIVIAL_CLASSES(Thing)          \
  template void def_non_trivial_classes<Thing>( \
      py::module&, std::string_view, std::string_view, doc)

  DEF_NON_TRIVIAL_CLASSES(ToddCoxeter<word_type>);
  DEF_NON_TRIVIAL_CLASSES(ToddCoxeter<std::string>);

  DEF_NON_TRIVIAL_CLASSES(Kambites<word_type>);
  DEF_NON_TRIVIAL_CLASSES(Kambites<MultiView<std::string>>);
  DEF_NON_TRIVIAL_CLASSES(Kambites<std::string>);

  DEF_NON_TRIVIAL_CLASSES(KnuthBendixStringRewriteTrie);
  DEF_NON_TRIVIAL_CLASSES(KnuthBendixStringRewriteFromLeft);
  DEF_NON_TRIVIAL_CLASSES(KnuthBendixWordRewriteTrie);
  DEF_NON_TRIVIAL_CLASSES(KnuthBendixWordRewriteFromLeft);

  DEF_NON_TRIVIAL_CLASSES(Congruence<word_type>);
  DEF_NON_TRIVIAL_CLASSES(Congruence<std::string>);

  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  template <typename Thing>
  void def_normal_forms(py::module&      m,
                        std::string_view class_name,
                        std::string_view func_prefix,
                        doc              extra_doc) {
    std::string func_name(func_prefix);
    func_name += "_normal_forms";
    m.def(
        func_name.c_str(),
        [](Thing& ci) {
          auto nf = congruence_common::normal_forms(ci);
          return py::make_iterator(rx::begin(nf), rx::end(nf));
        },
        py::arg(extra_doc.var.data()),
        make_doc(R"pbdoc(
:sig=({var}: {name}) -> collections.abc.Iterator[str | list[int]]:
{only_document_once}

Returns an iterator yielding normal forms.

This function returns an iterator yielding normal forms of the classes of
the congruence represented by an instance of :any:`{name}`.

{detail}

:param {var}: the :any:`{name}` instance.
:type {var}: {name}

:returns: An iterator yielding normal forms.
:rtype: collections.abc.Iterator[str | list[int]]

{raises}

{example}
)pbdoc",
                 class_name,
                 extra_doc));
  }

  ////////////////////////////////////////////////////////////////////////

#define DEF_NORMAL_FORMS(Thing)          \
  template void def_normal_forms<Thing>( \
      py::module&, std::string_view, std::string_view, doc)

  DEF_NORMAL_FORMS(ToddCoxeter<word_type>);
  DEF_NORMAL_FORMS(ToddCoxeter<std::string>);

  DEF_NORMAL_FORMS(Kambites<word_type>);
  DEF_NORMAL_FORMS(Kambites<MultiView<std::string>>);
  DEF_NORMAL_FORMS(Kambites<std::string>);

  DEF_NORMAL_FORMS(KnuthBendixStringRewriteTrie);
  DEF_NORMAL_FORMS(KnuthBendixStringRewriteFromLeft);
  DEF_NORMAL_FORMS(KnuthBendixWordRewriteTrie);
  DEF_NORMAL_FORMS(KnuthBendixWordRewriteFromLeft);

  // TODO(1) uncomment when implemented in libsemigroups
  // DEF_NORMAL_FORMS(Congruence<word_type>);
  // DEF_NORMAL_FORMS(Congruence<std::string>);

  ////////////////////////////////////////////////////////////////////////
  // The init function for detail::CongruenceCommon
  ////////////////////////////////////////////////////////////////////////

  void init_detail_cong_common(py::module& m) {
    py::class_<detail::CongruenceCommon, Runner> thing(
        m, "detail::CongruenceCommon");

    thing.def(
        "kind",
        [](detail::CongruenceCommon const& self) { return self.kind(); },
        R"pbdoc(
:sig=(self: Congruence | Kambites | KnuthBendix | ToddCoxeter) -> congruence_kind:

The kind of the congruence (1- or 2-sided).

This function returns the kind of the congruence represented by *self*. See
:any:`congruence_kind` for details.

:returns:
   The kind of the congruence (1- or 2-sided).
:rtype:
   congruence_kind

:complexity:
   Constant.
)pbdoc");

    // The following does not appear in the doc, this is intentional.
    thing.def("internal_generating_pairs",
              &detail::CongruenceCommon::internal_generating_pairs);

    thing.def("number_of_generating_pairs",
              &detail::CongruenceCommon::number_of_generating_pairs,
              R"pbdoc(
:sig=(self: Congruence | Kambites | KnuthBendix | ToddCoxeter) -> int:

Returns the number of generating pairs.

This function returns the number of generating pairs of the congruence.

:returns:
   The number of generating pairs.
:rtype:
   int

:complexity:
   Constant.
)pbdoc");
  }  // init_detail_cong_common
}  // namespace libsemigroups
