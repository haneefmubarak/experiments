buddysystem
===========

An experiment on buddy memory allocation schemes, 
comparing the conventional binary allocation
algorithm to the novel fibonacci allocation
algorithm. Results will be used in the
implementation of [sfDB5](https://github.com/haneefmubarak/sfDB5).

Compile & Run
-------------

Set constants in `buddywaste.c` to desired values:

```c
#define LOOKUP	binary	// which lookup table to use
			// ie: which algorithm

#define SIZE	8388608	// 8M
			// try for sizes up to this number
			// in bytes

#define INCR	1	// how much to increase the tested
			// size by each time

#define	MINS	1	// the minimum size to test
```

Compile:

```bash
# OSX
$ gcc -Wa,-q -march=native -x c buddywaste.c -O2 -Wall

# Linux
$ gcc -march=native -x c buddywaste.c -O2 -Wall
```

This code is intended to test both time and space
consumption. However, as all of the code executes
in userspace, it was deemed much better to measure
running time externally. Thus, this should do the
trick:

```bash
$ time ./a.out
```

Interpreting Results
--------------------

After running the above, you should get the
approximate percentage of allocated wasted space
along with the time required to run the program.

Here is an example output on OSX:

 - binary partitioning

   ```
   Wasted Space:	33%
   ./a.out  0.11s user 0.00s system 98% cpu 0.109 total
   ```

 - fibonacci partitioning

   ```
   Wasted Space:	24%
   ./a.out  0.15s user 0.00s system 98% cpu 0.153 total
   ```

The first value, reported by the program itself,
gives a rather accurate approximation of how much
space will typically be wasted with an even
distribution of the specified workload when using
the chose algorithm. Usually, fibonacci allocation
will waste less space than binary allocation.

The second value, reported by `time`, tells how much
time was used by each algorithm. While this is given
in seconds, in reality, this is not an exact
comparison of the timings due to various overheads.
However, this can be used as an excellent estimation
of the amount of time a single allocation takes via
the following:

```
single allocation time ~= (reported time * nominal cycles per second) / floor ((SIZE - MINS) / INCR)
```

Whereas fibonacci allocation usually wastes less
space than binary allocation, binary allocation
is usually faster than fibonacci allocation. This
is where the [space-time tradeoff](https://en.wikipedia.org/wiki/Space%E2%80%93time_tradeoff)
comes into play.

If speed is of the utmost essence, then you should
use binary allocation. If minimal wastage
of space is critical, you should use fibonacci
allocation.

In all other cases, since fibonacci allocation is
not _significantly_ slower than binary allocation
yet still utilizes space more efficiently, you
ought to use fibonacci allocation.
