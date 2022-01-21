/* rdtsc_test.c - test program for rdtsc
# For documentation, see https://github.com/fordsfords/rdtsc
#
# This code and its documentation is Copyright 2022 Steven Ford
# and licensed "public domain" style under Creative Commons "CC0":
#   http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/rdtsc
*/

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include "rdtsc.h"


int main(int argc, char **argv)
{
  uint64_t start;
  uint64_t end;
  int i;
  double duration;

  rdtsc_calibrate();
  printf("ticks_per_sec=%" PRId64 ".\n", ticks_per_sec);

  /* Test calibration. */
  RDTSC(start);
  usleep(1000);
  RDTSC(end);
  duration = (double)(end - start) / (double)ticks_per_sec;
  printf("1ms~=%" PRId64 " ticks (%f sec).\n", (end - start), duration);

  /* Measure duration of rdtsc. */
  struct timespec start_ts;
  struct timespec end_ts;
  uint64_t start_ns, end_ns;
  clock_gettime(CLOCK_MONOTONIC, &start_ts);
  for (i = 0; i < 1000000; ++i) {
    RDTSC(end);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_ts);
  start_ns = (uint64_t)start_ts.tv_sec * UINT64_C(1000000000)
      + (uint64_t)start_ts.tv_nsec;
  end_ns = (uint64_t)end_ts.tv_sec * UINT64_C(1000000000)
      + (uint64_t)end_ts.tv_nsec;
  duration = (double)(end_ns - start_ns) / 1000000000.0;
  printf("1m rdtsc=%" PRId64 " ns.\n", (end_ns - start_ns));
  printf("  %f ns/rdtsc\n", duration * 1000.0);

  /* Measure duration of clock_gettime(). */
  clock_gettime(CLOCK_MONOTONIC, &start_ts);
  for (i = 0; i < 1000000; ++i) {
    clock_gettime(CLOCK_MONOTONIC, &end_ts);
  }
  clock_gettime(CLOCK_MONOTONIC, &end_ts);
  start_ns = (uint64_t)start_ts.tv_sec * UINT64_C(1000000000)
      + (uint64_t)start_ts.tv_nsec;
  end_ns = (uint64_t)end_ts.tv_sec * UINT64_C(1000000000)
      + (uint64_t)end_ts.tv_nsec;
  duration = (double)(end_ns - start_ns) / 1000000000.0;
  printf("1m clock_gettime=%" PRId64 " ns.\n", (end_ns - start_ns));
  printf("  %f ns/clock_gettime\n", duration * 1000.0);
 
  return 0;
}  /* main */
