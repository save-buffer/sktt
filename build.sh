#!/usr/bin/env bash

set -e

COMPILE_FLAGS="-std=c++20 -I$TT_METAL_HOME/tt_metal/api -I$TT_METAL_HOME/build_Release/include -I$TT_METAL_HOME/build_Release/include/metalium-thirdparty -L$TT_METAL_HOME/build_Release/lib/ -L$TT_METAL_HOME/build_Release/lib64/ -lfmt -ltt_metal"
export LD_LIBRARY_PATH=$TT_METAL_HOME/build_Release/lib/:$TT_METAL_HOME/build_Release/lib64/:$LD_LIBRARY_PATH

g++ test_loopback.cpp -o test_loopback $COMPILE_FLAGS
./test_loopback
