
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

  void init_fpsemi_examples(py::module& m) {
    m.def("stellar_monoid_GH19",
          &fpsemigroup::stellar_monoid_GH19,
          py::arg("l"),
          R"pbdoc(
A presentation for the stellar monoid.

:param l: the number of generators.
:type l: int
This function returns a monoid presentation defining the stellar monoid with ``l`` generators, as in Theorem 4.39 of :any:`[24]`.

:raises LibsemigroupsError:  if ``l < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("dual_symmetric_inverse_monoid_EEF07",
          &fpsemigroup::dual_symmetric_inverse_monoid_EEF07,
          py::arg("n"),
          R"pbdoc(
A presentation for the dual symmetric inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the dual symmetric inverse monoid of degree ``n`` , as in Section 3 of :any:`[15]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("uniform_block_bijection_monoid_Fit03",
          &fpsemigroup::uniform_block_bijection_monoid_Fit03,
          py::arg("n"),
          R"pbdoc(
A presentation for the uniform block bijection monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the uniform block bijection monoid of degree ``n`` , as in :any:`[20]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partition_monoid_HR05",
          &fpsemigroup::partition_monoid_HR05,
          py::arg("n"),
          R"pbdoc(
A presentation for the partition monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the partition monoid of degree ``n`` , as in :any:`[27]`.

:raises LibsemigroupsError:  if ``n < 1``.

.. seealso::  ``partition_monoid`` and ``partition_monoid_Eas11``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partition_monoid_Eas11",
          &fpsemigroup::partition_monoid_Eas11,
          py::arg("n"),
          R"pbdoc(
A presentation for the partition monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the partition monoid of degree ``n`` , as in Theorem 41 of :any:`[16]`.

:raises LibsemigroupsError:  if ``n < 4``.

.. seealso::  ``partition_monoid`` and ``partition_monoid_HR05``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("singular_brauer_monoid_MM07",
          &fpsemigroup::singular_brauer_monoid_MM07,
          py::arg("n"),
          R"pbdoc(
A presentation for the singular part of the Brauer monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation for the singular part of the Brauer monoid of degree ``n`` , as in Theorem 5 of :any:`[37]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("orientation_preserving_monoid_AR00",
          &fpsemigroup::orientation_preserving_monoid_AR00,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of orientation preserving mappings.

:param n: the order of the chain.
:type n: int
This function returns a monoid presentation defining the monoid of orientation preserving mappings on a finite chain of order ``n`` , as described in :any:`[5]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("orientation_preserving_reversing_monoid_AR00",
          &fpsemigroup::orientation_preserving_reversing_monoid_AR00,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of orientation preserving or reversing mappings.

:param n: the order of the chain.
:type n: int
This function returns a monoid presentation defining the monoid of orientation preserving or reversing mappings on a finite chain of order ``n`` , as described in :any:`[5]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("temperley_lieb_monoid_Eas21",
          &fpsemigroup::temperley_lieb_monoid_Eas21,
          py::arg("n"),
          R"pbdoc(
A presentation for the Temperley-Lieb monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the Temperley-Lieb monoid with ``n`` generators, as described in Theorem 2.2 of :any:`[17]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("brauer_monoid_KM07",
          &fpsemigroup::brauer_monoid_KM07,
          py::arg("n"),
          R"pbdoc(
A presentation for the Brauer monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the Brauer monoid of degree ``n`` , as described in Theorem 3.1 of :any:`[36]`.

:raises LibsemigroupsError:  if ``n < 1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_brauer_monoid_KM07",
          &fpsemigroup::partial_brauer_monoid_KM07,
          py::arg("n"),
          R"pbdoc(
A presentation for the partial Brauer monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the partial Brauer monoid of degree ``n`` , as described in Theorem 5.1 of :any:`[36]`.

:raises LibsemigroupsError:  if ``n < 1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("motzkin_monoid_PHL13",
          &fpsemigroup::motzkin_monoid_PHL13,
          py::arg("n"),
          R"pbdoc(
A presentation for the Motzkin monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the Motzkin monoid of degree ``n`` , as described in Theorem 4.1 of :any:`[43]` , with the additional relations :math:` r_i t_i l_i = r_i
^ 2 ` added to fix a hole in Lemma 4.10 which rendered the presentation as stated in the paper incorrect.

:raises LibsemigroupsError:  if ``n < 1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("fibonacci_semigroup_CRRT94",
          &fpsemigroup::fibonacci_semigroup_CRRT94,
          py::arg("r"),
          py::arg("n"),
          R"pbdoc(
A presentation for a Fibonacci semigroup.

:param r: the length of the left hand sides of the relations. 
:type r: int

:param n: the number of generators.
:type n: int
This function returns a semigroup presentation defining the Fibonacci semigroup :math:`F(r, n)` , where :math:`r` is ``r`` and :math:`n` is ``n`` , as described in :any:`[10]`.

:raises LibsemigroupsError:  if ``n < 1``.

:raises LibsemigroupsError:  if ``r < 1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("plactic_monoid_Knu70",
          &fpsemigroup::plactic_monoid_Knu70,
          py::arg("n"),
          R"pbdoc(
A presentation for the plactic monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the plactic monoid with ``n`` generators, as in see Theorem 6 of :any:`[35]`.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("stylic_monoid_AR22",
          &fpsemigroup::stylic_monoid_AR22,
          py::arg("n"),
          R"pbdoc(
A presentation for the stylic monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the stylic monoid with ``n`` generators, as in Theorem 8.1 of :any:`[1]`.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_group_Bur12",
          &fpsemigroup::symmetric_group_Bur12,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric group.

:param n: the degree of the symmetric group.
:type n: int
This function returns a monoid presentation for the symmetric group of degree ``n`` , as in p.464 of :any:`[8]` . This presentation has :math:`n - 1` generators and :math:`n^3 - 5n^2 + 9n - 5` relations.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_group_Car56",
          &fpsemigroup::symmetric_group_Car56,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric group.

:param n: the degree of the symmetric group.
:type n: int
This function returns a monoid presentation for the symmetric group of degree ``n`` , as on page 169 of :any:`[11]` . This presentation has :math:`n - 1` generators and :math:`(n - 1)^2` relations.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_group_Moo97_a",
          &fpsemigroup::symmetric_group_Moo97_a,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric group.

:param n: the degree of the symmetric group.
:type n: int
This function returns a monoid presentation for the symmetric group of degree ``n`` , as in Theorem A of :any:`[41]` . This presentation has :math:`n - 1` generators and :math:`\frac{1}{2}n(n - 1)` relations.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_group_Moo97_b",
          &fpsemigroup::symmetric_group_Moo97_b,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric group.

:param n: the degree of the symmetric group.
:type n: int
This function returns a monoid presentation for the symmetric group of degree ``n`` , as in in Theorem A' of :any:`[41]` . This presentation has :math:`2` generators and :math:`n + 1` relations for :math:`n \geq 4` . If :math:`n<4` then there are :math:`4` relations.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("alternating_group_Moo97",
          &fpsemigroup::alternating_group_Moo97,
          py::arg("n"),
          R"pbdoc(
A presentation for the alternating group.

:param n: the degree of the alternating group.
:type n: int
This function returns a monoid presentation defining the alternating group of degree ``n`` , as in Theorem B of :any:`[41]`.

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("rectangular_band_ACOR00",
          &fpsemigroup::rectangular_band_ACOR00,
          py::arg("m"),
          py::arg("n"),
          R"pbdoc(
A presentation for a rectangular band.

:param m: the number of rows. 
:type m: int

:param n: the number of columns.
:type n: int
This function returns a semigroup presentation defining the ``m`` by ``n`` rectangular band, as given in Proposition 4.2 of :any:`[6]`.

:raises LibsemigroupsError:  if ``m == 0``.

:raises LibsemigroupsError:  if ``n == 0``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("full_transformation_monoid_Aiz58",
          &fpsemigroup::full_transformation_monoid_Aiz58,
          py::arg("n"),
          R"pbdoc(
A presentation for the full transformation monoid.

:param n: the degree of the full transformation monoid.
:type n: int
This function returns a monoid presentation defining the full transformation monoid of degree ``n`` , as in Section 5, Theorem 2 of :any:`[3]` (Russian) and Chapter 3, Proposition 1.7 of :any:`[45]` (English).

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("full_transformation_monoid_II74",
          &fpsemigroup::full_transformation_monoid_II74,
          py::arg("n"),
          R"pbdoc(
A presentation for the full transformation monoid.

:param n: the degree of the full transformation monoid.
:type n: int
This function returns a monoid presentation defining the full transformation monoid of degree ``n`` due to Iwahori and Iwahori :any:`[31]` , as in Theorem 9.3.1 of :any:`[23]`.

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("full_transformation_monoid_MW24_a",
          &fpsemigroup::full_transformation_monoid_MW24_a,
          py::arg("n"),
          R"pbdoc(
A presentation for the full transformation monoid.

:param n: the degree of the full transformation monoid.
:type n: int
This function returns a monoid presentation defining the full transformation monoid of degree ``n`` , corresponding to :math:`\mathcal{T}` in Theorem 1.5 of :any:`[40]` . For ``n >= 4`` this presentation has five non-symmetric-group relations.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("full_transformation_monoid_MW24_b",
          &fpsemigroup::full_transformation_monoid_MW24_b,
          py::arg("n"),
          R"pbdoc(
A presentation for the full transformation monoid.

:param n: the degree of the full transformation monoid.
:type n: int
This function returns a monoid presentation defining the full transformation monoid of degree ``n`` , corresponding to :math:`\mathcal{T}'` in Theorem 1.5 of :any:`[40]` . This presentation is only valid for odd values of ``n`` , and for ``n >= 5`` this presentation has four non-symmetric-group relations.

:raises LibsemigroupsError:  if ``n < 3``.

:raises LibsemigroupsError:  if ``n`` is not odd.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_transformation_monoid_Shu60",
          &fpsemigroup::partial_transformation_monoid_Shu60,
          py::arg("n"),
          R"pbdoc(
A presentation for the partial transformation monoid.

:param n: the degree of the partial transformation monoid.
:type n: int
This function returns a monoid presentation defining the partial transformation monoid of degree ``n`` due to Shutov :any:`[46]` , as in Theorem 9.4.1 of :any:`[23]`.

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_transformation_monoid_MW24",
          &fpsemigroup::partial_transformation_monoid_MW24,
          py::arg("n"),
          R"pbdoc(
A presentation for the partial transformation monoid.

:param n: the degree of the partial transformation monoid.
:type n: int
This function returns a monoid presentation defining the partial transformation monoid of degree ``n`` , as in Theorem 1.6 of :any:`[40]`.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_inverse_monoid_Gay18",
          &fpsemigroup::symmetric_inverse_monoid_Gay18,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric inverse monoid.

:param n: the degree of the symmetric inverse monoid.
:type n: int
This function returns a monoid presentation defining the symmetric inverse monoid of degree ``n`` , as in Example 7.1.2 of :any:`[25]`.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_inverse_monoid_Shu60",
          &fpsemigroup::symmetric_inverse_monoid_Shu60,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric inverse monoid.

:param n: the degree of the symmetric inverse monoid.
:type n: int
This function returns a monoid presentation defining the symmetric inverse monoid of degree ``n`` due to Shutov :any:`[46]` , as in Theorem 9.2.2 of :any:`[23]`.

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_inverse_monoid_MW24",
          &fpsemigroup::symmetric_inverse_monoid_MW24,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric inverse monoid.

:param n: the degree of the symmetric inverse monoid.
:type n: int
This function returns a monoid presentation defining the partial transformation monoid of degree ``n`` , as in Theorem 1.4 of :any:`[40]`.

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("chinese_monoid_CEKNH01",
          &fpsemigroup::chinese_monoid_CEKNH01,
          py::arg("n"),
          R"pbdoc(
A presentation for the Chinese monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the Chinese monoid with ``n`` generators, as in :any:`[13]`.

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("order_preserving_monoid_AR00",
          &fpsemigroup::order_preserving_monoid_AR00,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of order-preserving mappings.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the monoid of order-preserving transformations of degree ``n`` , as described in Section 2 of :any:`[5]`.This presentation has :math:`2n - 2` generators and :math:`n^2` relations.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("cyclic_inverse_monoid_Fer22_a",
          &fpsemigroup::cyclic_inverse_monoid_Fer22_a,
          py::arg("n"),
          R"pbdoc(
A presentation for the cyclic inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the cyclic inverse monoid of degree ``n`` , as in Theorem 2.6 of :any:`[19]`.This has :math:`n + 1` generators and :math:`\frac{1}{2} \left(n^2 + 3n +
4\right)` relations.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("cyclic_inverse_monoid_Fer22_b",
          &fpsemigroup::cyclic_inverse_monoid_Fer22_b,
          py::arg("n"),
          R"pbdoc(
A presentation for the cyclic inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the cyclic inverse monoid of degree ``n`` , as in Theorem 2.7 of :any:`[19]`.This presentation has :math:`2` generators and :math:`\frac{1}{2}\left(n^2 - n
+ 6\right)` relations.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("order_preserving_cyclic_inverse_monoid_Fer22",
          &fpsemigroup::order_preserving_cyclic_inverse_monoid_Fer22,
          py::arg("n"),
          R"pbdoc(
A presentation for the order-preserving part of the cyclic inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the order-preserving part of the cyclic inverse monoid of degree ``n`` , as in Theorem 2.17 of :any:`[19]`.

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_isometries_cycle_graph_monoid_FP22",
          &fpsemigroup::partial_isometries_cycle_graph_monoid_FP22,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of partial isometries of a cycle graph.

:param n: the number of vertices of the cycle graph.
:type n: int
This function returns a monoid presentation defining the monoid of partial isometries of an :math:`n` -cycle graph, as in Theorem 2.8 of :any:`[18]`

:raises LibsemigroupsError:  if ``n < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_symmetric_group_GKKL08",
          &fpsemigroup::not_symmetric_group_GKKL08,
          py::arg("n"),
          R"pbdoc(
A non-presentation for the symmetric group.

:param n: the claimed degree of the symmetric group.
:type n: int
This function returns a monoid presentation which is claimed to define the symmetric group of degree ``n`` , but does not, as in Section 2.2 of Guralnick2008aa.

:raises LibsemigroupsError:  if ``n < 4``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("special_linear_group_2_CR80",
          &fpsemigroup::special_linear_group_2_CR80,
          py::arg("q"),
          R"pbdoc(
A presentation for the special linear group $\mathrm{SL}(2, q))$.

:param q: the order of the finite field over which the special linear group is constructed. This should be an odd prime for the returned presentation to define claimed group.
:type q: int
This function returns a presentation for the special linear group :math:`\mathrm{SL}(2, q)` (also written :math:`\mathrm{SL(2,
\mathbb{Z}_q)}` ), where ``q`` is an odd prime, as in Theorem 4 of :any:`[9]`.

:raises LibsemigroupsError:  if ``q < 3``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("hypo_plactic_monoid_Nov00",
          &fpsemigroup::hypo_plactic_monoid_Nov00,
          py::arg("n"),
          R"pbdoc(
A presentation for the hypoplactic monoid.

:param n: the number of generators.
:type n: int
This function returns a presentation for the hypoplactic monoid with ``n`` generators, as in Definition 4.2 of :any:`[42]`.This monoid is a quotient monoid of the plactic monoid, and this presentation includes the rules from :any:`plactic_monoid_Knu70`.

:raises LibsemigroupsError:  if ``n < 1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("zero_rook_monoid_Gay18",
          &fpsemigroup::zero_rook_monoid_Gay18,
          py::arg("n"),
          R"pbdoc(
A presentation for the $0$-rook monoid.

:param n: the degree.
:type n: int
This function returns a presentation for the :math:`0` -rook monoid of degree ``n`` , as in Definition 4.1.1 in :any:`[25]`

:raises LibsemigroupsError:  if ``n < 2``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("renner_type_B_monoid_Gay18_a",
          &fpsemigroup::renner_type_B_monoid_Gay18_a,
          py::arg("l"),
          R"pbdoc(
A presentation for the Renner monoid of type B.

:param l: the size of the monoid.
:type l: int
This functions returns a presentation for the Renner monoid of type B, as in Definition 8.4.1 and Example 8.4.2 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("renner_type_B_monoid_Gay18_b",
          &fpsemigroup::renner_type_B_monoid_Gay18_b,
          py::arg("l"),
          R"pbdoc(
A presentation for the Renner monoid of type B.

:param l: the size of the monoid.
:type l: int
This functions returns a presentation for the Renner monoid of type B, as in Theorem 8.4.19 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_renner_type_B_monoid_Gay18",
          &fpsemigroup::not_renner_type_B_monoid_Gay18,
          py::arg("l"),
          R"pbdoc(
A presentation that incorrectly claims to be the Renner monoid of type B.

:param l: the size of the claimed monoid.
:type l: int
This functions returns a presentation that incorrectly claims to be the Renner monoid of type B, as in Example 7.1.2 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_renner_type_B_monoid_God09",
          &fpsemigroup::not_renner_type_B_monoid_God09,
          py::arg("l"),
          R"pbdoc(
A presentation that incorrectly claims to be the Renner monoid of type B.

:param l: the size of the claimed monoid.
:type l: int
This functions returns a presentation that incorrectly claims to be the Renner monoid of type B, as in Section 3.2 of :any:`[26]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("renner_type_D_monoid_Gay18_a",
          &fpsemigroup::renner_type_D_monoid_Gay18_a,
          py::arg("l"),
          R"pbdoc(
A presentation for the Renner monoid of type D.

:param l: the size of the monoid.
:type l: int
This functions returns a presentation for the Renner monoid of type D, as in Definition 8.4.22 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("renner_type_D_monoid_Gay18_b",
          &fpsemigroup::renner_type_D_monoid_Gay18_b,
          py::arg("l"),
          R"pbdoc(
A presentation for the Renner monoid of type D.

:param l: the size of the monoid.
:type l: int
This functions returns a presentation for the Renner monoid of type D, as in Theorem 8.4.43 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_renner_type_D_monoid_Machine",
          &fpsemigroup::not_renner_type_D_monoid_Machine,
          py::arg("l"),
          R"pbdoc(
A presentation that incorrectly claims to be the Renner monoid of type D.

:param l: the size of the claimed monoid.
:type l: int
This functions returns a presentation that incorrectly claims to be the Renner monoid of type D.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_renner_type_D_monoid_God09",
          &fpsemigroup::not_renner_type_D_monoid_God09,
          py::arg("l"),
          R"pbdoc(
A presentation that incorrectly claims to be the Renner monoid of type D.

:param l: the size of the claimed monoid.
:type l: int
This functions returns a presentation that incorrectly claims to be the Renner monoid of type D, as in Section 3.3 of :any:`[26]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("stellar_monoid",
          &fpsemigroup::stellar_monoid,
          py::arg("l"),
          R"pbdoc(
A presentation for the stellar monoid.

:param l: the number of generators.
:type l: int
This function returns a monoid presentation defining the stellar monoid with ``l`` generators, as in Theorem 4.39 of :any:`[24]`.

:raises LibsemigroupsError:  if ``l < 2``.

.. seealso::  ``stellar_monoid_GH19``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("dual_symmetric_inverse_monoid",
          &fpsemigroup::dual_symmetric_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the dual symmetric inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the dual symmetric inverse monoid of degree ``n`` , as in Section 3 of :any:`[15]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``dual_symmetric_inverse_monoid``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("uniform_block_bijection_monoid",
          &fpsemigroup::uniform_block_bijection_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the uniform block bijection monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the uniform block bijection monoid of degree ``n`` , as in :any:`[20]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``uniform_block_bijection_monoid_Fit03``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partition_monoid",
          &fpsemigroup::partition_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the partition monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the partition monoid of degree ``n`` , as in Theorem 41 of :any:`[16]`.

:raises LibsemigroupsError:  if ``n < 4``.

.. seealso::  ``partition_monoid_Eas11`` and ``partition_monoid_HR05``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("singular_brauer_monoid",
          &fpsemigroup::singular_brauer_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the singular part of the Brauer monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation for the singular part of the Brauer monoid of degree ``n`` , as in Theorem 5 of :any:`[37]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``singular_brauer_monoid_MM07``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("orientation_preserving_monoid",
          &fpsemigroup::orientation_preserving_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of orientation preserving mappings.

:param n: the order of the chain.
:type n: int
This function returns a monoid presentation defining the monoid of orientation preserving mappings on a finite chain of order ``n`` , as described in :any:`[5]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``orientation_preserving_monoid_AR00``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("orientation_preserving_reversing_monoid",
          &fpsemigroup::orientation_preserving_reversing_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of orientation preserving or reversing mappings.

:param n: the order of the chain.
:type n: int
This function returns a monoid presentation defining the monoid of orientation preserving or reversing mappings on a finite chain of order ``n`` , as described in :any:`[5]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``orientation_preserving_reversing_monoid_AR00``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("temperley_lieb_monoid",
          &fpsemigroup::temperley_lieb_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the Temperley-Lieb monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the Temperley-Lieb monoid with ``n`` generators, as described in Theorem 2.2 of :any:`[17]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``temperley_lieb_monoid_Eas21``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("brauer_monoid",
          &fpsemigroup::brauer_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the Brauer monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the Brauer monoid of degree ``n`` , as described in Theorem 3.1 of :any:`[36]`.

:raises LibsemigroupsError:  if ``n < 1``.

.. seealso::  ``brauer_monoid_KM07``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_brauer_monoid",
          &fpsemigroup::partial_brauer_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the partial Brauer monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the partial Brauer monoid of degree ``n`` , as described in Theorem 5.1 of :any:`[36]`.

:raises LibsemigroupsError:  if ``n < 1``.

.. seealso::  ``partial_brauer_monoid_KM07``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("motzkin_monoid",
          &fpsemigroup::motzkin_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the Motzkin monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the Motzkin monoid of degree ``n`` , as described in Theorem 4.1 of :any:`[43]` , with the additional relations :math:` r_i t_i l_i = r_i
^ 2 ` added to fix a hole in Lemma 4.10 which rendered the presentation as stated in the paper incorrect.

:raises LibsemigroupsError:  if ``n < 1``.

.. seealso::  ``motzkin_monoid_PHL13``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("fibonacci_semigroup",
          &fpsemigroup::fibonacci_semigroup,
          py::arg("r"),
          py::arg("n"),
          R"pbdoc(
A presentation for a Fibonacci semigroup.

:param r: the length of the left hand sides of the relations. 
:type r: int

:param n: the number of generators.
:type n: int
This function returns a semigroup presentation defining the Fibonacci semigroup :math:`F(r, n)` , where :math:`r` is ``r`` and :math:`n` is ``n`` , as described in :any:`[10]`.

:raises LibsemigroupsError:  if ``n < 1``.

:raises LibsemigroupsError:  if ``r < 1``.

.. seealso::  ``fibonacci_semigroup_CRRT94``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("plactic_monoid",
          &fpsemigroup::plactic_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the plactic monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the plactic monoid with ``n`` generators, as in see Theorem 6 of :any:`[35]`.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::  ``plactic_monoid_Knu70``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("stylic_monoid",
          &fpsemigroup::stylic_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the stylic monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the stylic monoid with ``n`` generators, as in Theorem 8.1 of :any:`[1]`.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::  ``stylic_monoid_AR22``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_group",
          &fpsemigroup::symmetric_group,
          py::arg("n"),
          R"pbdoc(
A presentation for the symmetric group.

:param n: the degree of the symmetric group.
:type n: int
This function returns a monoid presentation for the symmetric group of degree ``n`` , as on page 169 of :any:`[11]` . This presentation has :math:`n - 1` generators and :math:`(n - 1)^2` relations.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso:: For a different presentation of the symmetric group, see one of the following functions:

*  ``symmetric_group_Bur12`` ;
*  ``symmetric_group_Car56`` ;
*  ``symmetric_group_Moo97_a`` ;
*  ``symmetric_group_Moo97_b``.




:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("alternating_group",
          &fpsemigroup::alternating_group,
          py::arg("n"),
          R"pbdoc(
A presentation for the alternating group.

:param n: the degree of the alternating group.
:type n: int
This function returns a monoid presentation defining the alternating group of degree ``n`` , as in Theorem B of :any:`[41]`.

:raises LibsemigroupsError:  if ``n < 4``.

.. seealso::  ``alternating_group_Moo97``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("rectangular_band",
          &fpsemigroup::rectangular_band,
          py::arg("m"),
          py::arg("n"),
          R"pbdoc(
A presentation for a rectangular band.

:param m: the number of rows. 
:type m: int

:param n: the number of columns.
:type n: int
This function returns a semigroup presentation defining the ``m`` by ``n`` rectangular band, as given in Proposition 4.2 of :any:`[6]`.

:raises LibsemigroupsError:  if ``m == 0``.

:raises LibsemigroupsError:  if ``n == 0``.

.. seealso::  ``rectangular_band_ACOR00``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("full_transformation_monoid",
          &fpsemigroup::full_transformation_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the full transformation monoid.

:param n: the degree of the full transformation monoid.
:type n: int
This function returns a monoid presentation defining the full transformation monoid of degree ``n`` , corresponding to :math:`\mathcal{T}` in Theorem 1.5 of :any:`[40]` . For ``n >= 4`` this presentation has five non-symmetric-group relations.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso:: For a different presentation of the full transformation monoid, see one of the following functions:

*  ``full_transformation_monoid_Aiz58`` ;
*  ``full_transformation_monoid_II74`` ;
*  ``full_transformation_monoid_MW24_a`` ;
*  ``full_transformation_monoid_MW24_b``.




:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_transformation_monoid",
          &fpsemigroup::partial_transformation_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the partial transformation monoid.

:param n: the degree of the partial transformation monoid.
:type n: int
This function returns a monoid presentation defining the partial transformation monoid of degree ``n`` , as in Theorem 1.6 of :any:`[40]`.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso:: For a different presentation of the full transformation monoid, see one of the following functions:

*  ``partial_transformation_monoid_Shu60`` ;
*  ``partial_transformation_monoid_MW24``.




:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("symmetric_inverse_monoid",
          &fpsemigroup::symmetric_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the partial transformation monoid.

:param n: the degree of the partial transformation monoid.
:type n: int
This function returns a monoid presentation defining the partial transformation monoid of degree ``n`` , as in Theorem 1.6 of :any:`[40]`.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso:: For a different presentation of the full transformation monoid, see one of the following functions:

*  ``symmetric_inverse_monoid_Gay18`` ;
*  ``symmetric_inverse_monoid_Shu60`` ;
*  ``symmetric_inverse_monoid_MW24``.




:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("chinese_monoid",
          &fpsemigroup::chinese_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the Chinese monoid.

:param n: the number of generators.
:type n: int
This function returns a monoid presentation defining the Chinese monoid with ``n`` generators, as in :any:`[13]`.

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::  ``chinese_monoid_CEKNH01``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("monogenic_semigroup",
          &fpsemigroup::monogenic_semigroup,
          py::arg("m"),
          py::arg("r"),
          R"pbdoc(
A presentation for a monogenic semigroup.

:param m: the index. 
:type m: int

:param r: the period.
:type r: int
This function returns a presentation defining the monogenic semigroup defined by the presentation :math:`\langle a \mid a^{m + r} = a^m
\rangle`.If ``m`` is 0, the presentation returned is a monoid presentation; otherwise, a semigroup presentation is returned.

:raises LibsemigroupsError:  if ``r == 0``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("order_preserving_monoid",
          &fpsemigroup::order_preserving_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of order-preserving mappings.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the monoid of order-preserving transformations of degree ``n`` , as described in Section 2 of :any:`[5]`.This presentation has :math:`2n - 2` generators and :math:`n^2` relations.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``order_preserving_monoid_AR00``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("cyclic_inverse_monoid",
          &fpsemigroup::cyclic_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the cyclic inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the cyclic inverse monoid of degree ``n`` , as in Theorem 2.7 of :any:`[19]`.This presentation has :math:`2` generators and :math:`\frac{1}{2}\left(n^2 - n
+ 6\right)` relations.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso:: For a different presentation of the full transformation monoid, see one of the following functions:

*  ``cyclic_inverse_monoid_Fer22_a`` ;
*  ``cyclic_inverse_monoid_Fer22_b``.




:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("order_preserving_cyclic_inverse_monoid",
          &fpsemigroup::order_preserving_cyclic_inverse_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the order-preserving part of the cyclic inverse monoid.

:param n: the degree.
:type n: int
This function returns a monoid presentation defining the order-preserving part of the cyclic inverse monoid of degree ``n`` , as in Theorem 2.17 of :any:`[19]`.

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``order_preserving_cyclic_inverse_monoid_Fer22``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("partial_isometries_cycle_graph_monoid",
          &fpsemigroup::partial_isometries_cycle_graph_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the monoid of partial isometries of a cycle graph.

:param n: the number of vertices of the cycle graph.
:type n: int
This function returns a monoid presentation defining the monoid of partial isometries of an :math:`n` -cycle graph, as in Theorem 2.8 of :any:`[18]`

:raises LibsemigroupsError:  if ``n < 3``.

.. seealso::  ``partial_isometries_cycle_graph_monoid_FP22``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_symmetric_group",
          &fpsemigroup::not_symmetric_group,
          py::arg("n"),
          R"pbdoc(
A non-presentation for the symmetric group.

:param n: the claimed degree of the symmetric group.
:type n: int
This function returns a monoid presentation which is claimed to define the symmetric group of degree ``n`` , but does not, as in Section 2.2 of Guralnick2008aa.

:raises LibsemigroupsError:  if ``n < 4``.

.. seealso::  ``not_symmetric_group_GKKL08``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("special_linear_group_2",
          &fpsemigroup::special_linear_group_2,
          py::arg("q"),
          R"pbdoc(
A presentation for the special linear group $\mathrm{SL}(2, q))$.

:param q: the order of the finite field over which the special linear group is constructed. This should be an odd prime for the returned presentation to define claimed group.
:type q: int
This function returns a presentation for the special linear group :math:`\mathrm{SL}(2, q)` (also written :math:`\mathrm{SL(2,
\mathbb{Z}_q)}` ), where ``q`` is an odd prime, as in Theorem 4 of :any:`[9]`.

:raises LibsemigroupsError:  if ``q < 3``.

.. seealso::  ``special_linear_group_2_CR80``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("hypo_plactic_monoid",
          &fpsemigroup::hypo_plactic_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the hypoplactic monoid.

:param n: the number of generators.
:type n: int
This function returns a presentation for the hypoplactic monoid with ``n`` generators, as in Definition 4.2 of :any:`[42]`.This monoid is a quotient monoid of the plactic monoid, and this presentation includes the rules from :any:`plactic_monoid_Knu70`.

:raises LibsemigroupsError:  if ``n < 1``.

.. seealso::  ``hypo_plactic_monoid_Nov00``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("sigma_plactic_monoid",
          &fpsemigroup::sigma_plactic_monoid,
          py::arg("sigma"),
          R"pbdoc(
A presentation for the $\sigma$-plactic monoid.

:param sigma: a vector representing the image of $\sigma$.
:type sigma: list
This function returns a presentation for the :math:`\sigma` -plactic monoid with ``sigma.size()`` generators, where the image of :math:`\sigma` is given by the values in ``sigma``.The :math:`\sigma` -plactic monoid is the quotient of the plactic monoid by the least congruence containing the relation :math:`a^{\sigma(a)} = a` for each :math:`a` in the alphabet. When :math:`\sigma(a) = 2` for all :math:`a` , the resultant :math:`\sigma` -plactic monoid is known as the stylic monoid, as studied in :any:`[1]`.

:raises LibsemigroupsError:  if ``n < 1``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("zero_rook_monoid",
          &fpsemigroup::zero_rook_monoid,
          py::arg("n"),
          R"pbdoc(
A presentation for the $0$-rook monoid.

:param n: the degree.
:type n: int
This function returns a presentation for the :math:`0` -rook monoid of degree ``n`` , as in Definition 4.1.1 in :any:`[25]`

:raises LibsemigroupsError:  if ``n < 2``.

.. seealso::  ``zero_rook_monoid_Gay18``.


:returns: A value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("renner_type_B_monoid",
          &fpsemigroup::renner_type_B_monoid,
          py::arg("l"),
          R"pbdoc(
A presentation for the Renner monoid of type B.

:param l: the size of the monoid.
:type l: int
This functions returns a presentation for the Renner monoid of type B, as in Definition 8.4.1 and Example 8.4.2 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

.. seealso:: For a different presentation of the Renner monoid of type B, see one of the following functions:

*  ``renner_type_B_monoid_Gay18_a`` , corresponding to Iwahori-Hecke deformation of 0;
*  ``renner_type_B_monoid_Gay18_b`` , corresponding to Iwahori-Hecke deformation of 1.




:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_renner_type_B_monoid",
          &fpsemigroup::not_renner_type_B_monoid,
          py::arg("l"),
          R"pbdoc(
A presentation that incorrectly claims to be the Renner monoid of type B.

:param l: the size of the claimed monoid.
:type l: int
This functions returns a presentation that incorrectly claims to be the Renner monoid of type B, as in Example 7.1.2 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

.. seealso:: For a different presentation that incorrectly claims to be the Renner monoid of type B, see one of the following functions:

*  ``not_renner_type_B_monoid_Gay18`` , corresponding to Iwahori-Hecke deformation of 0;
*  ``not_renner_type_B_monoid_God09`` , corresponding to Iwahori-Hecke deformation of 1.




:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("renner_type_D_monoid",
          &fpsemigroup::renner_type_D_monoid,
          py::arg("l"),
          R"pbdoc(
A presentation for the Renner monoid of type D.

:param l: the size of the monoid.
:type l: int
This functions returns a presentation for the Renner monoid of type D, as in Definition 8.4.22 of :any:`[25]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

.. seealso:: For a different presentation of the Renner monoid of type D, see one of the following functions:

*  ``renner_type_D_monoid_Gay18_a`` , corresponding to Iwahori-Hecke deformation of 0;
*  ``renner_type_D_monoid_Gay18_b`` , corresponding to Iwahori-Hecke deformation of 1.




:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
    m.def("not_renner_type_D_monoid",
          &fpsemigroup::not_renner_type_D_monoid,
          py::arg("l"),
          R"pbdoc(
A presentation that incorrectly claims to be the Renner monoid of type D.

:param l: the size of the claimed monoid.
:type l: int
This functions returns a presentation that incorrectly claims to be the Renner monoid of type D, as in Section 3.3 of :any:`[26]`.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

.. seealso:: For a different presentation that incorrectly claims to be the Renner monoid of type D, see one of the following functions:

*  ``not_renner_type_D_monoid_Machine`` , corresponding to Iwahori-Hecke deformation of 0;
*  ``not_renner_type_D_monoid_God09`` , corresponding to Iwahori-Hecke deformation of 1.




:returns:  a value of type ``Presentation``.

:rtype: Presentation
)pbdoc");
  }  // init_fpsemi_examples

}  // namespace libsemigroups
