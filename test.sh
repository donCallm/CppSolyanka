#!/bin/bash

cd build/src/test

EXECUTABLE="./tests"

if [ -x "$EXECUTABLE" ]; then
    $EXECUTABLE
else
    echo "Error: Executable file not found."
fi