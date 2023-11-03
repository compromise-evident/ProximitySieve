<!--
Generates 50-50k-digit prime checked with p<65536 having a prime gap of ~308.
-->



<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/ProximitySieve/main/Other/Terminal_68bd99f10d91939b360e11642d3e93170138399324b867e7c2b0986fbfc5f5cd.png">
</p>

[Auditing in the browser?](https://coliru.stacked-crooked.com/a/30b8b92221fd7d5d)

<br>
<br>

### You can have secure communication using raw semiprimes only! [1](https://twitter.com/redNVR/status/1715952926626103454), [2](https://github.com/compromise-evident/WhatNot/blob/main/Primality-adjusting%20branded%20strings.pdf)

**unsigned char random_digits[50000]** contains randomness used for n (the prime.) For example:
if prime_length (one of the user knobs you may adjust) is equal to 5000, then the first 5000
elements of random_digits[] will be used to make n. Go ahead and partially brand random_digits[]
with personal data (but only after the randomness is generated, yet before it's adjusted for primality.)
And remember: do not disturb the last few digits of n, ever.
