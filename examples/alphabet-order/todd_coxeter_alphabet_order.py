# pylint: disable=redefined-outer-name, redefined-loop-name, import-error
"""In this file, we demonstrate the impact of changing the order of a ToddCoxeter alphabet"""

from collections.abc import Callable
from datetime import timedelta
from itertools import permutations
from math import factorial
from time import perf_counter

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from numpy.typing import NDArray

from libsemigroups_pybind11 import (
    Presentation,
    ReportGuard,
    ToddCoxeter,
    congruence_kind,
    presentation,
)
from libsemigroups_pybind11.words import parse


def generate_alphabet_order_data(
    p: Presentation, f: Callable[[ToddCoxeter], float], t: int | None = None, repeats: int = 5
) -> tuple[NDArray, NDArray]:
    """For each permutation of the alphabet of <p>, construct a ToddCoxeter. On
    this instance, call the function <f> both before and after running, and
    record the difference between these two values. If <t> is not specified, the
    runner will be run using `run()`. Otherwise, the runner will be run for <t>
    seconds.

    Return the list of alphabets and the corresponding list of data.
    """

    alphabet = p.alphabet()
    n = factorial(len(alphabet))
    alphabets = np.empty((n,), dtype=type(alphabet))
    data = np.empty((n, repeats), dtype=float)

    for i, new_alphabet in enumerate(permutations(alphabet)):
        if isinstance(alphabet, list):
            new_alphabet = list(new_alphabet)
        else:
            new_alphabet = "".join(new_alphabet)

        alphabets[i] = new_alphabet
        p.alphabet(new_alphabet)

        print(f"alphabet: {new_alphabet}")

        for j in range(repeats):
            tc = ToddCoxeter(congruence_kind.twosided, p)
            # We record an initial value so the that the time taken can be measured,
            # if desired.
            initial = f(tc)
            if t is None:
                tc.run()
            else:
                tc.run_for(timedelta(seconds=t))

            final = f(tc)
            print(f"run {j + 1}: {final - initial}")
            data[i, j] = final - initial

    return alphabets, data


ReportGuard(False)

#########################################################################
# Mathieu Group 12
#########################################################################

p = Presentation("abAB")
p.contains_empty_word(True)
presentation.add_inverse_rules(p, "ABab")
presentation.add_rule(p, parse("a^2"), "")
presentation.add_rule(p, parse("b^3"), "")
presentation.add_rule(p, parse("(ab)^11"), "")
presentation.add_rule(p, parse("(a,b)^6"), "")
presentation.add_rule(p, parse("(ababaB)^6"), "")

alphabets, times = generate_alphabet_order_data(p, lambda tc: perf_counter())
times = times.mean(axis=1)

# Plot the alphabet vs time
ax = sns.barplot(x=alphabets, y=times)
ax.set(xlabel="alphabet", ylabel="time", title="Mathieu 12")
plt.show()

# Plot the time distribution
ax = sns.displot(x=times, kde=True)
ax.set(xlabel="time", title="Mathieu Group 12")
plt.show()

#########################################################################
# Mathieu Group 22
#########################################################################

p = Presentation("abAB")
p.contains_empty_word(True)
presentation.add_inverse_rules(p, "ABab")
presentation.add_rule(p, parse("a^2"), "")
presentation.add_rule(p, parse("b^4"), "")
presentation.add_rule(p, parse("(ab)^11"), "")
presentation.add_rule(p, parse("(ab^2)^5"), "")
presentation.add_rule(p, parse("(a,bab)^3"), "")
presentation.add_rule(p, parse("(ababaB)^5"), "")

alphabets, times = generate_alphabet_order_data(p, lambda tc: perf_counter())
times = times.mean(axis=1)

# Plot the time distribution
ax = sns.displot(x=times, kde=True)
ax.set(xlabel="time", title="Mathieu Group 22")
plt.show()

#########################################################################
# Shutov's partial transformation monoid of degree 5
#########################################################################

p = presentation.examples.partial_transformation_monoid_Shu60(5)
alphabets, times = generate_alphabet_order_data(p, lambda tc: perf_counter())
times = times.mean(axis=1)

# Plot the time distribution
ax = sns.displot(x=times, kde=True)
ax.set(xlabel="time", title="Partial transformation monoid degree 5")
plt.show()

#########################################################################
# Example 6.6 in Sims
#########################################################################

p = Presentation("abcd")
presentation.add_rule(p, "aa", "a")
presentation.add_rule(p, "ba", "b")
presentation.add_rule(p, "ab", "b")
presentation.add_rule(p, "ca", "c")
presentation.add_rule(p, "ac", "c")
presentation.add_rule(p, "da", "d")
presentation.add_rule(p, "ad", "d")
presentation.add_rule(p, "bb", "a")
presentation.add_rule(p, "cd", "a")
presentation.add_rule(p, "ccc", "a")
presentation.add_rule(p, "bcbcbcbcbcbcbc", "a")
presentation.add_rule(p, "bcbdbcbdbcbdbcbdbcbdbcbdbcbdbcbd", "a")

alphabets, times = generate_alphabet_order_data(p, lambda tc: perf_counter())
times = times.mean(axis=1)

# Plot the time distribution
ax = sns.displot(x=times, kde=True)
ax.set(xlabel="time", title="Example 6.6 in Sims")
plt.show()
