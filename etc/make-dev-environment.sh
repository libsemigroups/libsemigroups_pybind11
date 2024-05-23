#!/bin/bash

if [[ $# -eq 0 ]]; then
    dev_env_pkg_manager=mamba
else
    dev_env_pkg_manager=$1
fi

echo $dev_env_pkg_manager

if { $dev_env_pkg_manager env list | grep 'libsemigroups_pybind11_dev'; } >/dev/null 2>&1; then
    echo The environment libsemigroups_pybind11_dev already exists. Stopping ...
    unset dev_env_pkg_manager
    return
else
    echo Making libsemigroups_pybind11_dev envinronment using $dev_env_pkg_manager ...
    echo
    $dev_env_pkg_manager env create -f dev-environment.yml
    if [[ $? -ne 0 ]]; then
        unset dev_env_pkg_manager
        return
    fi
    echo Activating libsemigroups_pybind11_dev ...
    echo
    $dev_env_pkg_manager activate libsemigroups_pybind11_dev
    if [[ $? -ne 0 ]]; then
        unset dev_env_pkg_manager
        return
    fi
fi

echo Setting envinronment variables ...
echo
./etc/set-conda-environment-variables.sh -m $dev_env_pkg_manager

echo
echo Reactivating environment ...
echo

$dev_env_pkg_manager deactivate
if [[ $? -ne 0 ]]; then
    unset dev_env_pkg_manager
    return
fi
$dev_env_pkg_manager activate libsemigroups_pybind11_dev
if [[ $? -ne 0 ]]; then
    unset dev_env_pkg_manager
    return
fi

echo
echo Checking environment variables ...
echo
echo LD_LIBRARY_PATH: $LD_LIBRARY_PATH
echo PKG_CONFIG_PATH: $PKG_CONFIG_PATH

unset dev_env_pkg_manager
