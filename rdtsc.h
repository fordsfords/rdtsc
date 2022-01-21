/* rdtsc.h - test program for rdtsc
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
#ifndef NSTM_H
#define NSTM_H

#ifdef __cplusplus
extern "C" {
#endif


#define RDTSC(rdtsc_val_) do { \
  uint32_t rdtsc_hi_, rdtsc_lo_; \
  __asm__ volatile ("rdtsc" : "=a" (rdtsc_lo_), "=d" (rdtsc_hi_)); \
  rdtsc_val_ = (uint64_t)rdtsc_hi_ << 32 | rdtsc_lo_; \
} while (0)


/* Global (set by rdtsc_calibrate()). */
extern uint64_t rdtsc_ticks_per_sec;

void rdtsc_calibrate();

#ifdef __cplusplus
}
#endif

#endif  /* NSTM_H */
