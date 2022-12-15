.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Running and reporting
=====================

This page contains information about the methods of the :py:class:`FroidurePin`
class related to running and reporting.

.. py:method:: FroidurePin.enumerate(self: FroidurePin, limit: int) -> None

   Enumerate until at least a specified number of elements are
   found.

   :param limit: the limit for :py:meth:`current_size`
   :type limit: int

   :return: None

.. py:method:: FroidurePin.run(self: FroidurePin) -> None

   Run the algorithm until it finishes.

   :Parameters: None

   :return: None

.. py:method:: FroidurePin.run_for(self: FroidurePin, t: datetime.timedelta) -> None

   Run for a specified amount of time.

   :param t: the time to run for.
   :type t: datetime.timedelta

   :return: None

.. py:method:: FroidurePin.run_until(self: FroidurePin, func: Callable[], bool) -> None

   Run until a nullary predicate returns ``True`` or :py:meth:`finished`.

   :param func: a function.
   :type func: Callable[], bool

   :return: None

.. py:method:: FroidurePin.kill(self: FroidurePin) -> None

   Stop running the main algorithm (thread-safe).

   :Parameters: None
   :return: None.

.. py:method:: FroidurePin.dead(self: FroidurePin) -> bool

   Check if the runner is dead.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: FroidurePin.finished(self: FroidurePin) -> bool

   Check if the main algorithm implemented in this class has been run to
   completion or not.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: FroidurePin.started(self: FroidurePin) -> bool

   Check if :py:meth:`run` has been called at least once before.

   Returns ``True`` if :py:meth:`run` has started to run (it can be running or
   not).

   :Parameters: None

   :return: A ``bool``.

   .. seealso:: :py:meth:`finished`.

.. py:method:: FroidurePin.report(self: FroidurePin) -> bool

  Check if it is time to report.

  :Parameters: None
  :return: A ``bool``.

.. py:method:: FroidurePin.report_every(self: FroidurePin, t: datetime.timedelta) -> None

  Set the minimum elapsed time between reports.

  :param t: the amount of time between reports.
  :type t: datetime.timedelta

  :Returns: None

.. py:method:: FroidurePin.report_why_we_stopped(self: FroidurePin) -> None

   Report why we stopped.

   :Parameters: None
   :return: None

.. py:method:: FroidurePin.running(self: FroidurePin) -> bool

   Check if currently running.

   :Parameters: None

   :return:
     ``True`` if :py:meth:`run` is in the process of running and ``False`` if
     it is not.

   .. seealso:: :py:meth:`run`.

.. py:method:: FroidurePin.timed_out(self: FroidurePin) -> bool

   Check if the main algorithm has or should timed out.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: FroidurePin.stopped(self: FroidurePin) -> bool

   Check if the main algorithm has or should stop.

   This function can be used to check whether or not :py:meth:`run` has
   been stopped for whatever reason. In other words, it checks if
   :py:meth:`timed_out`, :py:meth:`finished`, or :py:meth:`dead`.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: FroidurePin.stopped_by_predicate(self: FroidurePin) -> bool

   Check if the main algorithm was, or should be, stopped by the nullary
   predicate passed as first argument to :py:meth:`run_until`.

   :Parameters: None
   :return: A ``bool``.
