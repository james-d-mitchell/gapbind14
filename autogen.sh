#!/bin/sh -ex
#
# gapbind14: Seamless operability between C++14 and GAP
#
# This file is part of the build system of a GAP kernel extension.
# Requires GNU autoconf, GNU automake and GNU libtool.
#
autoreconf -vif `dirname "$0"`
