# PDC@UNL
## Module 2.0
**[School of Computing](https://computing.unl.edu/)**  
**[College of Engineering](https://engineering.unl.edu/)**  
**[University of Nebraska-Lincoln](https://unl.edu)**  

In this module, you will use parallel computing to solve a computationally
resource-intensive problem.  

# Passwords & Hashing

Secure systems typically require users to authenticate themselves using
a personal password before granting access.  However, passwords themselves
are not usually stored by the system, instead hashes of passwords are
stored.  A *hash* is a mapping from the set of all possible passwords to
values of a fixed length.  Typically these values are represented using
hexadecimal (base-16) strings.  There are a variety of *cryptographic*
hash functions that are used in practice.  For example a SHA-256 mapping
of the password `"password1234"` would result in the following hexadecimal
string value:

`"0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3"`

This hash value is stored by the system instead of the actual password.
When a user attempts to login, they provide the password, the hash
value is recomputed, and is then compared to the stored hash.  If it
matches, access is granted.  

In practice, this process is a bit more complex, but for this module
we'll consider this simplified version and attempt to *break* a password.

## Breaking Passwords

Hackers often break into systems by stealing hashed passwords and then
attempting to "break" them by finding the password that mapped to the
hash value(s).  Mathematically, it is impossible to "work backwards".  
Simply given a hash value such as:

`"0xb9a172c2c7028ca0d299cb77ac5e727cf0c28bcf0ab42bb93f45b73c22a06d9a"`

it is *impossible* to find the actual password that resulted in this
hash (since there may be *multiple* passwords that mapped to it!).  However,
you *can* try a lot of different values until you find one that works.

For example, it is common for users to use a password that is a common
dictionary word ("apple" for example).  A *dictionary attack* involves
using a common English dictionary, hashing every possible word and seeing
if it matches the given hash.  If it does, then we know what the original
password must have been!

To defeat this "easy" attack, most systems require users to use a combination
of upper/lowercase letters, numbers and symbols.  This doesn't prevent
the "brute force" approach of trying every possibility, it just makes it a
lot more difficult.  

# Demonstration & Exercises - C version

To keep things simple, we'll assume that the password(s) we're trying to
break consist of a dictionary word followed by a 2-digit number, `"password42"`
for example.  We have provided C code that performs a dictionary attack
against these types of passwords.  Clone our project from:

https://github.com/cbourke/PDCatUNL

and navigate to the `C/passwordCracker` directory.

## Sequential Benchmarking

Along with the code, we have included a standard American dictionary
with 247,406 different words.  With 100 2-digit number suffixes, that's
24,740,600 possible passwords.  First, we'll run a program that attempts
to crack passwords sequentially.  We recommend compiling and running these
programs on the CSE server (which has 64 cores).

1. Compile the program by executing the following command:

`make crackSequential`

2. You can *time* and run the program by executing the following command:

`time ./crackSequential "0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3"`

The program will hash all possible passwords as described above until
it finds a match to the given hash at which point it will stop and report
the matched password.  The `time` utility will report how much processor time
as well as "wall clock" time it took to execute the program.  If the program
does not find a match it will report this as well.  The output
should look *something* like the following:

```text
time ./crackSequential "0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3"
Password not found!
Hashes Computed: 24740600
225.251u 0.087s 3:45.35 99.9%	0+0k 0+0io 0pf+0w
```

The output indicates that the program tested all 24,740,600 possible passwords
but did not find a match.  It took `225.251` seconds (or `3:45.35`) to execute
and used (nearly) an entire processor (`99.9%`) the entire time.  

3. Run the program and attempt to crack the following hashed passwords.  Note the
time it took for each.

* `"0x0fb123ac46c55452dad8fdc41ba47944f57d929f06f0f1830574dd7f8c262d07"`
* `"0x194892d83bf082f174fe57ebdf10c9b9e4eaf5d0e16f24d1ffb848e95ccbd4b4"`
* `"0x618e0fcd87c82dea5cbc48b6ca32415951c79d21fc17a9a2e8a6170994b0cb0b"`

4. Answer the following questions:

a) How much time did it take for each password?
b) What percentage of processor power did each run take?
c) Why did it seem that the program was fast with some of the hashes but
   slower with others?
d) *About* how many hashes were computed/tested per second?  How long
   would it take to test 1 trillion hashes?

### Discussion

Here are the (partial) results of one of our executions of the program.

```text
Hashes Computed: 1943
0.077u 0.004s 0:00.08 87.5%	0+0k 0+0io 0pf+0w
...
Hashes Computed: 12349843
148.171u 0.007s 2:28.18 99.9%	0+0k 0+0io 0pf+0w
...
Hashes Computed: 24740543
328.519u 0.007s 5:28.54 99.9%	0+0k 0+0io 0pf+0w
```

Observations:
  * The CPU time and the "real" (or wall clock) time is roughly the same in
    every run of the program.  This is because the program is running on one
    processor nearly the entire time (as indicated by the `99.9%` of CPU
    usage).  
  * The program is faster for passwords that appear earlier in the dictionary
    and is slower for those that appear later in the dictionary and slowest of
    all when it fails to find a matching password and has to hash and check
    all 24 million hashes.  The program is only "faster" because it gets lucky.
  * In the last run of the program, it ran for
    328 seconds (or about 75,309 hashes per second).  In practice, rules for
    passwords are more strict.  For example,
    a system may require a combination of upper and lowercase letters as well
    as numbers (26+26+10 = 62 possible characters) and a minimum length.  Even
    with these simple rules and a minimum length of only 10 characters, there are
    over $62^{10} = 8.39 \times 10^{17}$ or 839 quadrillion possible hashes to check.
    At 75,309 hashes per second it would take up to 353,271 *years* to crack such a password!
    This is why complex password rules are required by secure systems.

## Parallel Speedup

Now let's see how we can use parallel computing to speed things up.  We have
provided a parallelized, configurable version of the same password cracking
program.  The program divides up the work of hashing/checking the 24 million
possible passwords among different *threads* that can each be run on a
different processor at the same time (in *parallel*).  A thread is simply
a chunk of code that can be run independently of the main program.  For example,
if we were to divide the 24 million different passwords among 4 threads/processors,
each would only need to compute 6 million possible hashes or 1/4 of the
work!  If all 4 are run at the same time, we *may* see the program run
4 times faster!

1. Compile the program by executing the following command:

`make crackParallel`

2. To make things fair, we'll try to crack one of the passwords that appears
   later in the dictionary (to ensure it takes the maximum amount of time).
   Run the program by executing the following command:

`time ./crackParallel "0x618e0fcd87c82dea5cbc48b6ca32415951c79d21fc17a9a2e8a6170994b0cb0b" 4`

   The `4` at the end is a configuration parameter: it specifies how many
   threads the program should use.

3. Run the program on the same hash but with a different number of threads;
   run it for 1, 2, 4, 8, 16, 32, 64, 128, and 256 threads (by changing the
   last parameter) and note the time it takes for the program to run.

4. Answer the following questions:

a) How much time did it take for each password?
b) What percentage of processor power did each run take and how does that
   relate to the number of threads?  Why did it not change for the final
   two runs?
c) What pattern did you observe for the CPU time versus actual (wall clock)
   time?  Explain the difference.
d) Estimate how many processors you would need to break a 10 character
   password (8.39 \times 10^17 possibilities) within a reasonable
   amount of time (say 1 hour)?

### Discussion

Here are the (partial) results of our executions of the program.

```text
1: 217.781u 0.000s 3:37.79 99.9%	0+0k 0+0io 0pf+0w
2: 218.104u 0.000s 1:49.08 199.9%	0+0k 0+0io 0pf+0w
4: 268.574u 0.031s 1:07.19 399.7%	0+0k 0+0io 1pf+0w
8: 234.905u 0.431s 0:29.49 797.9%	0+0k 0+0io 2pf+0w
16: 246.727u 0.327s 0:16.18 1526.8%	0+0k 0+0io 0pf+0w
32: 254.646u 0.671s 0:09.10 2805.6%	0+0k 0+0io 0pf+0w
64: 464.585u 1.246s 0:07.76 6002.8%	0+0k 0+0io 22pf+0w
128: 443.308u 0.811s 0:07.05 6299.4%	0+0k 0+0io 0pf+0w
256: 463.341u 0.854s 0:07.46 6222.3%	0+0k 0+0io 0pf+0w
```

Observations
  * We observed a speed up nearly proportional to the number of threads
    we used.  In other words: when we doubled the number of threads,
    the "wall clock" time (which is the third number; `3:37.79` for 1
    thread) was essentially *halved*.
  * However, the total *processor* time stayed the same or went up
    (the first column).  Though we split the work up among all
    of the processors and each processor only did a fraction of the
    work, the sum *total* of all the work did not change.
  * The percentage of processor usage/utilization nearly matched the
    number of threads we used.  For example, with 16 threads, we
    utilized `1526.8%` of the machine.  However, it maxed out when
    we got to 64 threads and did not increase with each higher thread
    count.  On the CSE server, there are only 64 cores so at most only 64
    processes or threads can be running at any one time.  
  * The wall-clock time also did not improve after 64 threads.  
  * Assuming 75,000 hashes can be computed *per thread* per second
    we would need roughly $3.107 \times 10^9$ or more than 3 billion
    processors to break an arbitrary lower/upper case/number combination
    password!
