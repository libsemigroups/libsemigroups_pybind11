import re
import os
from os import listdir
from os.path import isfile, splitext


def dict_sub(replacements, string):
    """replacements has the form {"regex1": "replacement", "regex2": "replacement2", ...}"""
    global_expression = re.compile(
        "|".join("(" + x + ")" for x in replacements)
    )
    replacements_by_group = {}
    group = 1
    for expr, replacement in replacements.items():
        replacements_by_group[group] = replacement
        group += re.compile(expr).groups + 1

    def choose(match):
        return replacements_by_group[match.lastindex]

    return re.subn(global_expression, choose, string)


def all_html_files(start):
    files = set()

    def dive(path):
        for entry in listdir(path):
            candidate = os.path.join(path, entry)
            if isfile(candidate) and splitext(entry)[1] == ".html":
                files.add(candidate)
            elif not isfile(candidate):
                dive(candidate)

    dive(start)
    return files


html_path = "docs/_build/html"
replacements = {
    "KnuthBendixRewriteTrie": "KnuthBendix",
    "_libsemigroups_pybind11\.": "",
}
files = all_html_files(html_path)

for file in files:
    with open(file, "r+") as f:
        content = f.read()
        content, num_matches = dict_sub(replacements, content)
        if num_matches > 0:
            print(f"String replacements made in {file}")
            f.write(content)
