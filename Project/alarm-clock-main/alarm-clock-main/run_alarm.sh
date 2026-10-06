#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

gcc "$SCRIPT_DIR/src/alarm_clock.c" -o "$SCRIPT_DIR/alarm_clock"

if [ $? -ne 0 ]
then
    echo "Compilation failed."
    exit 1
fi

cmd.exe /c start "" wsl.exe -d Ubuntu bash -c "cd '$SCRIPT_DIR' && ./alarm_clock"
