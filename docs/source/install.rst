..
    Copyright (c) 2021-2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

Installation
============

Installing with pip
-------------------

It's possible to install ``libsemigroups_pybind11`` using ``pip`` by doing one
of the following (depending on your system and setup):

.. highlight:: console

::

    $ pip install libsemigroups_pybind11


::
  
    $ pip3 install libsemigroups_pybind11
  
::
  
    $ python -m pip install libsemigroups_pybind11


..  Installing with conda
    ---------------------
    
    This installation method assumes that you have anaconda or miniconda installed.
    See the `getting started`_ and `miniconda download page`_ on the conda_ website.
    
    .. _conda: https://conda.io/
    
    .. _getting started: http://bit.ly/33B0Vfs
    
    .. _miniconda download page: https://conda.io/miniconda.html
    
    It might be a good idea to create and activate a conda environment to contain
    the installation of the ``libsemigroups_pybind11``:
    
    ::
    
        conda create --name libsemigroups
        conda activate libsemigroups
    
    Install ``libsemigroups_pybind11``:
    
    ::
    
        conda install -c conda-forge libsemigroups_pybind11
    
    .. warning::
    
        At present this does not work for Macs with M1 processors.

From the sources
----------------

Before installing ``libsemigroups_pybind11`` from its sources, you should first
perform a system install of the C++ library ``libsemigroups``. For information
about how to do this, see the `libsemigroups installation guide
<https://libsemigroups.github.io/libsemigroups/md_install.html>`_.

Assuming that you have ``libsemigroups`` installed, you can install
``libsemigroups_pybind11`` as follows:

::

    $ git clone https://github.com/libsemigroups/libsemigroups_pybind11
    $ cd libsemigroups_pybind11
    $ pip install .

From a release archive
~~~~~~~~~~~~~~~~~~~~~~

To build ``libsemigroups_pybind11`` from a release archive:

.. Unfortunately, text replacement doesn't work inside of code blocks, so it is
   necessary to update the version number below manually.

::

    $ curl -L -O https://github.com/libsemigroups/libsemigroups_pybind11/releases/latest/download/libsemigroups_pybind11-1.0.1.tar.gz
    $ tar -xf libsemigroups_pybind11-1.0.1.tar.gz
    $ rm -f libsemigroups_pybind11-1.0.1.tar.gz
    $ cd libsemigroups_pybind11-1.0.1
    $ pip install .

Building the documentation
--------------------------

Assuming you have ``python3`` and ``make`` installed, you can build the doc by
using:

::

    $ python3 -m pip3 install -r requirements.txt
    $ make doc

If you don't have ``make``, you can run the executable ``./etc/make-doc.sh``
instead of running ``make doc`` (which is precisely what ``make doc`` does).

Issues
------

If you find any problems with ``libsemigroups_pybind11``, or have any
suggestions for features that you'd like to see, please use the
`issue tracker`_.
