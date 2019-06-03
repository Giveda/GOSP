#!/bin/bash

if [ $# -lt 1 ]; then
    echo usage ./run.sh  /path/to/GOSP/code/example
    exit 1
fi

TOOL_DIR=`dirname $0`

for item in $1/*
do
    if [ -d $item ]; then
        cd $item
        ./$item
        cd -
    fi
done
