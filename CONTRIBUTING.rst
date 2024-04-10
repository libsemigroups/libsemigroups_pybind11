Information for developers
==========================
How to compile
--------------
To build ``libsemigroups_pybind11``, it is first required to have a system
install of ``libsemigroups``. This is explained in full detail in the
``libsemigroups``
`documentation <https://libsemigroups.readthedocs.io/en/latest/install.html>`_.

It is recommended to install libsemigroups without ``hpcombi`` and with an 
external version of ``fmt`` that can be found using the environment variable
``$LD_LIBRARY_PATH``. Then, with ``libsemigroups`` installed, the python
bindings can be ``pip`` installed. This may require the environment variable
``$PKG_CONFIG_PATH`` to be edited.

To create a conda environment with ``fmt``, ``pip``, and correct environment
variables :

.. code-block:: bash
    
    conda create -n libsemigroups python pip fmt
    conda activate libsemigroups
    conda env config vars set LD_LIBRARY_PATH="$CONDA_PREFIX/lib"
    conda env config vars set PKG_CONFIG_PATH="$CONDA_PREFIX/lib/pkgconfig:$CONDA_PREFIX/share/pkgconfig:/usr/local/lib/pkgconfig"

To build libsemigroups (with the above environment active):

.. code-block:: bash

    git clone https://github.com/libsemigroups/libsemigroups
    cd libsemigroups
    ./autogen.sh && ./configure --disable-hpcombi --with-external-fmt && sudo make install -j8

where ``-j8`` instructs the compiler to use 8 threads.

To build the python bindings (with CCache):

.. code-block:: bash

    CC="ccache gcc" CXX="ccache g++"  pip install .

Contributing to a file
----------------------

Building the skeleton of a class
________________________________
If you are adding the bindings for a ``libsemigroups`` class that does not yet
exist in ``libsemigroups_pybind11``, please consider running the script
``generate_pybind11.py`` found in the ``python/`` directory of
``libsemigroups``.

For example:

.. code-block:: bash

    python/generate_pybind11.py libsemigroups::KnuthBendix

to create a skeleton of the code required to bind the ``KnuthBendix`` class.

Binding a function
__________________

To bind the function ``bar`` from the ``libsemigroups`` class ``foo`` to the
module ``m``:

.. code-block:: cpp

    // This is the left-hand side of the file
        m.def("bar",
            &libsemigroups::foo::bar,
            py::arg("a"),
            py::arg("b"),
            R"pbdoc(
    A brief description of bar that is one line long

    A more detailed description of bar, that may explain how each of the
    parameters *a* and *b* should be used. This can be more than one line long,
    and can contain references to other functions such as :py:meth:`baz`. It can
    also contain literals like ``True`` or ``False``.

    :param a: an explanation of what *a* is.
    :type a: str
    :param b: an explanation of what *b* is.
    :type b: int

    :return: The value that should be returned

    .. seealso:: Something that might be interesting.
            )pbdoc")

Notice that there should be **NO BLOCK INDENTATION** in the docstring. This is
so that ``sphinx`` build the docs correctly. For more information on how to
write python documentation with reStructuredText, see 
`this page <https://devguide.python.org/documentation/markup/>`__ in the Python
Developer's guide, and
`this page <https://www.sphinx-doc.org/en/master/usage/restructuredtext/index.html>`__
in the ``sphinx`` documentation.

Adding documentation
--------------------
Each class should have a file that looks like this:

.. code-block:: rest

    .. Copyright (c) 20XX, Name

        Distributed under the terms of the GPL license version 3.

        The full license is in the file LICENSE, distributed with this software.

    .. currentmodule:: _libsemigroups_pybind11

    Class-Name
    ==========

    A description of what the methods in this class do.

    .. doctest::
        
        >>> # This should be a quick example of how to create an instance of
        >>> # YourClass, and run a few functions.
        >>> from libsemigroups_pybind11 import YourClass
        >>> y = YourClass()
        >>> y.run()
        True
        >>> y.count()
        42
    
    Contents
    --------
    .. autosummary::
        :nosignatures:

        YourClass.foo
        YourClass.bar
        YourClass.baz
        YourClass.qux
        YourClass.quux
        YourClass.corge


    Full API
    --------
    .. autoclass:: YourClass
        :members:

For an example, see :doc:`docs/source/knuth-bendix/knuth-bendix.rst` 
