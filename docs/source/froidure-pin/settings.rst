.. Copyright (c) 2021, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Settings
--------

This page contains information about the methods of the :py:class:`FroidurePin`
that control various settings.

.. py:method:: FroidurePin.batch_size(self: FroidurePin, val: int) -> FroidurePin

   Set a new value for the batch size.

   :Parameters: **val** (int) - the new value for the batch size.

   :Returns: The calling instance returns itself.

.. py:method:: FroidurePin.batch_size(self: FroidurePin) -> int
   :noindex:

   Returns the current value of the batch size.

   :Parameters: None
   :Returns: An ``int``.
                 
.. py:method:: FroidurePin.concurrency_threshold(self: FroidurePin, val: int) -> FroidurePin

   Set the threshold for concurrency to be used by methods of
   :py:class:`FroidurePin`.

   :Parameters: **val** (int) - the new threshold.

   :Returns: The calling instance returns itself.
                 
.. py:method:: FroidurePin.concurrency_threshold(self: FroidurePin) -> int
   :noindex:

   Returns the current value of the concurrency threshold.

   :Parameters: None
   :Returns: An ``int``.

.. py:method:: FroidurePin.immutable(self: FroidurePin, val: int) -> FroidurePin

   Set immutability.

   :Parameters: **val** (bool) - the new value.

   :Returns: The calling instance returns itself.

.. py:method:: FroidurePin.immutable(self: FroidurePin) -> int
   :noindex:

   Returns the current value of immutability.

   :Parameters: None
   :Returns: A ``bool``.

.. py:method:: FroidurePin.max_threads(self: FroidurePin, val: int) -> FroidurePin

   Set the maximum number of threads.

   :Parameters: **number_of_threads** (int) - the maximum number of
                threads to use.

   :Returns: The calling instance returns itself.

.. py:method:: FroidurePin.max_threads(self: FroidurePin) -> int
   :noindex:

   Returns the current value of the maximum number of threads.

   :Parameters: None
   :Returns: An ``int``.

.. py:method:: FroidurePin.reserve(self: FroidurePin, val: int) -> None

   Requests the given capacity for elements.

   :param val: the number of elements to reserve space for.
   :type val: int

   :return: None
