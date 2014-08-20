rendezvous
==========

An experiment on rendezvous hash distribution,
to be used for in the implementation of [sfDB5](https://github.com/haneefmubarak/sfDB5).

Compile & Run
-------------

Set constants in `sbins.c` to desired values:

```c
#define NODES	// the number of servers

#define KEYS	// the number of keys to
		// distribute accross the
		// servers

#define CSKIP	// will print a progress
		// update every CSKIP
		// cycles
```

Compile:

```bash
# OSX
gcc -Wa,-q -march=native -x c sbins.c -O3 -Wall -lm

# Linux
gcc -march=native -x c sbins.c -O3 -Wall -lm
```

A source of good quality fast random data is
required. On OSX and Linux platforms, this
should work fairly well:

```bash
$ cat /dev/urandom | ./a.out
```

Interpreting Results
--------------------

After running the above, you should get a progress
output, followed by key/bin (key/node) counts,
followed by some simple statistics.

Here is an example output on OSX with:

 - 20 nodes
 - 16777216 keys
 - 1048576 CSKIP

```
0
1048576
2097152
3145728
4194304
5242880
6291456
7340032
8388608
9437184
10485760
11534336
12582912
13631488
14680064
15728640








Bin 0	: 838966
Bin 1	: 838596
Bin 2	: 839535
Bin 3	: 838835
Bin 4	: 839220
Bin 5	: 838759
Bin 6	: 838907
Bin 7	: 838903
Bin 8	: 838302
Bin 9	: 838726
Bin 10	: 838522
Bin 11	: 838034
Bin 12	: 839020
Bin 13	: 839334
Bin 14	: 838898
Bin 15	: 838905
Bin 16	: 838984
Bin 17	: 838753
Bin 18	: 838956
Bin 19	: 839061








avg: keys/bin	= 838860.812500
mad: keys/bin	= 235.500000
mad / avg	= 00.03 %
```

The first sequence of values tell how many
keys have been assigned to bins so far. This
is mainly useful as a progress indicator,
and can also be used to gauge the relative
speed of different numbers of keys and nodes.

The second sequence of values tells how many
keys have been assigned to each particular bin.
Optimally, this number should be as close to
the `avg` value (see below), as this controls
the hopefully relatively even distribution of
the data across the servers.

The `avg` value is the arithmetic mean number of
keys per bin (node). Since there is a constant
number of keys and bins, all runs with the
same constants set will yield the same `avg`
value. This value is the ideal number of
keys per bin.

The `mad` value is the mean absolute deviation
of the keys per bin. Preferably, this number
should be as close to zero as possible. This
number does not mean much (pun intended)
without being compared to the total number
of keys set by the compile-time constant
`KEYS`.

The last value is simply the `mad` value
divided by the `avg` value. This gives an
approximation of the amount of data that
will be unbalanced across the cluster.
The value in the above example, `00.03 %`
means that the servers should in theory
be balanced give or take 00.03 percent,
which is phenomenal. Of course, this
will depend on the particular data that
is being stored. However, as larger and
larger amounts of data are stored,
assuming that a good, uniformly
distributed hash is used, the real
distribution of the data should begin
to closely approach the theoretical
distribution.

Hashing
-------

Used BLAKE2 originally. Code was too slow.

Switched to multiplication. Code was fast.

Did bit inversion to ID to improve
distribution slightly. Code is still fast.

[1]: https://github.com/haneefmubarak/sfDB5 "sfDB5"
