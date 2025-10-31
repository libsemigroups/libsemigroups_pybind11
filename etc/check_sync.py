#!/usr/bin/env python3
"""
This module checks for missing features from libsemigroups in
libsemigroups_pybind11.
"""

import argparse
import os
import re
import sys

from bs4 import BeautifulSoup
import bs4

from os.path import exists, isfile
from glob import glob
from rich.console import Console
from rich.syntax import Syntax

__DOXY_DICT = {}


def __parse_args():
    parser = argparse.ArgumentParser(
        prog="check_sync.py", usage="%(prog)s [options]"
    )
    parser.add_argument(
        "things",
        nargs="+",
        help="the name(s) of the subpackage(s) or class(es) to check",
    )
    parser.add_argument(
        "--libsemigroups-dir",
        nargs=1,
        type=str,
        help="the path to the libsemigroups dir",
    )
    parser.add_argument(
        "--cpp-files",
        nargs="+",
        type=str,
        help="the libsemigroup_pybind11 cpp file to check",
    )
    # TODO(1) check args ok

    result = parser.parse_args()
    result.libsemigroups_dir = result.libsemigroups_dir[0]
    return result


########################################################################
# Taken from libsemigroups/etc/generate_pybind11.py
########################################################################


def __error(msg: str) -> None:
    sys.stderr.write(f"\033[0;31m{msg}\n\033[0m")


def __bold(msg: str) -> None:
    sys.stderr.write(f"\033[1m{msg}\n\033[0m")


def is_namespace(args, thing: str) -> bool:
    return "namespace" in doxygen_filename(args, thing)


def is_public(args, thing: str, fn: str) -> bool:
    if is_namespace(args, thing):
        return True
    xml = get_xml(args, thing, fn)
    prot = xml.get("prot")
    return prot is not None and prot == "public"


def is_typedef(args, thing: str, fn: str) -> bool:
    xml = get_xml(args, thing, fn)
    kind = xml.get("kind")
    return kind is not None and kind == "typedef"


def is_variable(args, thing: str, fn: str) -> bool:
    xml = get_xml(args, thing, fn)
    kind = xml.get("kind")
    return kind is not None and kind == "variable"


def is_operator(args, thing: str, fn: str) -> bool:
    return fn.startswith("operator") and fn != "operator()"


def doxygen_filename(args, thing: str) -> str:
    """
    Returns the xml filename used by Doxygen for the class with thing
    <thing>.

    Arguments:
        thing -- a string containing the fully qualified thing of a C++ class,
        struct, or namespace.
    """
    orig = thing

    thing = re.sub("_", "__", thing)
    if thing.endswith("_group"):
        fname = f"{args.libsemigroups_dir}/docs/xml/group__{thing}.xml"
        if exists(fname) and isfile(fname):
            return fname
    p = re.compile(r"::")
    thing = p.sub("_1_1", thing)
    p = re.compile(r"([A-Z])")
    thing = p.sub(r"_\1", thing).lower()
    for possible in ("class", "struct", "namespace"):
        fname = f"{args.libsemigroups_dir}/docs/xml/{possible}{thing}.xml"
        if exists(fname) and isfile(fname):
            return fname
    thing = thing.split("_1_1")[-1]
    pattern = re.compile(rf">{thing}<")
    for fname in glob(f"{args.libsemigroups_dir}/docs/xml/group__*.xml"):
        with open(fname, "r", encoding="utf-8") as file:
            lines = file.read()
        if pattern.search(lines):
            return fname
    __error(f'Can\'t find the doxygen file for "{orig}" IGNORING!!!')


def get_xml(
    args, thing: str, fn: str | None = None
) -> dict[str, bs4.element.Tag]:  # FIXME the return type is not correct
    """
    TODO
    """
    if thing not in __DOXY_DICT:
        doxy_file = doxygen_filename(args, thing)
        if doxy_file is None:
            return
        with open(
            doxy_file,
            "r",
            encoding="utf-8",
        ) as xml:
            xml = BeautifulSoup(xml, "xml")
            fn_list = xml.find_all("memberdef")
            fn_dict = {}

            for x in fn_list:
                nm = x.find("name").text
                if nm not in fn_dict:
                    fn_dict[nm] = {}
                fn_dict[nm] = x
            __DOXY_DICT[thing] = fn_dict
    if fn is not None:
        return __DOXY_DICT[thing][fn]
    return __DOXY_DICT[thing]


def _skip(console: Console, args, thing: str, fn: str) -> bool:
    if (
        fn.endswith("_no_checks")
        or fn.startswith("cend")
        or fn.startswith("end")
        or fn.startswith("cbegin")
        or fn.startswith("begin")
        or fn.startswith("_")
        or fn.endswith("_type")
        or "iterator" in fn
        or not is_public(args, thing, fn)
        or thing.endswith(fn)  # for constructors
        or fn in ("operator=", "operator<<")
        or is_typedef(args, thing, fn)
    ):
        # TODO print why
        if is_public(args, thing, fn):
            console.print(
                f":warning-emoji: [dim]skipping [yellow]{thing}::{fn}[/yellow] . . .[/dim]"
            )
        return True
    return False


def translate_to_py(fn: str) -> str:
    translator = {
        "operator==": "py::self == py::self",
        "operator!=": "py::self != py::self",
        "operator<": "py::self < py::self",
        "operator>": "py::self > py::self",
        "operator<=": "py::self <= py::self",
        "operator>=": "py::self >= py::self",
        "operator+": r"py::self \+ py::self",
        "operator*": r"py::self \* py::self",
        "operator*=": r"py::self \*= py::self",
        "operator+=": r"py::self \+= py::self",
        "operator()": "__call__",
        "at": "__getitem__",
        "hash_value": "__hash__",
        "operator[]": "__getitem__",
        "to_human_readable_repr": "__repr__",
    }
    if fn in translator:
        return translator[fn]
    return fn


########################################################################
# finders
########################################################################


def _strip_cxx_comments(lines: list[str]) -> list[str]:
    """Removes comments from C++ lines"""
    # remove comments
    for i, line in enumerate(lines):
        pos = line.find("//")
        if pos != -1:
            lines[i] = lines[i][:pos]
    return lines


def _regex_pattern(args, thing: str, fn: str) -> str:
    cxx_fn = fn[:]
    fn = translate_to_py(fn)
    if is_namespace(args, thing):
        namespace = thing.split(":")[-1]
        pattern = f"(){namespace}_{fn}"
    else:
        pattern = rf"(\w*){fn}"
    if not is_operator(args, thing, cxx_fn):
        pattern = f'"{pattern}"'
    if not is_variable(args, thing, cxx_fn):
        pattern = rf".def\w*\(\s*{pattern}"
    else:
        pattern = rf".attr\(\s*{pattern}"
    return pattern


def find_in_cpp(args, thing: str, fn: str, info: dict) -> bool:
    console = Console()
    if _skip(console, args, thing, fn):
        return
    pattern = _regex_pattern(args, thing, fn)
    for cpp_file_name in args.cpp_files:
        with open(f"{cpp_file_name}", "r") as cpp_file:
            lines = cpp_file.read().split("\n")
        lines = "\n".join(_strip_cxx_comments(lines))
        matches = [x for x in re.finditer(pattern, lines, re.DOTALL)]
        if len(matches) == 0:
            console.print(
                f":x: [bright_red]{thing}::{fn} not found![/bright_red]"
            )
            return

        for match in matches:
            line_num = lines[: match.start()].count("\n") + 1
            if len(match.group(1)) == 0:
                console.print(
                    f":white_heavy_check_mark: found [green]{thing}::{fn}[/green] "
                    f"in [green]{cpp_file_name}:{line_num}:[/green]"
                )
            else:
                console.print(
                    f":grey_question: possibly found [purple]{thing}::{fn}[/purple] "
                    f"in [purple]{cpp_file_name}:{line_num}:[/purple]"
                )

            end = lines.find("\n", match.end() + 1)
            chunk = lines[match.start() : end]
            if ";" not in chunk:
                end = lines.find("\n", end + 1)
                chunk = lines[match.start() : end]

            console.print(
                Syntax(
                    f"{chunk}",
                    "python",
                    line_numbers=True,
                    start_line=line_num,
                )
            )


def check_thing(args, thing: str) -> None:
    if not thing.startswith("libsemigroups::"):
        thing = f"libsemigroups::{thing}"
    xml = get_xml(args, thing)
    if xml is None:
        return
    for fn, info in xml.items():
        if fn != thing:
            find_in_cpp(args, thing, fn, info)


def main():
    if not os.getcwd().endswith("libsemigroups_pybind11"):
        raise Exception(
            "This script must be run in the libsemigroups_pybind11 directory!"
        )
    args = __parse_args()
    for thing in args.things:
        check_thing(args, thing)

    sys.exit(0)


if __name__ == "__main__":
    main()
