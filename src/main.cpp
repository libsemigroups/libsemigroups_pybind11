#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "libsemigroups/libsemigroups.hpp"

namespace py = pybind11;
using FroidurePinTransf256 =
    libsemigroups::FroidurePin<libsemigroups::Transformation<uint8_t>>;

using Transf256 = libsemigroups::Transformation<uint8_t>;
using BMat8 = libsemigroups::BMat8;
using word_type = libsemigroups::word_type;

namespace detail {
std::string transf_repr(libsemigroups::Transformation<uint8_t> const &a) {
  auto out = std::string("Transf256([");
  for (size_t i = 0; i < a.degree(); ++i) {
    out += libsemigroups::detail::to_string(size_t(a[i]));
    if (i != a.degree() - 1) {
      out += ", ";
    }
  }
  out += "])";
  return out;
}

template <typename T>
void bind_froidure_pin(py::module &m, std::string typestr) {
  using Class = libsemigroups::FroidurePin<T>;
  std::string pyclass_name = std::string("FroidurePin") + typestr;
  py::class_<Class>(m, pyclass_name.c_str(), py::buffer_protocol(),
                    py::dynamic_attr())
      .def(py::init<std::vector<T> const &>())
      .def(py::init<>())
      .def("size", &Class::size)
      .def("add_generator", &Class::add_generator)
      .def("number_of_generators", &Class::nr_generators)
      .def("batch_size", py::overload_cast<size_t>(&Class::batch_size))
      .def("batch_size", py::overload_cast<>(&Class::batch_size, py::const_))
      .def("max_threads", py::overload_cast<size_t>(&Class::max_threads))
      .def("max_threads", py::overload_cast<>(&Class::max_threads, py::const_))
      .def("concurrency_threshold",
           py::overload_cast<size_t>(&Class::concurrency_threshold))
      .def("concurrency_threshold",
           py::overload_cast<>(&Class::concurrency_threshold, py::const_))
      .def("reserve", &Class::reserve);
}
}  // namespace detail

PYBIND11_MODULE(libsemigroups_pybind11, m) {
  // py::options options;
  // options.disable_function_signatures();
  m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: libsemigroups_pybind11

        .. autosummary::
           :toctree: _generate

        .. autofunction:: silo
    )pbdoc";

  ////////////////////////////////////////////////////////////////////////
  // element.hpp
  ////////////////////////////////////////////////////////////////////////

  py::class_<libsemigroups::Transformation<uint8_t>>(m, "Transf256")
      .def(py::init<std::vector<uint8_t> const &>())
      .def("rank", &libsemigroups::Transformation<uint8_t>::crank,
           R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc")
      .def("degree", &libsemigroups::Transformation<uint8_t>::degree)
      .def(py::self * py::self)
      .def(
          "__getitem__",
          [](const libsemigroups::Transformation<uint8_t> &a, size_t b) {
            return a[b];
          },
          py::is_operator())
      .def("__repr__", &detail::transf_repr);

  ////////////////////////////////////////////////////////////////////////
  // bmat8.hpp
  ////////////////////////////////////////////////////////////////////////

  py::class_<libsemigroups::BMat8>(m, "BMat8")
      .def(py::init<>(), R"pbdoc(
      Returns an uninitalised BMat8.
      )pbdoc")
      .def(py::init<uint64_t>())
      .def(py::init<BMat8 const &>())
      .def(py::init<std::vector<std::vector<bool>> const &>())
      .def("__eq__", &BMat8::operator==)
      .def("__lt__", &BMat8::operator<)
      .def("get", &BMat8::get, py::arg("i"), py::arg("j"),
           R"pbdoc(
           Returns the entry in the (i, j)th position.
           )pbdoc")
      .def("set", &BMat8::set, py::arg("i"), py::arg("j"), py::arg("val"),
           R"pbdoc(
           Sets the (i, j)th entry to ``val``.
           )pbdoc")
      .def("to_int", &BMat8::to_int,
           R"pbdoc(
          Returns the integer representation of the ``BMat8``, that is
          an integer obtained by interpreting an 8 x 8 ``BMat8`` as a
          sequence of 64 bits (reading rows left to right, from top to bottom)
          and then realising this sequence as an integer.
           )pbdoc")
      .def("transpose", &BMat8::transpose,
           R"pbdoc(
           Returns the transpose of ``self``.
           )pbdoc")
      .def(py::self * py::self)
      .def_static("random", py::overload_cast<>(&BMat8::random))
      .def_static("random", py::overload_cast<size_t>(&BMat8::random))
      .def("swap", &BMat8::swap, py::arg("other"),
           R"pbdoc(
            Swaps the contents of ``self`` and ``other``.
           )pbdoc")
      .def("row_space_basis", &BMat8::row_space_basis,
           R"pbdoc(
           This method returns a ``BMat8`` whose non-zero rows form a basis for
           the row space of ``self``.
           )pbdoc")
      .def("col_space_basis", &BMat8::col_space_basis,
           R"pbdoc(
           This method returns a ``BMat8`` whose non-zero columns form a basis for
           the column space of ``self``.
           )pbdoc")
      .def("rows", &BMat8::rows,
           R"pbdoc(
           This method returns a list of integers representing the rows of
           ``self``. The list will always be of length 8, even if ``self``
           was constructed with fewer rows.
           )pbdoc")
      .def("row_space_size", &BMat8::row_space_size,
           R"pbdoc(
           Returns the size of the row space of ``self``.
           )pbdoc")
      .def("nr_rows", &BMat8::nr_rows,
           R"pbdoc(
           Returns the number of non-zero rows in ``self``.
           )pbdoc")
      .def("is_regular_element", &BMat8::is_regular_element,
           R"pbdoc(
           Check whether ``self`` is a regular element of the full boolean
           matrix monoid of appropriate dimension.
           )pbdoc")
      .def("one", &BMat8::one,  // py::arg("dim") = 8,
           R"pbdoc(
           This method returns the ``BMat8`` with the first ``dim`` entries in
           the main diagonal equal to ``1`` and every other value equal to ``0``.
           )pbdoc")
      .def("__repr__", &libsemigroups::detail::to_string<BMat8>)
      .def("nr_cols", &libsemigroups::bmat8_helpers::nr_cols,
           R"pbdoc(
           Returns the number of non-zero columns in ``self``.
           )pbdoc")
      .def("column_space_size", &libsemigroups::bmat8_helpers::col_space_size,
           R"pbdoc(
           Returns the size of the row space of ``self``.
           )pbdoc")
      .def("minimum_dim", &libsemigroups::bmat8_helpers::minimum_dim,
           R"pbdoc(
            This member function returns the maximal ``i`` such that row ``i``
            or column ``i`` contains a ``1``.
            )pbdoc");

  ////////////////////////////////////////////////////////////////////////
  // cong.hpp
  ////////////////////////////////////////////////////////////////////////

  py::enum_<libsemigroups::congruence_type>(m, "congruence_type")
      .value("left", libsemigroups::congruence_type::left)
      .value("right", libsemigroups::congruence_type::right)
      .value("twosided", libsemigroups::congruence_type::twosided)
      .export_values();

  py::enum_<libsemigroups::tril>(m, "tril")
      .value("True", libsemigroups::tril::TRUE)
      .value("False", libsemigroups::tril::FALSE)
      .value("unknown", libsemigroups::tril::unknown)
      .export_values();

  py::class_<libsemigroups::Congruence>(m, "Congruence")
      .def(py::init<libsemigroups::congruence_type>())
      .def(py::init<libsemigroups::congruence_type,
                    libsemigroups::FpSemigroup &>())
      .def("set_nr_generators", &libsemigroups::Congruence::set_nr_generators,
           py::arg("n"),
           R"pbdoc(
           Set the number of generators of the congruence.

           :param n: the number of generators.
           :type n: int

           :return: (None)
           )pbdoc")
      .def("nr_generators", &libsemigroups::Congruence::nr_generators,
           R"pbdoc(
           Returns the number of generators.

           :return: The number of generators of the semigroup of the congruence that an object of this type represents, or libsemigroups::UNDEFINED.
           )pbdoc")
      .def("add_pair",
           py::overload_cast<word_type const &, word_type const &>(
               &libsemigroups::Congruence::add_pair),
           py::arg("u"), py::arg("v"),
           R"pbdoc(
           Add a generating pair to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: (None)
           )pbdoc")
      // overload cast didn't work for report_every
      .def("report_every",
           (void (libsemigroups::Congruence::*)(std::chrono::nanoseconds)) &
               libsemigroups::Runner::report_every,
           py::arg("t"),
           R"pbdoc(
           Set the minimum elapsed time between reports.

           :param t: the amount of time (in nanoseconds) between reports.
           :type t: int

           :return: (None)
           )pbdoc")
      .def("report", &libsemigroups::Congruence::report,
           R"pbdoc(
           Check if it is time to report.

           :return: A bool.
           )pbdoc")
      .def("report_why_we_stopped",
           &libsemigroups::Congruence::report_why_we_stopped,
           R"pbdoc(
           Report why Runner::run stopped.

           :return: (None)
           )pbdoc")
      .def("kill", &libsemigroups::Congruence::kill,
           R"pbdoc(
           Stop Runner::run from running (thread-safe).

           :return: (None).
           )pbdoc")
      .def("run", &libsemigroups::Congruence::run,
           R"pbdoc(
           Attempt to enumerate the congruence.

           :return: (None)
           )pbdoc")
      .def("run_for",
           (void (libsemigroups::Congruence::*)(std::chrono::nanoseconds)) &
               libsemigroups::Runner::run_for,
           py::arg("t"),
           R"pbdoc(
           Run for a specified amount of time.

           :param t: the time in nanoseconds to run for.
           :type t: int

           :return: (None)

           Example
           -------

           .. code-block:: python

              from datetime import timedelta
              from libsemigroups_pybind11 import Congruence
              from libsemigroups_pybind11 import congruence_type

              C = Congruence(congruence_type.twosided)
              C.set_nr_generators(1)
              C.add_pair([0] * 1000, [0] * 999)
              C.run_for(timedelta(microseconds=10))
           )pbdoc")
      .def("run_until",
           (void (libsemigroups::Congruence::*)(std::function<bool()> &)) &
               libsemigroups::Runner::run_until,
           py::arg("func"),
           R"pbdoc(
           Run until a nullary predicate returns true or Runner::finished.

           :param func: a function.
           :type func: func

           :return: (None)
           )pbdoc")
      .def("less", &libsemigroups::Congruence::less, py::arg("u"), py::arg("v"),
           R"pbdoc(
           This function returns ``True`` if the congruence class of ``u`` is
           less than the class of ``v`` in a total ordering of congruence
           classes.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: ``True`` if the class of ``u`` is less than that of ``v``.
           )pbdoc")
      .def("const_contains", &libsemigroups::Congruence::const_contains,
           py::arg("u"), py::arg("v"),
           R"pbdoc(
           Check if a pair of words is known to belong to the congruence.

           :param u: a word (list of integers) over the generators of the semigroup.
           :type u: list
           :param v: a word (list of integers) over the generators of the semigroup.
           :type v: list

           :return: libsemigroups::tril::TRUE if the words u and v are known to belong to the same congruence class libsemigroups::tril::FALSE if the words are known to not belong to the same congruence class libsemigroups::tril::unknown otherwise.
           )pbdoc")
      .def("nr_classes", &libsemigroups::Congruence::nr_classes,
           R"pbdoc(
           Computes the total number of classes in the congruence represented
           by an instance of this type.

           :return: The number of congruences classes of this if this number is finite, or libsemigroups::POSITIVE_INFINITY in some cases if this number is not finite.
           )pbdoc");

  ////////////////////////////////////////////////////////////////////////
  // siso.hpp
  ////////////////////////////////////////////////////////////////////////

  m.def(
      "silo",
      [](std::string const &alphabet, size_t upper_bound,
         std::string const &first, std::string const &last) {
        return py::make_iterator(
            libsemigroups::cbegin_silo(alphabet, upper_bound, first, last),
            libsemigroups::cend_silo(alphabet, upper_bound, first, last));
      },
      py::arg("alphabet"), py::arg("upper_bound"), py::arg("first"),
      py::arg("last"),
      R"pbdoc(
      Returns an iterator to strings in lexicographic order (silo).

      :param alphabet: the alphabet
      :type alphabet: str
      :param upper_bound: the maximum length of string to return
      :type upper_bound: int
      :param first: the first string
      :type first: str
      :param last: one past the last string
      :type last: str

      :return: An iterator to strings in lexicographic order over the alphabet ``alphabet`` of length at most ``upper_bound`` in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in silo("ba", 3, "b", "aaa")]
         # ["b", "bb", "ba", "a", "ab", "aa"]

      )pbdoc");

  m.def(
      "sislo",
      [](std::string const &alphabet, std::string const &first,
         std::string const &last) {
        return py::make_iterator(
            libsemigroups::cbegin_sislo(alphabet, first, last),
            libsemigroups::cend_sislo(alphabet, first, last));
      },
      py::arg("alphabet"), py::arg("first"), py::arg("last"),
      R"pbdoc(
      Returns an iterator to strings in short-lex order (sislo).

      :param alphabet: the alphabet
      :type alphabet: str
      :param first: the first string
      :type first: str
      :param last: one past the last string
      :type last: str

      :return: An iterator to strings over the alphabet ``alphabet`` in short-lex order and in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in sislo("ba", "b", "aaa")]
         # ['b', 'a', 'bb', 'ba', 'ab', 'aa', 'bbb', 'bba', 'bab', 'baa', 'abb', 'aba', 'aab']
      )pbdoc");

  ////////////////////////////////////////////////////////////////////////
  // wilo.hpp
  ////////////////////////////////////////////////////////////////////////

  m.def(
      "wilo",
      [](size_t const n, size_t const upper_bound, word_type const &first,
         word_type const &last) {
        return py::make_iterator(
            libsemigroups::cbegin_wilo(n, upper_bound, first, last),
            libsemigroups::cend_wilo(n, upper_bound, first, last));
      },
      py::arg("n"), py::arg("upper_bound"), py::arg("first"), py::arg("last"),
      R"pbdoc(
      Returns an iterator to words in lexicographic order (wilo).

      :param n: the number of letters
      :type n: int
      :param upper_bound: the maximum length of string to return
      :type upper_bound: int
      :param first: the first word
      :type first: list
      :param last: one past the last word
      :type last: list

      :return: An iterator to words in lexicographic order over an alphabet with  ``n`` letters of length at most ``upper_bound`` in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in wilo(2, 3, [0], [1, 1, 1])]
         # [[0], [0, 0], [0, 1], [1], [1, 0], [1, 1]]
      )pbdoc");

  ////////////////////////////////////////////////////////////////////////
  // wislo.hpp
  ////////////////////////////////////////////////////////////////////////

  m.def(
      "wislo",
      [](size_t const n, word_type const &first, word_type const &last) {
        return py::make_iterator(libsemigroups::cbegin_wislo(n, first, last),
                                 libsemigroups::cend_wislo(n, first, last));
      },
      py::arg("n"), py::arg("first"), py::arg("last"),
      R"pbdoc(
      Returns an iterator to strings in short-lex order (sislo).

      :param n: the number of letter
      :type n: int
      :param first: the first word
      :type first: list
      :param last: one past the last word
      :type last: list

      :return: An iterator to words over an alphabet with ``n`` in short-lex order and in the range ``[first, last)``.

      Example
      -------
      .. code-block:: python

         [x for x in wislo(2, [0], [1, 1])]
         # [[0], [1], [0, 0], [0, 1], [1, 0]]
      )pbdoc");

  m.def("number_of_words", &libsemigroups::number_of_words, py::arg("n"),
        py::arg("min"), py::arg("max"),
        R"pbdoc(
      Returns the number of words over a given alphabet in some range.

      :param n: the number of letter
      :type n: int
      :param min: the minimum length of a word
      :type min: int
      :param max: one greater than the maximum length of a word
      :type max: int

      :return: The number words over an alphabet with ``n`` with length in the range ``[min, max)``.

      Example
      -------
      .. code-block:: python

         number_of_words(2, 0, 10)
         # 1023
      )pbdoc");

  detail::bind_froidure_pin<Transf256>(m, "Transf256");
  detail::bind_froidure_pin<BMat8>(m, "BMat8");

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
