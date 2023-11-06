/// ProximitySieve - generates 100-100k-digit prime checked with p<1B and preceded by ~715 verified
///                  consecutive composites (largest group found in billion-element sieve.) 1m run-time.
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net)


/* Version 5.0.0   Eats ~2GB RAM!
Set testing_mode to true, and you won't have to enter randomness  for the seeds.
Applies the sieve of Eratosthenes on an  interval near the x-digit random number
--listing nearby primes, and testing only one & the same n by all p < 1 billion.
This is not a very strong primality test but if n = the product of at least some
large primes, AND your protocol expects whole length n, then this sieve is OK.*/

#include <fstream>
#include <gmp.h> //For GMP
//(GNU Multiple Precision Arithmetic Library)
//To get GMP running, do apt install libgmp-dev
//then append "-lgmp" to both compile & build commands.
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
	
	ifstream in_stream;
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
		{	//Constructively fills random_digits[] LEFT to right based on seeds.
			for(int b = 0; b < 200000; b++)
			{	random_digits[b] += (rand() % 10);
				random_digits[b] %= 10;
			}
		}
		else
		{	//Constructively fills random_digits[] RIGHT to left based on seeds.
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
	for(int prime = 2; prime < 31623; prime++) //31,623 is sqrt(1,000,000,000). There are 50,847,534 primes. 
	{	for(; sieve[prime] == 1;) {prime++;} //Moves up the list if number already marked off.
		for(int a = prime + prime; a < 1000000000; a += prime) {sieve[a] = 1;} //Marks multiples (composites.)
	}
	
	//Begins.
	mpz_t       dividend, divisor, quotient;
	mpz_init   (dividend);
	mpz_init   (divisor );
	mpz_init   (quotient);
	mpz_set_str(dividend, random_digits, 10);
	static bool proximity_sieve[1000000000] = {0};
	for(int a = 0; a < 1000000000; a++)
	{	if(sieve[a] == 0)
		{	//..........Mod operation.
			int prime = a;
			mpz_set_si(divisor, prime);
			mpz_mod(quotient, dividend, divisor);
			int extracted_quotient = mpz_get_si(quotient);
			
			//..........Proximity sieve operation.
			int natural_prime_position = (a - extracted_quotient);
			for(; natural_prime_position < 1000000000;)
			{	proximity_sieve[natural_prime_position] = 1;
				natural_prime_position += a;
			}
		}
	}
	
	//Finds prime element (having maximum verified consecutive composites in negative direction.)
	int prime_element;
	int largest_negative_gap = 0;
	for(int a = 1000; a < 1000000000; a++)
	{	if(proximity_sieve[a] == 0)
		{	//..........Gets "prime gap" size in negative direction.
			int temp_negative_gap = 0;
			for(int b = (a - 1); proximity_sieve[b] == 1; b--) {temp_negative_gap++;}
			
			//..........Retains larger "prime gap."
			if(temp_negative_gap > largest_negative_gap)
			{	largest_negative_gap = temp_negative_gap;
				prime_element = a;
			}
		}
	}
	
	//Creates a command for doing n+prime_element which adjusts n for primality.
	char python_sum_command[50300] = {"python3 -c 'with open(\"prime_value\", \"a\") as file: file.write(str("}; //..........66-char init.
	char command_appendix  [   11] = {") + \"\\n\")'"                                                         }; //..........10-char init.
	
	//Prepares command for branding.
	for(int a = 66; a < 50300; a++) {python_sum_command[a        ] = ' '                ;}
	for(int a =  0; a <    11; a++) {python_sum_command[a + 50289] = command_appendix[a];}
	
	//Brands python_sum_command[] with the n-digit random number.
	for(int a = 0; a < prime_length; a++) {python_sum_command[a + 66] = (random_digits[a]);}
	python_sum_command[50100] = '+';
	
	//Brands prime_element (index value) on python_sum_command[].
	long long prime_branding = 1000000000000000000;
	prime_branding += prime_element;
	for(int b = 50200; b > 50183; b--)
	{	python_sum_command[b] = (prime_branding % 10);
		python_sum_command[b] += 48;
		
		prime_branding /= 10;
	}
	
	//Removes zeros in front of additive.
	for(int b = 50180;; b++)
	{	if(python_sum_command[b] == '0') {python_sum_command[b] = ' ';}
		if(python_sum_command[b]  >  48) {break                      ;}
	}
	
	//Append-writes prime to file.
	system(python_sum_command);
	
	//Overwrites RAM of array char python_sum_command[50300].
	for(int a = 0; a < 50300; a++) {python_sum_command[a] = '\0'; python_sum_command[a] = -1;} //Binary: 00000000, 111111111.
	
	//Overwrites RAM of array unsigned int user_seeds[50].
	for(int a = 0; a < 50; a++) {user_seeds[a] = 0; user_seeds[a] = 12345678; user_seeds[a] = 87654321; user_seeds[a] = 99999999;}
	
	//Overwrites RAM of array char random_digits[200000].
	for(int a = 0; a < 200000; a++) {random_digits[a] = '\0'; random_digits[a] = -1;} //Binary: 00000000, 111111111.
	
	//Overwrites RAM of array static bool proximity_sieve[1000000000].
	for(int a = 0; a < 1000000000; a++) {proximity_sieve[a] = 0; proximity_sieve[a] = 1;}
	
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
