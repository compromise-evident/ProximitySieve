Requirement: GNU Multiple Precision Arithmetic Library.
Just do ```apt install libgmp-dev``` then append "-lgmp" to both compile & build
commands in Geany, or then compile using ```g++ /path_to_this.cpp -lgmp```

<br>
<br>

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/ProximitySieve/main/Other/Terminal_7a2edfe8e17abb7037dd336b87d2d2c2e0d86d8ed5b2ef4b57d117c8a77fed0b.png">
</p>

<br>
<br>

# Encryption

You can have secure communication sending raw semiprimes only!
After random_digits[] is filled with randomness, write your message
onto it in base 10, but ONLY in the middle e.g. if you set prime_length to 300,
your message can be branded between elements 100 & 200. It will be adjusted for
primality and written to file. Multiply that prime by another of similar length.
Now you have a semiprime difficult to "un-multiply".

<br>
<br>

# Why 713?

713 "consecutive composites" before hitting another prime is
majority-natural for 310-digit primes. Finding these groups next to n,
is what makes n much more likely to be prime.
[ln(10^310) ~= 713](https://www.wolframalpha.com/input?i=natural+log+of+%2810%5E310%29).

<br>
<br>

# What it does

Applies the sieve of Eratosthenes on an  interval near the n-digit
random number--listing nearby primes, and testing only n by all primes < 1 billion,
then searches the sieve for a prime with the most preceding consecutive composites.

<br>
<br>

# Test it now

Set prime_length to 47.
Then copy n from file and paste it into [WolframAlpha](https://www.wolframalpha.com/)
as follows: "is n prime". If the number of preceding consecutive composites
is ~713 or higher, then your 47-digit n is extremely likely prime.
