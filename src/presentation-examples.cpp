
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

// libsemigroups headers
#include <libsemigroups/libsemigroups.hpp>

// pybind11....
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_presentation_examples

namespace libsemigroups {
  namespace py = pybind11;

  void init_presentation_examples(py::module& m) {
    namespace examples = presentation::examples;
    m.def("presentation_examples_stellar_monoid_GH19",
          &examples::stellar_monoid_GH19,
          py::arg("l"),
          R"pbdoc(
:sig=(l: int) -> Presentation:
A presentation for the stellar monoid.

This function returns a monoid presentation defining the stellar monoid with *l*
generators, as in Theorem 4.39 of :cite:`Gay1999aa`.

:param l: the number of generators.
:type l: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``l < 2``.
)pbdoc");
    m.def("presentation_examples_dual_symmetric_inverse_monoid_EEF07",
          &examples::dual_symmetric_inverse_monoid_EEF07,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the dual symmetric inverse monoid.

This function returns a monoid presentation defining the dual symmetric inverse
monoid of degree *n*, as in Section 3 of :cite:`Easdown2007aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_uniform_block_bijection_monoid_Fit03",
          &examples::uniform_block_bijection_monoid_Fit03,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the uniform block bijection monoid.

This function returns a monoid presentation defining the uniform block bijection
monoid of degree *n*, as in :cite:`FitzGerald2003aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_partition_monoid_HR05",
          &examples::partition_monoid_HR05,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partition monoid.

This function returns a monoid presentation defining the partition monoid of
degree *n*, as in :cite:`Halverson2005aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.
)pbdoc");
    m.def("presentation_examples_partition_monoid_Eas11",
          &examples::partition_monoid_Eas11,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partition monoid.

This function returns a monoid presentation defining the partition monoid of
degree *n*, as in Theorem 41 of :cite:`East2011aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_sigma_plactic_monoid_AHMNT24",
          &examples::sigma_plactic_monoid_AHMNT24,
          py::arg("sigma"),
          R"pbdoc(
:sig=(sigma: list[int]) -> Presentation:
A presentation for the :math:`\sigma`-plactic monoid.

This function returns a presentation for the :math:`\sigma`-plactic monoid with
``sigma.size()`` generators as in Section 3.1 :cite:`Abram2024aa`. The image of
:math:`\sigma` is given by the values in *sigma*.

The :math:`\sigma`-plactic monoid is the quotient of the plactic monoid by the
least congruence containing the relation :math:`a^{\sigma(a)} = a` for each
:math:`a` in the alphabet. When :math:`\sigma(a) = 2` for all :math:`a`, the
resultant :math:`\sigma`-plactic monoid is known as the stylic monoid, and is
given in :any:`stylic_monoid`.

:param sigma: a list representing the image of $\sigma$.
:type sigma: list[int]

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``len(sigma) < 1``.
)pbdoc");
    m.def("presentation_examples_singular_brauer_monoid_MM07",
          &examples::singular_brauer_monoid_MM07,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the singular part of the Brauer monoid.

This function returns a monoid presentation for the singular part of the Brauer
monoid of degree *n*, as in Theorem 5 of :cite:`Maltcev2007aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_orientation_preserving_monoid_AR00",
          &examples::orientation_preserving_monoid_AR00,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of orientation preserving mappings.

This function returns a monoid presentation defining the monoid of orientation
preserving mappings on a finite chain of order *n*, as described in
:cite:`Arthur2000aa`.

:param n: the order of the chain.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_orientation_preserving_reversing_monoid_AR00",
          &examples::orientation_preserving_reversing_monoid_AR00,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of orientation preserving or reversing mappings.

This function returns a monoid presentation defining the monoid of orientation
preserving or reversing mappings on a finite chain of order *n*, as
described in :cite:`Arthur2000aa`.

:param n: the order of the chain.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_temperley_lieb_monoid_Eas21",
          &examples::temperley_lieb_monoid_Eas21,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Temperley-Lieb monoid.

This function returns a monoid presentation defining the Temperley-Lieb monoid
with *n* generators, as described in Theorem 2.2 of :cite:`East2022aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_brauer_monoid_KM07",
          &examples::brauer_monoid_KM07,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Brauer monoid.

This function returns a monoid presentation defining the Brauer monoid of degree
*n*, as described in Theorem 3.1 of :cite:`Kudryavtseva2006aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.
)pbdoc");
    m.def("presentation_examples_partial_brauer_monoid_KM07",
          &examples::partial_brauer_monoid_KM07,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partial Brauer monoid.

This function returns a monoid presentation defining the partial Brauer monoid
of degree *n*, as described in Theorem 5.1 of :cite:`Kudryavtseva2006aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.
)pbdoc");
    m.def("presentation_examples_motzkin_monoid_PHL13",
          &examples::motzkin_monoid_PHL13,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Motzkin monoid.

This function returns a monoid presentation defining the Motzkin monoid of
degree *n*, as described in Theorem 4.1 of :cite:`Posner2013aa`, with the
additional relations :math:` r_i t_i l_i = r_i^ 2 ` added to fix a hole in
Lemma 4.10 which rendered the presentation as stated in the paper incorrect.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.
)pbdoc");
    m.def("presentation_examples_fibonacci_semigroup_CRRT94",
          &examples::fibonacci_semigroup_CRRT94,
          py::arg("r"),
          py::arg("n"),
          R"pbdoc(
:sig=(r: int, n: int) -> Presentation:
A presentation for a Fibonacci semigroup.

This function returns a semigroup presentation defining the Fibonacci semigroup
:math:`F(r, n)`, where :math:`r` is *r* and :math:`n` is *n*, as described in
:cite:`Campbell1994aa`.

:param r: the length of the left hand sides of the relations.
:type r: int

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.
:raises LibsemigroupsError:  if ``r < 1``.
)pbdoc");
    m.def("presentation_examples_plactic_monoid_Knu70",
          &examples::plactic_monoid_Knu70,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the plactic monoid.

This function returns a monoid presentation defining the plactic monoid with *n*
generators, as in see Theorem 6 of :cite:`Knuth1970aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_stylic_monoid_AR22",
          &examples::stylic_monoid_AR22,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the stylic monoid.

This function returns a monoid presentation defining the stylic monoid with *n*
generators, as in Theorem 8.1 of :cite:`Abram2022aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_symmetric_group_Bur12",
          &examples::symmetric_group_Bur12,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric group.

This function returns a monoid presentation for the symmetric group of degree
*n*, as in p.464 of :cite:`Burnside2012aa`. This presentation has :math:`n - 1`
generators and :math:`n^3 - 5n^2 + 9n - 5` relations.

:param n: the degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_symmetric_group_Car56",
          &examples::symmetric_group_Car56,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric group.

This function returns a monoid presentation for the symmetric group of degree
*n*, as on page 169 of :cite:`Carmichael1956aa`. This presentation has
:math:`n - 1` generators and :math:`(n - 1)^2` relations.

:param n: the degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_symmetric_group_Moo97_a",
          &examples::symmetric_group_Moo97_a,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric group.

This function returns a monoid presentation for the symmetric group of degree
*n*, as in Theorem A of :cite:`Moore1897aa`. This presentation has :math:`n - 1`
generators and :math:`\frac{1}{2}n(n - 1)` relations.

:param n: the degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_symmetric_group_Moo97_b",
          &examples::symmetric_group_Moo97_b,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric group.

This function returns a monoid presentation for the symmetric group of degree
*n*, as in in Theorem A' of :cite:`Moore1897aa`. This presentation has :math:`2`
generators and :math:`n + 1` relations for :math:`n \geq 4`. If :math:`n<4`
then there are :math:`4` relations.

:param n: the degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.



)pbdoc");
    m.def("presentation_examples_alternating_group_Moo97",
          &examples::alternating_group_Moo97,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the alternating group.

This function returns a monoid presentation defining the alternating group of
degree *n*, as in Theorem B of :cite:`Moore1897aa`.

:param n: the degree of the alternating group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_rectangular_band_ACOR00",
          &examples::rectangular_band_ACOR00,
          py::arg("m"),
          py::arg("n"),
          R"pbdoc(
:sig=(m: int, n: int) -> Presentation:
A presentation for a rectangular band.

This function returns a semigroup presentation defining the *m* by *n*
rectangular band, as given in Proposition 4.2 of :cite:`Ayik2000aa`.

:param m: the number of rows.
:type m: int

:param n: the number of columns.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``m == 0``.

:raises LibsemigroupsError:  if ``n == 0``.

)pbdoc");
    m.def("presentation_examples_full_transformation_monoid_Aiz58",
          &examples::full_transformation_monoid_Aiz58,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the full transformation monoid.

This function returns a monoid presentation defining the full transformation
monoid of degree *n*, as in Section 5, Theorem 2 of :cite:`Aizenstat1958aa`
(Russian) and Chapter 3, Proposition 1.7 of :cite:`Ruskuc1995aa` (English).

:param n: the degree of the full transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_full_transformation_monoid_II74",
          &examples::full_transformation_monoid_II74,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the full transformation monoid.

This function returns a monoid presentation defining the full transformation
monoid of degree *n* due to Iwahori and Iwahori :cite:`Iwahori1974aa`, as in
Theorem 9.3.1 of :cite:`Ganyushkin2009aa`.

:param n: the degree of the full transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_full_transformation_monoid_MW24_a",
          &examples::full_transformation_monoid_MW24_a,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the full transformation monoid.

This function returns a monoid presentation defining the full transformation
monoid of degree *n*, corresponding to :math:`\mathcal{T}` in Theorem 1.5 of
:cite:`Mitchell2024aa`. For ``n >= 4`` this presentation has five
non-symmetric-group relations.

:param n: the degree of the full transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_full_transformation_monoid_MW24_b",
          &examples::full_transformation_monoid_MW24_b,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the full transformation monoid.

This function returns a monoid presentation defining the full transformation
monoid of degree *n*, corresponding to :math:`\mathcal{T}'` in Theorem 1.5 of
:cite:`Mitchell2024aa`. This presentation is only valid for odd values of *n*,
and for ``n >= 5`` this presentation has four non-symmetric-group relations.

:param n: the degree of the full transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if *n* is not odd.
)pbdoc");
    m.def("presentation_examples_partial_transformation_monoid_Shu60",
          &examples::partial_transformation_monoid_Shu60,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partial transformation monoid.

This function returns a monoid presentation defining the partial transformation
monoid of degree *n* due to Shutov :cite:`Shutov1960aa`, as in
Theorem 9.4.1 of :cite:`Ganyushkin2009aa`.

:param n: the degree of the partial transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_partial_transformation_monoid_MW24",
          &examples::partial_transformation_monoid_MW24,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partial transformation monoid.

This function returns a monoid presentation defining the partial transformation
monoid of degree *n*, as in Theorem 1.6 of :cite:`Mitchell2024aa`.

:param n: the degree of the partial transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_symmetric_inverse_monoid_Sol04",
          &examples::symmetric_inverse_monoid_Sol04,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric inverse monoid.

This function returns a monoid presentation defining the symmetric inverse
monoid of degree *n*, as in Example 7.1.2 of :cite:`Gay2018aa`.

:param n: the degree of the symmetric inverse monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_symmetric_inverse_monoid_Shu60",
          &examples::symmetric_inverse_monoid_Shu60,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric inverse monoid.

This function returns a monoid presentation defining the symmetric inverse
monoid of degree *n* due to Shutov :cite:`Shutov1960aa`, as in Theorem
9.2.2 of :cite:`Ganyushkin2009aa`.

:param n: the degree of the symmetric inverse monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_symmetric_inverse_monoid_MW24",
          &examples::symmetric_inverse_monoid_MW24,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric inverse monoid.

This function returns a monoid presentation defining the partial transformation
monoid of degree *n*, as in Theorem 1.4 of :cite:`Mitchell2024aa`.

:param n: the degree of the symmetric inverse monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_chinese_monoid_CEKNH01",
          &examples::chinese_monoid_CEKNH01,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Chinese monoid.

This function returns a monoid presentation defining the Chinese monoid with *n*
generators, as in :cite:`Cassaigne2001aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_order_preserving_monoid_AR00",
          &examples::order_preserving_monoid_AR00,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of order preserving mappings.

This function returns a monoid presentation defining the monoid of
order preserving transformations of degree *n*, as described in Section 2 of
:cite:`Arthur2000aa`. This presentation has :math:`2n - 2` generators and
:math:`n^2` relations.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_cyclic_inverse_monoid_Fer22_a",
          &examples::cyclic_inverse_monoid_Fer22_a,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the cyclic inverse monoid.

This function returns a monoid presentation defining the cyclic inverse monoid
of degree *n*, as in Theorem 2.6 of :cite:`Fernandes2022aa`. This has
:math:`n + 1` generators and :math:`\frac{1}{2} \left(n^2 + 3n + 4\right)`
relations.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_cyclic_inverse_monoid_Fer22_b",
          &examples::cyclic_inverse_monoid_Fer22_b,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the cyclic inverse monoid.

This function returns a monoid presentation defining the cyclic inverse monoid
of degree *n*, as in Theorem 2.7 of :cite:`Fernandes2022aa`. This presentation
has :math:`2` generators and :math:`\frac{1}{2}\left(n^2 - n + 6\right)`
relations.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_order_preserving_cyclic_inverse_monoid_Fer22",
          &examples::order_preserving_cyclic_inverse_monoid_Fer22,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the order preserving part of the cyclic inverse monoid.

This function returns a monoid presentation defining the order preserving part
of the cyclic inverse monoid of degree *n*, as in Theorem 2.17 of
:cite:`Fernandes2022aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_partial_isometries_cycle_graph_monoid_FP22",
          &examples::partial_isometries_cycle_graph_monoid_FP22,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of partial isometries of a cycle graph.

This function returns a monoid presentation defining the monoid of partial
isometries of an :math:`n` -cycle graph, as in Theorem 2.8 of
:cite:`Fernandes2022ab`

:param n: the number of vertices of the cycle graph.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.
)pbdoc");
    m.def("presentation_examples_not_symmetric_group_GKKL08",
          &examples::not_symmetric_group_GKKL08,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A non-presentation for the symmetric group.

This function returns a monoid presentation which is claimed to define the
symmetric group of degree *n*, but does not, as in Section 2.2 of
:cite:`Guralnick2008aa`.

:param n: the claimed degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.
)pbdoc");
    m.def("presentation_examples_special_linear_group_2_CR80",
          &examples::special_linear_group_2_CR80,
          py::arg("q"),
          R"pbdoc(
:sig=(q: int) -> Presentation:
A presentation for the special linear group :math:`\mathrm{SL}(2, q)`.

This function returns a presentation for the special linear group
:math:`\mathrm{SL}(2, q)` (also written :math:`\mathrm{SL(2,
\mathbb{Z}_q)}`), where *q* is an odd prime, as in Theorem 4 of
:cite:`Campbell1980aa`.

:param q: the order of the finite field over which the special linear group is
      constructed. This should be an odd prime for the returned presentation to
      define claimed group.
:type q: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``q < 3``.
)pbdoc");
    m.def("presentation_examples_hypo_plactic_monoid_Nov00",
          &examples::hypo_plactic_monoid_Nov00,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the hypoplactic monoid.

This function returns a presentation for the hypoplactic monoid with *n*
generators, as in Definition 4.2 of :cite:`Novelli2000aa`. This monoid is a
quotient monoid of the plactic monoid, and this presentation includes the rules
from :any:`plactic_monoid_Knu70`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.
)pbdoc");
    m.def("presentation_examples_zero_rook_monoid_Gay18",
          &examples::zero_rook_monoid_Gay18,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the :math:`0`-rook monoid.

This function returns a presentation for the :math:`0` -rook monoid of degree
*n*, as in Definition 4.1.1 in :cite:`Gay2018aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.
)pbdoc");
    m.def("presentation_examples_renner_type_B_monoid_Gay18",
          &examples::renner_type_B_monoid_Gay18,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation for the Renner monoid of type B.

This functions returns a presentation for the Renner monoid of type B with size
*l* and Iwahori-Hecke deformation *q*.

When ``q == 0``, this corresponds to Definition 8.4.1 and Example 8.4.2 of
:cite:`Gay2018aa`.

When ``q == 1``, this corresponds to Theorem 8.4.19 of :cite:`Gay2018aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.
)pbdoc");
    m.def("presentation_examples_not_renner_type_B_monoid_Gay18",
          &examples::not_renner_type_B_monoid_Gay18,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation that incorrectly claims to be the Renner monoid of type B.

This functions returns a presentation that incorrectly claims to be the Renner
monoid of type B with size *l* and Iwahori-Hecke deformation *q*.

When ``q == 0``, this corresponds to Example 7.1.2 of :cite:`Gay2018aa`.

When ``q == 1``, this corresponds to Section 3.2 of :cite:`Godelle2009aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.
)pbdoc");
    m.def("presentation_examples_renner_type_D_monoid_Gay18",
          &examples::renner_type_D_monoid_Gay18,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation for the Renner monoid of type D.

This functions returns a presentation for the Renner monoid of type D with size
*l* and Iwahori-Hecke deformation *q*.

When ``q == 0``, this corresponds Definition 8.4.22 of :cite:`Gay2018aa`.

When ``q == 1``, this corresponds to Theorem 8.4.43 of :cite:`Gay2018aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.
)pbdoc");
    m.def("presentation_examples_not_renner_type_D_monoid_God09",
          &examples::not_renner_type_D_monoid_God09,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation that incorrectly claims to be the Renner monoid of type D.

This functions returns a presentation that incorrectly claims to be the Renner
monoid of type D with size *l* and Iwahori-Hecke deformation *q*.

When ``q == 1``, this corresponds to Section 3.3 of :cite:`Godelle2009aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.
)pbdoc");
    m.def("presentation_examples_stellar_monoid",
          &examples::stellar_monoid,
          py::arg("l"),
          R"pbdoc(
:sig=(l: int) -> Presentation:
A presentation for the stellar monoid.

This function returns a monoid presentation defining the stellar monoid with
*l* generators, as in Theorem 4.39 of :cite:`Gay1999aa`.

:param l: the number of generators.
:type l: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``l < 2``.

.. note::

      This function returns exactly the same presentation as :any:`stellar_monoid_GH19`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_dual_symmetric_inverse_monoid",
          &examples::dual_symmetric_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the dual symmetric inverse monoid.

This function returns a monoid presentation defining the dual symmetric inverse
monoid of degree *n*, as in Section 3 of :cite:`Easdown2007aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`dual_symmetric_inverse_monoid`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_uniform_block_bijection_monoid",
          &examples::uniform_block_bijection_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the uniform block bijection monoid.

This function returns a monoid presentation defining the uniform block bijection
monoid of degree *n*, as in :cite:`FitzGerald2003aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`uniform_block_bijection_monoid_Fit03`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_partition_monoid",
          &examples::partition_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partition monoid.

This function returns a monoid presentation defining the partition monoid of
degree *n*, as in Theorem 41 of :cite:`East2011aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.

.. seealso::

      For a specific presentation of the symmetric group, see one of the
      following functions:

            * :any:`partition_monoid_Eas11`;
            * :any:`partition_monoid_HR05`.
)pbdoc");
    m.def("presentation_examples_singular_brauer_monoid",
          &examples::singular_brauer_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the singular part of the Brauer monoid.

This function returns a monoid presentation for the singular part of the Brauer
monoid of degree *n*, as in Theorem 5 of :cite:`Maltcev2007aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`singular_brauer_monoid_MM07`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_orientation_preserving_monoid",
          &examples::orientation_preserving_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of orientation preserving mappings.

This function returns a monoid presentation defining the monoid of orientation
preserving mappings on a finite chain of order *n*, as described in
:cite:`Arthur2000aa`.

:param n: the order of the chain.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`orientation_preserving_monoid_AR00`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_orientation_preserving_reversing_monoid",
          &examples::orientation_preserving_reversing_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of orientation preserving or reversing mappings.

This function returns a monoid presentation defining the monoid of orientation
preserving or reversing mappings on a finite chain of order *n*, as described in
:cite:`Arthur2000aa`.

:param n: the order of the chain.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`orientation_preserving_reversing_monoid_AR00`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_temperley_lieb_monoid",
          &examples::temperley_lieb_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Temperley-Lieb monoid.

This function returns a monoid presentation defining the Temperley-Lieb monoid
with *n* generators, as described in Theorem 2.2 of :cite:`East2022aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`temperley_lieb_monoid_Eas21`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_brauer_monoid",
          &examples::brauer_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Brauer monoid.

This function returns a monoid presentation defining the Brauer monoid of
degree *n*, as described in Theorem 3.1 of :cite:`Kudryavtseva2006aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.

.. note::

      This function returns exactly the same presentation as :any:`brauer_monoid_KM07`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_partial_brauer_monoid",
          &examples::partial_brauer_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partial Brauer monoid.

This function returns a monoid presentation defining the partial Brauer monoid
of degree *n*, as described in Theorem 5.1 of :cite:`Kudryavtseva2006aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.

.. note::

      This function returns exactly the same presentation as :any:`partial_brauer_monoid_KM07`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_motzkin_monoid",
          &examples::motzkin_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Motzkin monoid.

This function returns a monoid presentation defining the Motzkin monoid of
degree *n*, as described in Theorem 4.1 of :cite:`Posner2013aa`, with the
additional relations :math:` r_i t_i l_i = r_i ^ 2 ` added to fix a hole in
Lemma 4.10 which rendered the presentation as stated in the paper incorrect.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.

.. note::

      This function returns exactly the same presentation as :any:`motzkin_monoid_PHL13`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_fibonacci_semigroup",
          &examples::fibonacci_semigroup,
          py::arg("r"),
          py::arg("n"),
          R"pbdoc(
:sig=(r: int, n: int) -> Presentation:
A presentation for a Fibonacci semigroup.

This function returns a semigroup presentation defining the Fibonacci semigroup
:math:`F(r, n)`, where :math:`r` is *r* and :math:`n` is *n*, as described in
:cite:`Campbell1994aa`.

:param r: the length of the left hand sides of the relations.
:type r: int

:param n: the number of generators.
:type n: int

:returns: The specified presentation.

:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.

:raises LibsemigroupsError:  if ``r < 1``.

.. note::

      This function returns exactly the same presentation as :any:`fibonacci_semigroup_CRRT94`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.

)pbdoc");
    m.def("presentation_examples_plactic_monoid",
          &examples::plactic_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the plactic monoid.

This function returns a monoid presentation defining the plactic monoid with
*n* generators, as in see Theorem 6 of :cite:`Knuth1970aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. note::

      This function returns exactly the same presentation as :any:`plactic_monoid_Knu70`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_stylic_monoid",
          &examples::stylic_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the stylic monoid.

This function returns a monoid presentation defining the stylic monoid with
*n* generators, as in Theorem 8.1 of :cite:`Abram2022aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. note::

      This function returns exactly the same presentation as :any:`stylic_monoid_AR22`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_symmetric_group",
          &examples::symmetric_group,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the symmetric group.

This function returns a monoid presentation for the symmetric group of degree
*n*, as on page 169 of :cite:`Carmichael1956aa`. This presentation has
:math:`n - 1` generators and :math:`(n - 1)^2` relations.

:param n: the degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::

      For a specific presentation of the symmetric group, see one of the
      following functions:

            * :any:`symmetric_group_Bur12`;
            * :any:`symmetric_group_Car56`;
            * :any:`symmetric_group_Moo97_a`;
            * :any:`symmetric_group_Moo97_b`.
)pbdoc");
    m.def("presentation_examples_alternating_group",
          &examples::alternating_group,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the alternating group.

This function returns a monoid presentation defining the alternating group of
degree *n*, as in Theorem B of :cite:`Moore1897aa`.

:param n: the degree of the alternating group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.

.. note::

      This function returns exactly the same presentation as :any:`alternating_group_Moo97`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_rectangular_band",
          &examples::rectangular_band,
          py::arg("m"),
          py::arg("n"),
          R"pbdoc(
:sig=(m: int, n: int) -> Presentation:
A presentation for a rectangular band.

This function returns a semigroup presentation defining the *m* by *n*
rectangular band, as given in Proposition 4.2 of :cite:`Ayik2000aa`.

:param m: the number of rows.
:type m: int

:param n: the number of columns.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``m == 0``.
:raises LibsemigroupsError:  if ``n == 0``.

.. note::

      This function returns exactly the same presentation as :any:`rectangular_band_ACOR00`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_full_transformation_monoid",
          &examples::full_transformation_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the full transformation monoid.

This function returns a monoid presentation defining the full transformation
monoid of degree *n*, corresponding to :math:`\mathcal{T}` in Theorem 1.5 of
:cite:`Mitchell2024aa`. For ``n >= 4`` this presentation has five
non-symmetric-group relations.

:param n: the degree of the full transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::

      For a specific presentation of the full transformation monoid, see one of
      the following functions:

            * :any:`full_transformation_monoid_Aiz58`;
            * :any:`full_transformation_monoid_II74`;
            * :any:`full_transformation_monoid_MW24_a`;
            * :any:`full_transformation_monoid_MW24_b`.
)pbdoc");
    m.def("presentation_examples_partial_transformation_monoid",
          &examples::partial_transformation_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partial transformation monoid.

This function returns a monoid presentation defining the partial transformation
monoid of degree *n*, as in Theorem 1.6 of :cite:`Mitchell2024aa`.

:param n: the degree of the partial transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::

      For a specific presentation of the full transformation monoid, see one
      of the following functions:

            * :any:`partial_transformation_monoid_Shu60`;
            * :any:`partial_transformation_monoid_MW24`.
)pbdoc");
    m.def("presentation_examples_symmetric_inverse_monoid",
          &examples::symmetric_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the partial transformation monoid.

This function returns a monoid presentation defining the partial transformation
monoid of degree *n*, as in Theorem 1.6 of :cite:`Mitchell2024aa`.

:param n: the degree of the partial transformation monoid.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::

      For a specific presentation of the full transformation monoid, see one of
      the following functions:

            * :any:`symmetric_inverse_monoid_Sol04`;
            * :any:`symmetric_inverse_monoid_Shu60`;
            * :any:`symmetric_inverse_monoid_MW24`.
)pbdoc");
    m.def("presentation_examples_chinese_monoid",
          &examples::chinese_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the Chinese monoid.

This function returns a monoid presentation defining the Chinese monoid with *n*
generators, as in :cite:`Cassaigne2001aa`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. note::

      This function returns exactly the same presentation as :any:`chinese_monoid_CEKNH01`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_monogenic_semigroup",
          &examples::monogenic_semigroup,
          py::arg("m"),
          py::arg("r"),
          R"pbdoc(
:sig=(m: int, r: int) -> Presentation:
A presentation for a monogenic semigroup.

This function returns a presentation defining the monogenic semigroup defined by
the presentation :math:`\langle a \mid a^{m + r} = a^m \rangle`.

If *m* is 0, the presentation returned is a monoid presentation; otherwise, a
semigroup presentation is returned.

:param m: the index.
:type m: int

:param r: the period.
:type r: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``r == 0``.
)pbdoc");
    m.def("presentation_examples_order_preserving_monoid",
          &examples::order_preserving_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of order preserving mappings.

This function returns a monoid presentation defining the monoid of
order preserving transformations of degree *n*, as described in Section 2 of
:cite:`Arthur2000aa`. This presentation has :math:`2n - 2` generators and
:math:`n^2` relations.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`order_preserving_monoid_AR00`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_cyclic_inverse_monoid",
          &examples::cyclic_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the cyclic inverse monoid.

This function returns a monoid presentation defining the cyclic inverse monoid
of degree *n*, as in Theorem 2.7 of :cite:`Fernandes2022aa`.

This presentation has :math:`2` generators and
:math:`\frac{1}{2}\left(n^2 - n + 6\right)` relations.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::

      For a specific presentation of the full transformation monoid, see one of
      the following functions:

            * :any:`cyclic_inverse_monoid_Fer22_a`;
            * :any:`cyclic_inverse_monoid_Fer22_b`.
)pbdoc");
    m.def("presentation_examples_order_preserving_cyclic_inverse_monoid",
          &examples::order_preserving_cyclic_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the order preserving part of the cyclic inverse monoid.

This function returns a monoid presentation defining the order preserving part
of the cyclic inverse monoid of degree *n*, as in Theorem 2.17 of
:cite:`Fernandes2022aa`.

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`order_preserving_cyclic_inverse_monoid_Fer22`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_partial_isometries_cycle_graph_monoid",
          &examples::partial_isometries_cycle_graph_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the monoid of partial isometries of a cycle graph.

This function returns a monoid presentation defining the monoid of partial
isometries of an :math:`n`-cycle graph, as in Theorem 2.8 of
:cite:`Fernandes2022ab`

:param n: the number of vertices of the cycle graph.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 3``.

.. note::

      This function returns exactly the same presentation as :any:`partial_isometries_cycle_graph_monoid_FP22`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_not_symmetric_group",
          &examples::not_symmetric_group,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A non-presentation for the symmetric group.

This function returns a monoid presentation which is claimed to define the
symmetric group of degree *n*, but does not, as in Section 2.2 of
:cite:`Guralnick2008aa`.

:param n: the claimed degree of the symmetric group.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 4``.

.. note::

      This function returns exactly the same presentation as :any:`not_symmetric_group_GKKL08`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_special_linear_group_2",
          &examples::special_linear_group_2,
          py::arg("q"),
          R"pbdoc(
:sig=(q: int) -> Presentation:
A presentation for the special linear group :math:`\mathrm{SL}(2, q)`.

This function returns a presentation for the special linear group
:math:`\mathrm{SL}(2, q)` (also written :math:`\mathrm{SL(2, \mathbb{Z}_q)}`),
where *q* is an odd prime, as in Theorem 4 of :cite:`Campbell1980aa`.

:param q: the order of the finite field over which the special linear group is
      constructed. This should be an odd prime for the returned presentation to
      define claimed group.
:type q: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``q < 3``.

.. note::

      This function returns exactly the same presentation as :any:`special_linear_group_2_CR80`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_hypo_plactic_monoid",
          &examples::hypo_plactic_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the hypoplactic monoid.

This function returns a presentation for the hypoplactic monoid with *n*
generators, as in Definition 4.2 of :cite:`Novelli2000aa`.

This monoid is a quotient monoid of the plactic monoid, and this presentation
includes the rules from :any:`plactic_monoid_Knu70`.

:param n: the number of generators.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 1``.

.. note::

      This function returns exactly the same presentation as :any:`hypo_plactic_monoid_Nov00`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_sigma_plactic_monoid",
          &examples::sigma_plactic_monoid,
          py::arg("sigma"),
          R"pbdoc(
:sig=(sigma: list[int]) -> Presentation:
A presentation for the :math:`\sigma`-plactic monoid.

This function returns a presentation for the :math:`\sigma`-plactic monoid with
``sigma.size()`` generators, where the image of :math:`\sigma` is given by the
values in *sigma*.

The :math:`\sigma`-plactic monoid is the quotient of the plactic monoid by the
least congruence containing the relation :math:`a^{\sigma(a)} = a` for each
:math:`a` in the alphabet. When :math:`\sigma(a) = 2` for all :math:`a`, the
resultant :math:`\sigma`-plactic monoid is known as the stylic monoid,
and is given in :any:`stylic_monoid`.

:param sigma: a list representing the image of :math:`\sigma`.
:type sigma: list[int]

:returns: The specified presentation.
:rtype: Presentation
)pbdoc");
    m.def("presentation_examples_zero_rook_monoid",
          &examples::zero_rook_monoid,
          py::arg("n"),
          R"pbdoc(
:sig=(n: int) -> Presentation:
A presentation for the :math:`0`-rook monoid.

This function returns a presentation for the :math:`0`-rook monoid of degree
*n*, as in Definition 4.1.1 in :cite:`Gay2018aa`

:param n: the degree.
:type n: int

:returns: The specified presentation.
:rtype: Presentation

:raises LibsemigroupsError:  if ``n < 2``.

.. note::

      This function returns exactly the same presentation as :any:`zero_rook_monoid_Gay18`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_renner_type_B_monoid",
          &examples::renner_type_B_monoid,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation for the Renner monoid of type B.

This functions returns a presentation for the Renner monoid of type B with size
*l* and Iwahori-Hecke deformation *q*.

When ``q == 0``, this corresponds to Definition 8.4.1 and Example 8.4.2 of
:cite:`Gay2018aa`.

When ``q == 1``, this corresponds to Theorem 8.4.19 of :cite:`Gay2018aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.

.. note::

      This function returns exactly the same presentation as :any:`renner_type_B_monoid_Gay18`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_not_renner_type_B_monoid",
          &examples::not_renner_type_B_monoid,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation that incorrectly claims to be the Renner monoid of type B.

This functions returns a presentation that incorrectly claims to be the Renner
monoid of type B with size *l* and Iwahori-Hecke deformation *q*.

When ``q == 0``, this corresponds to Example 7.1.2 of :cite:`Gay2018aa`.

When ``q == 1``, this corresponds to Section 3.2 of :cite:`Godelle2009aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.

.. note::

      This function returns exactly the same presentation as :any:`not_renner_type_B_monoid_Gay18`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_renner_type_D_monoid",
          &examples::renner_type_D_monoid,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation for the Renner monoid of type D.

This functions returns a presentation for the Renner monoid of type D with size
*l* and Iwahori-Hecke deformation *q*.

When ``q == 0``, this corresponds Definition 8.4.22 of :cite:`Gay2018aa`.

When ``q == 1``, this corresponds to Theorem 8.4.43 of :cite:`Gay2018aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.

.. note::

      This function returns exactly the same presentation as :any:`renner_type_D_monoid_Gay18`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
    m.def("presentation_examples_not_renner_type_D_monoid",
          &examples::not_renner_type_D_monoid,
          py::arg("l"),
          py::arg("q"),
          R"pbdoc(
:sig=(l: int, q: int) -> Presentation:
A presentation that incorrectly claims to be the Renner monoid of type D.

This functions returns a presentation that incorrectly claims to be the Renner
monoid of type D with size *l* and Iwahori-Hecke deformation *q*.

When ``q == 1``, this corresponds to Section 3.3 of :cite:`Godelle2009aa`.

:param l: the size of the monoid.
:type l: int

:param q: the Iwahori-Hecke deformation.
:type q: int

:returns: The specified presentation
:rtype: Presentation

:raises LibsemigroupsError:  if ``q != 0`` or ``q != 1``.

.. note::

      This function returns exactly the same presentation as :any:`not_renner_type_D_monoid_God09`,
      and exists as a convenience function for when a presentation for the
      alternating group is required, but the specific presentation
      is not important.
)pbdoc");
  }  // init_presentation_examples

}  // namespace libsemigroups
