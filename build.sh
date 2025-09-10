#!/bin/sh
set -xe
gcc -o graph main.c -lraylib -lm -O3
