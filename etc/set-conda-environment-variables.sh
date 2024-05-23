#!/bin/bash
set -e

while getopts ":fm:" option; do
    case $option in
    f)
        force=true
        ;;
    m)
        dev_env_pkg_manager=$OPTARG
        ;;
    \?) # Invalid option
        echo "Error: Invalid option"
        exit
        ;;
    esac
done

if [[ -z "${dev_env_pkg_manager}" ]]; then
    dev_env_pkg_manager=mamba
fi

if [[ "$force" != true ]] && [[ $CONDA_DEFAULT_ENV != "libsemigroups_pybind11_dev" ]]; then
    echo The current $dev_env_pkg_manager environment is \"$CONDA_DEFAULT_ENV\", but this script is intended to be run in
    echo \"libsemigroups_pybind11_dev\". If you wish to run this script anyway, please use the option -f:
    echo ./$(basename "$0") -f
    exit
fi

echo Setting LD_LIBRARY_PATH to: $CONDA_PREFIX/lib:/usr/local/lib
echo Setting PKG_CONFIG_PATH to: $CONDA_PREFIX/lib/pkgconfig:$CONDA_PREFIX/share/pkgconfig:/usr/local/lib/pkgconfig

$dev_env_pkg_manager env config vars set -n $CONDA_DEFAULT_ENV LD_LIBRARY_PATH=$CONDA_PREFIX/lib:/usr/local/lib PKG_CONFIG_PATH=$CONDA_PREFIX/lib/pkgconfig:$CONDA_PREFIX/share/pkgconfig:/usr/local/lib/pkgconfig
