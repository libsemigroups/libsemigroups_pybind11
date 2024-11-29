import subprocess
import sys
import re
from packaging.version import Version

# Adapted from https://peps.python.org/pep-0440/#appendix-b-parsing-version-strings-with-regular-expressions
version_pattern = re.compile(
    r"\b(?:[1-9][0-9]*!)?(?:0|[1-9][0-9]*)(?:\.(?:0|[1-9][0-9]*))*(?:(?:a|b|rc)(?:0|[1-9][0-9]*))?(?:\.post(?:0|[1-9][0-9]*))?(?:\.dev(?:0|[1-9][0-9]*))?\b"
)
package_pattern = re.compile(r"^[\w-]+")

versions = [
    Version("3.9"),
    Version("3.10"),
    Version("3.11"),
    Version("3.12"),
    Version("3.13"),
]


def get_versions(pkg_name, py_version):
    result = subprocess.run(
        [
            sys.executable,
            "-m",
            "pip",
            "index",
            "versions",
            pkg_name,
            "--python-version",
            str(py_version),
        ],
        capture_output=True,
        text=True,
        check=True,
    )
    available_versions = result.stdout.split("\n")[1]
    return set(re.findall(version_pattern, available_versions))


pkgs = []
with open("requirements.txt") as file:
    for line in file:
        pkgs.append(package_pattern.match(line).group())

print(
    f"Computing valid requirement versions for Python versions {min(versions)}"
    f" to {max(versions)} . . ."
)

satisfiable = True

pkg_to_versions = {}
for pkg in pkgs:
    print(f"{pkg}:")
    valid_versions = set.intersection(
        *(get_versions(pkg, version) for version in versions)
    )
    if not valid_versions:
        satisfiable = False
    sorted_versions = sorted(list(valid_versions), key=Version, reverse=True)
    pkg_to_versions[pkg] = sorted_versions
    print(sorted_versions)

print("\n")
if not satisfiable:
    print("Not all requirements can be satisfied for all versions of Python.")
    print(f"Consider dropping support for Python {min(versions)}")

else:
    print("Potential requirements.txt:")

    for pkg in pkgs:
        print(f"{pkg}=={pkg_to_versions[pkg][0]}")
