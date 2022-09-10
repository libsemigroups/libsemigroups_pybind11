#!/usr/bin/env python3

from distutils.core import Extension

from setuptools.dist import Distribution

dummy = Extension("dummy", ["dummy.pyx"])
dist = Distribution(
    attrs={"name": "dummy", "version": "0.0.0", "ext_modules": [dummy]}
)
bdist_wheel_cmd = dist.get_command_obj("bdist_wheel")
bdist_wheel_cmd.ensure_finalized()

print(bdist_wheel_cmd.get_tag()[-1])
