from collections.abc import Callable
from datetime import timedelta
from itertools import permutations
from typing import Any

from libsemigroups_pybind11 import Presentation, congruence_kind


def generate_alphabet_order_data(
    RunnerType: type, p: Presentation, f: Callable[[Any], float], t: int | None = None
) -> tuple[list, list]:
    """For each permutation of the alphabet of <p>, construct a runner of type
    <RunnerType>. On this runner instance, call the function <f> both before and
    after running, and record the difference between these two values. If <t>
    is not specified, the runner will be run using `run()`. Otherwise, the
    runner will be run for <t> seconds.
    
    Return the list of alphabets and the corresponding list of data.
    """
    
    alphabet = p.alphabet()
    alphabets = []
    data = []

    for new_alphabet in permutations(alphabet):
        if isinstance(alphabet, list):
            new_alphabet = list(new_alphabet)
        else:
            new_alphabet = "".join(new_alphabet)

        alphabets.append(new_alphabet)
        p.alphabet(new_alphabet)

        runner = RunnerType(congruence_kind.twosided, p)
        # We record an initial value so the that the time taken can be measured,
        # if desired.
        initial = f(runner)
        if t is None:
            runner.run()
        else:
            runner.run_for(timedelta(seconds=t))

        final = f(runner)
        data.append(final - initial)

    return alphabets, data
