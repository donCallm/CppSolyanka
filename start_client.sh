#!/bin/bash

cd build/src/client

EXECUTABLE="./client"

if [ -x "$EXECUTABLE" ]; then
    $EXECUTABLE
else
    echo "Error: Executable file not found."
fi
