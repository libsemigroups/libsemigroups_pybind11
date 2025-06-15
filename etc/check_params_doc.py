#!/usr/bin/env python3

import re
import os
from os import listdir
from os.path import isfile, splitext

begin_warn_col = "\033[93m"
end_warn_col = "\033[0m"


def all_cpp_files(start):
    files = set()

    def dive(path):
        for entry in listdir(path):
            candidate = os.path.join(path, entry)
            if isfile(candidate) and splitext(entry)[1] == ".cpp":
                files.add(candidate)
            elif not isfile(candidate):
                dive(candidate)

    dive(start)
    return files


src_path = "src"
files = all_cpp_files(src_path)

param_pattern = re.compile(r":param (\w+):")

print("Checking docstring indentation in .cpp files . . .")
for file in files:
    with open(file, "r") as f:
        line_is_doc = False
        for line_no, line in enumerate(f):
            if line_is_doc and ")pbdoc" in line:
                line_is_doc = False
            if not line_is_doc and "pbdoc(" in line:
                params = []
                line_is_doc = True
            if not line_is_doc:
                continue
            match = param_pattern.match(line)
            if match:
                params.append(match.group(1))
            for param in params:
                if f"``{param}``" in line:
                    print(
                        f"{begin_warn_col}Warning: found ``{param}`` "
                        f"in {file}:{line_no + 1}{end_warn_col}"
                    )
