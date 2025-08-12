<h1 align="center">
<img src ="docs/pictures/libsemigroups_pybind11_banner.svg" width="75%">
</h1>

[![DOI](https://zenodo.org/badge/299898305.svg)](https://zenodo.org/badge/latestdoi/299898305)

## What is [libsemigroups][]?

Before explaining what `libsmigroups_pybind11` is, it is first necessary to
explain [libsemigroups][]. [libsemigroups][] is a C++17 library containing
implementations of several algorithms for computing finite, and finitely
presented, semigroups and monoids. The main algorithms implemented in
[libsemigroups][] are:

- the [Froidure-Pin algorithm][] for computing semigroups and monoids defined
  by a generating set consisting of elements whose multiplication and equality
  is decidable (such as transformations, partial permutations, permutations,
  bipartitions, and matrices over a semiring);
- Kambites' algorithm for solving the word problem in small overlap monoids
  from [Small overlap monoids I: The word problem][],
  and the algorithm from
  [An explicit algorithm for normal forms in small overlap monoids][];
- the [Knuth-Bendix algorithm][] for finitely presented semigroups and monoids;
  a version of Sims' low index subgroup algorithm for computing congruences of a
  semigroup or monoid from
  [Computing finite index congruences of finitely presented semigroups and monoids][]
  in the classes;
- a generalized version of the algorithms described in
  [Green's equivalences in finite semigroups of binary relations][] by
  Konieczny, and
  [On the determination of Green's relations in finite transformation semigroups][]
  by Lallement and Mcfadden for computing finite semigroups and monoids
  admitting a pair of actions with particular properties;
- the algorithm from
  [Efficient Testing of Equivalence of Words in a Free Idempotent Semigroup][]
  by Radoszewski and Rytter;
- a non-random version of the [Schreier-Sims algorithm][] for permutation groups;
- a version of Stephen's procedure from
  [Applications of automata theory to presentations of monoids and inverse monoids][]
  for finitely presented inverse semigroups and monoids (for a given word `w`
  this procedure is for determining words equivalent to `w` or that are
  left divisors of `w`);
- the [Todd-Coxeter algorithm][] for finitely presented semigroups and monoids;
  see also [The Todd-Coxeter algorithm for semigroups and monoids][].

[libsemigroups][] is partly based on
[Algorithms for computing finite semigroups][Froidure-Pin algorithm],
[Expository Slides][], and [Semigroupe 2.01][] by [Jean-Eric Pin][].

[Froidure-Pin algorithm]: https://www.irif.fr/~jep/PDF/Rio.pdf
[Small overlap monoids I: The word problem]: https://doi.org/10.1016/j.jalgebra.2008.09.038
[An explicit algorithm for normal forms in small overlap monoids]: https://doi.org/10.1016/j.jalgebra.2023.04.019
[Knuth-Bendix algorithm]: https://en.wikipedia.org/wiki/Knuth%E2%80%93Bendix_completion_algorithm
[Computing finite index congruences of finitely presented semigroups and monoids]: https://arxiv.org/abs/2302.06295
[Green's equivalences in finite semigroups of binary relations]: https://link.springer.com/article/10.1007/BF02573672
[On the determination of Green's relations in finite transformation semigroups]: https://www.sciencedirect.com/science/article/pii/S0747717108800570
[Efficient Testing of Equivalence of Words in a Free Idempotent Semigroup]: https://link.springer.com/chapter/10.1007/978-3-642-11266-9_55
[Applications of automata theory to presentations of monoids and inverse monoids]: https://digitalcommons.unl.edu/dissertations/AAI8803771/
[Todd-Coxeter algorithm]: https://en.wikipedia.org/wiki/Todd%E2%80%93Coxeter_algorithm
[The Todd-Coxeter algorithm for semigroups and monoids]: https://doi.org/10.1007/s00233-024-10431-z
[Schreier-Sims algorithm]: https://en.wikipedia.org/wiki/Schreier%E2%80%93Sims_algorithm
[Expository Slides]: https://www.irif.fr/~jep/PDF/Exposes/StAndrews.pdf
[Semigroupe 2.01]: https://www.irif.fr/~jep/Logiciels/Semigroupe2.0/semigroupe2.html
[Jean-Eric Pin]: https://www.irif.fr/~jep/

## What is `libsemigroups_pybind11`?

`libsemigroups_pybind11` is a package for Python 3.9+ exposing much
(but not all) of the functionality of [libsemigroups][]. It is built with the help
of the excellent library [pybind11][], for which we are very grateful.

## How to install `libsemigroups_pybind11`

### Installing with pip

It's possible to install `libsemigroups_pybind11` using `pip` by doing one
of the following (depending on your system and setup):

```console
pip install libsemigroups_pybind11
```

```console
pip3 install libsemigroups_pybind11
```

```console
python -m pip install libsemigroups_pybind11
```

### From the sources

Before installing `libsemigroups_pybind11` from its sources, you should first
perform a system install of `libsemigroups`. For information
about how to do this, see the
[libsemigroups installation guid](https://libsemigroups.github.io/libsemigroups/md_install.html).

Assuming that you have `libsemigroups` installed, you can install
`libsemigroups_pybind11` as follows:

```console
git clone https://github.com/libsemigroups/libsemigroups_pybind11
cd libsemigroups_pybind11
pip install .
```

### From a release archive

To build `libsemigroups_pybind11` from a release archive:

```console
curl -L -O https://github.com/libsemigroups/libsemigroups_pybind11/releases/latest/download/libsemigroups_pybind11-1.0.1.tar.gz
tar -xf libsemigroups_pybind11-1.0.1.tar.gz
rm -f libsemigroups_pybind11-1.0.1.tar.gz
cd libsemigroups_pybind11-1.0.1
pip install .
```

## Building the documentation

Assuming you have `python3` and `make` installed, you can build the doc by
using:

```console
python3 -m pip3 install -r requirements.txt
make doc
```

If you don't have `make`, you can run the executable `./etc/make-doc.sh`
instead of running `make doc` (which is precisely what `make doc` does).

## Issues

If you find any problems with `libsemigroups_pybind11`, or have any
suggestions for features that you'd like to see, please use the
[issue tracker][].

## Acknowledgements

In addition to [libsemigroups][], there are several excellent projects that are
utilised in the development of `libsemigroups_pybind11`, specifically:

- [codespell][], [cpplint][], [Pylint][] and [Ruff][] for code quality;
- [Graphviz][] for graph visualisation;
- [pybind11][] for assistance in the binding of [libsemigroups][];
- [pytest][] for testing; and
- [Sphinx][] for the documentation.

We would like to thank the authors and contributors of these projects!

[codespell]: https://github.com/codespell-project/codespell
[cpplint]: https://github.com/cpplint/cpplint
[Pylint]: https://pylint.readthedocs.io/en/latest/
[Ruff]: https://docs.astral.sh/ruff
[pytest]: https://docs.pytest.org/en/stable/
[Sphinx]: https://www.sphinx-doc.org/en/master/
[libsemigroups]: https://libsemigroups.github.io/libsemigroups/
[issue tracker]: https://github.com/libsemigroups/libsemigroups_pybind11/issues
[Graphviz]: https://www.graphviz.org
[pybind11]: https://pybind11.readthedocs.io/en/stable/#
