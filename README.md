# rdtsc
C module to demonstrate getting low-impact time stamps using
x86 assembly language.

The "RDTSC()" macro invokes the rdtsc assembly language instruction,
which basically transfers the contents of the CPU's internal clock counter
register to a 64-bit variable.
This is typically the number of ticks since the last CPU reset.

Note that the value is not synchronized to "wall clock" time,
nor is it calibrated to any particular time period.

Also included in this project is a function "rdtsc_calibrate()" function,
which empirically measures the timer tick frequency.


## Usage

Let's see how long usleep(1000) *really* takes.

````C
#include "rdtsc.h"

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


## Measurements

On an Intel(R) Core(TM) i7-9800X CPU @ 3.80GHz
running CentOS Linux release 7.9.2009 (Core),
the output of the "rdtsc" program is:

````
$ ./rdtsc
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
% ./rdtsc
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

As usual, portability greatly suffers when you're trying to maximize
performance.

Note: according to
[Microsoft](https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps):

> We strongly discourage using the RDTSC or RDTSCP processor instruction to directly query the TSC because you won't get reliable results on some versions of Windows, across live migrations of virtual machines, and on hardware systems without invariant or tightly synchronized TSCs.

(That entire page has lots of interesting information.)

In my opinion, in 2022, using reasonably modern hardware
in a non-virtualized environment, you should be pretty safe using rdtsc.


## License

I want there to be NO barriers to using this code, so I am releasing it to the public domain.  But "public domain" does not have an internationally agreed-upon definition, so I use CC0:

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
