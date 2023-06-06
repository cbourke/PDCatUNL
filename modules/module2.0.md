# PDC@UNL
## Module 2.0
**[School of Computing](https://computing.unl.edu/)**  
**[College of Engineering](https://engineering.unl.edu/)**  
**[University of Nebraska-Lincoln](https://unl.edu)**  

In this module, you will use parallel computing to speed up computations which would otherwise take a significant amount of time using traditional methods.

# Passwords & Hashing

Secure systems typically require users to authenticate their identities using a password before granting access. However, it is best practice to not save the actual passwords within the system because that could allow some users to see everyone else's passwords. To avoid this problem, *hashes* of passwords are stored instead.

A *hash* is a mapping from the set of all possible passwords to fixed-length values which are represented in hexadecimal (base 16) strings. There are many different *cryptographic* hash functions used for this purpose. For example, a SHA-256 mapping of the password `"password1234"` maps to the following hexadecimal string value:

`"0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3"`

This hash value is stored instead of the actual password. When users attempt to login with their passwords, the system rehashes those inputs to generate a hexadecimal string which is then compared to the stored hash. If the rehased value from the user matches the stored value, access is granted.

This process is more complicated in practice, but we will use this basic understanding to show how you could *break* password encryption schemes.

## Breaking Passwords

Hackers often break into systems by stealing lists of hashed passwords and then attempting to "break" them by determining which passwords map to those hash values. Mathematically, these type of encryption schemes make it *impossible* for you to "work backwards."

Given the hash value:

`"0xb9a172c2c7028ca0d299cb77ac5e727cf0c28bcf0ab42bb93f45b73c22a06d9a"`

you would never be able to find the correct password which maps to this
hash because there could be *multiple* passwords which map to it! However,
you *can* make guesses until you find one that matches.

For example, users tend to use passwords which are common words or sequences of characters like "secret" or "qwerty". These types of passwords are bad choices because they are weak to *dictionary attacks*. A dictionary attack involves using a common English dictionary or list of known popular passwords as your starting guesses. You then hash every entry in your dictionary and check whether it matches the given hash. If you find a match, then you know the original password!

One approach for defeating this "easy" attack is to require users to create passwords which contain at least one character which is: an uppercase letter, a lowercase letter, a number, and a special symbol. This doesn't prevent anyone from trying to "brute force" every possibility, but it makes it more difficult than if you only had to guess common words.

# Demonstration & Exercises - C version

To keep things simple, lets break passwords which are limited to the following format: a dictionary word followed by a 2-digit number. For example, `"password42"` or `"jordan23"`. We have provided starter code to perform a dictionary attack
against these types of passwords. To start, clone our project from:

https://github.com/cbourke/PDCatUNL

and navigate to the `C/passwordCracker` directory.

## Sequential Benchmarking

Included in the code is a standard American dictionary of 247,406 words. With 100 possible 2-digit number, that means there are
24,740,600 possible passwords in our limited format. First, we'll try to crack passwords sequentially, trying each possible password one after another until all possible passwords have been hashed and checked for a match. We recommend compiling and running these programs on the CSE server to speed up runtime because it has 64 cores.

1. Compile the program by executing the following command:

```text
make crackSequential
```

2. You can *time* and run the program by executing the following command:

```text
time ./crackSequential "0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3"
```

The program will hash all possible passwords until it finds a match with the given hash. If it finds a match, it will stop and report the matched password.

The `time` utility will report processor time and "wall clock" time of program execution. If the program fails to find a match it will report this as well. The output should look *something* like the following:

```text
time ./crackSequential "0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3"
Password not found!
Hashes Computed: 24740600
225.251u 0.087s 3:45.35 99.9%	0+0k 0+0io 0pf+0w
```

This specific output indicates the program hashed all 24,740,600 possible passwords but did not find a match for the given hash. It took `225.251` seconds (or `3:45.35`) to execute and used nearly one entire processor's capacity (`99.9%`) the entire time it ran.

3. Run the program and see if you crack the following hashed passwords. For each hash, make sure to record the time required.

   * `"0x0fb123ac46c55452dad8fdc41ba47944f57d929f06f0f1830574dd7f8c262d07"`
   * `"0x194892d83bf082f174fe57ebdf10c9b9e4eaf5d0e16f24d1ffb848e95ccbd4b4"`
   * `"0x618e0fcd87c82dea5cbc48b6ca32415951c79d21fc17a9a2e8a6170994b0cb0b"`

4. Answer the following questions:
   * How much time did it take for each password?
   * What percentage of processor capacity did each run take?
   * Why do you think it might take more or less time to break different passwords using this method?
   * *About* how many hashes did the program test per second? How long would it take to test one billion hashes? How long would it take to test one trillion hashes?

### Discussion

Below are some partial example outputs from the program.

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
  * The CPU time and the "real" (wall clock) time is roughly the same for every run of the program. This is because the program is running on one processor nearly the entire time, as indicated by the `99.9%` of CPU usage.
  * The program is faster for passwords which appear earlier in the dictionary, slower for those later in the dictionary, and slowest overal when it fails to find a matching password because it hashes and compares all 24 million possibilities. The program only runs "faster" because it gets lucky with an early entry.
  * In the last run of the program, it ran for 328 seconds (about 75,309 hashes per second). In practice, rules for passwords are more strict. For example, a system may require a combination of uppercase and lowercase letters as well as numbers ($26+26+10 = 62$ possible characters) with a specific overall minimum number of characters. With these simple additional requirements and a minimum length of 10 characters, there are over $62^{10} = 8.39 \times 10^{17}$, or 839 quadrillion possible hashes to check. If we can process 75,309 hashes per second, it would take over 353,271 *years* to try every possible password! This is why most systems implement more complicated password requirements.

## Parallel Speedup

Now let's see how we can use parallel computing to speed things up. We have provided a parallelized, configurable version of the same password cracking program. The program divides the work of hashing and checking all possible passwords among different *threads* which can each run on their own different processors at the same time, thus in *parallel*. A thread is simply a chunk of code that can run independently of the main program. For example, if we divide all 24 million passwords among four threads (and four processors), each would only need to compute six million possible hashes, or only 25% of the work! If all 4 run at the same time, we *might* see a 4x speed improvement!

1. Compile the program by executing the following command:

`make crackParallel`

2. To make things fair, we'll try to crack one of the passwords that appears
   later in the dictionary (to ensure it takes the maximum amount of time).
   Run the program by executing the following command:

```text
time ./crackParallel "0x618e0fcd87c82dea5cbc48b6ca32415951c79d21fc17a9a2e8a6170994b0cb0b" 4
```

   The **4** at the end is a configuration parameter which specifies how many threads the program should use.

3. Run the program on the same hashes from the prior task, but try different numbers of threads: try 1, 2, 4, 8, 16, 32, 64, 128, and 256 threads by changing the last parameter and note the time it takes for the program to run.

4. Answer the following questions:

   * How much time did it take for each password the different number of threads?
   * What percentage of processor power did each run take and how does that relate to the number of threads? Why did it not change for the last two runs?
   * What pattern did you observe for CPU time compared to actual (wall clock) time? Explain the difference.
   * Estimate how many processors you would need to break a 10-character password ($8.39 \times 10^{17}$ possibilities) within a reasonable amount of time, say one hour?

### Discussion

Below are some partial example outputs from the program running parallel threads.

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
  * We observed a speed up nearly proportional to the number of threads we used. In other words: when we doubled the number of threads, the "wall clock" time (which is the third number; `3:37.79` for 1 thread) was essentially *halved*.
  * However, the total *processor* time stayed the same or went up (the first column). Alhough we split the work among all processors and each processor only did a fraction of the work, the *total* work did not change.
  * The percentage of processor utilization nearly matched the number of threads we used. For example, with 16 threads, we utilized `1526.8%` of the machine. However, it maxed out when we got to 64 threads and did not increase with each higher thread count. On the CSE server, there are only 64 cores, so at most only 64 processes or threads can run at any one time.
  * The wall-clock time also did not improve after 64 threads.
  * Assuming we can compute 75,000 hashes *per thread* per second, we would need roughly $3.107 \times 10^9$, or more than three billion processors to break an arbitrary lower/upper case/number combination password!
