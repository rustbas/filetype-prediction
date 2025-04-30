#!/bin/bash

set -xe

gcc -Wall -Wextra		\
    file2mat.c			\
    --shared			\
    -I /usr/include/python3.11	\
    -fPIC                       \
    -o file2mat.so
