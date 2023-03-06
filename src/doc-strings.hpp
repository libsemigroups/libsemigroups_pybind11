//
// libsemigroups_pybind11 - C++ library for semigroups and monoids
//
// Copyright (C) 2021 James D. Mitchell
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

#ifndef SRC_DOC_STRINGS_HPP_
#define SRC_DOC_STRINGS_HPP_

namespace runner_doc_strings {

  auto const report_every = R"pbdoc(
               Set the minimum elapsed time between reports.

               :param t: the amount of time between reports.
               :type t: datetime.timedelta

               :Returns: (None)
             )pbdoc";

  auto const report =
      R"pbdoc(
               Check if it is time to report.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc";

  auto const report_why_we_stopped = R"pbdoc(
               Report why we stopped.

               :Parameters: None
               :return: (None)
             )pbdoc";

  auto const kill =
      R"pbdoc(
               Stop running the main algorithm(s) (thread-safe).

               :Parameters: None
               :return: (None).
             )pbdoc";

  auto const run_for =
      R"pbdoc(
        Run for a specified amount of time.

        :param t: the time to run for.
        :type t: datetime.timedelta

        :return: (None)

        .. doctest::

           >>> from datetime import timedelta
           >>> from libsemigroups_pybind11 import ToddCoxeter, congruence_kind
           >>> tc = ToddCoxeter(congruence_kind.twosided)
           >>> tc.set_number_of_generators(1)
           >>> tc.add_pair([0] * 1000, [0] * 999)
           >>> tc.run_for(timedelta(microseconds=10))
      )pbdoc";

  auto const run_until =
      R"pbdoc(
               Run until a nullary predicate returns ``True`` or
               :py:meth:`finished`.

               :param func: a function.
               :type func: Callable[], bool

               :return: (None)
             )pbdoc";

  auto const run =
      R"pbdoc(
               Run the algorithm until it finishes.
               :Parameters: None

               :return: (None)
           )pbdoc";

  auto const dead = R"pbdoc(
               Check if the runner is dead.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc";

  auto const finished = R"pbdoc(
               Check if the main algorithm, implemented in this class,
               has been run to completion or not.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc";

  auto const timed_out = R"pbdoc(
               Check if the main algorithm has or should timed out.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc";

  auto const stopped_by_predicate =
      R"pbdoc(
             Check if the main algorithm was, or should be, stopped by the nullary
             predicate passed as first argument to :py:meth:`run_until`.

             :Parameters: None
             :return: A ``bool``.
             )pbdoc";
  auto const stopped =
      R"pbdoc(
        Check if the runner is stopped.

        This function can be used to check whether or not :py:meth:`run` has
        been stopped for whatever reason. In other words, it checks if
        :py:meth:`timed_out`, :py:meth:`finished`, or :py:meth:`dead`.

        :Parameters: None
        :return: A ``bool``.
             )pbdoc";

  auto const running =
      R"pbdoc(
     Check if currently running.

     :Parameters: (None)
     :return:
       ``True`` if :py:meth:`run` is in the process of running and ``False`` if
       it is not.

     .. seealso:: :py:meth:`run`.
             )pbdoc";

  auto const started =
      R"pbdoc(
     Check if :py:meth:`run` has been called at least once before.

     Returns ``True`` if :py:meth:`run` has started to run (it can be running or
     not).

     :Parameters: (None)

     :return: A ``bool``.

     .. seealso:: :py:meth:`finished`.
             )pbdoc";

}  // namespace runner_doc_strings

namespace cong_intf_doc_strings {
  auto const const_contains =
      R"pbdoc(
        Check if a pair of words is known to belong to the congruence.

        :param u: a word  over the generators of the semigroup.
        :type u: List[int]
        :param v: a word  over the generators of the semigroup.
        :type v: List[int]

        :return:
          :py:obj:`tril.TRUE` if the words ``u`` and ``v`` are known to
          belong to the same congruence class :py:obj:`tril.FALSE` if
          the words are known to not belong to the same congruence class
          :py:obj:`tril.unknown` otherwise.
      )pbdoc";

  auto const set_number_of_generators =
      R"pbdoc(
        Set the number of generators of the congruence.

        :param n: the number of generators.
        :type n: int
        :return: (None)
      )pbdoc";

  auto const number_of_generators = R"pbdoc(
               Returns the number of generators specified by
               :py:meth:`set_number_of_generators`.

               :Parameters: None

               :return:
                 The number of generators of the semigroup of the congruence,
                 or :py:obj:`UNDEFINED`.
             )pbdoc";
  auto const add_pair             = R"pbdoc(
               Add a generating pair to the congruence.

               :param u: a word over the generators of the semigroup.
               :type u: List[int]
               :param v: a word over the generators of the semigroup.
               :type v: List[int]

               :return: (None)
             )pbdoc";

  auto const number_of_generating_pairs =
      R"pbdoc(
               Returns the number of generating pairs added by
               :py:meth:`add_pair`.

               :return:
                 The number of generating pairs of the congruence that an
                 object of this type represents.
             )pbdoc";
  auto const less =
      R"pbdoc(
               This function returns ``True`` if the congruence class of ``v``
               is less than the class of ``v`` in a total ordering of
               congruence classes.

               :param u: a word over the generators of the semigroup.
               :type u: List[int]
               :param v: a word over the generators of the semigroup.
               :type v: List[int]

               :return:
                 ``True`` if the class of ``u`` is less than that of ``v``.
             )pbdoc";

  auto const contains = R"pbdoc(
               Check if a pair of words belongs to the congruence.

               :param u: a word over the generators of the semigroup.
               :type u: List[int]
               :param v: a word over the generators of the semigroup.
               :type v: List[int]

               :return:
                 ``True`` if the words ``u`` and ``v`` belong to the same
                 congruence class, and ``False`` otherwise.
             )pbdoc";

  auto const number_of_classes = R"pbdoc(
               Returns the number of classes in the congruence.

               :Parameters: None

               :return:
                 The number of congruences classes of this if this
                 number is finite, or :py:obj:`POSITIVE_INFINITY` in some cases
                 if this number is not finite.
             )pbdoc";

  auto const number_of_non_trivial_classes = R"pbdoc(
               Returns the number of non-trivial classes (size > 1) of the
               congruence.

               :Parameters: None

               :return: The number of non-trivial classes of the congruence.
             )pbdoc";

  auto const non_trivial_classes = R"pbdoc(
              Returns the words belonging to non-trivial class with given index.

              :param i: the index of the non-trivial class.
              :type i: int

              :return: A ``List[List[int]]``.
            )pbdoc";

  auto const quotient_froidure_pin =
      R"pbdoc(
               Returns a semigroup represented as :py:class:`FroidurePin` that
               is isomorphic to the quotient of the parent semigroup by
               the 2-sided congruence that this represents.

               :Parameters: None

               :return: A :py:class:`FroidurePin`.
             )pbdoc";
  auto const has_quotient_froidure_pin =

      R"pbdoc(
               Returns ``True`` if the congruence knows an isomorphic quotient
               semigroup represented by an instance of :py:class:`FroidurePin`.

               :Parameters: None

               :return: A ``bool``.
             )pbdoc";
  auto const parent_froidure_pin =
      R"pbdoc(
               Returns the :py:class:`FroidurePin` over which the
               congruence was defined, if it exists.

               :Parameters: None

               :return: A :py:class:`FroidurePin`.
             )pbdoc";

  auto const has_parent_froidure_pin =
      R"pbdoc(
               Returns ``True`` if the congruence was created from a
               :py:obj:`FroidurePin` instance.

               :Parameters: None

               :return: A ``bool``.
             )pbdoc";

  auto const is_quotient_obviously_finite =
      R"pbdoc(
               Return ``True`` if the number of classes in the congruence
               is obviously finite, and ``False`` if it is not obviously
               finite.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc";

  auto const is_quotient_obviously_infinite =
      R"pbdoc(
               Return ``True`` if the number of classes in the congruence is
               obviously infinite, and ``False`` if it is not obviously
               infinite.

               :Parameters: None
               :return: A ``bool``.
             )pbdoc";

  auto const word_to_class_index = R"pbdoc(
               If the congruence, that an object of this type represents, is
               defined over a semigroup with generators :math:`A`, then this
               function defines a surjective function from the set of all words
               over :math:`A` to either :math:`\{0, 1, \ldots, n - 1\}`, where
               :math:`n` is the number of classes, or to the non-negative
               integers :math:`\{0, 1, \ldots\}` if this has infinitely many
               classes.

               :param w:
                 the word whose class index we want to find.
               :type w: List[int]

               :return:
                  The index of the congruence class corresponding to the word.
               )pbdoc";

  auto const class_index_to_word =
      R"pbdoc(
               If the congruence is defined over a semigroup with generators
               :math:`A`, then this function defines a injective function from
               :math:`\{0, 1, \ldots, n - 1\}`, where :math:`n` is the number
               of classes, or to the non-negative integers :math:`\{0, 1,
               \ldots\}` if this has infinitely many classes, to a fixed set of
               words over :math:`A` representing distinct congruences classes.

               :param i:
                  the index of the class whose representative we want to find
               :type i: int

               :return:
                 The word representing the ``i``-th class of the congruence
             )pbdoc";

  auto const kind = R"pbdoc(
               Return if the congruence was created as a left, right, or
               two-sided congruence.

               :Parameters: None
               :return: A :py:class:`congruence_kind`.
             )pbdoc";
  auto const generating_pairs =
      R"pbdoc(
               Returns an iterator pointing to the first generating pair
               of the congruence (if any).

               :Parameters: None
               :return: An iterator.
            )pbdoc";
}  // namespace cong_intf_doc_strings
#endif  //  SRC_DOC_STRINGS_HPP_
