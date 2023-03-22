# libsemigroups_pybind11 - Version 0.9.2

## python bindings for the C++ library libsemigroups

[![DOI](https://zenodo.org/badge/299898305.svg)](https://zenodo.org/badge/latestdoi/299898305)

### What is `libsemigroups`?

`libsemigroups` is a C++14 library containing implementations of several
algorithms for computing finite and finitely presented semigroups.
Namely:

-   the [Froidure-Pin algorithm](https://www.irif.fr/~jep/PDF/Rio.pdf)
    for computing finite semigroups
-   the [Todd-Coxeter
    algorithm](https://en.wikipedia.org/wiki/Todd%E2%80%93Coxeter_algorithm)
    for finitely presented semigroups and monoids;
-   the [Knuth-Bendix
    algorithm](https://en.wikipedia.org/wiki/Knuth%E2%80%93Bendix_completion_algorithm)
    for finitely presented semigroups and monoids;
-   the [Schreier-Sims
    algorithm](https://en.wikipedia.org/wiki/Schreier%E2%80%93Sims_algorithm)
    for permutation groups;
-   a preliminary implementation of the
    [Konieczny](https://link.springer.com/article/10.1007/BF02573672)
    and
    [Lallement-McFadden](https://www.sciencedirect.com/science/article/pii/S0747717108800570)
    algorithm for computing finite semigroups which act on sets.

`libsemigroups_pybind11` is a python package exposing much (but not all)
of the functionality of `libsemigroups`.

The development version of `libsemigroups_pybind11` is available on
[github](https://github.com/libsemigroups/libsemigroups_pybind11), and
some related projects are [here](https://github.com/libsemigroups).

# Installation

## Installing with pip

It's possible to install `libsemigroups_pybind11` using `pip` via one of:

    pip install libsemigroups_pybind11
    pip3 install libsemigroups_pybind11
    python -m pip install libsemigroups_pybind11
    python3 -m pip install libsemigroups_pybind11

## Installing with conda

This installation method assumes that you have anaconda or miniconda
installed. See the [getting started](http://bit.ly/33B0Vfs) and
[miniconda download page](https://conda.io/miniconda.html) on the
[conda](https://conda.io/) website.

It might be a good idea to create and activate a conda environment to
contain the installation of the `libsemigroups_pybind11`:

    conda create --name libsemigroups
    conda activate libsemigroups

Install `libsemigroups_pybind11`:

    conda install -c conda-forge libsemigroups_pybind11

At present this does not work for Macs with M1 processors.

## From the sources

Before installing `libsemigroups_pybind11` from its sources you should
first perform a system install of the C++ library `libsemigroups`. For
information about how to install `libsemigroups` see [the installation
guide](https://libsemigroups.readthedocs.io/en/latest/install.html).

Assuming that you have `libsemigroups` installed you can install
`libsemigroups_pybind11` as follows:

    git clone https://github.com/libsemigroups/libsemigroups_pybind11
    cd libsemigroups_pybind11
    pip install .

### From a release archive

To build `libsemigroups_pybind11` from a release archive:

    curl -L -O https://github.com/libsemigroups/libsemigroups_pybind11/releases/latest/download/libsemigroups_pybind11-0.9.2.tar.gz
    tar -xf libsemigroups_pybind11-0.9.2.tar.gz
    rm -f libsemigroups_pybind11-0.9.2.tar.gz
    cd libsemigroups_pybind11-0.9.2
    pip install .

## Building the documentation

The following are required to be able to build the documentation:

1.  `python3`
2.  the python packages:
    `jinja2 sphinx sphinx_rtd_theme sphinxcontrib-bibtex sphinx_copybutton`

Assuming you already have `python3` install, on Mac OSX you can install
all of the above by doing:

    python3 -m pip3 install -r requirements.txt

Then it ought to be possible to just run `make doc` in the
`libsemigroups_pybind11` directory.

## Issues

If you find any problems with `libsemigroups_pybind11`, or have any
suggestions for features that you'd like to see, please use the [issue
tracker](https://github.com/libsemigroups/libsemigroups_pybind11/issues).
