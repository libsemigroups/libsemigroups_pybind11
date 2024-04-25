.. Copyright (c) 2022-2024, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Running and reporting
=====================

This page contains information about the methods of the :py:class:`Konieczny`
class related to running and reporting.

.. py:method:: Konieczny.run(self: Konieczny) -> None

   Run the algorithm until it finishes.

   :Parameters: None

   :return: None

.. py:method:: Konieczny.run_for(self: Konieczny, t: datetime.timedelta) -> None

   Run for a specified amount of time.

   :param t: the time to run for.
   :type t: datetime.timedelta

   :return: None

.. py:method:: Konieczny.run_until(self: Konieczny, func: Callable[], bool) -> None

   Run until a nullary predicate returns ``True`` or :py:meth:`finished`.

   :param func: a function.
   :type func: Callable[], bool

   :return: None

.. py:method:: Konieczny.kill(self: Konieczny) -> None

   Stop running the main algorithm (thread-safe).

   :Parameters: None
   :return: None.

.. py:method:: Konieczny.dead(self: Konieczny) -> bool

   Check if the runner is dead.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: Konieczny.finished(self: Konieczny) -> bool

   Check if the main algorithm implemented in this class has been run to
   completion or not.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: Konieczny.started(self: Konieczny) -> bool

   Check if :py:meth:`run` has been called at least once before.

   Returns ``True`` if :py:meth:`run` has started to run (it can be running or
   not).

   :Parameters: None

   :return: A ``bool``.

   .. seealso:: :py:meth:`finished`.

.. py:method:: Konieczny.report(self: Konieczny) -> bool

  Check if it is time to report.

  :Parameters: None
  :return: A ``bool``.

.. py:method:: Konieczny.report_every(self: Konieczny, t: datetime.timedelta) -> None

  Set the minimum elapsed time between reports.

  :param t: the amount of time between reports.
  :type t: datetime.timedelta

  :Returns: None

.. py:method:: Konieczny.report_why_we_stopped(self: Konieczny) -> None

   Report why we stopped.

   :Parameters: None
   :return: None

.. py:method:: Konieczny.running(self: Konieczny) -> bool

   Check if currently running.

   :Parameters: None

   :return:
     ``True`` if :py:meth:`run` is in the process of running and ``False`` if
     it is not.

   .. seealso:: :py:meth:`run`.

.. py:method:: Konieczny.timed_out(self: Konieczny) -> bool

   Check if the main algorithm has or should timed out.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: Konieczny.stopped(self: Konieczny) -> bool

   Check if the main algorithm has or should stop.

   This function can be used to check whether or not :py:meth:`run` has
   been stopped for whatever reason. In other words, it checks if
   :py:meth:`timed_out`, :py:meth:`finished`, or :py:meth:`dead`.

   :Parameters: None
   :return: A ``bool``.

.. py:method:: Konieczny.stopped_by_predicate(self: Konieczny) -> bool

   Check if the main algorithm was, or should be, stopped by the nullary
   predicate passed as first argument to :py:meth:`run_until`.

   :Parameters: None
   :return: A ``bool``.
