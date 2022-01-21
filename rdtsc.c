/* rdtsc.c - code for rdtsc module.
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

#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include "rdtsc.h"


uint64_t rdtsc_ticks_per_sec = 0;

void rdtsc_calibrate()
{
  struct timespec start_ts;
  struct timespec end_ts;
  uint64_t start_ns, end_ns, duration_ns;
  uint64_t start_ticks, end_ticks, duration_ticks;

  /* We will calibrate with an approx 2 ms sleep. But usleep() is
   * not very accurate, so we will use clock_gettime() to measure
   * precisely how long the sleep was.
   */
  clock_gettime(CLOCK_MONOTONIC, &start_ts);
  RDTSC(start_ticks);
  usleep(2000);  /* ~2 ms. */
  RDTSC(end_ticks);
  clock_gettime(CLOCK_MONOTONIC, &end_ts);

  start_ns = (uint64_t)start_ts.tv_sec * UINT64_C(1000000000)
      + (uint64_t)start_ts.tv_nsec;
  end_ns = (uint64_t)end_ts.tv_sec * UINT64_C(1000000000)
      + (uint64_t)end_ts.tv_nsec;
  duration_ns = end_ns - start_ns;
  duration_ticks = end_ticks - start_ticks;
  /* sec * ns/sec * ticks/ns = ticks. */
  rdtsc_ticks_per_sec = (UINT64_C(1000000000) * duration_ticks) / duration_ns;
}  /* rdtsc_calibrate */
