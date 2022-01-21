#!/bin/sh


OPTS=""
if echo "$OSTYPE" | egrep -i linux >/dev/null; then :
  OPTS="-l rt"
fi

gcc -Wall -o rdtsc $OPTS rdtsc.c
if [ $? -ne 0 ]; then exit 1; fi
