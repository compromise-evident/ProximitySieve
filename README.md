Requirement: GNU Multiple Precision Arithmetic Library.
Just do ```apt install libgmp-dev``` then append "-lgmp" to both compile & build
commands in Geany, or then compile using ```g++ /path_to_this.cpp -lgmp```

<br>
<br>

<p align="center">
  <img src="https://raw.githubusercontent.com/compromise-evident/ProximitySieve/main/Other/Terminal_1e7e80a733141b26347773097d387d7af7f1352f0ebeac197513aef59582e1b1.png">
</p>

<br>
<br>

## Encryption

You can have secure communication sending raw semiprimes only!
After random_digits[] is filled with randomness, write your message
onto it in base 10, but ONLY in the middle e.g. if you set prime_length to 500,
your message can be branded between elements 200 & 300. It will be adjusted for
primality and written to file. Multiply that prime by another of similar length.
Now you have a semiprime difficult to "un-multiply".

<br>
<br>

## What it does

Applies the sieve of Eratosthenes on an  interval near the n-digit
random number--listing nearby primes, and testing only n by all primes < 1 billion,
then searches the sieve for a prime with the most preceding consecutive composites.

<br>
<br>

## Search the sieve for other possible primes

Search through the boolean proximity_sieve[ ] before it's overwritten.
Any element containing '0' is not divisible by p<1B. Once you find an element
you like, set prime_element equal to the index of your element.
Just erase the block containing "prime_element = a;".

<br>
<br>

## Test ProximitySieve now

Set testing_mode to true, and prime_length to 47.
Then copy n and paste it into [WolframAlpha](https://www.wolframalpha.com/)
as follows: "is n prime" If the number of preceding consecutive composites
is ~675 or higher, then your 47-digit n is extremely likely prime.
