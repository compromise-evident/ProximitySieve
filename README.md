<!--
Generates 100-100k-digit prime checked with p<1B and preceded by ~715 verified
consecutive composites (largest group found in billion-element sieve.) 1m run-time.
-->



Just do ```apt install libgmp-dev``` then append "-lgmp" to both compile & build commands in Geany, or just compile using ```g++ /path_to/ProximitySieve.cpp -lgmp```

<br>
<br>

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/ProximitySieve/main/Other/Terminal_1e7e80a733141b26347773097d387d7af7f1352f0ebeac197513aef59582e1b1.png">
</p>

[Auditing in the browser?](https://coliru.stacked-crooked.com/a/a45981fab7e7f7af)

<br>
<br>

## You can have secure communication using raw semiprimes only! [1](https://twitter.com/redNVR/status/1715952926626103454), [2](https://github.com/compromise-evident/WhatNot/blob/main/Primality-adjusting%20branded%20strings.pdf)

Brand random_digits[] with personal data before it's adjusted for primality.

<br>
<br>

## What it does

Applies the sieve of Eratosthenes on an  interval near the n-digit
random number--listing nearby primes, and testing only n by all primes < 1 billion,
then searches the sieve for a prime with the most preceding consecutive composites.
