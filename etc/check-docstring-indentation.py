import re
import os
from os import listdir
from os.path import isfile, splitext

being_warn_col = "\033[93m"
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


def process(match):
    for group in match.groups():
        if group is not None:
            return group


src_path = "src"
files = all_cpp_files(src_path)

bad_doc = re.compile(r"(?:\s+:(param))|(?:\s+:(return))|(?:\s+:(r?type))")
check_doc = True

print("Checking docstring indentation in .cpp files . . .")
for file in files:
    with open(file, "r") as f:
        for line_no, line in enumerate(f):
            if check_doc and "/*" in line:
                check_doc = False
            if not check_doc and "*/" in line:
                check_doc = True
            if not check_doc:
                continue
            m = bad_doc.match(line)
            if m:
                print(
                    f"{being_warn_col}Warning: Bad indentation for :{process(m)}: in docstring on line at {file}:{line_no+1}{end_warn_col}"
                )
