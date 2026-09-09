# Copyright (c) 2026, J. D. Mitchell
#
# Distributed under the terms of the GPL license version 3.
#
# The full license is in the file LICENSE, distributed with this software.

"""Regression tests for Python callbacks in Sims worker threads (issue #417)."""

import os
import subprocess
import sys
import textwrap

import pytest


@pytest.mark.quick
@pytest.mark.parametrize("sims_class", ["Sims1", "Sims2"])
@pytest.mark.parametrize("threads", [1, 2])
@pytest.mark.parametrize("with_pruner", [False, True])
@pytest.mark.parametrize("operation", ["find_match", "find_no_match", "for_each", "count"])
def test_sims_python_callbacks(sims_class, threads, with_pruner, operation):
    """Callbacks must run without deadlocking the calling Python thread."""
    # A Python thread timeout cannot stop C++ code holding the GIL. Run in a
    # subprocess so a regression fails this test instead of hanging pytest.
    code = textwrap.dedent(f"""
        from libsemigroups_pybind11 import (
            Presentation, ReportGuard, {sims_class}, word_graph,
        )

        report = ReportGuard(False)
        p = Presentation([0])
        p.contains_empty_word(True)
        p.rules = [[0, 0, 0, 0], []]
        s = {sims_class}(p).number_of_threads({threads})
        if {threads} == 2 and s.number_of_threads() < 2:
            raise SystemExit(77)

        pruned = []
        def pruner(wg):
            pruned.append(wg.number_of_nodes())
            return True

        if {with_pruner}:
            s.add_pruner(pruner)

        visited = []
        def callback(wg):
            size = word_graph.number_of_nodes_reachable_from(wg, 0)
            visited.append(size)
            return size == 4 and {operation!r} == "find_match"

        if {operation!r} == "count":
            assert s.number_of_congruences(4) == 3
        elif {operation!r} == "for_each":
            s.for_each(4, callback)
            assert sorted(visited) == [1, 2, 4]
        elif {operation!r} == "find_match":
            result = s.find_if(4, callback)
            assert word_graph.number_of_nodes_reachable_from(result, 0) == 4
            assert 4 in visited
        else:
            result = s.find_if(4, callback)
            assert result.number_of_nodes() == 0
            assert sorted(visited) == [1, 2, 4]

        if {with_pruner}:
            assert pruned
    """)
    # Preserve the test runner's import paths, including editable/source builds.
    env = dict(os.environ, PYTHONPATH=os.pathsep.join(sys.path))
    result = subprocess.run(
        [sys.executable, "-c", code],
        capture_output=True,
        text=True,
        timeout=20,
        env=env,
        check=False,
    )
    if result.returncode == 77:
        pytest.skip("requires support for at least two Sims worker threads")
    assert result.returncode == 0, result.stdout + result.stderr
