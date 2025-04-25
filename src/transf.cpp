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

namespace libsemigroups {

  namespace py = pybind11;

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
1)f]`, and this function returns an iterator yielding these values.

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

:returns: The degree.
:rtype: int
)pbdoc");
      thing.def("__hash__",
                &PTransfBase_::hash_value,
                py::is_operator(),
                R"pbdoc(
Returns a hash value.

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

:returns: The number of distinct image values.
:rtype: int

:complexity: Linear in :py:meth:`degree`.
)pbdoc");
    }

    template <typename PyClass>
    void bind_ptransf_subclass(py::module&      m,
                               PyClass&         thing,
                               std::string_view long_name,
                               std::string_view type_name) {
      using PTransfSubclass = typename PyClass::type;
      using Scalar          = typename PTransfSubclass::point_type;

      thing.def(py::self * py::self);

      thing.def(py::init<>(),
                fmt::format(R"pbdoc(
Constructs an uninitialized {} of degree ``0``.
)pbdoc",
                            long_name)
                    .c_str());

      thing.def("__copy__", [](PTransfSubclass const& self) {
        return PTransfSubclass(self);
      });
      thing.def(
          "copy",
          [](PTransfSubclass const& self) { return PTransfSubclass(self); },
          fmt::format(
              R"pbdoc(
:sig=(self: {1}) -> {1}:
Copy a {0}.

:param self: the {0} to copy.
:type self: {1}

:returns: A copy of the argument.
:rtype: {1}
)pbdoc",
              long_name,
              type_name)
              .c_str());

      std::string exceptions = "";
      if (IsPPerm<PTransfSubclass>) {
        exceptions = R"pbdoc(
:raises LibsemigroupsError:
  if there are repeated values in *imgs* that do not equal ``PPerm.undef()``.
:raises LibsemigroupsError:
  if any value in *imgs* if not ``PPerm.undef()`` and exceeds ``len(imgs)``.
)pbdoc";
      } else {
        exceptions = R"pbdoc(
:raises LibsemigroupsError: if any value in *imgs* exceeds ``len(imgs)``.
)pbdoc";
      }
      if (IsPerm<PTransfSubclass>) {
        exceptions += R"pbdoc(
:raises LibsemigroupsError: if any value in *imgs* exceeds ``len(imgs)``.
:raises LibsemigroupsError: if there are repeated values in *imgs*.
)pbdoc";
      }
      thing.def(py::init([](std::vector<Scalar> const& imgs) {
                  return make<PTransfSubclass>(imgs);
                }),
                py::arg("imgs"),
                fmt::format(
                    R"pbdoc(
A {0} can be constructed from a list of images, as follows:
the image of the point ``i`` under the {0} is ``imgs[i]``.

:param imgs: the list of images
:type imgs: List[int]

:complexity: Linear in :py:meth:`degree`.

{1})pbdoc",
                    long_name,
                    exceptions)
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

Replaces the contents of ``self`` by the product of *x* and *y*.

:param x: a {0}.
:type x: {1}
:param y: a {0}.
:type y: {1}

:complexity: Linear in :py:meth:`degree`.)pbdoc",
              long_name,
              type_name)
              .c_str());
      thing.def_static("one",
                       &PTransfSubclass::one,
                       py::arg("N"),
                       fmt::format(R"pbdoc(
Returns the identity {0} on *N* points. This function returns a
newly constructed {0} with degree equal to the degree of *N*
that fixes every value from ``0`` to *N*.

:param N: the degree.
:type N: int

:returns: The identity {0}.
:rtype: {1}
)pbdoc",
                                   long_name,
                                   type_name)
                           .c_str());
      thing.def(
          "increase_degree_by",
          [](PTransfSubclass& self, size_t m) -> void {
            self.increase_degree_by(m);
          },
          py::arg("m"),
          fmt::format(
              R"pbdoc(
Increases the degree of ``self`` in-place, leaving existing values unaltered.

:param m: the number of points to add.
:type m: int

:returns: ``self``
:rtype: {}

:complexity:
   At worst linear in the sum of the parameter *m* and :py:meth:`degree`.
)pbdoc",
              type_name)
              .c_str());

      thing.def("swap",
                &PTransfSubclass::swap,
                py::arg("that"),
                fmt::format(
                    R"pbdoc(
Swap with another {0} of the same type.

:param that: the {0} to swap with.

:type that: {1}
)pbdoc",
                    long_name,
                    type_name)
                    .c_str());

      if (IsPPerm<PTransfSubclass>) {
        thing.def_static("undef",
                         &PTransfSubclass::undef,
                         R"pbdoc(
Returns the value used to represent "undefined". This static function returns
the integer value used to represent an "undefined" value for this type of
partial permutation.

:returns:
   The integer representation of undefined.

:rtype:
   int
)pbdoc");
      }

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

:complexity: Linear in the degree of *f*.

:returns: The identity.
:rtype: Transf | PPerm | Perm
         )pbdoc");
      // TODO The following are documented transf-helpers.rst for some reason
      m.def("transf_image", [](PTransfSubclass const& f) { return image(f); });
      m.def("transf_domain",
            [](PTransfSubclass const& f) { return domain(f); });
    }

    template <size_t N, typename Scalar>
    void bind_transf(py::module& m, std::string const& name) {
      using Transf_ = Transf<N, Scalar>;

      using container_type = typename Transf_::container_type;

      py::class_<Transf_, PTransfBase<Scalar, container_type>> thing(
          m,
          name.c_str(),
          R"pbdoc(
Class for representing transformations on up to ``2 ** 32`` points.

A *transformation* :math:`f` is just a function defined on the whole of
:math:`\{0, 1, \ldots, n - 1\}` for some integer :math:`n` called the *degree*
of :math:`f`. A transformation is stored as a list of the images of :math:`\{0,
1, \ldots, n - 1\}` , i.e. :math:`[(0)f, (1)f, \ldots, (n - 1)f]`.

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

      // thing.attr("__name__") = "Transf";
      // thing.attr("__qualname__") = "Transf";
      // thing.attr("__module__") = "libsemigroups_pybind11";

      bind_ptransf_subclass(m, thing, "transformation", name);

      thing.def("__repr__",
                [name](Transf_ const& f) { return transf_repr(name, f); });
    }  // bind_transf

    template <size_t N, typename Scalar>
    void bind_pperm(py::module& m, std::string const& name) {
      using PPerm_ = PPerm<N, Scalar>;

      using container_type = typename PPerm_::container_type;

      py::class_<PPerm_, PTransfBase<Scalar, container_type>> thing(
          m,
          name.c_str(),
          R"pbdoc(
Class for representing partial permutations on up to ``2 ** 32`` points.

A *partial permutation* :math:`f` is just an injective partial transformation,
which is stored as a list of the images of :math:`\{0, 1, \ldots, n - 1\}` ,
i.e. i.e. :math:`((0)f, (1)f, \ldots, (n - 1)f)` where the value
:any:`UNDEFINED` is used to indicate that :math:`(i)f` is undefined (i.e. not
among the points where :math:`f` is defined).

.. doctest::

   >>> from libsemigroups_pybind11.transf import PPerm, one, inverse, right_one, left_one, domain, image
   >>> x = PPerm([1, 0, 2], [0, 1, 2], 4)
   >>> x.degree()
   4
   >>> x[0]
   1
   >>> x[3] == x.undef()
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
   [0, 1, 2, 255]
   >>> x.rank()
   3
   >>> one(x)
   PPerm([0, 1, 2, 3], [0, 1, 2, 3], 4)
   >>> x = PPerm.one(8)
   >>> x
   PPerm([0, 1, 2, 3, 4, 5, 6, 7], [0, 1, 2, 3, 4, 5, 6, 7], 8)
   >>> x.degree()
   8
   >>> x.undef()
   255
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

      bind_ptransf_subclass(m, thing, "partial perm", name);

      thing.def("__repr__",
                [name](PPerm_ const& f) { return pperm_repr(name, f); });

      thing.def(py::init([](std::vector<Scalar> const& dom,
                            std::vector<Scalar> const& im,
                            size_t deg) { return make<PPerm_>(dom, im, deg); }),
                py::arg("dom"),
                py::arg("im"),
                py::arg("M"),
                R"pbdoc(
Construct from domain, range, and degree.

Constructs a partial perm of degree *M* such that ``(dom[i])f = ran[i]`` for
all ``i`` and which is :any:`UNDEFINED` on every other value in the range
:math:`[0, M)`.

:param dom: the domain
:type dom: List[int]
:param ran: the image
:type ran: List[int]
:param M: the degree
:type M: int

:raises LibsemigroupsError: the value *M* is not compatible with the type.
:raises LibsemigroupsError: *dom* and *ran* do not have the same size.
:raises LibsemigroupsError: any value in *dom* or *ran* is greater than *M*.
:raises LibsemigroupsError: there are repeated entries in *dom* or *ran*.
)pbdoc");

      m.def("transf_inverse",
            py::overload_cast<PPerm_ const&>(&inverse<N, Scalar>));
      m.def("transf_right_one", &right_one<N, Scalar>);
      m.def("transf_left_one", &left_one<N, Scalar>);
    }  // bind_pperm

    template <size_t N, typename Scalar>
    void bind_perm(py::module& m, std::string const& name) {
      using Perm_          = Perm<N, Scalar>;
      using container_type = typename Perm_::container_type;

      // If we derive from Transf in the next line then the wrong overload of
      // "one" (for example) gets applied (I think pybind11 selects the first
      // matching overload, which is the one for Transf)
      py::class_<Perm_, PTransfBase<Scalar, container_type>> thing(m,
                                                                   name.c_str(),
                                                                   R"pbdoc(
Class for representing permutations on up to ``2 ** 32`` points.

A *permutation* :math:`f` is an injective transformation defined on the whole
of :math:`\{0, 1, \ldots, n - 1\}` for some integer :math:`n` called the
*degree* of :math:`f`. A permutation is stored as a vector of the images of
:math:`(0, 1, \ldots, n - 1)` , i.e. :math:`((0)f, (1)f, \ldots, (n - 1)f)`.

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

      bind_ptransf_subclass(m, thing, "permutation", name);

      thing.def("__repr__",
                [name](Perm_ const& f) { return transf_repr(name, f); });
      m.def("transf_inverse",
            py::overload_cast<Perm_ const&>(&inverse<N, Scalar>));
    }  // bind_perm
  }  // namespace

  void init_transf(py::module& m) {
    // Base classes
    bind_ptransf_base<uint8_t, typename Transf<0, uint8_t>::container_type>(
        m, "PTransfBase1");
    bind_ptransf_base<uint16_t, typename Transf<0, uint16_t>::container_type>(
        m, "PTransfBase2");
    bind_ptransf_base<uint32_t, typename Transf<0, uint32_t>::container_type>(
        m, "PTransfBase4");

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
