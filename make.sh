#!/bin/bash

set -xe
CC=gcc
FLAGS='-Wextra -Wall -Wpedantic'
LIB='./shape.c'

$CC $FLAGS -o output ./main.c
./output
