
//
// libsemigroups_pybind11
// Copyright (C) 2024 Joseph Edwards
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

// TODO(0) Check types

// C std headers....
// TODO complete or delete

// C++ stl headers....
#include <memory>  // for allocator, make_unique, unique_ptr

// libsemigroups headers
#include <libsemigroups/libsemigroups.hpp>
#include <libsemigroups/schreier-sims.hpp>

// pybind11....
#include <pybind11/pybind11.h>
// #include <pybind11/chrono.h>
// #include <pybind11/functional.h>
// #include <pybind11/stl.h>
// TODO uncomment/delete

// libsemigroups_pybind11....
#include "main.hpp"  // for init_schreier_sims

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <size_t N, typename Point, typename Element>
    void bind_schreier_sims(py::module& m, std::string const& name) {
      using SchreierSims_ = SchreierSims<N, Point, Element>;

      std::string pyclass_name = std::string("SchreierSims") + name;

      py::class_<SchreierSims_, std::unique_ptr<SchreierSims_>> thing(
          m,
          pyclass_name.c_str(),
          R"pbdoc(
This class implements a deterministic version of the Schreier-Sims algorithm
acting on a relatively small number of points (< 1000).

:example: 

.. doctest:: python

    >>> from libsemigroups_pybind11 import SchreierSims, Perm
    >>> SchreierSims S
    >>> S.add_generator(Perm([1,  0,  2,  3,  4]))
    >>> S.add_generator(Perm([1,  2,  3,  4,  0]))
    >>> S.size()
    120
)pbdoc");
      thing.def("__repr__", [](SchreierSims_ const& S) {
        return to_human_readable_repr(S);
      });
      thing.def(py::init<>(), R"pbdoc(
Default constructor.

Construct a :any:`SchreierSims` object representing the trivial group.

:complexity: Constant.
)pbdoc");

      thing.def(py::init<SchreierSims_ const&>(), R"pbdoc(
      Default copy constructor.
)pbdoc");
      thing.def("__copy__",
                [](SchreierSims_ const& S) { return SchreierSims_(S); });
      thing.def("add_base_point",
                &SchreierSims_::add_base_point,
                py::arg("pt"),
                R"pbdoc(
Add a base point to the stabiliser chain.

:param pt: the base point to add.
:type pt: point_type
Add a base point to the stabiliser chain.

:raises LibsemigroupsError:  if ``pt`` is out of range.

:raises LibsemigroupsError:  if :any:`finished()` returns ``True``.

:raises LibsemigroupsError:  if ``pt`` is already a base point.

:complexity: Linear in the current number of base points.)pbdoc");
      thing.def("add_generator",
                &SchreierSims_::add_generator,
                py::arg("x"),
                R"pbdoc(
Add a generator.

:param x: a const reference to the generator to add.
:type x: const_element_reference
This functions adds the argument ``x`` as a new generator if and only if ``x`` is not already an element of the group represented by the Schreier-Sims object.

:raises LibsemigroupsError:  if the degree of ``x`` is not equal to the first template parameter ``N`` , or if ``self`` already contains the maximum number of elements.

:complexity: Constant


:returns:  ``True`` if ``x`` is added as a generator and ``False`` if it is not.

:rtype: bool
)pbdoc");
      thing.def("base",
                &SchreierSims_::base,
                py::arg("index"),
                R"pbdoc(
Get a base point.

:param index: the index of the base point.
:type index: index_type
Get a base point, having checked ``index`` if not out of range.

:raises LibsemigroupsError:  if ``index`` is out of range.

:complexity: Constant.


:returns:  the base point with index ``index``.

:rtype: point_type
)pbdoc");
      thing.def("base_size",
                &SchreierSims_::base_size,
                R"pbdoc(
Get the size of the current base.
Get the size of the current base.

:exceptions: This function is ``noexcept`` and is guaranteed never to throw.

:complexity: Constant.

:returns: A ``int``.

:rtype: int
)pbdoc");
      thing.def("contains",
                &SchreierSims_::contains,
                py::arg("x"),
                R"pbdoc(
Test membership of an element.

:param x: a const reference to the possible element.
:type x: const_element_reference
Test membership of an element.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns: A ``bool``.

:rtype: bool
)pbdoc");
      thing.def("currently_contains",
                &SchreierSims_::currently_contains,
                py::arg("x"),
                R"pbdoc(
Test membership of an element without running.

:param x: a const reference to the possible element.
:type x: const_element_reference
Test membership of an element without running.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.


:returns: A ``bool``.

:rtype: bool
)pbdoc");
      thing.def("empty",
                &SchreierSims_::empty,
                R"pbdoc(
Check if any generators have been added so far.
Check if any generators have been added so far.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:complexity: Constant.

:returns:  ``True`` if ``number_of_generators() == 0`` and ``False`` otherwise.

:rtype: bool
)pbdoc");
      thing.def("finished",
                &SchreierSims_::finished,
                R"pbdoc(
Check if the stabiliser chain is fully enumerated.
Check if the stabiliser chain is fully enumerated.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:complexity: Constant.

:returns:  ``True`` if the stabiliser chain is fully enumerated and ``False`` otherwise.

:rtype: bool
)pbdoc");
      thing.def("generator",
                &SchreierSims_::generator,
                py::arg("index"),
                R"pbdoc(
Get a generator.

:param index: the index of the generator we want.
:type index: index_type
Get a generator with a given index, having checked that the index is in bounds.

:raises LibsemigroupsError:  if the ``index`` is out of bounds.

:complexity: Constant.


:returns: A const reference to the generator of ``self`` with index ``index``.

:rtype: const_element_reference
)pbdoc");
      thing.def("init",
                &SchreierSims_::init,
                R"pbdoc(
Reset to the trivial group.
Removes all generators, and orbits, and resets ``self`` so that it represents the trivial group, as if ``self`` had been newly constructed.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:complexity:  :math:`O(N ^ 2)` where ``N`` is the first template parameter.)pbdoc");
      thing.def("inverse_transversal_element",
                &SchreierSims_::inverse_transversal_element,
                py::arg("depth"),
                py::arg("pt"),
                R"pbdoc(
Get an inverse of a transversal element.

:param depth: the depth. 
:type depth: index_type

:param pt: the point to map to the base point under the inverse_transversal_element.
:type pt: point_type
Get an inverse of a transversal element.

:raises LibsemigroupsError:  if the ``depth`` is out of bounds.

:raises LibsemigroupsError:  if ``pt`` is not in the orbit of the basepoint.

:complexity: Constant.


:returns: A const reference to the inverse_transversal_element element of ``self`` at depth ``depth`` moving the corresponding point ``pt`` to the basepoint.

:rtype: const_element_reference
)pbdoc");
      thing.def("number_of_generators",
                &SchreierSims_::number_of_generators,
                R"pbdoc(
The number of generators.
Return the number of generators.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:complexity: Constant.

:returns: The number of generators, a value of ``int``.

:rtype: int
)pbdoc");
      thing.def("number_of_strong_generators",
                &SchreierSims_::number_of_strong_generators,
                py::arg("depth"),
                R"pbdoc(
The number of strong generators at a given depth.

:param depth: the depth.
:type depth: index_type
Return the number of strong generators at a given depth.

:raises LibsemigroupsError:  if the ``depth`` is out of bounds.

:complexity: Constant.


:returns: The number of strong generators, a value of ``int`` , at depth ``depth`` of the stabiliser chain.

:rtype: int
)pbdoc");
      thing.def("one",
                &SchreierSims_::one,
                R"pbdoc(
Returns a const reference to the identity.
Returns a const reference to the identity.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns: A const reference to the identity element.

:rtype: const_element_reference
)pbdoc");
      thing.def("orbit_lookup",
                &SchreierSims_::orbit_lookup,
                py::arg("depth"),
                py::arg("pt"),
                R"pbdoc(
Check if a point is in the orbit of a basepoint.

:param depth: the depth. 
:type depth: index_type

:param pt: the point.
:type pt: point_type
Check if a point is in the orbit of a basepoint.

:raises LibsemigroupsError:  if the ``depth`` is out of bounds or if ``pt`` is out of bounds.

:complexity: Constant.


:returns: A boolean indicating if the point ``pt`` is in the orbit of the basepoint of ``self`` at depth ``depth``.

:rtype: bool
)pbdoc");
      thing.def("run",
                &SchreierSims_::run,
                R"pbdoc(
Run the Schreier-Sims algorithm.
Run the Schreier-Sims algorithm.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:complexity:  :math:`O(N^2\log^3|G|+|T|N^2\log|G|)` time and :math:`O(N^2\log|G|+|T|N)` space, where ``N`` is the first template parameter, :math:`|G|` is the size of the group and :math:`|T|` is the number of generators of the group.)pbdoc");
      thing.def("sift",
                &SchreierSims_::sift,
                py::arg("x"),
                R"pbdoc(
Sift an element through the stabiliser chain.

:param x: a const reference to a group element.
:type x: const_element_reference
Sift an element through the stabiliser chain, having checked the degree of ``x`` is equal to the first template parameter ``N``.

:raises LibsemigroupsError:  if the degree of ``x`` is not equal to the first template parameter ``N``.


:returns: A value of type :any:`element_type`.

:rtype: const_element_reference
)pbdoc");
      thing.def("sift_inplace",
                &SchreierSims_::sift_inplace,
                py::arg("x"),
                R"pbdoc(
Sift an element through the stabiliser chain in-place.

:param x: a const reference to a group element.
:type x: element_reference
Sift an element through the stabiliser chain in-place, having checked the degree of ``x`` is equal to the first template parameter ``N``.

:raises LibsemigroupsError:  if the degree of ``x`` is not equal to the first template parameter ``N``.)pbdoc");
      thing.def("size",
                &SchreierSims_::size,
                R"pbdoc(
Returns the size of the group represented by this.
Returns the size of the group represented by this.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns:  the size, a value of ``int``.

:rtype: int
)pbdoc");
      thing.def("current_size",
                &SchreierSims_::current_size,
                R"pbdoc(
Returns the size of the group represented by this.
Returns the size of the group represented by this.

:exceptions: This function guarantees not to throw a :any:`LibsemigroupsError`.

:returns:  the size, a value of ``int``.

:rtype: int
)pbdoc");
      thing.def("strong_generator",
                &SchreierSims_::strong_generator,
                py::arg("depth"),
                py::arg("index"),
                R"pbdoc(
Get a strong generator.

:param depth: the depth. 
:type depth: index_type

:param index: the index of the generator we want.
:type index: index_type
Get a strong generator.

:raises LibsemigroupsError:  if the ``depth`` is out of bounds.

:raises LibsemigroupsError:  if the ``index`` is out of bounds.

:complexity: Constant.


:returns: A const reference to the strong generator of ``self`` at depth ``depth`` and with index ``index``.

:rtype: const_element_reference
)pbdoc");
      thing.def("transversal_element",
                &SchreierSims_::transversal_element,
                py::arg("depth"),
                py::arg("pt"),
                R"pbdoc(
Get a transversal element.

:param depth: the depth. 
:type depth: index_type

:param pt: the image of the base point under the traversal.
:type pt: point_type
Get a transversal element.

:raises LibsemigroupsError:  if the ``depth`` is out of bounds.

:raises LibsemigroupsError:  if ``pt`` is not in the orbit of the basepoint.

:complexity: Constant.


:returns: A const reference to the transversal element of ``self`` at depth ``depth`` moving the corresponding basepoint to the point ``pt``.

:rtype: const_element_reference
)pbdoc");

      m.def(
          "intersection",
          [](SchreierSims_& T, SchreierSims_& S1, SchreierSims_& S2) {
            return schreier_sims::intersection(T, S1, S2);
          },
          py::arg("T"),
          py::arg("S1"),
          py::arg("S2"),
          R"pbdoc(
Find the intersection of two permutation groups.

:param T: an empty Schreier-Sims object that will hold the result. 
:type T: SchreierSims

:param S1: the first semigroup of the intersection. 
:type S1: SchreierSims

:param S2: the second group of the intersection.
:type S2: SchreierSims
This function finds the intersection of two permutation groups. It modifies the first parameter ``T`` to be the Schreier-Sims object corresponding to the intersection of ``S1`` and ``S2``.

:raises LibsemigroupsError:  if ``T`` is not empty.)pbdoc");

    }  // bind_schreier_sims
  }    // namespace

  void init_schreier_sims(py::module& m) {
    // One call to bind is required per list of types
    bind_schreier_sims<255, uint8_t, Perm<0, uint8_t>>(m, "Perm1");
    bind_schreier_sims<511, uint16_t, Perm<0, uint16_t>>(m, "Perm2");
  }

}  // namespace libsemigroups
