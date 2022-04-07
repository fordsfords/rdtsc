/* hist.c - fast histogrammer. */
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
is https://github.com/fordsfords/hist

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

#include "hist.h"


hist_t *hist_create(int size)
{
  hist_t *hist = NULL;
  hist = (hist_t *)malloc(size * sizeof(hist_t));
  if (hist == NULL) {
    return NULL;
  }

  hist->buckets = (uint64_t *)malloc(size * sizeof(uint64_t));
  if (hist->buckets == NULL) {
    free(hist);
    return NULL;
  }

  hist->size = size;

  hist_init(hist);

  return hist;
}

void hist_init(hist_t *hist)
{
  /* Re-initialize the data. */
  hist->min_raw = UINT64_MAX;
  hist->max_raw = 0;
  hist->raw_sum = 0;
  hist->overflows = 0;  /* Number of values above the last bucket. */
  hist->num_samples = 0;

  /* Init histogram (also makes sure it is mapped to physical memory. */
  int i;
  for (i = 0; i < hist->size; i++) {
    hist->buckets[i] = 0;
  }
}  /* hist_init */


void hist_input(hist_t *hist, uint64_t raw_sample, int bucket)
{
  hist->num_samples++;
  hist->raw_sum += raw_sample;

  if (raw_sample > hist->max_raw) {
    hist->max_raw = raw_sample;
  }
  if (raw_sample < hist->min_raw) {
    hist->min_raw = raw_sample;
  }

  if (bucket >= hist->size) {
    hist->overflows++;
  }
  else {
    hist->buckets[bucket]++;
  }
}  /* hist_input */


void hist_print(hist_t *hist)
{
  int i;
  for (i = 0; i < hist->size; i++) {
    printf("%"PRIu64"\n", hist->buckets[i]);
  }
}  /* hist_print */


void hist_delete(hist_t *hist)
{
  free(hist->buckets);
  free(hist);
}  /* hist_delete */
