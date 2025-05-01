#!/bin/bash

set -xe

# gcc \
#     src/file2data.c \
#     --shared \
#     -o file2data.so

gcc 				\
    src/file2mat.c              \
    --shared			\
    -I ${CONDA_PREFIX}/lib/python3.13/site-packages/numpy/_core/include \
    -I ${CONDA_PREFIX}/include/python3.13 \
    -fPIC                       \
    -o file2mat.so
