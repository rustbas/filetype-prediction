#!/bin/bash

set -xe

gcc -Wall -Wextra		\
    file2mat.c			\
    --shared			\
    -I /usr/include/python3.11	\
    -o file2mat.so
