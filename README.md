<!--
Generates 500-50k-digit prime checked with p<4M and preceded by ~399 verified
consecutive composites (largest group found in billion-element sieve.)
-->



<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/ProximitySieve/main/Other/Terminal_a438435ae6fd19ed6eabacd2601c662270288d067fb42481101a0e6ce67859f1.png">
</p>

[Auditing in the browser?](https://coliru.stacked-crooked.com/a/36d064a21f485255)

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

### That gap is simply the largest in the billion-element sieve

n's gap to another prime is not representative of this billion-element sieve
that's being marked with primes less than said limit. n's gap is just the
largest found. Statistical analysis shows a larger gap means higher primality likeliness.

<br>
<br>

### Version 5 plans--overkill

Checks with p<100M, preceding composite quantity not yet known, wait time not yet known.
