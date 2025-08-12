.. important:: 

   The class |name| has all the methods of the :any:`Runner` class but, for
   boring technical reasons, is not a subclass of :any:`Runner`. If ``thing`` is
   an instance of |name|, then you can use ``thing`` as if it were an instance
   of :any:`Runner` but ``isinstance(thing, Runner)`` will return ``False``.
