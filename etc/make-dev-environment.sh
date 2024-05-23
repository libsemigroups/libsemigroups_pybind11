#!/bin/bash
is_sourced() {
    if [ -n "$ZSH_VERSION" ]; then
        case $ZSH_EVAL_CONTEXT in *:file:*) return 0 ;; esac
    else
        case ${0##*/} in dash | -dash | bash | -bash | ksh | -ksh | sh | -sh) return 0 ;; esac
    fi
    return 1 # NOT sourced.
}

is_sourced && sourced=1 || sourced=0
if [[ $sourced -ne 1 ]]; then
    echo This script must be sourced, rather than run directly. Please try again with:
    echo source $(basename "$0")
    exit
fi

unset sourced

if [[ $# -eq 0 ]]; then
    dev_env_pkg_manager=mamba
else
    dev_env_pkg_manager=$1
fi

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
