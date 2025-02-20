#! /bin/bash

cmake -B build -S . -G Ninja
pushd build
ninja