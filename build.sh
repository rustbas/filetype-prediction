#!/bin/bash

set -xe

# gcc \
#     src/file2data.c \
#     --shared \
#     -o file2data.so

gcc 				\
    src/file2mat.c              \
    --shared			\
    -I /usr/include/python3.11	\
    -fPIC                       \
    -o file2mat.so
