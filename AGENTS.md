# AGENTS.md

## AI disclosure

Any use of AI tools for preparing code, documentation, tests, commit messages,
pull requests, issue comments, or reviews for this repository must be
disclosed. Include a brief note saying which AI tool was used and what kind of
assistance it provided. Add the AI tool as a Git co-author on all commits
created by that tool (via a `Co-authored-by:` line).

## Project overview

`libsemigroups_pybind11` provides Python bindings for the C++17 library
`libsemigroups`. It uses pybind11 and setuptools, with configuration in
`pyproject.toml` and extension compilation in `setup.py`.

- `src/*.cpp`, `src/*.hpp` — bindings for the compiled extension
  `_libsemigroups_pybind11`
- `src/main.hpp`, `src/main.cpp` — binding initializer declarations and module
  registration
- `src/libsemigroups_pybind11/` — public Python package, wrappers, and helpers;
  `detail/` contains internal implementation support
- `tests/` — pytest tests
- `docs/source/` — Sphinx documentation and custom extensions
- `build_tools/` — dependency validation and packaging helpers
- `etc/` — development, documentation, and release scripts
- `.github/workflows/` — CI build, test, lint, and packaging commands

## Guardrails

- NEVER push directly to `main`.
- Work on a separate branch. Before starting changes, inspect the working tree,
  fetch upstream changes from `github.com/libsemigroups/libsemigroups_pybind11`,
  and create a new branch incorporating the current upstream `main`. Preserve
  existing user changes; do not reset or discard them to update the checkout.
- Do not make assumptions about how `libsemigroups` source code functions
  without reviewing the relevant `libsemigroups` source code. Inspect the
  declarations and implementations for the version being bound before making
  claims or decisions about behavior, validation, exceptions, ownership, or
  lifetimes. Use an available local checkout (often `../libsemigroups`); verify
  that it corresponds to the library used to build the bindings. If the relevant
  source is unavailable, obtain it or report the gap before proceeding with
  changes that depend on that behavior.
- Preserve compatibility of the public Python API, including names, imports,
  signatures, return types, and exceptions. Breaking changes require an explicit
  requirement confirmed by the user.
- Do not modify a sibling `libsemigroups` checkout unless the task requires it.
  Follow that repository's own `AGENTS.md` when doing so.
- Do not install system packages without the user's confirmation. Use available
  development environments; report checks that cannot be run.

## Setup and rebuilding

Read the build instructions in `CONTRIBUTING.rst`. Treat `pyproject.toml` as the
authority for Python requirements: the package requires Python 3.10+, while the
`dev` dependency group requires Python 3.12+.

The extension needs an installed, compatible `libsemigroups` library discoverable
through `pkg-config`. The minimum version is defined in
`build_tools/__init__.py` and checked by `build_tools/packaging_helpers.py`.
Check `pkg-config --modversion libsemigroups`; set `PKG_CONFIG_PATH` to the
directory containing `libsemigroups.pc` if necessary. A sibling source checkout
alone does not satisfy this dependency.

For development of both repositories, `CONTRIBUTING.rst` recommends building
`libsemigroups` with `--disable-hpcombi`. Prefer installation into the active
environment over a system-wide installation. Some bindings are conditional on
the underlying library's build options.

With the C++ dependency installed, the CI-style setup is:

```sh
uv sync --locked --all-extras --no-dev --group lint
```

On Python 3.12+, `uv sync --locked --all-extras` also installs the development
tools, including pre-commit. A pip alternative for the package, tests, and docs
is `python -m pip install '.[test,docs]'`; lint tools are separate dependencies.

A conda/mamba environment can be created with
`source etc/make-dev-environment.sh` (or pass `conda`). If the environment already
exists, activate `libsemigroups_pybind11_dev` explicitly: the creation script
returns without activating an existing environment.

Rebuild the extension after changing C++ bindings, and after changes to the
underlying C++ library that require recompilation. For uv, repeat your setup
command with `--reinstall-package libsemigroups_pybind11`, preserving your chosen
dependency groups; for pip, rerun the installation command. Editable installs do not
automatically recompile C++. Ensure tests import the intended package and
extension rather than a stale installation. `NPY_NUM_BUILD_JOBS` controls build
parallelism; check available CPUs and memory and leave headroom.

## Binding conventions and repository wiring

Before adding or changing bindings, read the binding and export sections of
`CONTRIBUTING.rst` and inspect a comparable existing component.

- New `src/*.cpp` files are discovered by `setup.py`; there is no Autotools
  source list to update in this repository.
- Declare new binding initializers in `src/main.hpp` and call them from
  `src/main.cpp`. Respect registration order: base classes and types used by
  other bindings must be registered early enough.
- Exposing an object in `_libsemigroups_pybind11` does not automatically expose
  it in the public package. Add the appropriate imports in
  `src/libsemigroups_pybind11/__init__.py` or the relevant Python module.
- Preserve C++ helper namespaces as Python modules. For templated classes,
  follow the existing dispatch wrappers based on `detail.CxxWrapper`, such as
  `src/libsemigroups_pybind11/sims.py`.
- Check overload selection, argument names and defaults, inheritance, exception
  translation, and return-value/iterator lifetime policies against the C++
  source and existing binding patterns.
- `src/libsemigroups_pybind11/_version.py` is generated by setuptools-scm; do
  not edit it manually. Review `MANIFEST.in` when changing packaged resources.

## Tests

Use the narrowest relevant pytest selection while iterating, then broaden checks
in proportion to the change. In a uv environment:

```sh
uv run --no-sync pytest tests/test_order.py
uv run --no-sync pytest tests/test_order.py -k compare
uv run --no-sync pytest -m quick
uv run --no-sync pytest
uv run --no-sync make doctest
```

In an activated pip/conda environment, use `python -m pytest` and `make doctest`.
`make check` runs documentation doctests followed by the Python test suite.

- Registered markers are `quick`, `standard`, `extreme`, and `fail`. Unmarked
  tests exist, so `-m quick` is not a substitute for running the relevant file.
  Avoid expensive `standard`/`extreme` runs for small edits unless needed.
- Follow existing pytest naming and parametrization patterns. Test new bindings
  through the public Python API, including inherited methods where applicable,
  relevant overloads, invalid inputs, and wrapper dispatch.
- `tests/conftest.py` automatically disables reporting for each test with
  `ReportGuard(False)`.
- Pytest treats warnings as errors and enables strict markers and strict xfail.
  Assert expected warnings explicitly rather than suppressing them globally.

## Documentation

Before editing Python docstrings, C++ binding docstrings, or `docs/`, read the
documentation sections of `CONTRIBUTING.rst`.

- Use Sphinx/reStructuredText conventions, including Python cross-references,
  parameter and return documentation, exceptions, and runnable doctest examples.
- C++ `R"pbdoc(...)pbdoc"` docstrings must have no block indentation. Use the
  documented `:sig=...:` override when inferred signatures need correction.
- Add new API pages to the relevant documentation toctree. Custom signature and
  docstring processing lives in
  `docs/source/_ext/libsemigroups_pybind11_extensions.py`; HTML post-processing
  lives in `etc/replace-strings-in-doc.py`.
- Rebuild changed C++ docstrings before building docs. Run `make doc` and
  `make doctest` in the selected environment. `make doc` also checks docstring
  indentation and parameters and performs project-specific post-processing.
  Review generated output in `docs/_build/html/`; do not edit generated HTML.

## Formatting, lint, and completion

The hooks in `.pre-commit-config.yaml` run Ruff checks/formatting and
clang-format 15. The pre-push stage additionally runs cpplint, codespell, and
Pylint. Pylint is a local hook: it must be installed in the active environment,
with the built extension importable. Other hooks provision their pinned tools.

```sh
pre-commit run --files <changed files>
pre-commit run --hook-stage pre-push --files <changed files>
```

Use `uv run --no-sync pre-commit ...` when using the uv development environment.
For broader checks, replace `--files <changed files>` with `--all-files`.
`make lint` runs Ruff, Pylint, and cpplint; it does not run formatting or spelling
checks. `make format` modifies files throughout the repository, so prefer hooks
on changed files for focused edits.

Before finishing:

- Review `git diff` and `git status`, including newly created files, and confirm
  only intended changes remain.
- Run the relevant rebuild, tests, documentation checks, and both hook stages
  on changed files. Scale checks to the change; guidance-only edits do not need
  a C++ build or runtime tests.
- State exactly what was tested, including pytest files/filters, and identify
  checks that could not be run and why.
- Include the AI assistance disclosure in the handoff and any commit or PR.
