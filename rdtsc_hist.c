/* rdtsc_hist.c - analyze ticks. */
/*
I want there to be NO barriers to using this code, so I am releasing it to the public domain.
But "public domain" does not have an internationally agreed-upon definition, so I use CC0:

Copyright 2022 Steven Ford http://geeky-boy.com and licensed
"public domain" style under
[CC0](http://creativecommons.org/publicdomain/zero/1.0/):
![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png "CC0")

To the extent possible under law, the contributors to this project have
waived all copyright and related or neighboring rights to this work.
In other words, you can use this code for any purpose without any
restrictions.  This work is published from: United States.  The project home
is https://github.com/fordsfords/rdtsc

To contact me, Steve Ford, project owner, you can find my email address
at http://geeky-boy.com.  Can't see it?  Keep looking.
*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if ! defined(_WIN32)
  #include <stdlib.h>
  #include <unistd.h>
#endif

#include "rdtsc.h"
#include "hist.h"

double ticks2ns(double ticks)
{
  double res = (double)ticks;
  res /= (double)rdtsc_ticks_per_sec;
  res *= 1000000000.0;
  return res;
}  /* ticks2ns */


int main(int argc, char **argv)
{
  rdtsc_calibrate();

  uint64_t ticks_1, ticks_2, ticks_3, ticks_4, ticks_5;

  /* Loop to warm up cache; only use the last cycle. */
  int i;
  for (i = 0; i < 100; i++) {
    RDTSC(ticks_1);
    RDTSC(ticks_2);
    RDTSC(ticks_3);
    RDTSC(ticks_4);
    RDTSC(ticks_5);
  }

  /* Accumulate a histogram of the lower 5 bits of each tick. */
  hist_t *hist = hist_create(32);
  if (hist == NULL) {
    fprintf(stderr, "hist_create returned null (malloc failure)\n");
    exit(1);
  }

  uint64_t ticks;
  for (i = 0; i < 1000000; i++) {
    RDTSC(ticks);

    hist_input(hist, ticks, ticks & 0x1f);
  }

  printf("ticks: %"PRIu64", %"PRIu64", %"PRIu64", %"PRIu64", %"PRIu64"\n",
      ticks_1, ticks_2, ticks_3, ticks_4, ticks_5);
  printf("histogram of rdtsc & 0x1f: min=%"PRIu64", max=%"PRIu64", avg=%f, ovf=%d\n",
      hist->min_raw, hist->max_raw,
      (double)hist->raw_sum / (double)hist->num_samples, hist->overflows);

  hist_print(hist);

  hist_delete(hist);

  return 0;
}  /* main */
