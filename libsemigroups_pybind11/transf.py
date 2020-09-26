from libsemigroups_pybind11 import Transf256

def Transf(l):
    if not isinstance(l, list):
        raise TypeError("expected a list, found something else")
    if len(l) <= 16:
        return Transf256(l)
