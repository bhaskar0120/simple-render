#!/bin/bash

set -xe
CC=gcc
FLAGS='-Wextra -Wall -Wpedantic'


$CC $FLAGS -o output ./main.c
./output
