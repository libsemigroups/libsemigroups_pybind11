#!/usr/bin/env python3
"""
This module checks for missing features from libsemigroups in
libsemigroups_pybind11.
"""
import argparse
import datetime
import os
import re
import sys

import yaml
from accepts import accepts
from bs4 import BeautifulSoup


def __parse_args():
    parser = argparse.ArgumentParser(
        prog="check_sync.py", usage="%(prog)s [options]"
    )
    parser.add_argument(
        "ymlfiles", nargs="+", help="the yml-files to check against"
    )
    return parser.parse_args()


@accepts(str)
def skip_section(name):
    return (
        name is None
        or name.find("types") != -1
        or name.find("Constructors") != -1
    )


@accepts(str, str)
def skip_mem_fn(name, params):
    return params.find("&&") != -1


@accepts(str)
def normalize_params_t(params_t):
    params_t = params_t.strip()
    # replace more than 1 space by a single space
    params_t = re.sub("\s{2,}", " ", params_t)
    # Add space after < if it's a non-space
    params_t = re.sub("(?<=[<])(?=[^\s])", " ", params_t)
    # Add space before > if it's a non-space
    params_t = re.sub("(?<=[^\s])(?=[>])", " ", params_t)
    # Add space before & if it's a non-space and not &
    params_t = re.sub("(?<=[^\s\&])(?=[\&])", " ", params_t)
    # Add space after & if it's a non-space and not &
    params_t = re.sub("(?<=[\&])(?=[^\s\&])", " ", params_t)
    # remove whitespace around commas
    params_t = re.sub("\s*,\s*", ",", params_t)
    return params_t


@accepts(str)
def extract_func_params_t(yml_entry):
    pos = yml_entry.find("(")
    if pos == -1:
        return yml_entry, ""
    else:
        params_t = normalize_params_t(yml_entry[pos + 1 : yml_entry.rfind(")")])
        return yml_entry[:pos], params_t


def check_src_for(class_, mem_fn, params):
    if skip_mem_fn(mem_fn, params):
        return
    class_ = class_.split("::")[-1]
    if len(class_) > 0:
        prefix = f"{class_}::{mem_fn}({params})"
        overload = f"py::overload_cast<{params}>\(&.*{class_}.*::{mem_fn}"
        non_overload = f"&.*{class_}.*::{mem_fn}"
    elif len(params) > 0:
        prefix = f"{mem_fn}({params})"
        # not currently used
    else:
        prefix = f"{mem_fn}"
        non_overload = f"&{mem_fn}"
        overload = "XYxyXYxyXYxyXYxyXYxyXYxyXYxyXYxyXYxyXYxyXYxyXYxy"
        weak_match = f"{mem_fn}"
        wm = []
    print(prefix + "." * (72 - len(prefix)) + " ", end="")
    iterator = f"py::make_iterator\(.*{mem_fn}"
    if mem_fn.startswith("cend"):
        print(f"\033[33mskipped!\033[0m")
        return

    for cpp_file_name in os.listdir("src/"):
        if not cpp_file_name.endswith(".cpp"):
            continue
        with open("src/" + cpp_file_name, "r") as cpp_file:
            lines = cpp_file.read()
            try:
                m = (
                    re.search(overload, lines)
                    or re.search(non_overload, lines)
                    or (
                        mem_fn.startswith("cbegin")
                        and re.search(iterator, lines)
                    )
                )
                if m:
                    line_nr = lines.count("\n", 0, m.end())
                    print(
                        f"\033[32mfound in src/{cpp_file_name}:{line_nr}\033[0m"
                    )
                    return
                if not weak_match:
                    continue

                m = re.search(weak_match, lines)
                if m:
                    line_nr = lines.count("\n", 0, m.end())
                    wm.append(f"src/{cpp_file_name}:{line_nr}")
            except re.error:
                print(f"\033[44mregex error!\033[0m")
                return

    if len(wm) > 0:
        print(f"\033[35mpossibly found in {wm}\033[0m")
        return
    print(f"\033[41mnot found!\033[0m")


def check_yml_file(ymlfname):
    with open(ymlfname, "r") as f:
        ymldic = yaml.load(f, Loader=yaml.FullLoader)
        class_name = next(iter(ymldic))
        # print(f'Processing "{class_name}" . . .')
        if ymldic[class_name] is None:
            return
        for sectiondic in ymldic[class_name]:
            name = next(iter(sectiondic))
            # print(f"- Section in {ymlfname} is {name} . . .")
            if skip_section(name):
                continue
            for x in sectiondic[name]:
                if not isinstance(x, str):
                    continue
                check_src_for(class_name, *extract_func_params_t(x))


def check_rst_file(rstfname):
    with open(rstfname, "r") as f:
        lines = f.read()
        p = ":cpp:any:`([^`]+)`"
        namespace = re.search(".. cpp:namespace:: libsemigroups::(\w+)", lines)

        for fn in re.finditer(p, lines):
            if namespace:
                fn = str(namespace.group(1)) + "::" + fn.group(1)
            else:
                fn = fn.group(1)
            check_src_for("", fn, "")


def main():
    if sys.version_info[0] < 3:
        raise Exception("Python 3 is required")
    args = __parse_args()
    if not os.getcwd().endswith("libsemigroups_pybind11"):
        raise Exception(
            "This script must be run in the libsemigroups_pybind11 directory!"
        )

    for fname in args.ymlfiles:
        if fname.endswith(".yml"):
            check_yml_file(fname)
        elif fname.endswith(".rst"):
            check_rst_file(fname)

    sys.exit(0)


if __name__ == "__main__":
    main()
