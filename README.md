<!--
Generates 100-100k-digit prime checked with p<1B and preceded by ~715 verified
consecutive composites (largest group found in billion-element sieve.) 1m run-time.
-->



<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/ProximitySieve/main/Other/Terminal_c805a87dc4e4eb1807505ff3124161e161d895651adb16fce335699f3b7ff643.png">
</p>

[Auditing in the browser?](https://coliru.stacked-crooked.com/a/1521b8e5f84d2a58)

<br>
<br>

### You can have secure communication using raw semiprimes only! [1](https://twitter.com/redNVR/status/1715952926626103454), [2](https://github.com/compromise-evident/WhatNot/blob/main/Primality-adjusting%20branded%20strings.pdf)

**unsigned char random_digits[50000]** contains randomness used for n (the prime.) For example:
if prime_length (one of the user knobs you may adjust) is equal to 5000, then the first 5000
elements of random_digits[] will be used to make n. Go ahead and partially brand random_digits[]
with personal data (but only after the randomness is generated, yet before it's adjusted for primality.)
And remember: do not disturb the last few digits of n, ever.

<br>
<br>

### How it works

**TLDR:** Applies the sieve of Eratosthenes on an  interval near the n-digit
random number--listing nearby primes, and testing only n by all primes < 4 million.
Then it searches the sieve for a prime with the most preceding composites.

char python_mod_command[50300] is a C++ array serving as a string, and is automatically
branded with a python3 command to perform n mod p<4M, results of which are append-written
to file. C++ then takes those results and applies them against the boolean proximity_sieve[1000000000]
whose first element represents n (n is the random digits representing a number that is to be
adjusted for primality.)
