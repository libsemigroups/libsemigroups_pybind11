
//
// libsemigroups_pybind11
// Copyright (C) 2022-2024 Murray T. Whyte
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

// C std headers....
// TODO complete or delete

// C++ stl headers....
// TODO complete or delete

// libsemigroups headers
#include <libsemigroups/libsemigroups.hpp>
// TODO uncomment/delete

// pybind11....
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
#include <pybind11/pybind11.h>
// #include <pybind11/stl.h>
// TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_TODO

namespace py = pybind11;

namespace libsemigroups {
  using fpsemigroup::author;
  namespace {
    std::string author_name(author const& a) {
      if (a == author::Any) {
        return "Any";
      }

      // TODO (much later) use std::to_underlying (C++23)
      uint64_t value = static_cast<uint64_t>(a);

      // Checks if value is not a power of 2
      if (value & (value - 1)) {
        return "Combination";
      }
      return fmt::to_string(a);
    }
  }  // namespace

  void init_fpsemi_examples(py::module& m) {
    py::options options;
    options.disable_enum_members_docstring();
    py::enum_<author>(m, "author", R"pbdoc(
The values in this enum class are used to specify the authors of a presentation.
Where there are different presentations by different authors, values of this
type can be passed as an argument to disambiguate which presentation is wanted.

To specify a combination of authors, values of this enum can be added together.

The valid values are:

.. py:attribute:: author.Any
  :value: <author.Any: 0>

  Encoded as the :any:`int` value ``0``, and is the value used when no
  particular author is to be specified.

.. py:attribute:: author.Machine
  :value: <author.Machine: 1>

  Encoded as the :any:`int` value ``1``.

.. py:attribute:: author.Aizenstat
  :value: <author.Aizenstat: 2>

  Encoded as the :any:`int` value ``2``.

.. py:attribute:: author.Burnside
  :value: <author.Burnside: 4>

  Encoded as the :any:`int` value ``4``.

.. py:attribute:: author.Carmichael
  :value: <author.Carmichael: 8>

  Encoded as the :any:`int` value ``8``.

.. py:attribute:: author.Coxeter
  :value: <author.Coxeter: 16>

  Encoded as the :any:`int` value ``16``.

.. py:attribute:: author.Easdown
  :value: <author.Easdown: 32>

  Encoded as the :any:`int` value ``32``.

.. py:attribute:: author.East
  :value: <author.East: 64>

  Encoded as the :any:`int` value ``64``.

.. py:attribute:: author.Fernandes
  :value: <author.Fernandes: 128>

  Encoded as the :any:`int` value ``128``.

.. py:attribute:: author.FitzGerald
  :value: <author.FitzGerald: 256>

  Encoded as the :any:`int` value ``256``.

.. py:attribute:: author.Gay
  :value: <author.Gay: 512>

  Encoded as the :any:`int` value ``512``.

.. py:attribute:: author.Godelle
  :value: <author.Godelle: 1024>

  Encoded as the :any:`int` value ``1024``.

.. py:attribute:: author.Guralnick
  :value: <author.Guralnick: 2048>

  Encoded as the :any:`int` value ``2048``.

.. py:attribute:: author.Iwahori
  :value: <author.Iwahori: 4096>

  Encoded as the :any:`int` value ``4096``.

.. py:attribute:: author.Kantor
  :value: <author.Kantor: 8192>

  Encoded as the :any:`int` value ``8192``.

.. py:attribute:: author.Kassabov
  :value: <author.Kassabov: 16384>

  Encoded as the :any:`int` value ``16384``.

.. py:attribute:: author.Lubotzky
  :value: <author.Lubotzky: 32768>

  Encoded as the :any:`int` value ``32768``.

.. py:attribute:: author.Miller
  :value: <author.Miller: 65536>

  Encoded as the :any:`int` value ``65536``.

.. py:attribute:: author.Mitchell
  :value: <author.Mitchell: 131072>

  Encoded as the :any:`int` value ``131072``.

.. py:attribute:: author.Moore
  :value: <author.Moore: 262144>

  Encoded as the :any:`int` value ``262144``.

.. py:attribute:: author.Moser
  :value: <author.Moser: 524288>

  Encoded as the :any:`int` value ``524288``.

.. py:attribute:: author.Sutov
  :value: <author.Sutov: 1048576>

  Encoded as the :any:`int` value ``1048576``.

.. py:attribute:: author.Whyte
  :value: <author.Whyte: 2097152>

  Encoded as the :any:`int` value ``2097152``.
)pbdoc")
        .value("Any", author::Any)
        .value("Machine", author::Machine)
        .value("Aizenstat", author::Aizenstat)
        .value("Burnside", author::Burnside)
        .value("Carmichael", author::Carmichael)
        .value("Coxeter", author::Coxeter)
        .value("Easdown", author::Easdown)
        .value("East", author::East)
        .value("Fernandes", author::Fernandes)
        .value("FitzGerald", author::FitzGerald)
        .value("Gay", author::Gay)
        .value("Godelle", author::Godelle)
        .value("Guralnick", author::Guralnick)
        .value("Iwahori", author::Iwahori)
        .value("Kantor", author::Kantor)
        .value("Kassabov", author::Kassabov)
        .value("Lubotzky", author::Lubotzky)
        .value("Miller", author::Miller)
        .value("Mitchell", author::Mitchell)
        .value("Moore", author::Moore)
        .value("Moser", author::Moser)
        .value("Sutov", author::Sutov)
        .value("Whyte", author::Whyte)
        .def("__add__", &fpsemigroup::operator+)
        .def(
            "__repr__",
            [](const author& a) {
              return fmt::format(
                  "<author.{}: {}>", author_name(a), fmt::underlying(a));
            },
            py::name("__repr__"),
            py::is_method(m))
        .def(
            "__str__",
            [](const author& a) {
              return fmt::format("author.{} with value {}",
                                 author_name(a),
                                 fmt::underlying(a));
            },
            py::name("__str__"),
            py::is_method(m));
    m.def("alternating_group",
          &fpsemigroup::alternating_group,
          py::arg("n"),
          py::arg("val") = author::Moore,
          R"pbdoc(
:sig=(n: int, val: author = author.Moore) -> PresentationStrings:
A presentation for the alternating group.

This function returns a monoid presentation defining the alternating group of
degree *n*. The argument *val* determines the specific presentation which is
returned. The options are:

* :any:`author.Moore` (see Ch. 3, Prop 1.3 of `hdl.handle.net/10023/2821 <http://hdl.handle.net/10023/2821>`_).

:param n: the degree of the alternating group.
:type n: int

:param val: the author of the presentation (default: :any:`author.Moore`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 4``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Moore`.
)pbdoc");
    m.def("brauer_monoid",
          &fpsemigroup::brauer_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the Brauer monoid.

This function returns a monoid presentation defining the Brauer monoid of degree
*n*, as described in Theorem 3.1 of the paper
`10.2478/s11533-006-0017-6 <https://doi.org/10.2478/s11533-006-0017-6>`_.

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("chinese_monoid",
          &fpsemigroup::chinese_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the Chinese monoid.

This function returns a monoid presentation defining the Chinese monoid, as
described in `10.1142/S0218196701000425 <https://doi.org/10.1142/S0218196701000425>`_.

:param n: the number of generators.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 2``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("cyclic_inverse_monoid",
          &fpsemigroup::cyclic_inverse_monoid,
          py::arg("n"),
          py::arg("val")   = author::Fernandes,
          py::arg("index") = 1,
          R"pbdoc(
:sig=(n: int, val: author = author.Fernandes, index: int = 1) -> PresentationStrings:
A presentation for the cyclic inverse monoid.

This function returns a monoid presentation defining the cyclic inverse monoid
of degree *n*. The arguments *val* and *index* determine the specific
presentation which is returned. The options are:

*  ``val = author.Fernandes``:

        *  ``index = 0`` (see Theorem 2.6 of `10.48550/arxiv.2211.02155 <https://doi.org/10.48550/arxiv.2211.02155>`_);
        *  ``index = 1`` (see Theorem 2.7 of `10.48550/arxiv.2211.02155 <https://doi.org/10.48550/arxiv.2211.02155>`_).

The presentation with ``val = author.Fernandes`` and ``index = 0`` has
:math:`n + 1` generators and :math:`\frac{1}{2} \left(n^2 + 3n + 4\right)`
relations. The presentation with ``val = author.Fernandes`` and ``index = 1``
has :math:`2` generators and :math:`\frac{1}{2}\left(n^2 - n + 6\right)`
relations.

:param n: the degree.
:type n: int

:param val: the author (default: :any:`author.Fernandes`).
:type val: author

:param index: the index (default: ``1``).
:type index: int

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Fernandes`.
:raises LibsemigroupsError:  if ``val == author.Fernandes`` and *index* is not ``0`` or ``1``.
)pbdoc");
    m.def("dual_symmetric_inverse_monoid",
          &fpsemigroup::dual_symmetric_inverse_monoid,
          py::arg("n"),
          py::arg("val") = author::Easdown + author::East + author::FitzGerald,
          R"pbdoc(
:sig=(n: int, val: author = author.Easdown + author.East + author.FitzGerald) -> PresentationStrings:
A presentation for the dual symmetric inverse monoid.

This function returns a monoid presentation defining the dual symmetric inverse
monoid of degree *n*. The argument *val* determines the specific presentation
which is returned. The only option is:

*  :any:`author.Easdown` + :any:`author.East` + :any:`author.FitzGerald` (from Section 3 of `10.48550/arxiv.0707.2439 <https://doi.org/10.48550/arxiv.0707.2439>`_).

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.Easdown` + :any:`author.East` + :any:`author.FitzGerald`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Easdown` + :any:`author.East` + :any:`author.FitzGerald`.
)pbdoc");
    m.def("fibonacci_semigroup",
          &fpsemigroup::fibonacci_semigroup,
          py::arg("r"),
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(r: int, n: int, val: author = author.Any) -> PresentationStrings:
A presentation for a Fibonacci semigroup.

This function returns a semigroup presentation defining the Fibonacci semigroup
:math:`F(r, n)`, as described in
`10.1016/0022-4049(94)90005-1 <https://doi.org/10.1016/0022-4049(94)90005-1>`_.

:param r: the length of the left hand sides of the relations.
:type r: int

:param n: the number of generators.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n == 0``.
:raises LibsemigroupsError:  if ``r == 0``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("full_transformation_monoid",
          &fpsemigroup::full_transformation_monoid,
          py::arg("n"),
          py::arg("val")   = author::Mitchell + author::Whyte,
          py::arg("index") = 0,
          R"pbdoc(
:sig=(n: int, val: author = author.Mitchell + author.Whyte, index: int = 0) -> PresentationStrings:
A presentation for the full transformation monoid.

This function returns a monoid presentation defining the full transformation
monoid. The arguments *val* and *index* determine the specific presentation
which is returned. The options are:

*  :any:`author.Aizenstat` and ``index = 0`` (see Ch. 3, Prop 1.7 of `http://hdl.handle.net/10023/2821 <http://hdl.handle.net/10023/2821>`_);
*  :any:`author.Iwahori` and ``index = 0`` (see Theorem 9.3.1 of `10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_);
*  :any:`author.Mitchell + :any:`author.Whyte` (see Theorem 1.5 of `10.48550/arXiv.2406.19294 <https://doi.org/10.48550/arXiv.2406.19294>`_):

        *  ``index = 0`` for the presentation with five non-symmetric-group relations;
        *  ``index = 1`` for the presentation with four non-symmetric-group relations, valid for odd degree.

:param n: the degree of the full transformation monoid.
:type n: int

:param val: the author of the presentation (default: :any:`author.Mitchell` + :any:`author.Whyte`).
:type val: author

:param index: which of the author's presentations to return, if multiple exist (default: ``0``).
:type index: int

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 4``.
:raises LibsemigroupsError:  if the author-index combination is not present in the table above.
)pbdoc");
    m.def("hypo_plactic_monoid",
          &fpsemigroup::hypo_plactic_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("monogenic_semigroup",
          &fpsemigroup::monogenic_semigroup,
          py::arg("m"),
          py::arg("r"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(m: int, r: int, val: author = author.Any) -> PresentationStrings:
A presentation for a monogenic semigroup.

This function returns a presentation defining the monogenic semigroup defined by
the presentation :math:`\langle a \mid a^{m + r} = a^m \rangle`. If ``m == 0``,
the presentation returned is a monoid presentation; otherwise, a semigroup
presentation is returned.

:param m: the index.
:type m: int

:param r: the period.
:type r: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``m < 0``.
:raises LibsemigroupsError:  if ``r = 0``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("not_renner_type_B_monoid",
          &fpsemigroup::not_renner_type_B_monoid,
          py::arg("l"),
          py::arg("q"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("not_renner_type_D_monoid",
          &fpsemigroup::not_renner_type_D_monoid,
          py::arg("l"),
          py::arg("q"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("not_symmetric_group",
          &fpsemigroup::not_symmetric_group,
          py::arg("n"),
          py::arg("val") = author::Guralnick + author::Kantor + author::Kassabov
                           + author::Lubotzky,
          R"pbdoc(
:sig=(n: int, val: author = author.Guralnick + author.Kantor + author.Kassabov + author.Lubotzky) -> PresentationStrings:
A non-presentation for the symmetric group.

This function returns a monoid presentation which is claimed to define the
symmetric group of degree *n*, but does not. The argument *val* determines the
specific non-presentation which is returned. The options are:

*  :any:`author.Guralnick` + :any:`author.Kantor + :any:`author.Kassabov + :any:`author.Lubotzky` `doi.org/10.1090/S0894-0347-08-00590-0 <https://doi.org/10.1090/S0894-0347-08-00590-0>`_.

:param n: the claimed degree of the symmetric group.
:type n: int

:param val: the author of the presentation (default: :any:`author.Guralnick` + :any:`author.Kantor` + :any:`author.Kassabov` + :any:`author.Lubotzky`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 4``.
:raises LibsemigroupsError:  if *val* is not listed above.
)pbdoc");
    m.def("order_preserving_cyclic_inverse_monoid",
          &fpsemigroup::order_preserving_cyclic_inverse_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the order-preserving part of the cyclic inverse monoid.

This function returns a monoid presentation defining the order-preserving part
of the cyclic inverse monoid of degree *n*, as described in Theorem 2.17 of the
paper `10.48550/arxiv.2211.02155 <https://doi.org/10.48550/arxiv.2211.02155>`_.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: PresentationStrings

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("order_preserving_monoid",
          &fpsemigroup::order_preserving_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the monoid of order-preserving mappings.

This function returns a monoid presentation defining the monoid of
order-preserving transformations of degree *n*, as described in Section 2 of the
paper `10.1007/s10012-000-0001-1 <https://doi.org/10.1007/s10012-000-0001-1>`_.
This presentation has :math:`2n - 2` generators and :math:`n^2` relations.

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("orientation_preserving_monoid",
          &fpsemigroup::orientation_preserving_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the monoid of orientation preserving mappings.

This function returns a monoid presentation defining the monoid of orientation
preserving mappings on a finite chain of order *n*, as described in
`10.1007/s10012-000-0001-1 <https://doi.org/10.1007/s10012-000-0001-1>`_.

:param n: the order of the chain.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("orientation_preserving_reversing_monoid",
          &fpsemigroup::orientation_preserving_reversing_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the monoid of orientation preserving or reversing mappings.

This function returns a monoid presentation defining the monoid of orientation
preserving or reversing mappings on a finite chain of order *n*, as described in
`10.1007/s10012-000-0001-1 <https://doi.org/10.1007/s10012-000-0001-1>`_.

:param n: the order of the chain.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("partial_brauer_monoid",
          &fpsemigroup::partial_brauer_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the partial Brauer monoid.

This function returns a monoid presentation defining the partial Brauer monoid
of degree *n*.

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("partial_isometries_cycle_graph_monoid",
          &fpsemigroup::partial_isometries_cycle_graph_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the monoid of partial isometries of a cycle graph.

This function returns a monoid presentation defining the monoid of partial
isometries of an :math:`n`-cycle graph, as described in Theorem 2.8 of
`10.48550/arxiv.2205.02196 <https://doi.org/10.48550/arxiv.2205.02196>`_.

:param n: the number of vertices of the cycle graph.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).

:returns: The specified presentation.
:rtype: PresentationStrings
:type val: author

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("partial_transformation_monoid",
          &fpsemigroup::partial_transformation_monoid,
          py::arg("n"),
          py::arg("val") = author::Mitchell + author::Whyte,
          R"pbdoc(
:sig=(n: int, val: author = author.Mitchell + author.Whyte) -> PresentationStrings:
A presentation for the partial transformation monoid.

This function returns a monoid presentation defining the partial transformation
monoid. The argument *val* determines the specific presentation which is
returned. The options are:

*  :any:`author.Machine`;
*  :any:`author.Sutov` (see Theorem 9.4.1 of `10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_);
*  :any:`author.Mitchell` + :any:`author.Whyte` (See Theorem 1.5 of `10.48550/arXiv.2406.19294 <https://doi.org/10.48550/arXiv.2406.19294>`_).

:param n: the degree of the partial transformation monoid.
:type n: int

:param val: the author of the presentation (default: :any:`author.Mitchell` + :any:`author.Whyte`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if *val* is not one of the options specified above.
)pbdoc");
    m.def("partition_monoid",
          &fpsemigroup::partition_monoid,
          py::arg("n"),
          py::arg("val") = author::East,
          R"pbdoc(
:sig=(n: int, val: author = author.East) -> PresentationStrings:
A presentation for the partition monoid.

This function returns a presentation defining the partition monoid of degree
*n*. The argument *val* determines the specific presentation which is returned.
The options are:

*  :any:`author.Machine`;
*  :any:`author.East` (see Theorem 41 of `10.1016/j.jalgebra.2011.04.008 <https://doi.org/10.1016/j.jalgebra.2011.04.008>`_).

Note that :any:`author.East` returns a monoid presentation, and :any:`author.Machine` returns a semigroup presentation.

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.East`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``val == author.Machine  n != 3``.
:raises LibsemigroupsError:  if ``val == author.East  n < 4``.
:raises LibsemigroupsError:  if *val* is not one of the options specified above.
)pbdoc");
    m.def("plactic_monoid",
          &fpsemigroup::plactic_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the plactic monoid.

This function returns a monoid presentation defining the plactic monoid with *n*
generators (see Section 3 of
`10.1007/s00233-022-10285-3 <https://doi.org/10.1007/s00233-022-10285-3>`_).

:param n: the number of generators.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 2``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("rectangular_band",
          &fpsemigroup::rectangular_band,
          py::arg("m"),
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(m: int, n: int, val: author = author.Any) -> PresentationStrings:

A presentation for a rectangular band.

This function returns a semigroup presentation defining the *m* by *n*
rectangular band, as given in Proposition 4.2 of
`10.1007/s002339910016 <https://doi.org/10.1007/s002339910016>`_.

:param m: the number of rows.
:type m: int

:param n: the number of columns.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``m == 0``.
:raises LibsemigroupsError:  if ``n == 0``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("renner_type_B_monoid",
          &fpsemigroup::renner_type_B_monoid,
          py::arg("l"),
          py::arg("q"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("renner_type_D_monoid",
          &fpsemigroup::renner_type_D_monoid,
          py::arg("l"),
          py::arg("q"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("sigma_stylic_monoid",
          &fpsemigroup::sigma_stylic_monoid,
          py::arg("sigma"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("singular_brauer_monoid",
          &fpsemigroup::singular_brauer_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the singular part of the Brauer monoid.

This function returns a monoid presentation for the singular part of the Brauer
monoid of degree *n*, as in Theorem 5 of
`10.21136/MB.2007.134125 <https://doi.org/10.21136/MB.2007.134125>`_).

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("special_linear_group_2",
          &fpsemigroup::special_linear_group_2,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");
    m.def("stellar_monoid",
          &fpsemigroup::stellar_monoid,
          py::arg("l"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(l: int, val: author = author.Any) -> PresentationStrings:
A presentation for the stellar monoid.

Returns the stellar monoid with *l* generators, as in Theorem 4.39 of
`10.48550/arXiv.1910.11740 <https://doi.org/10.48550/arXiv.1910.11740>`_.

:param l: the number of generators.
:type l: int

:param val: the author (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``l < 2``.
:raises LibsemigroupsError:  if *val* is not :any:`author::Any`.
)pbdoc");
    m.def("stylic_monoid",
          &fpsemigroup::stylic_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the stylic monoid.

This function returns a monoid presentation defining the stylic monoid with *n*
generators (see Theorem 8.1 of
`10.1007/s00233-022-10285-3 <https://doi.org/10.1007/s00233-022-10285-3>`_).

:param n: the number of generators.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 2``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("symmetric_group",
          &fpsemigroup::symmetric_group,
          py::arg("n"),
          py::arg("val")   = author::Carmichael,
          py::arg("index") = 0,
          R"pbdoc(
:sig=(n: int, val: author = author.Carmichael, index: int = 0) -> PresentationStrings:
A presentation for the symmetric group.

This function returns a monoid presentation for the symmetric group of degree
*n*. The arguments *val* and *index* determine the specific presentation which
is returned. The options are:

+-----------------------------------------------+-------+----------------+-----------------------------+------------------------------------------------------------------------------------------+
| Author                                        | Index | No. generators | No. relations               | Reference                                                                                |
+===============================================+=======+================+=============================+==========================================================================================+
| :any:`author.Burnside` + :any:`author.Miller` | ``0`` | :math:`n - 1`  | :math:`n^3 - 5n^2 + 9n - 5` | p.464 of `10.1017/CBO9781139237253 <https://doi.org/10.1017/CBO9781139237253>`_          |
+-----------------------------------------------+-------+----------------+-----------------------------+------------------------------------------------------------------------------------------+
| :any:`author.Carmichael`                      | ``0`` | :math:`n - 1`  | :math:`(n - 1)^2`           | Comment 9.5.2 of `10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_|
+-----------------------------------------------+-------+----------------+-----------------------------+------------------------------------------------------------------------------------------+
| :any:`author.Coxeter` + :any:`author.Moser`   | ``0`` | :math:`n - 1`  | :math:`n(n + 1)/2`          | Ch.3, Prop 1.2 of `hdl.handle.net/10023/2821 <http://hdl.handle.net/10023/2821>`_        |
+-----------------------------------------------+-------+----------------+-----------------------------+------------------------------------------------------------------------------------------+
| :any:`author.Moore`                           | ``0`` | :math:`2`      | :math:`n + 1`               | Ch. 3, Prop 1.1 of `hdl.handle.net/10023/2821 <http://hdl.handle.net/10023/2821>`_       |
+                                               +-------+----------------+-----------------------------+------------------------------------------------------------------------------------------+
|                                               | ``1`` | :math:`n - 1`  | :math:`(n + 1)/2`           | Comment 9.5.3 of `10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_|
+-----------------------------------------------+-------+----------------+-----------------------------+------------------------------------------------------------------------------------------+

:param n: the degree of the symmetric group.
:type n: int

:param val: the author of the presentation (default: :any:`author.Carmichael`).
:type val: author

:param index: which of the author's presentations to return, if multiple exist (default: ``0``).
:type index: int

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 4``.
:raises LibsemigroupsError:  if the author-index combination is not present in the table above.
)pbdoc");
    m.def("symmetric_inverse_monoid",
          &fpsemigroup::symmetric_inverse_monoid,
          py::arg("n"),
          py::arg("val") = author::Mitchell + author::Whyte,
          R"pbdoc(
:sig=(n: int, val: author = author.Mitchell + author.Whyte) -> PresentationStrings:
A presentation for the symmetric inverse monoid.

This function returns a monoid presentation defining the symmetric inverse
monoid. The argument val determines the specific presentation which is returned.
The options are:

*  :any:`author.Gay`;
*  :any:`author.Sutov` (see Theorem 9.2.2 of `10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_);
*  :any:`author.Mitchell` + :any:`author.Whyte` (see Theorem 1.4 of `10.48550/arXiv.2406.19294 <https://doi.org/10.48550/arXiv.2406.19294>`_).

:param n: the degree of the symmetric inverse monoid.
:type n: int

:param val: the author of the presentation (default: :any:`author.Mitchell` + :any:`author.Whyte`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if *val* is not one of the options specified above.
)pbdoc");
    m.def("temperley_lieb_monoid",
          &fpsemigroup::temperley_lieb_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
:sig=(n: int, val: author = author.Any) -> PresentationStrings:
A presentation for the Temperley-Lieb monoid.

This function returns a monoid presentation defining the Temperley-Lieb monoid
with *n* generators, as described in Theorem 2.2 of 
`10.1093/qmath/haab001 <https://doi.org/10.1093/qmath/haab001>`_.

:param n: the number of generators.
:type n: int

:param val: the author of the presentation (default: :any:`author.Any`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.Any`.
)pbdoc");
    m.def("uniform_block_bijection_monoid",
          &fpsemigroup::uniform_block_bijection_monoid,
          py::arg("n"),
          py::arg("val") = author::FitzGerald,
          R"pbdoc(
:sig=(n: int, val: author = author.FitzGerald) -> PresentationStrings:
A presentation for the uniform block bijection monoid.

This function returns a monoid presentation defining the uniform block bijection
monoid of degree *n*. The argument *val* determines the specific presentation
which is returned. The only option is:

*  :any:`author.FitzGerald` (see `10.1017/s0004972700037692 <https://doi.org/10.1017/s0004972700037692>`_).

:param n: the degree.
:type n: int

:param val: the author of the presentation (default: :any:`author.FitzGerald`).
:type val: author

:returns: The specified presentation.
:rtype: PresentationStrings

:raises LibsemigroupsError:  if ``n < 3``.
:raises LibsemigroupsError:  if *val* is not :any:`author.FitzGerald`.
)pbdoc");
    m.def("zero_rook_monoid",
          &fpsemigroup::zero_rook_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");

  }  // init_fpsemi_examples

}  // namespace libsemigroups
