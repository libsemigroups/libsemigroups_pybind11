
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

  void init_fpsemi_examples(py::module& m) {
    py::enum_<author>(m, "author", R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Any", author::Any, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Machine", author::Machine, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Aizenstat", author::Aizenstat, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Burnside", author::Burnside, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Carmichael", author::Carmichael, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Coxeter", author::Coxeter, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Easdown", author::Easdown, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("East", author::East, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Fernandes", author::Fernandes, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("FitzGerald", author::FitzGerald, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Gay", author::Gay, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Godelle", author::Godelle, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Guralnick", author::Guralnick, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Iwahori", author::Iwahori, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Kantor", author::Kantor, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Kassabov", author::Kassabov, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Lubotzky", author::Lubotzky, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Miller", author::Miller, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Mitchell", author::Mitchell, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Moore", author::Moore, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Moser", author::Moser, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Sutov", author::Sutov, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .value("Whyte", author::Whyte, R"pbdoc(
Encoded as the uint64_t value 0
)pbdoc")
        .def("__add__", &fpsemigroup::operator+);
    m.def("alternating_group",
          &fpsemigroup::alternating_group,
          py::arg("n"),
          py::arg("val") = author::Moore,
          R"pbdoc(
A presentation for the alternating group.

:param n: the degree of the alternating group. 
:type n: int

:param val: the author of the presentation (default: author::Moore).
:type val: author
Returns a monoid presentation defining the alternating group of degree ``n`` . The argument ``val`` determines the specific presentation which is returned. The options are:

*  ``author::Moore`` (see Ch. 3, Prop 1.3 of`hdl.handle.net/10023/2821 <http://hdl.handle.net/10023/2821>`_ ).



:raises LibsemigroupsError:  if ``n < 4``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Moore``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("brauer_monoid",
          &fpsemigroup::brauer_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the Brauer monoid.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the Brauer monoid of degree ``n`` , as described in Theorem 3.1 of the paper`10.2478/s11533-006-0017-6 <https://doi.org/10.2478/s11533-006-0017-6>`_.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("chinese_monoid",
          &fpsemigroup::chinese_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the Chinese monoid.

:param n: the number of generators. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the Chinese monoid, as described in`10.1142/S0218196701000425 <https://doi.org/10.1142/S0218196701000425>`_.

:raises LibsemigroupsError:  if ``n < 2``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("cyclic_inverse_monoid",
          &fpsemigroup::cyclic_inverse_monoid,
          py::arg("n"),
          py::arg("val")   = author::Fernandes,
          py::arg("index") = 1,
          R"pbdoc(
A presentation for the cyclic inverse monoid.

:param n: the degree. 
:type n: int

:param val: the author (default: author::Fernandes). 
:type val: author

:param index: the index (default: 1).
:type index: int
Returns a monoid presentation defining the cyclic inverse monoid of degree ``n``.The arguments ``val`` and ``index`` determine the specific presentation which is returned. The options are:

*  ``val = author::Fernandes`` :

*  ``index = 0`` (see Theorem 2.6 of`10.48550/arxiv.2211.02155 <https://doi.org/10.48550/arxiv.2211.02155>`_ );
*  ``index = 1`` (see Theorem 2.7 of`10.48550/arxiv.2211.02155 <https://doi.org/10.48550/arxiv.2211.02155>`_ ).



The presentation with ``val = author::Fernandes`` and ``index = 0`` has :math:`n + 1` generators and :math:`\frac{1}{2} \left(n^2 + 3n + 4\right)` relations.The presentation with ``val = author::Fernandes`` and ``index = 1`` has :math:`2` generators and :math:`\frac{1}{2}\left(n^2 - n + 6\right)` relations.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Fernandes``.

:raises LibsemigroupsError:  if ``val == author::Fernandes`` and ``index`` is not ``0`` or ``1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("dual_symmetric_inverse_monoid",
          &fpsemigroup::dual_symmetric_inverse_monoid,
          py::arg("n"),
          py::arg("val") = author::Easdown + author::East + author::FitzGerald,
          R"pbdoc(
A presentation for the dual symmetric inverse monoid.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::Easdown + author::East + author::FitzGerald).
:type val: author
Returns a monoid presentation defining the dual symmetric inverse monoid of degree ``n`` . The argument ``val`` determines the specific presentation which is returned. The only option is:

*  ``author::Easdown + author::East + author::FitzGerald`` (from Section 3 of`10.48550/arxiv.0707.2439 <https://doi.org/10.48550/arxiv.0707.2439>`_ ).



:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Easdown + author::East + author::FitzGerald``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("fibonacci_semigroup",
          &fpsemigroup::fibonacci_semigroup,
          py::arg("r"),
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for a Fibonacci semigroup.

:param r: the length of the left hand sides of the relations. 
:type r: int

:param n: the number of generators. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a semigroup presentation defining the Fibonacci semigroup :math:`F(r, n)` , where :math:`r` is ``r`` and :math:`n` is ``n`` , as described in`10.1016/0022-4049(94)90005-1 <https://doi.org/10.1016/0022-4049(94)90005-1>`_.

:raises LibsemigroupsError:  if ``n == 0``.

:raises LibsemigroupsError:  if ``r == 0``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("full_transformation_monoid",
          &fpsemigroup::full_transformation_monoid,
          py::arg("n"),
          py::arg("val")   = author::Mitchell + author::Whyte,
          py::arg("index") = 0,
          R"pbdoc(
A presentation for the full transformation monoid.

:param n: the degree of the full transformation monoid. 
:type n: int

:param val: the author of the presentation (default: author::Mitchell + author::Whyte). 
:type val: author

:param index: which of the author's presentations to return, if multiple exist (default: 0).
:type index: int
Returns a monoid presentation defining the full transformation monoid. The arguments ``val`` and ``index`` determine the specific presentation which is returned. The options are:

*  ``author::Aizenstat`` and ``index = 0`` (see Ch. 3, Prop 1.7 of`http://hdl.handle.net/10023/2821 <http://hdl.handle.net/10023/2821>`_ );
*  ``author::Iwahori`` and ``index = 0`` (see Theorem 9.3.1 of`10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_ );
*  ``author::Mitchell + author::Whyte`` (see Theorem 1.5 of`10.48550/arXiv.2406.19294 <https://doi.org/10.48550/arXiv.2406.19294>`_ ):

*  ``index = 0`` for the presentation with five non-symmetric-group relations;
*  ``index = 1`` for the presentation with four non-symmetric-group relations, valid for odd degree.





:raises LibsemigroupsError:  if ``n < 4``.

:raises LibsemigroupsError:  if the author-index combination is not present in the table above.


:returns: A value of type ``Presentation``.

:rtype: Presentation
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
A presentation for a monogenic semigroup.

:param m: the index. 
:type m: int

:param r: the period. 
:type r: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a presentation defining the monogenic semigroup defined by the presentation :math:`\langle a \mid a^{m + r} = a^m \rangle`.If ``m`` = 0, the presentation returned is a monoid presentation; otherwise, a semigroup presentation is returned.

:raises LibsemigroupsError:  if ``m < 0``.

:raises LibsemigroupsError:  if ``r = 0``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
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
A non-presentation for the symmetric group.

:param n: the claimed degree of the symmetric group. 
:type n: int

:param val: the author of the presentation (default: author::Guralnick + author::Kantor + author::Kassabov + author::Lubotzky).
:type val: author
Returns a monoid presentation which is claimed to define the symmetric group of degree ``n`` , but does not. The argument ``val`` determines the specific non-presentation which is returned. The options are:

*  ``author::Guralnick + author::Kantor + author::Kassabov + author::Lubotzky```doi.org/10.1090/S0894-0347-08-00590-0 <https://doi.org/10.1090/S0894-0347-08-00590-0>`_.



:raises LibsemigroupsError:  if ``n < 4``.

:raises LibsemigroupsError:  if ``val`` is not listed above.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("order_preserving_cyclic_inverse_monoid",
          &fpsemigroup::order_preserving_cyclic_inverse_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
 ``A`` presentation for the order-preserving part of the cyclic inverse monoid.Returns a monoid presentation defining the order-preserving part of the cyclic inverse monoid of degree ``n`` , as described in Theorem 2.17 of the paper`10.48550/arxiv.2211.02155 <https://doi.org/10.48550/arxiv.2211.02155>`_.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("order_preserving_monoid",
          &fpsemigroup::order_preserving_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the monoid of order-preserving mappings.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the monoid of order-preserving transformations of degree ``n`` , as described in Section 2 of the paper`10.1007/s10012-000-0001-1 <https://doi.org/10.1007/s10012-000-0001-1>`_.This presentation has :math:`2n - 2` generators and :math:`n^2` relations.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("orientation_preserving_monoid",
          &fpsemigroup::orientation_preserving_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the monoid of orientation preserving mappings.

:param n: the order of the chain. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the monoid of orientation preserving mappings on a finite chain of order ``n`` , as described in`10.1007/s10012-000-0001-1 <https://doi.org/10.1007/s10012-000-0001-1>`_.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("orientation_preserving_reversing_monoid",
          &fpsemigroup::orientation_preserving_reversing_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the monoid of orientation preserving or reversing mappings.

:param n: the order of the chain. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the monoid of orientation preserving or reversing mappings on a finite chain of order ``n`` , as described in`10.1007/s10012-000-0001-1 <https://doi.org/10.1007/s10012-000-0001-1>`_.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_brauer_monoid",
          &fpsemigroup::partial_brauer_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the partial Brauer monoid.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the partial Brauer monoid of degree ``n`` ..

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_isometries_cycle_graph_monoid",
          &fpsemigroup::partial_isometries_cycle_graph_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the monoid of partial isometries of a cycle graph.

:param n: the number of vertices of the cycle graph. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the monoid of partial isometries of an :math:`n` -cycle graph, as described in Theorem 2.8 of`10.48550/arxiv.2205.02196 <https://doi.org/10.48550/arxiv.2205.02196>`_.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_transformation_monoid",
          &fpsemigroup::partial_transformation_monoid,
          py::arg("n"),
          py::arg("val") = author::Mitchell + author::Whyte,
          R"pbdoc(
A presentation for the partial transformation monoid.

:param n: the degree of the partial transformation monoid. 
:type n: int

:param val: the author of the presentation (default: author::Mitchell + author::Whyte).
:type val: author
Returns a monoid presentation defining the partial transformation monoid. The argument ``val`` determines the specific presentation which is returned. The options are:

*  ``author::Machine`` ;
*  ``author::Sutov`` (see Theorem 9.4.1 of`10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_ );
*  ``author::Mitchell + author::Whyte`` (See Theorem 1.5 of`10.48550/arXiv.2406.19294 <https://doi.org/10.48550/arXiv.2406.19294>`_ ).



:raises LibsemigroupsError:  if ``val`` is not one of the options specified above.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partition_monoid",
          &fpsemigroup::partition_monoid,
          py::arg("n"),
          py::arg("val") = author::East,
          R"pbdoc(
A presentation for the partition monoid.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::East).
:type val: author
Returns a presentation defining the partition monoid of degree ``n`` . The argument ``val`` determines the specific presentation which is returned. The options are:

*  ``author::Machine`` ;
*  ``author::East`` (see Theorem 41 of`10.1016/j.jalgebra.2011.04.008 <https://doi.org/10.1016/j.jalgebra.2011.04.008>`_ ).

Note that ``author::East`` returns a monoid presentation, and ``author::Machine`` returns a semigroup presentation.

:raises LibsemigroupsError:  if ``val == author::Machine  n != 3``.

:raises LibsemigroupsError:  if ``val == author::East  n < 4``.

:raises LibsemigroupsError:  if ``val`` is not one of the options specified above.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("plactic_monoid",
          &fpsemigroup::plactic_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the plactic monoid.

:param n: the number of generators. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the plactic monoid with ``n`` generators (see Section 3 of`10.1007/s00233-022-10285-3 <https://doi.org/10.1007/s00233-022-10285-3>`_ ).

:raises LibsemigroupsError:  if ``n < 2``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("rectangular_band",
          &fpsemigroup::rectangular_band,
          py::arg("m"),
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for a rectangular band.

:param m: the number of rows. 
:type m: int

:param n: the number of columns. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a semigroup presentation defining the ``m`` by ``n`` rectangular band, as given in Proposition 4.2 of`10.1007/s002339910016 <https://doi.org/10.1007/s002339910016>`_.

:raises LibsemigroupsError:  if ``m == 0``.

:raises LibsemigroupsError:  if ``n == 0``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
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
A presentation for the singular part of the Brauer monoid.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation for the singular part of the Brauer monoid of degree ``n`` , as in Theorem 5 of`10.21136/MB.2007.134125 <https://doi.org/10.21136/MB.2007.134125>`_ ).

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
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
A presentation for the stellar monoid.

:param l: the number of generators. 
:type l: int

:param val: the author (default: author::Any).
:type val: author
Returns the stellar monoid with ``l`` generators, as in Theorem 4.39 of`10.48550/arXiv.1910.11740 <https://doi.org/10.48550/arXiv.1910.11740>`_.

:raises LibsemigroupsError:  if ``l < 2``.

:raises LibsemigroupsError:  if ``val`` is not :any:`author::Any`.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("stylic_monoid",
          &fpsemigroup::stylic_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the stylic monoid.

:param n: the number of generators. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the stylic monoid with ``n`` generators (see Theorem 8.1 of`10.1007/s00233-022-10285-3 <https://doi.org/10.1007/s00233-022-10285-3>`_ ).

:raises LibsemigroupsError:  if ``n < 2``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_group",
          &fpsemigroup::symmetric_group,
          py::arg("n"),
          py::arg("val")   = author::Carmichael,
          py::arg("index") = 0,
          R"pbdoc(
A presentation for the symmetric group.

:param n: the degree of the symmetric group. 
:type n: int

:param val: the author of the presentation (default: author::Carmichael). 
:type val: author

:param index: which of the author's presentations to return, if multiple exist (default: 0).
:type index: int
Returns a monoid presentation for the symmetric group of degree ``n`` . The arguments ``val`` and ``index`` determine the specific presentation which is returned. The options are:The default for ``val`` is ``author::Carmichael`` . The default for ``index`` is ``0``.

:raises LibsemigroupsError:  if ``n < 4``.

:raises LibsemigroupsError:  if the author-index combination is not present in the table above.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_inverse_monoid",
          &fpsemigroup::symmetric_inverse_monoid,
          py::arg("n"),
          py::arg("val") = author::Mitchell + author::Whyte,
          R"pbdoc(
A presentation for the symmetric inverse monoid.

:param n: the degree of the symmetric inverse monoid. 
:type n: int

:param val: the author of the presentation (default: author::Mitchell + author::Whyte).
:type val: author
Returns a monoid presentation defining the symmetric inverse monoid. The argument val determines the specific presentation which is returned. The options are:

*  ``author::Gay`` ;
*  ``author::Sutov`` (see Theorem 9.2.2 of`10.1007/978-1-84800-281-4 <https://doi.org/10.1007/978-1-84800-281-4>`_ );
*  ``author::Mitchell + author::Whyte`` (see Theorem 1.4 of`10.48550/arXiv.2406.19294 <https://doi.org/10.48550/arXiv.2406.19294>`_ ).



:raises LibsemigroupsError:  if ``val`` is not one of the options specified above.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("temperley_lieb_monoid",
          &fpsemigroup::temperley_lieb_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
A presentation for the Temperley-Lieb monoid.

:param n: the number of generators. 
:type n: int

:param val: the author of the presentation (default: author::Any).
:type val: author
Returns a monoid presentation defining the Temperley-Lieb monoid with ``n`` generators, as described in Theorem 2.2 of`10.1093/qmath/haab001 <https://doi.org/10.1093/qmath/haab001>`_.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::Any``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("uniform_block_bijection_monoid",
          &fpsemigroup::uniform_block_bijection_monoid,
          py::arg("n"),
          py::arg("val") = author::FitzGerald,
          R"pbdoc(
A presentation for the uniform block bijection monoid.

:param n: the degree. 
:type n: int

:param val: the author of the presentation (default: author::FitzGerald).
:type val: author
Returns a monoid presentation defining the uniform block bijection monoid of degree ``n`` . The argument ``val`` determines the specific presentation which is returned. The only option is:

*  ``author::FitzGerald`` (see`10.1017/s0004972700037692 <https://doi.org/10.1017/s0004972700037692>`_ ).



:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``val`` is not ``author::FitzGerald``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("zero_rook_monoid",
          &fpsemigroup::zero_rook_monoid,
          py::arg("n"),
          py::arg("val") = author::Any,
          R"pbdoc(
)pbdoc");

  }  // init_fpsemi_examples

}  // namespace libsemigroups
