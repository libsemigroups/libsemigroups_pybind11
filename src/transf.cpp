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
#include <libsemigroups/ranges.hpp>
#include <libsemigroups/transf.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "debug.hpp"
#include "main.hpp"  // for init_transf

namespace libsemigroups {

  namespace py = pybind11;

  namespace {

    template <typename Thing>
    void bind_ptransf_subclass(py::module&      m,
                               Thing&           thing,
                               std::string_view long_name,
                               std::string_view type_name,
                               std::string_view doc_type_name) {
      using PTransfSubclass = typename Thing::type;
      using Scalar          = typename PTransfSubclass::point_type;

      ////////////////////////////////////////////////////////////////////////
      // Operators
      ////////////////////////////////////////////////////////////////////////

      thing.def(py::self != py::self);
      thing.def(py::self * py::self);
      thing.def(py::self < py::self);
      thing.def(py::self <= py::self);
      thing.def(py::self == py::self);
      thing.def(py::self > py::self);
      thing.def(py::self >= py::self);

      ////////////////////////////////////////////////////////////////////////
      // Constructors/initialisers
      ////////////////////////////////////////////////////////////////////////

      std::string exceptions;

      if (IsPerm<PTransfSubclass>) {
        exceptions += R"pbdoc(
:raises LibsemigroupsError: if there are repeated values in *imgs*.
)pbdoc";
      }
      if (!IsPPerm<PTransfSubclass>) {
        thing.def(py::init([](std::vector<Scalar> const& imgs) {
                    return make<PTransfSubclass>(imgs);
                  }),
                  py::arg("imgs"),
                  fmt::format(R"pbdoc(
:sig=(self: {2}, imgs: list[int]) -> None:

A {0} can be constructed from a list of images, as follows:
the image of the point ``i`` under the {0} is ``imgs[i]``.

:param imgs: the list of images.
:type imgs: list[int]

:raises LibsemigroupsError: if any value in *imgs* exceeds ``len(imgs)``.

{1}

:complexity: Linear in :py:meth:`degree`.
)pbdoc",
                              long_name,
                              exceptions,
                              doc_type_name)
                      .c_str());
      }
      ////////////////////////////////////////////////////////////////////////
      // Special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def("__copy__", [](PTransfSubclass const& self) {
        return PTransfSubclass(self);
      });

      thing.def(
          "__getitem__",
          [](PTransfSubclass const& a,
             size_t                 b) -> int_or_unsigned_constant<Scalar> {
            auto result = a.at(b);
            if (result != UNDEFINED) {
              return {result};
            }
            return {UNDEFINED};
          },
          py::is_operator());

      thing.def("__hash__", &PTransfSubclass::hash_value, py::is_operator());

      ////////////////////////////////////////////////////////////////////////
      // Non-special methods
      ////////////////////////////////////////////////////////////////////////

      thing.def(
          "copy",
          [](PTransfSubclass const& self) { return PTransfSubclass(self); },
          fmt::format(
              R"pbdoc(
:sig=(self: {1}) -> {1}:
Copy a {0}.

:returns: A copy of the argument.
:rtype: {1}
)pbdoc",
              long_name,
              doc_type_name)
              .c_str());

      thing.def(
          "degree",
          [](PTransfSubclass const& self) { return self.degree(); },
          fmt::format(R"pbdoc(
:sig=(self: {0}) -> int:

Returns the degree of a {1}.

The *degree* of a {1} is the number of points used in its
definition, which is equal to the size of :any:`{0}.images`.

:returns: The degree.
:rtype: int
)pbdoc",
                      doc_type_name,
                      long_name)
              .c_str());

      thing.def(
          "images",
          [](PTransfSubclass& self) {
            auto r = rx::iterator_range(self.begin(), self.end())
                     | rx::transform(
                         [](auto val) -> int_or_unsigned_constant<Scalar> {
                           if (val != UNDEFINED) {
                             return {val};
                           }
                           return {UNDEFINED};
                         });
            return py::make_iterator(rx::begin(r), rx::end(r));
          },
          fmt::format(R"pbdoc(
:sig=(self: {0}) -> collections.abc.Iterator[int]:

Returns an iterator to the images of a {1}.

A {1} is stored as a list of the images of :math:`\{{0, 1, \ldots, n - 1\}}`,
i.e. :math:`[(0)f, (1)f, \ldots, (n - 1)f]`, and this function returns an iterator
yielding these values.

:returns: An iterator to the image values.
:rtype: collections.abc.Iterator[int]
)pbdoc",
                      doc_type_name,
                      long_name)
              .c_str());
      thing.def(
          "increase_degree_by",
          [](PTransfSubclass& self, size_t m) -> PTransfSubclass& {
            return static_cast<PTransfSubclass&>(self.increase_degree_by(m));
          },
          py::arg("m"),
          fmt::format(
              R"pbdoc(
:sig=(self: {0}, m: int) -> {0}:

Increases the degree of *self* in-place, leaving existing values unaltered.

:param m: the number of points to add.
:type m: int

:returns: *self*
:rtype: {0}

:complexity:
   At worst linear in the sum of the parameter *m* and :py:meth:`degree`.
)pbdoc",
              doc_type_name)
              .c_str());

      thing.def_static("one",
                       &PTransfSubclass::one,
                       py::arg("N"),
                       fmt::format(R"pbdoc(
:sig=(N: int) -> {1}:

Returns the identity {0} on *N* points. This function returns a newly
constructed {0} with degree equal to *N* that fixes every value from ``0`` to
*N*.

:param N: the degree.
:type N: int

:returns: The identity {0}.
:rtype: {1}
)pbdoc",
                                   long_name,
                                   doc_type_name)
                           .c_str());

      thing.def(
          "product_inplace",
          [](PTransfSubclass&       xy,
             PTransfSubclass const& x,
             PTransfSubclass const& y) { xy.product_inplace(x, y); },
          py::arg("x"),
          py::arg("y"),
          fmt::format(
              R"pbdoc(
:sig=(self: {1}, x: {1}, y: {1}) -> None:

Replaces the contents of *self* by the product of *x* and *y*.

:param x: a {0}.
:type x: {1}
:param y: a {0}.
:type y: {1}

:complexity: Linear in :py:meth:`degree`.)pbdoc",
              long_name,
              doc_type_name)
              .c_str());

      thing.def("rank",
                &PTransfSubclass::rank,
                fmt::format(R"pbdoc(
:sig=(self: {0}) -> int:

Returns the number of distinct image values in a {1}.

The *rank* of a {1} is the number of its distinct
image values, not including :any:`UNDEFINED`.

:returns: The number of distinct image values.
:rtype: int

:complexity: Linear in :py:meth:`degree`.
)pbdoc",
                            doc_type_name,
                            long_name)
                    .c_str());

      thing.def(
          "swap",
          [](PTransfSubclass& self, PTransfSubclass& other) {
            self.swap(other);
          },
          py::arg("other"),
          fmt::format(
              R"pbdoc(
:sig=(self: {1}, other: {1}) -> None:

Swap with another {0} of the same type.

:param other: the {0} to swap with.
:type other: {1}
)pbdoc",
              long_name,
              doc_type_name)
              .c_str());

      ////////////////////////////////////////////////////////////////////////
      // Helper functions, the "transf_" prefix is to indicate that these
      // functions are included in the "transf" subpackage.
      ////////////////////////////////////////////////////////////////////////

      m.def("transf_one",
            &one<PTransfSubclass>,
            R"pbdoc(
:sig=(f: Transf | PPerm | Perm) -> Transf | PPerm | Perm:
:only-document-once:

Returns the identity on the same number of points as the degree of *f*.
This function returns a newly constructed object of the same type as *f*
that fixes every value from ``0`` to ``f.degree()``.

:param f: the transformation, partial perm, or permutation.
:type f: Transf | PPerm | Perm

:returns: The identity.
:rtype: Transf | PPerm | Perm

:complexity: Linear in the degree of *f*.
)pbdoc");

      m.def(
          "transf_image",
          [](PTransfSubclass const& f) { return image(f); },
          py::arg("f"),
          R"pbdoc(
:sig=(f: Transf | PPerm | Perm) -> list[int]:
:only-document-once:

Returns the set of points in the image of a partial transformation.

Returns a ``list[int]`` containing those values ``f[i]`` such that:

* :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of *f*; and
* ``f[i] != UNDEFINED``.

:param f: the transformation.
:type f: Transf | PPerm | Perm

:returns: The sorted list of points in the image.
:rtype: list[int]

:complexity: Linear in the degree of *f*.
)pbdoc");

      m.def(
          "transf_domain",
          [](PTransfSubclass const& f) { return domain(f); },
          py::arg("f"),
          R"pbdoc(
:sig=(f: Transf | PPerm | Perm) -> list[int]:
:only-document-once:

Returns the set of points where a partial transformation is defined.

This function returns a ``list[int]`` containing those values ``i`` such that:

* :math:`i\in \{0, \ldots, n - 1\}` where `n` is the degree of *f*; and
* ``f[i] != UNDEFINED`` (only applies to partial perms).

:param f: the transformation, partial perm, or permutation.
:type f: Transf | PPerm | Perm

:returns: The sorted list of points in the domain.
:rtype: list[int]

:complexity: Linear in the degree of *f*.
)pbdoc");
    }

    template <size_t N, typename Scalar>
    void bind_transf(py::module& m, std::string const& name) {
      using Transf_ = Transf<N, Scalar>;

      py::class_<Transf_> thing(m,
                                name.c_str(),
                                R"pbdoc(
Class for representing transformations on up to ``2 ** 32`` points.

A *transformation* :math:`f` is just a function defined on the whole of
:math:`\{0, 1, \ldots, n - 1\}` for some integer :math:`n` called the *degree*
of :math:`f`. A transformation is stored as a list of the images of :math:`\{0,
1, \ldots, n - 1\}`, i.e. :math:`[(0)f, (1)f, \ldots, (n - 1)f]`.

Transformations are optimised for the number of points in the image with
fewer points requiring less space per point.

.. doctest::

   >>> from libsemigroups_pybind11.transf import Transf, one
   >>> x = Transf([0, 0, 2, 2, 0, 1])
   >>> x.degree()
   6
   >>> x[0]
   0
   >>> x[5]
   1
   >>> x
   Transf([0, 0, 2, 2, 0, 1])
   >>> x * x
   Transf([0, 0, 2, 2, 0, 0])
   >>> x < x * x
   False
   >>> y = Transf([9, 7, 3, 5, 3, 4, 2, 7, 7, 1])
   >>> x = one(y)
   >>> x.product_inplace(y, y)
   >>> x
   Transf([1, 7, 5, 4, 5, 3, 3, 7, 7, 7])
   >>> list(x.images())
   [1, 7, 5, 4, 5, 3, 3, 7, 7, 7]
   >>> x.rank()
   5
   >>> one(x)
   Transf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
   >>> x = Transf.one(8)
   >>> x
   Transf([0, 1, 2, 3, 4, 5, 6, 7])
   >>> x.degree()
   8
   >>> x.swap(y)
   >>> x, y
   (Transf([9, 7, 3, 5, 3, 4, 2, 7, 7, 1]), Transf([0, 1, 2, 3, 4, 5, 6, 7]))
   >>> x = Transf([1, 0, 2])
   >>> y = x.copy()
   >>> x is y
   False
   >>> x == y
   True
   >>> {x, y}
   {Transf([1, 0, 2])}
)pbdoc");

      // This is how we could change the appearance of the
      // _libsemigroups_pybind11 types in exceptions. We decided against doing
      // this because it would make it more or less impossible to debug, given
      // that Transf (for example) would mean the python class and any of the
      //
      // libsemigroups classes. thing.attr("__name__") = "Transf";
      // thing.attr("__qualname__") = "Transf";
      // thing.attr("__module__") = "libsemigroups_pybind11";

      bind_ptransf_subclass(m, thing, "transformation", name, "Transf");

      thing.def("__repr__", [name](Transf_ const& f) {
        return to_input_string(f, name, "[]");
      });
      thing.def("__str__", [name](Transf_ const& f) {
        return to_input_string(f, name, "[]");
      });
    }  // bind_transf

    template <size_t N, typename Scalar>
    void bind_pperm(py::module& m, std::string const& name) {
      using PPerm_ = PPerm<N, Scalar>;

      py::class_<PPerm_> thing(m,
                               name.c_str(),
                               R"pbdoc(
Class for representing partial permutations on up to ``2 ** 32`` points.

A *partial permutation* :math:`f` is just an injective partial transformation,
which is stored as a list of the images of :math:`\{0, 1, \ldots, n - 1\}`,
i.e. :math:`((0)f, (1)f, \ldots, (n - 1)f)` where the value
:any:`UNDEFINED` is used to indicate that :math:`(i)f` is undefined (i.e. not
among the points where :math:`f` is defined).

These partial permutations are optimised for the number of points in the image
with fewer points requiring less space per point.

.. doctest::

   >>> from libsemigroups_pybind11.transf import PPerm, one, inverse, right_one, left_one, domain, image
   >>> from libsemigroups_pybind11 import UNDEFINED
   >>> x = PPerm([1, 0, 2], [0, 1, 2], 4)
   >>> x.degree()
   4
   >>> x[0]
   1
   >>> x[3] == UNDEFINED
   True
   >>> x * x
   PPerm([0, 1, 2], [0, 1, 2], 4)
   >>> x * x == x
   False
   >>> x < x * x
   False
   >>> y = x.copy()
   >>> x.product_inplace(y, y)
   >>> x
   PPerm([0, 1, 2], [0, 1, 2], 4)
   >>> list(x.images())
   [0, 1, 2, UNDEFINED]
   >>> x.rank()
   3
   >>> one(x)
   PPerm([0, 1, 2, 3], [0, 1, 2, 3], 4)
   >>> x = PPerm.one(8)
   >>> x
   PPerm([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], 8)
   >>> x.degree()
   8
   >>> x.swap(y)
   >>> x, y
   (PPerm([0, 1, 2], [1, 0, 2], 4), PPerm([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], 8))
   >>> y = x.copy()
   >>> {x, y}
   {PPerm([0, 1, 2], [1, 0, 2], 4)}
   >>> x = PPerm([255, 3, 255, 0])
   >>> x
   PPerm([1, 3], [3, 0], 4)
   >>> x * inverse(x) * x == x and inverse(x) * x * inverse(x) == inverse(x)
   True
   >>> x * right_one(x) == x
   True
   >>> left_one(x) * x == x
   True
   >>> domain(left_one(x) * right_one(x)) == list(set(domain(x)) & set(image(x)))
   True
)pbdoc");

      bind_ptransf_subclass(m, thing, "partial perm", name, "PPerm");

      thing.def("__repr__", [name](PPerm_ const& f) {
        return to_input_string(f, name, "[]");
      });
      thing.def("__str__", [name](PPerm_ const& f) {
        return to_input_string(f, name, "[]");
      });

      thing.def(
          py::init(
              [](std::vector<int_or_unsigned_constant<Scalar>> const& imgs) {
                return make<PPerm_>(to_ints<Scalar>(imgs));
              }),
          py::arg("imgs"),
          R"pbdoc(
:sig=(self: PPerm, imgs: list[int | Undefined]) -> None:

A partial perm can be constructed from a list of images, as follows:
the image of the point ``i`` under the {1} is ``imgs[i]``.

:param imgs: the list of images.
:type imgs: list[int | Undefined]

:raises LibsemigroupsError:
  if there are repeated values in *imgs* that do not equal :any:`UNDEFINED`.
:raises LibsemigroupsError:
  if any integer value in *imgs* exceeds ``len(imgs)``.

:complexity: Linear in :py:meth:`degree`.
)pbdoc");

      thing.def(py::init([](std::vector<Scalar> const& dom,
                            std::vector<Scalar> const& im,
                            size_t deg) { return make<PPerm_>(dom, im, deg); }),
                py::arg("dom"),
                py::arg("im"),
                py::arg("n"),
                R"pbdoc(
:sig=(self: PPerm, dom: list[int], im: list[int], n:int) -> None:

Construct from domain, image, and degree.

Constructs a partial perm of degree *n* such that ``(dom[i])f = im[i]`` for
all ``i`` and which is :any:`UNDEFINED` on every other value in the range
:math:`[0, n)`.

:param dom: the domain.
:type dom: list[int]
:param im: the image.
:type im: list[int]
:param n: the degree.
:type n: int

:raises LibsemigroupsError: the value *n* is not compatible with the type.
:raises LibsemigroupsError: *dom* and *im* do not have the same size.
:raises LibsemigroupsError: any value in *dom* or *im* is greater than *n*.
:raises LibsemigroupsError: there are repeated entries in *dom* or *im*.
)pbdoc");

      ////////////////////////////////////////////////////////////////////////
      // Helper functions, the "transf_" prefix is to indicate that the
      // functions will belong in the subpackage "transf".
      ////////////////////////////////////////////////////////////////////////

      m.def("transf_inverse",
            py::overload_cast<PPerm_ const&>(&inverse<N, Scalar>),
            py::arg("f"),
            R"pbdoc(
:sig=(f: PPerm | Perm) -> PPerm | Perm:
:only-document-once:

Returns the inverse of a partial perm or permutation.

This function returns a newly constructed inverse of *f*. The *inverse* of
a partial permutation *f* is the partial perm ``g`` such that ``fgf =
f`` and ``gfg = g``.

:param f: the partial perm.
:type f: PPerm | Perm

:returns: The inverse of *f*.
:rtype: PPerm | Perm

:complexity: Linear in the degree of *f*.
)pbdoc");

      m.def("transf_right_one",
            &right_one<N, Scalar>,
            py::arg("f"),
            R"pbdoc(
:sig=(f: PPerm) -> PPerm:
:only-document-once:

Returns the right one of a partial perm.

This function returns a newly constructed partial perm with degree equal
to that of *f* that fixes every value in the image of *f*, and is
:any:`UNDEFINED` on any other values.

:param f: the partial perm.
:type f: PPerm

:returns: The right one of *f*.
:rtype: PPerm

:complexity: Linear in the degree of *f*.
)pbdoc");

      m.def("transf_left_one",
            &left_one<N, Scalar>,
            py::arg("f"),
            R"pbdoc(
:sig=(f: PPerm) -> PPerm:
:only-document-once:

Returns the left one of a partial perm.

This function returns a newly constructed partial perm with degree equal
to that of *f* that fixes every value in the domain of *f*, and is
:any:`UNDEFINED` on any other values.

:param f: the partial perm.
:type f: PPerm

:returns: The left one of *f*.
:rtype: PPerm

:complexity: Linear in the degree of *f*.
)pbdoc");
    }  // bind_pperm

    template <size_t N, typename Scalar>
    void bind_perm(py::module& m, std::string const& name) {
      using Perm_ = Perm<N, Scalar>;

      // If we derive from Transf in the next line then the wrong overload of
      // "one" (for example) gets applied (I think pybind11 selects the first
      // matching overload, which is the one for Transf)
      py::class_<Perm_> thing(m,
                              name.c_str(),
                              R"pbdoc(
Class for representing permutations on up to ``2 ** 32`` points.

A *permutation* :math:`f` is an injective transformation defined on the whole
of :math:`\{0, 1, \ldots, n - 1\}` for some integer :math:`n` called the
*degree* of :math:`f`. A permutation is stored as a list of the images of
:math:`(0, 1, \ldots, n - 1)`, i.e. :math:`((0)f, (1)f, \ldots, (n - 1)f)`.

Permutations are optimised for the number of points in the image with
fewer points requiring less space per point.

.. doctest::

   >>> from libsemigroups_pybind11.transf import Perm, one, inverse
   >>> x = Perm([0, 2, 1, 3, 4, 5])
   >>> x.degree()
   6
   >>> x[0]
   0
   >>> x[5]
   5
   >>> x
   Perm([0, 2, 1, 3, 4, 5])
   >>> x * x
   Perm([0, 1, 2, 3, 4, 5])
   >>> x < x * x
   False
   >>> y = Perm([ 5, 2, 0, 1, 3, 4, 6 ])
   >>> x = one(y)
   >>> x.product_inplace(y, y)
   >>> x
   Perm([4, 0, 5, 2, 1, 3, 6])
   >>> x == y * y
   True
   >>> list(x.images())
   [4, 0, 5, 2, 1, 3, 6]
   >>> x.rank()
   7
   >>> one(x)
   Perm([0, 1, 2, 3, 4, 5, 6])
   >>> x = Perm.one(8)
   >>> x
   Perm([0, 1, 2, 3, 4, 5, 6, 7])
   >>> x.degree()
   8
   >>> x.swap(y)
   >>> x, y
   (Perm([5, 2, 0, 1, 3, 4, 6]), Perm([0, 1, 2, 3, 4, 5, 6, 7]))
   >>> x = Perm([1, 0, 2])
   >>> y = x.copy()
   >>> x is y
   False
   >>> x == y
   True
   >>> {x, y}
   {Perm([1, 0, 2])}
)pbdoc");

      bind_ptransf_subclass(m, thing, "permutation", name, "Perm");

      thing.def("__repr__", [name](Perm_ const& f) {
        return to_input_string(f, name, "[]");
      });
      thing.def("__str__", [name](Perm_ const& f) {
        return to_input_string(f, name, "[]");
      });
      // Documented above for PPerm
      m.def("transf_inverse",
            py::overload_cast<Perm_ const&>(&inverse<N, Scalar>));
    }  // bind_perm
  }    // namespace

  void init_transf(py::module& m) {
    // Transformations
    bind_transf<0, uint8_t>(m, "Transf1");
    bind_transf<0, uint16_t>(m, "Transf2");
    bind_transf<0, uint32_t>(m, "Transf4");

    // Perms
    bind_perm<0, uint8_t>(m, "Perm1");
    bind_perm<0, uint16_t>(m, "Perm2");
    bind_perm<0, uint32_t>(m, "Perm4");

    // Partial perms
    bind_pperm<0, uint8_t>(m, "PPerm1");
    bind_pperm<0, uint16_t>(m, "PPerm2");
    bind_pperm<0, uint32_t>(m, "PPerm4");
  }
}  // namespace libsemigroups
