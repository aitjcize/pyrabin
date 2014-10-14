#!/bin/bash -ex

dir=$(dirname $0)
cd $dir

./build.sh

./test_Rabin.py
./test_delta.py
./test_insert.py
./test_split.sh
./test_streaming.py
./test_swap.py
./test_swap_twofile.py
