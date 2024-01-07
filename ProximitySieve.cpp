/// ProximitySieve - generates 100-100k-digit prime checked with p<1B and
///                  preceded by ~715 verified consecutive composites (largest
///                  group found in billion-element sieve.) 1m run-time.


/* Version 5.1.0
#########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                             Eats ~2GB RAM!                             '###
##                                                                            ##
#,         You'll need the GNU Multiple Precision Arithmetic Library.         ,#
#'       Do apt install libgmp-dev then append "-lgmp" to both compile &      '#
##        build commands. Or then compile: g++ /path_to_this.cpp -lgmp        ##
###,            Set testing_mode to true for no randomness enrty.           ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#########

Applies the sieve of Eratosthenes on an  interval near the x-digit random number
--listing nearby primes, and testing only one & the same n by all p < 1 billion.

Encryption - after random_digits[] is filled with randomness, write your message
onto it in base 10, but ONLY in the middle  e.g. if you set prime_length to 500,
your message can be branded between elements 200 & 300.  It will be adjusted for
primality and written to file. Multiply that prime by another of similar length.
Now you have a semiprime difficult to  "un-multiply".  This is not a very strong 
primality test. But large numbers of preceding composites will up the chances.*/

#include <fstream>
#include <gmp.h> //For GMP.
#include <iostream>
using namespace std;

int main()
{	//                               user knobs
	
	/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  /////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\    ////////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      ///////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\        //////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\            ////////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\              ///////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\                  /////////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\\\\\                      ///////////////////////////
	\\\\\\\\\\\\\\\\\\\\\\\                              ///////////////////////
	\\\\\\\\\\\\\\\\\\                                        ////////////////*/
	
	//                                                                                                               |
	bool testing_mode = false; // DEFAULT = false                                              fatal if broken >     |
	//                                                                                                               |
	
	int prime_length = 5000; //Range: 100 to 100000.
	//(Actually, safe limits are 19 to 199000.)
	
	/*////////////////                                        \\\\\\\\\\\\\\\\\\
	///////////////////////                              \\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////                      \\\\\\\\\\\\\\\\\\\\\\\\\\\
	/////////////////////////////                  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////              \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////            \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//////////////////////////////////        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	/////////////////////////////////////  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
	
	ofstream out_stream;
	
	//Gets seeds for RNG.
	unsigned int user_seeds[50] = {0};
	if(testing_mode == false)
	{	//..........Gets 50 seeds from the user.
		cout << "\nEnter a random 8-digit integer, repeat 50 times.\n\n";
		for(int a = 0; a < 50; a++)
		{	if(a < 9) {cout << " " << (a + 1) << " of 50: ";} //..........Blank for aesthetics.
			else      {cout <<        (a + 1) << " of 50: ";}
			
			//..........Gets and checks input.
			cin >> user_seeds[a];
			if((user_seeds[a] > 99999999) || (user_seeds[a] < 10000000)) {cout << "\nOut of bounds, try again.\n"; return 0;}
		}
	}
	else
	{	//..........Fills seeds automatically (not secure, please turn off testing_mode.)
		cout << "\nCAUTION: testing_mode is turned on!  Use of these files is strongly discouraged!\n";
		srand(time(0));
		for(int a = 0; a < 50; a++) {user_seeds[a] = (rand() % 100000000);}
	}
	
	if(testing_mode == false) {system("clear");}
	cout << "\nGenerating " << prime_length << "-digit prime, wait ~1 minute...\n";
	
	//Fills random_digits[200000] with random digits. Its first x digits will be used to make n. (x = prime_length.)
	char random_digits[200000] = {0};
	for(int a = 0; a < 50; a++)
	{	srand(user_seeds[a]);
		
		if((user_seeds[a] % 2) == 0)
		{	//..........Constructively fills random_digits[] LEFT to right based on seeds.
			for(int b = 0; b < 200000; b++)
			{	random_digits[b] += (rand() % 10);
				random_digits[b] %= 10;
			}
		}
		else
		{	//..........Constructively fills random_digits[] RIGHT to left based on seeds.
			for(int b = 199999; b >= 0; b--)
			{	random_digits[b] += (rand() % 10);
				random_digits[b] %= 10;
			}
		}
	}
	
	//Ensures first digit is not zero, and that all are digits.
	if(random_digits[0] == 0) {random_digits[0]++;}
	for(int a = 0; a < 200000; a++) {random_digits[a] += 48;}
	
	//Cuts random_digits[] to length set by prime_length.
	random_digits[prime_length] = '\0';
	
	//Boolean sieve of Eratosthenes. Zeros are mapped to prime elements. Laughably, bool[] & char[] both consume 1 Byte.
	static bool sieve[1000000000] = {1, 1};
	for(int prime = 2; prime < 31623; prime++) //..........31,623 is sqrt(1,000,000,000). There are 50,847,534 primes. 
	{	for(; sieve[prime] == 1;) {prime++;} //..........Moves up the list if number already marked off.
		for(int a = prime + prime; a < 1000000000; a += prime) {sieve[a] = 1;} //..........Marks multiples (composites.)
	}
	
	
	
	
	
	//Begins.
	mpz_t       dividend, divisor, remainder;
	mpz_init   (dividend );
	mpz_init   (divisor  );
	mpz_init   (remainder);
	mpz_set_str(dividend, random_digits, 10);
	static bool proximity_sieve[1000000000] = {0};
	for(int a = 0; a < 1000000000; a++)
	{	if(sieve[a] == 0)
		{	//..........Mod operation.
			int prime = a;
			mpz_set_si(divisor, prime              );
			mpz_mod   (remainder, dividend, divisor);
			int extracted_remainder = mpz_get_si(remainder);
			
			//..........Proximity sieve operation.
			int natural_prime_position = (a - extracted_remainder);
			for(; natural_prime_position < 1000000000;)
			{	proximity_sieve[natural_prime_position] = 1;
				natural_prime_position += a;
			}
		}
	}
	
	//Finds prime element (having maximum verified consecutive composites in negative direction.)
	int prime_element;
	int largest_negative_gap = 0;
	for(int temp_ones_counter = 0, a = 0; a < 1000000000; a++)
	{	if(proximity_sieve[a] == 1) {temp_ones_counter++;}
		else
		{	if(temp_ones_counter > largest_negative_gap)
			{	largest_negative_gap = temp_ones_counter;
				prime_element = a;
			}
			
			temp_ones_counter = 0;
		}
	}
	
	
	
	
	
	//Append-writes to file: n + prime_element which adjusts n for primality.
	char char_sum[200000];
	mpz_t       n, addend, sum;
	mpz_init   (n     );
	mpz_init   (addend);
	mpz_init   (sum   );
	
	mpz_set_str(n, random_digits,  10);
	mpz_set_si (addend, prime_element);
	mpz_set_str(sum, char_sum,     10);
	
	//..........Sum operation.
	mpz_add    (sum, n, addend       );
	mpz_get_str(char_sum, 10, sum    );
	
	//..........Writes.
	out_stream.open("prime_values", ios::app);
	for(int a = 0; char_sum[a] != '\0'; a++) {out_stream.put(char_sum[a]);}
	out_stream << " (preceded by " << largest_negative_gap << " verified consecutive composites... And not divisible by primes less than one billion.\n";
	out_stream.close();
	
	//Overwrites RAM of array unsigned int user_seeds[50].
	for(int a = 0; a < 50; a++) {user_seeds[a] = 0; user_seeds[a] = 12345678; user_seeds[a] = 87654321; user_seeds[a] = 99999999;}
	
	//Overwrites RAM of array char random_digits[200000].
	for(int a = 0; a < 200000; a++) {random_digits[a] = '\0'; random_digits[a] = -1;} //Binary: 00000000, 111111111.
	
	//Overwrites RAM of array static bool proximity_sieve[1000000000].
	for(int a = 0; a < 1000000000; a++) {proximity_sieve[a] = 0; proximity_sieve[a] = 1;}
	
	//Overwrites RAM of array char char_sum[200000].
	for(int a = 0; a < 200000; a++) {char_sum[a] = '\0'; char_sum[a] = -1;} //Binary: 00000000, 111111111.
	
	cout << "\nDone! Verified consecutive composites preceding n: " << largest_negative_gap
	     << "\n(largest group found in billion-element sieve marked"
	     << "\nwith p<1B, aim for ~715+ so n is more likely prime.)\n\n\n\n\n";
}





/*#######*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                              TLDR: run it                              '###
##                                                                            ##
#,                                                                            ,#
#'                              apt install g++                               '#
##                           g++ /path/this_file.cpp                          ##
###,                          /path/resulting_file                          ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/

/*How to alter this code - Software package repositories for GNU+Linux operating
systems have all the tools you can imagine. Open a terminal and use this command
as root to install Geany and g++ on your computer: apt install geany g++   Geany
is a fast & lightweight text editor and Integrated Development Environment where
you can write and run code. g++ is the GNU compiler for C++  (industry standard)
which creates an executable file out of written code. It also displays errors in
code as you will see in the lower Geany box. Make a new folder somewhere on your
machine. Paste this code into Geany. For clarity in auditing, enable indentation
guides: go to View >> Show Indentation Guides. Save the document as anything.cpp
within the newly-created folder. Use these shortcuts to run the program: F9, F5.
Once  F9  is used, it needs not be used again unless you had modified this code.
You may paste over this code with other  .cpp files, or open a new tab & repeat.
Additionally, executables created by the  F9  command can be drag & dropped into
terminals (lazy directory pasting) so the executable can run.  exe is in folder.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
How to make an executable with g++  -  Save this program as anything.cpp, open a
terminal, and type g++ then space. Drag & drop this saved file into the terminal
and its directory will be  appended to your pending entry. Click on the terminal
and press enter.   a.out now resides in the user directory, you may rename it to
anything.  To run that executable, simply drag and drop it into a terminal, then
click on the terminal and press enter.  Reminder:  executable's effect-directory
is /home/user or where you put the executable. Opening  .cpp  files in Geany and
hitting F9 creates the same executable--movable, renameable, drag & droppable.*/
