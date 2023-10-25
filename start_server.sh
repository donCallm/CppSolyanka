#!/bin/bash

cd build/src/server

EXECUTABLE="./server"

if [ -x "$EXECUTABLE" ]; then
    $EXECUTABLE
else
    echo "Error: Executable file not found."
fi
