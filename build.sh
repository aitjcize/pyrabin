#!/bin/bash

python setup.py build
find build -name 'rabin.so' -exec cp {} test \;
