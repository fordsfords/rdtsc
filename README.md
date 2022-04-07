# rdtsc
C module to demonstrate getting low-impact time stamps using
x86 assembly language.

# Table of contents 

- [rdtsc](#rdtsc)
- [Table of contents](#table-of-contents)
- [COPYRIGHT AND LICENSE](#copyright-and-license)
- [REPOSITORY](#repository)
- [DEPENDENCIES](#dependencies)
- [INTRODUCTION](#introduction)
- [USAGE](#usage)
- [MEASUREMENTS](#measurements)
- [RDTSC STATEMENT CONSIDERED HARMFUL](#rdtsc-statement-considered-harmful)
- [PRECISION VS ACCURACY](#precision-vs-accuracy)

<sup>(table of contents from https://luciopaiva.com/markdown-toc/)</sup>

# COPYRIGHT AND LICENSE

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

# REPOSITORY

See https://github.com/fordsfords/rdtsc for code and documentation.

# DEPENDENCIES

The core module (rdtsc.c) does not depend on other modules.

However, the "rdtsc_hist.c" example program depends on:
* https://github.com/fordsfords/hist - fast histogrammer.

# INTRODUCTION

The "RDTSC()" macro invokes the rdtsc assembly language instruction,
which basically transfers the contents of the CPU's internal clock counter
register to a 64-bit variable.
This is typically the number of ticks since the last CPU reset.

Note that the value is not synchronized to "wall clock" time,
nor is it calibrated to any particular time period.

Also included in this project is a function "rdtsc_calibrate()" function,
which empirically measures the timer tick frequency.

# USAGE

Let's see how long usleep(1000) *really* takes.

````C
  uint64_t start_ticks, end_ticks;

  rdtsc_calibrate();  /* Sets global varialbe rdtsc_ticks_per_sec. */

  RDTSC(start_ticks);
  usleep(1000);
  RDTSC(end_ticks)

  printf("duration of usleep(1000) is %f sec.\n",
      (double)(end_ticks - start_ticks) / (double)rdtsc_ticks_per_sec);
````

The "rdtsc_calibrate()" function is defined in the "rdtsc.c" module.
The "RDTSC()" macro is defined in the "rdtsc.h" include file.
The "rdtsc_ticks_per_sec" global varialbe is defined in the "rdtsc.c" module,
and declared external in the "rdtsc.h" include file.

# MEASUREMENTS

Linux:

````
$ ./rdtsc_test
rdtsc_ticks_per_sec=3789854845.
1ms~=4022648 ticks (0.001061 sec).
1m rdtsc=17577256 ns.
  17.577256 ns/rdtsc
1m clock_gettime=19876259 ns.
  19.876259 ns/clock_gettime
````

So clock_gettime() takes about 13% more time than rdtsc.
Using rdtsc might not be worth the loss of portability.

Then again, on Mac, the story is different:

````
$ ./rdtsc_test
rdtsc_ticks_per_sec=3093626031.
1ms~=3324780 ticks (0.001075 sec).
1m rdtsc=8854000 ns.
  8.854000 ns/rdtsc
1m clock_gettime=76192000 ns.
  76.192000 ns/clock_gettime
````

clock_gettime(CLOCK_MONOTONIC) is a LOT slower on Mac,
and rdtsc is noticeably faster.
To be fair, clock_gettime(CLOCK_MONOTONIC) is not the fastest way to get
high-resolution time on Mac; see [my notes](https://github.com/fordsfords/fordsfords.github.io/wiki/Timing-software#Conclusions).

# RDTSC STATEMENT CONSIDERED HARMFUL

There are risks in using the rdtsc instruction for timing measurements,
especially on older hardware.

According to
[Microsoft](https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps):

> We strongly discourage using the RDTSC or RDTSCP processor instruction to directly query the TSC because you won't get reliable results on some versions of Windows, across live migrations of virtual machines, and on hardware systems without invariant or tightly synchronized TSCs.

(That entire page has lots of interesting information.)

Not to mention that it won't work on SPARC or ARM machines.
As usual, portability greatly suffers when you're trying to maximize
performance.

In my opinion, in 2022, using reasonably modern hardware
in a non-virtualized environment, you should be pretty safe using rdtsc.

# PRECISION VS ACCURACY

Let's look at the actual values returned by rdtsc.
````
$ ./rdtsc_hist >rdtsc.out
$ head -1 rdtsc.out
ticks: 37881104805477176, 37881104805477194, 37881104805477214, 37881104805477232, 37881104805477254
````

Interesting that they're all even.
But that's a small sample size; let's look at the histogram of a million ticks,
bucketed by the lower 5 bits of the tick value ("ticks & 0x1f"):
````
$ cat rdtsc.out
ticks: 37881104805477176, 37881104805477194, 37881104805477214, 37881104805477232, 37881104805477254
histogram of rdtsc & 0x1f: min=37881104805549848, max=37881104830579304, avg=9939234737277.886719, ovf=0
69358
0
62888
0
62263
0
62343
0
62675
0
62194
0
62500
0
55553
0
69841
0
62557
0
61910
0
62728
0
62331
0
62415
0
62468
0
55976
0
````

All million ticks are even.
And even among the even values, the distribution is not uniform,
with ticks ending with binary 1110 being less frequent than other tick
4-bit even endings.

For the above Linux host (dual socket),
the precision (64 bits) is greater than the accuracy (63 bits),
which explains the even-only ticks.
I don't have an explanation for the non-uniform distribution of the even values.

I've seen different results on other hosts.
For example, another Linux host (single-socket) also had only even ticks,
but the distribution of the even tick values is uniform.

On my Mac, there is a very uniform distribution of counts
across *all* 32 5-bit endings.
I.e., there's no preference for even ticks,
suggesting that the Mac hardware has 64-bit precision and accuracy.

The bottom line is that you should analyze the "rdtsc" ticks
on a given machine before you use them.
The apparent precision may not equal the accuracy.
