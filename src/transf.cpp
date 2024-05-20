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
#include <libsemigroups/transf.hpp>

// pybind11....
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// libsemigroups_pybind11....
#include "main.hpp"  // for init_transf

namespace py = pybind11;

namespace libsemigroups {

  namespace {
    template <typename T>
    std::string transf_repr(std::string_view prefix, T const& f) {
      return fmt::format(
          "{}([{}])", prefix, fmt::join(f.begin(), f.end(), ", "));
    }

    template <typename T>
    std::string pperm_repr(std::string_view prefix, T const& f) {
      auto dom = domain(f);
      auto im  = dom;
      std::for_each(im.begin(), im.end(), [&f](auto& val) { val = f[val]; });
      return fmt::format("{}({}, {}, {})", prefix, dom, im, f.degree());
    }

    // This is the main function that installs common methods for derived
    // classes of PTransf
    template <typename Point, typename Container>
    void bind_ptransf_base(py::module& m, std::string const& name) {
      using PTransfBase_ = PTransfBase<Point, Container>;

      py::class_<PTransfBase_> thing(m, name.c_str());
      thing.def_static("undef",
                       &PTransfBase_::undef,
                       R"pbdoc(
Returns the value used to represent "undefined". This static function returns
the integer value used to represent an "undefined" value for this type of
partial transformation.

:returns:
   The integer representation of undefined.

:rtype:
   int
)pbdoc");

      // TODO this does nothing
      thing.def(py::init<>(), R"pbdoc(
Constructs an uninitialized partial transformation of degree ``0``.

:complexity:
   Constant.
)pbdoc");
      thing.def(
          "__getitem__",
          [](PTransfBase_ const& a, size_t b) { return a.at(b); },
          py::is_operator());

      thing.def(
          "images",
          [](PTransfBase_& self) {
            return py::make_iterator(self.begin(), self.end());
          },
          R"pbdoc(
Returns an iterator to the images of a partial transformation.

A partial transformation is stored as a list of the images of
:math:`\{0, 1, \ldots, n - 1\}` , i.e. :math:`[(0)f, (1)f, \ldots, (n -
1)f]`, and this function returns an iterator pointing at these values.

:returns: An iterator to the image values.
:rtype: Iterator
)pbdoc");
      thing.def("degree",
                &PTransfBase_::degree,
                R"pbdoc(
Returns the degree of a partial transformation.

The *degree* of a partial transformation is the number of points used
in its definition, which is equal to the size of the underlying
container.

Note that if the type of ``self`` is :py:class:`StaticTransf16`,
:py:class:`StaticPPerm16`, or :py:class:`StaticPerm16`, then the return value
of this function is always `16`.

:returns: The degree.
:rtype: int
)pbdoc");
      thing.def("__hash__",
                &PTransfBase_::hash_value,
                py::is_operator(),
                R"pbdoc(
Returns a hash value.

:exceptions:
   This function guarantees not to throw a
   :any:`libsemigroups::LibsemigroupsException`.

:complexity:
   Linear in :any:`degree()`.

:returns:
   A value of type ``int``.

:rtype:
   int
)pbdoc");
      thing.def(py::self != py::self);
      thing.def(py::self < py::self);
      thing.def(py::self <= py::self);
      thing.def(py::self == py::self);
      thing.def(py::self > py::self);
      thing.def(py::self >= py::self);
      thing.def("rank",
                &PTransfBase_::rank,
                R"pbdoc(
Returns the number of distinct image values.

The *rank* of a partial transformation is the number of its distinct
image values, not including :any:`UNDEFINED`.

:returns: The number of distinct image values.  rtype: int

:complexity: Linear in :py:meth:`degree`.
)pbdoc");
      thing.def("swap",
                &PTransfBase_::swap,
                py::arg("that"),
                R"pbdoc(
Swap with another partial transformation of the same type.

:param that:
   the partial transformation to swap with.

:type that:
   Transf | PPerm | Perm
)pbdoc");
    }

    template <size_t N, typename Scalar>
    void bind_transf(py::module& m, std::string const& name) {
      using Transf_ = Transf<N, Scalar>;

      // TODO default constructor for Transf

      using container_type = typename Transf_::container_type;

      py::class_<Transf_, PTransfBase<Scalar, container_type>> thing(
          m,
          name.c_str(),
          R"pbdoc(
Transformations with static or dynamic degree.

A *transformation* :math:`f` is just a function defined on the whole of
:math:`\{0, 1, \ldots, n - 1\}` for some integer :math:`n` called the *degree*
of :math:`f`. A transformation is stored as a list of the images of :math:`\{0,
1, \ldots, n - 1\}` , i.e. :math:`[(0)f, (1)f, \ldots, (n - 1)f]`.

There are numerous functions for computing things about :any:`Transf` objects
detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

.. doctest::

   >>> from libsemigroups_pybind11 import Transf1, one
   >>> x = Transf1([0, 0, 2, 2, 0, 1])
   >>> x.degree()
   6
   >>> x[0]
   0
   >>> x[5]
   1
   >>> x
   Transf1([0, 0, 2, 2, 0, 1])
   >>> x * x
   Transf1([0, 0, 2, 2, 0, 0])
   >>> x < x * x
   False
   >>> y = Transf1([9, 7, 3, 5, 3, 4, 2, 7, 7, 1])
   >>> x = one(y)
   >>> x.product_inplace(y, y)
   >>> x
   Transf1([1, 7, 5, 4, 5, 3, 3, 7, 7, 7])
   >>> list(x.images())
   [1, 7, 5, 4, 5, 3, 3, 7, 7, 7]
   >>> x.rank()
   5
   >>> one(x)
   Transf1([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
   >>> x = Transf1.one(8)
   >>> x
   Transf1([0, 1, 2, 3, 4, 5, 6, 7])
   >>> x.degree()
   8
   >>> x.undef()
   255
   >>> x.swap(y)
   >>> x, y
   (Transf1([9, 7, 3, 5, 3, 4, 2, 7, 7, 1]), Transf1([0, 1, 2, 3, 4, 5, 6, 7]))
   >>> x = Transf1([1, 0, 2])
   >>> y = x.copy()
   >>> x is y
   False
   >>> x == y
   True
   >>> {x, y}
   {Transf1([1, 0, 2])}
)pbdoc");

      thing.def("__repr__",
                [name](Transf_ const& f) { return transf_repr(name, f); });

      thing.def(py::self * py::self);
      thing.def(
          "copy",
          [](Transf_ const& f) { return Transf_(f); },
          R"pbdoc(
Copy a transformation.

:param f: the transformation to copy.
:type f: Transf

:returns: A copy of the argument.
:rtype: Transf
)pbdoc");
      thing.def(
          py::init([](std::vector<typename Transf_::point_type> const& imgs) {
            return Transf_::make(imgs);
          }),
          R"pbdoc(
A transformation can be constructed from a list of images, as follows:
the image of the point ``i`` under the transformation is ``imgs[i]``.

:param imgs: the list of images
:type imgs: List[int]

:raises LibsemigroupsError: if any of the size of **imgs** is not compatible with the type.
:raises LibsemigroupsError: if any value in **imgs** exceeds ``len(imgs)``.

:complexity: Linear in :py:meth:`degree`.)pbdoc");
      thing.def("product_inplace",
                &Transf_::product_inplace,
                py::arg("x"),
                py::arg("y"),
                R"pbdoc(

Replaces the contents of ``self`` by the product of ``x`` and ``y``.

:param x: a transformation.
:type x: Transf
:param y: a transformation.
:type y: Transf

:complexity: Linear in :py:meth:`degree`.)pbdoc");
      thing.def_static("one",
                       &Transf_::one,
                       py::arg("N"),
                       R"pbdoc(
Returns the identity transformation on ``N`` points. This function returns a
newly constructed transformation with degree equal to the degree of ``N``
that fixes every value from ``0`` to ``N``.

:param N: the degree.
:type N: int

:returns: The identity transformation.
:rtype: Transf.
)pbdoc");
      thing.def(
          "increase_degree_by",
          [](Transf_& self, size_t m) -> void { self.increase_degree_by(m); },
          py::arg("m"),
          R"pbdoc(
Increases the degree of ``self`` in-place, leaving existing values unaltered.

:param m: the number of points to add.
:type m: int

:returns: ``self``
:rtype: Transf

:complexity:
   At worst linear in the sum of the parameter ``m`` and :py:meth:`degree`.
)pbdoc");
      m.def("one", &one<Transf_>, R"pbdoc(TODO)pbdoc");
      m.def("image", [](Transf_ const& f) { return image(f); });
      m.def("domain", [](Transf_ const& f) { return domain(f); });
    }  // bind_transf

    template <size_t N, typename Scalar>
    void bind_pperm(py::module& m, std::string const& name) {
      using PPerm_ = PPerm<N, Scalar>;

      using container_type = typename PPerm_::container_type;

      py::class_<PPerm_, PTransfBase<Scalar, container_type>> thing(
          m,
          name.c_str(),
          R"pbdoc(
Partial permutations with static or dynamic degree.

A *partial permutation* :math:`f` is just an injective partial transformation,
which is stored as a list of the images of :math:`\{0, 1, \ldots, n - 1\}` ,
i.e. i.e. :math:`((0)f, (1)f, \ldots, (n - 1)f)` where the value
:any:`UNDEFINED` is used to indicate that :math:`(i)f` is undefined (i.e. not
among the points where :math:`f` is defined).

There are numerous functions for computing things about :any:`PPerm` objects
detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

.. doctest::

   >>> from libsemigroups_pybind11 import PPerm1, one, inverse, right_one, left_one, domain, image
   >>> x = PPerm1([1, 0, 2], [0, 1, 2], 4)
   >>> x.degree()
   4
   >>> x[0]
   1
   >>> x[3] == x.undef()
   True
   >>> x * x
   PPerm1([0, 1, 2], [0, 1, 2], 4)
   >>> x * x == x
   False
   >>> x < x * x
   False
   >>> y = x.copy()
   >>> x.product_inplace(y, y)
   >>> x
   PPerm1([0, 1, 2], [0, 1, 2], 4)
   >>> list(x.images())
   [0, 1, 2, 255]
   >>> x.rank()
   3
   >>> one(x)
   PPerm1([0, 1, 2, 3], [0, 1, 2, 3], 4)
   >>> x = PPerm1.one(8)
   >>> x
   PPerm1([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], 8)
   >>> x.degree()
   8
   >>> x.undef()
   255
   >>> x.swap(y)
   >>> x, y
   (PPerm1([0, 1, 2], [1, 0, 2], 4), PPerm1([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], 8))
   >>> y = x.copy()
   >>> {x, y}
   {PPerm1([0, 1, 2], [1, 0, 2], 4)}
   >>> x = PPerm1([255, 3, 255, 0])
   >>> x
   PPerm1([1, 3], [3, 0], 4)
   >>> PPerm1().degree()
   0
   >>> x * inverse(x) * x == x and inverse(x) * x * inverse(x) == inverse(x)
   True
   >>> x * right_one(x) == x
   True
   >>> left_one(x) * x == x
   True
   >>> domain(left_one(x) * right_one(x)) == list(set(domain(x)) & set(image(x)))
   True
)pbdoc");

      thing.def("__repr__",
                [name](PPerm_ const& f) { return pperm_repr(name, f); });

      thing.def(py::init([]() { return PPerm_::make(); }),
                R"pbdoc(
Construct an empty :any:`PPerm` of rank 0.
)pbdoc");
      thing.def(py::init([](std::vector<Scalar> const& imgs) {
                  return PPerm_::make(imgs);
                }),
                py::arg("imgs"),
                R"pbdoc(
A partial perm can be constructed from a list of images, as follows:
the image of the point ``i`` under the partial perm is ``imgs[i]``.
The degree of the partial perm is the same as the length of **imgs**.

:param imgs: the list of images
:type imgs: List[int]

:raises LibsemigroupsError: if any of the size of **imgs** is not compatible with the type.
:raises LibsemigroupsError:
  if any value in **imgs** if not ``PPerm.undef()`` and exceeds ``len(imgs)``.
:raises LibsemigroupsError:
  if there are repeated values in **imgs** that do not equal ``PPerm.undef()``.

:complexity: Linear in ``len(imgs)``.
)pbdoc");
      thing.def(py::init([](std::vector<Scalar> const& dom,
                            std::vector<Scalar> const& im,
                            Scalar deg) { return PPerm_::make(dom, im, deg); }),
                py::arg("dom"),
                py::arg("im"),
                py::arg("M"),
                R"pbdoc(
Construct from domain, range, and degree.

Constructs a partial perm of degree ``M`` such that ``(dom[i])f = ran[i]`` for
all ``i`` and which is :any:`UNDEFINED` on every other value in the range
:math:`[0, M)`.

:param dom: the domain
:type dom: List[int]
:param ran: the image
:type ran: List[int]
:param M: the degree
:type M: int

:raises LibsemigroupsError: the value **M** is not compatible with the type.
:raises LibsemigroupsError: **dom** and **ran** do not have the same size.
:raises LibsemigroupsError: any value in **dom** or **ran** is greater than **M**.
:raises LibsemigroupsError: there are repeated entries in **dom** or **ran**.
)pbdoc");

      thing.def(
          "copy",
          [](PPerm_ const& f) { return PPerm_(f); },
          R"pbdoc(
Copy a partial perm.

:param f: the partial perm to copy.
:type f: PPerm

:returns: A copy of the argument.
:rtype: PPerm
)pbdoc");

      thing.def_static("one",
                       &PPerm_::one,
                       py::arg("M"),
                       R"pbdoc(
Returns the identity partial perm on the given number of points. This function
returns a newly constructed partial perm with degree equal to ``M`` that fixes
every value from ``0`` to ``M``.

:param M: the degree.
:type M: int

:complexity: Linear in ``M``.

:returns: The identity partial perm.
:rtype: PPerm

:seealso: :py:func:`right_one` and :py:func:`left_one`.
)pbdoc");
      thing.def("product_inplace",
                &PPerm_::product_inplace,
                py::arg("x"),
                py::arg("y"),
                R"pbdoc(
Multiply two partial perms and store the product in ``self``.
Replaces the contents of ``self`` by the product of **x** and **y**.

:param x: a partial perm.
:type x: PPerm
:param y: a partial perm.
:type y: PPerm

:complexity: Linear in :py:meth:`degree`.)pbdoc");

      thing.def(py::self * py::self);

      m.def("one", &one<PPerm_>);
      m.def("inverse", py::overload_cast<PPerm_ const&>(&inverse<N, Scalar>));
      m.def("right_one", &right_one<N, Scalar>);
      m.def("left_one", &left_one<N, Scalar>);
      m.def("image", [](PPerm_ const& f) { return image(f); });
      m.def("domain", [](PPerm_ const& f) { return domain(f); });
    }  // bind_pperm

    template <size_t N, typename Scalar>
    void bind_perm(py::module& m, std::string const& name) {
      using Perm_ = Perm<N, Scalar>;

      using container_type = typename Perm_::container_type;
      py::class_<Perm_, PTransfBase<Scalar, container_type>> thing(m,
                                                                   name.c_str(),
                                                                   R"pbdoc(
Permutations with static or dynamic degree.

A *permutation* :math:`f` is an injective transformation defined on the whole
of :math:`\{0, 1, \ldots, n - 1\}` for some integer :math:`n` called the
*degree* of :math:`f` . A permutation is stored as a vector of the images of
:math:`(0, 1, \ldots, n - 1)` , i.e. :math:`((0)f, (1)f, \ldots, (n - 1)f)`.

There are numerous functions for computing things about :any:`Perm` objects
detailed below.

.. toctree::
   :maxdepth: 1

   transf-helpers

.. doctest::

   >>> from libsemigroups_pybind11 import Perm1, one, inverse
   >>> x = Perm1([0, 2, 1, 3, 4, 5])
   >>> x.degree()
   6
   >>> x[0]
   0
   >>> x[5]
   5
   >>> x
   Perm1([0, 2, 1, 3, 4, 5])
   >>> x * x
   Perm1([0, 1, 2, 3, 4, 5])
   >>> x < x * x
   False
   >>> y = Perm1([ 5, 2, 0, 1, 3, 4, 6 ])
   >>> x = one(y)
   >>> x.product_inplace(y, y)
   >>> x
   Perm1([4, 0, 5, 2, 1, 3, 6])
   >>> x == y * y
   True
   >>> list(x.images())
   [4, 0, 5, 2, 1, 3, 6]
   >>> x.rank()
   7
   >>> one(x)
   Perm1([0, 1, 2, 3, 4, 5, 6])
   >>> x = Perm1.one(8)
   >>> x
   Perm1([0, 1, 2, 3, 4, 5, 6, 7])
   >>> x.degree()
   8
   >>> x.undef()
   255
   >>> x.swap(y)
   >>> x, y
   (Perm1([5, 2, 0, 1, 3, 4, 6]), Perm1([0, 1, 2, 3, 4, 5, 6, 7])
   >>> x = Perm1([1, 0, 2])
   >>> y = x.copy()
   >>> x is y
   False
   >>> x == y
   True
   >>> {x, y}
   {Perm1([1, 0, 2])}
)pbdoc");

      thing.def("__repr__",
                [name](Perm_ const& f) { return transf_repr(name, f); });
      thing.def(py::init(
          [](std::vector<Scalar> const& imgs) { return Perm_::make(imgs); }));
      thing.def_static("one",
                       &Perm_::one,
                       py::arg("M"),
                       R"pbdoc(
Returns the identity permutation on the given number of points.This function
returns a newly constructed permutation with degree equal to ``M`` that fixes
every value from ``0`` to ``M``.

:param M: the degree.
:type M: int

:returns: The identity permutation of the given degree.
:rtype: Perm

:complexity: Linear in ``M``.
)pbdoc");
      thing.def(
          "product_inplace",
          [](Perm_& self, Perm_ const& x, Perm_ const& y) {
            self.product_inplace(x, y);
          },
          py::arg("x"),
          py::arg("y"),
          R"pbdoc(

Replaces the contents of ``self`` by the product of ``x`` and ``y``.

:param x: a perm.
:type x: Perm
:param y: a perm.
:type y: Perm

:complexity: Linear in :py:meth:`degree`.)pbdoc");
      m.def("inverse", py::overload_cast<Perm_ const&>(&inverse<N, Scalar>));
      m.def("one", &one<Perm_>);
      m.def("image", [](Perm_ const& f) { return image(f); });
      m.def("domain", [](Perm_ const& f) { return domain(f); });
      thing.def(py::self * py::self);
      thing.def(
          "copy",
          [](Perm_ const& f) { return Perm_(f); },
          R"pbdoc(
Copy a permutation.

:param f: the permutation to copy.
:type f: Perm

:returns: A copy of the argument.
:rtype: Perm
)pbdoc");
    }  // bind_perm
  }    // namespace

  void init_transf(py::module& m) {
    // Base classes
    bind_ptransf_base<uint8_t, typename Transf<16>::container_type>(
        m, "PTransfBase16");
    bind_ptransf_base<uint8_t, typename Transf<0, uint8_t>::container_type>(
        m, "PTransfBase1");
    bind_ptransf_base<uint16_t, typename Transf<0, uint16_t>::container_type>(
        m, "PTransfBase2");
    bind_ptransf_base<uint32_t, typename Transf<0, uint32_t>::container_type>(
        m, "PTransfBase4");

    // Transformations
    bind_transf<16, uint8_t>(m, "StaticTransf16");
    bind_transf<0, uint8_t>(m, "Transf1");
    bind_transf<0, uint16_t>(m, "Transf2");
    bind_transf<0, uint32_t>(m, "Transf4");

    // Partial perms
    bind_pperm<16, uint8_t>(m, "StaticPPerm16");
    bind_pperm<0, uint8_t>(m, "PPerm1");
    bind_pperm<0, uint16_t>(m, "PPerm2");
    bind_pperm<0, uint32_t>(m, "PPerm4");

    // Perms
    bind_perm<16, uint8_t>(m, "StaticPerm16");
    bind_perm<0, uint8_t>(m, "Perm1");
    bind_perm<0, uint16_t>(m, "Perm2");
    bind_perm<0, uint32_t>(m, "Perm4");
  }
}  // namespace libsemigroups
