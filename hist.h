/* hist.h - fast histogrammer. */
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

#include <inttypes.h>

#ifndef HIST_H
#define HIST_H

/* Histogram. */
struct hist_s {
  uint64_t *buckets;
  uint64_t min_raw;
  uint64_t max_raw;
  uint64_t raw_sum;
  int size;  /* Number of buckets. */
  int overflows;  /* Number of values above the last bucket. */
  int num_samples;
};
typedef struct hist_s hist_t;


hist_t *hist_create(int size);
void hist_init(hist_t *hist);  /* Clear a histogram. */
void hist_input(hist_t *hist, uint64_t raw_sample, int bucket);
void hist_print(hist_t *hist);  /* Print to stdout the buckets. */
void hist_delete(hist_t *hist);

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif  /* HIST_H */
