#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "libsemigroups/libsemigroups.hpp"

namespace py = pybind11;
using FroidurePinTransf256 =
    libsemigroups::FroidurePin<libsemigroups::Transformation<uint8_t>>;

using Transf256 = libsemigroups::Transformation<uint8_t>;
using BMat8 = libsemigroups::BMat8;

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

  py::class_<libsemigroups::BMat8>(m, "BMat8")
      .def(py::init<>())
      .def(py::init<uint64_t>())
      .def(py::init<BMat8 const &>())
      .def(py::init<std::vector<std::vector<bool>> const &>())
      .def("__eq__", &BMat8::operator==)
      .def("__lt__", &BMat8::operator<)
      .def("get", &BMat8::get)
      .def("set", &BMat8::set)
      .def("to_int", &BMat8::to_int)
      .def("transpose", &BMat8::transpose)
      .def(py::self * py::self)
      .def_static("random", py::overload_cast<>(&BMat8::random))
      .def_static("random", py::overload_cast<size_t>(&BMat8::random))
      .def("swap", &BMat8::swap)
      .def("row_space_basis", &BMat8::row_space_basis)
      .def("col_space_basis", &BMat8::col_space_basis)
      .def("rows", &BMat8::rows)
      .def("row_space_size", &BMat8::row_space_size)
      .def("nr_rows", &BMat8::nr_rows)
      .def("is_regular_element", &BMat8::is_regular_element)
      .def("one", &BMat8::one)
      .def("__repr__", &libsemigroups::detail::to_string<BMat8>);

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

      :return: An iterator to strings over the alphabet ``alphabet`` of length at most ``upper_bound`` in the range ``[first, last)``.
      )pbdoc");

  // upper_bound (int):   // first (str): the first string
  // last (str): one past the last string

  detail::bind_froidure_pin<Transf256>(m, "Transf256");
  detail::bind_froidure_pin<BMat8>(m, "BMat8");

#ifdef VERSION_INFO
  m.attr("__version__") = VERSION_INFO;
#else
  m.attr("__version__") = "dev";
#endif
}
