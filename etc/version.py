#!/usr/bin/env python3

import re

with open("setup.py", "r", encoding="UTF-8") as f:
    s = f.read()
    print(re.search(r"__version__\s*=\s*\"(\d+\.\d+\.\d+)", s).group(1))
