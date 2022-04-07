#!/bin/sh


OPTS=""
if echo "$OSTYPE" | egrep -i linux >/dev/null; then :
  OPTS="-l rt"
fi

gcc -Wall -o rdtsc_test $OPTS rdtsc.c rdtsc_test.c
if [ $? -ne 0 ]; then exit 1; fi

gcc -Wall -o rdtsc_hist $OPTS rdtsc.c hist.c rdtsc_hist.c
if [ $? -ne 0 ]; then exit 1; fi
