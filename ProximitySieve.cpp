/// ProximitySieve - keeps generating 310-digit primes
///                  checked with p<1B and preceded by
///                  713+ verified consecutive composites.


/* Version 5.2.0
#########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
#####'`                                                                  `'#####
###'                             Eats ~2GB RAM!                             '###
##                                                                            ##
#,         You'll need the GNU Multiple Precision Arithmetic Library.         ,#
#'       Do apt install libgmp-dev then append "-lgmp" to both compile &      '#
##        build commands. Or then compile: g++ /path_to_this.cpp -lgmp        ##
###,                                                                        ,###
#####,.                                                                  .,#####
##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#########

Encryption - after random_digits[] is filled with randomness, write your message
onto it in base 10, but ONLY in the middle  e.g. if you set prime_length to 300,
your message can be branded between elements 100 & 200.  It will be adjusted for
primality and written to file. Multiply that prime by another of similar length.
Now you have a semiprime difficult to  "un-multiply".  This is not a very strong 
primality test.  But large numbers of preceding composites  will up the chances.
However, try searching this file for   "Here's your chance to brand each n".  */

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
	
	int prime_length = 310; //DEFAULT = 310. Only smaller is allowed.
	
	int minimum_preceding_composites = 713; //DEFAULT = 713.
	
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
	
	//Creates file "Seeds" if missing.
	in_stream.open("Seeds");
	bool existence_of_file_Seeds = true;
	if(in_stream.fail() == true) {existence_of_file_Seeds = false;}
	in_stream.close();
	
	char garbage_byte;
	if(existence_of_file_Seeds == false)
	{	//..........Gets path to file from user.
		cout << "\nJust this one time, provide any file"
		     << "\nwith at least 1,000 random first Bytes."
		     << "\nDrag & drop it into terminal or enter path:\n";
		
		//..........Gets path then fixes it if drag-n-dropped, regardless of single-quote presence and "enter"
		//..........not being cleared, meaning you can have options before this, where the user presses enter.
		char path_to_file[10000] = {'\0'};
		{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
			cin.getline(path_to_file, 10000);
			if(path_to_file[0] == '\0')
			{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
				cin.getline(path_to_file, 10000);
			}
			if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
			
			//..........Removes last space in path_to_file[].
			bool existence_of_last_space = false;
			for(int a = 9999; a > 0; a--)
			{	if(path_to_file[a] != '\0')
				{	if(path_to_file[a] == ' ') {path_to_file[a] = '\0'; existence_of_last_space = true;}
					break;
				}
			}
			
			//..........Removes encapsulating single-quotes in path_to_file[].
			bool existence_of_encapsulating_single_quotes = false;
			if(path_to_file[0] == '\'')
			{	for(int a = 0; a < 9999; a++)
				{	path_to_file[a] = path_to_file[a + 1];
					if(path_to_file[a] == '\0') 
					{	if(path_to_file[a - 1] != '\'') {cout << "\nBad path.\n"; return 0;}
						path_to_file[a - 1] = '\0';
						existence_of_encapsulating_single_quotes = true;
						break;
					}
				}
			}
			
			//..........Replaces all "'\''" with "'" in path_to_file[].
			int single_quote_quantity = 0;
			for(int a = 0; a < 10000; a++)
			{	if(path_to_file[a] == '\'') {single_quote_quantity++;}
			}
			
			if((single_quote_quantity                     >    0)
			&& (existence_of_last_space                  == true)
			&& (existence_of_encapsulating_single_quotes == true))
			{	if((single_quote_quantity % 3) != 0) {cout << "\nBad path.\n"; return 0;}
				
				for(int a = 0; a < 9997; a++)
				{	if(path_to_file[a] == '\'')
					{	int temp = (a + 1);
						for(; temp < 9997; temp++)
						{	path_to_file[temp] = path_to_file[temp + 3];
						}
					}
				}
			}
		}
		
		//..........Checks if given file has at least 1,000 Bytes.
		in_stream.open(path_to_file);
		int thousand = 0;
		for(int a = 0; a < 1000; a++)
		{	in_stream.get(garbage_byte);
			if(in_stream.eof() == false) {thousand++;}
		}
		in_stream.close();
		
		if(thousand < 1000) {cout << "\n\nFile too small.\n\n"; return 0;}
		
		//..........Creates file "Seeds" (copies first 1,000 Bytes from given file.)
		in_stream.open(path_to_file);
		out_stream.open("Seeds");
		in_stream.get(garbage_byte);
		for(int a = 0; a < 1000; a++) {out_stream.put(garbage_byte); in_stream.get(garbage_byte);}
		in_stream.close();
		out_stream.close();
	}
	
	//Checks if file "Seeds" contains at least 1,000 Bytes.
	in_stream.open("Seeds");
	int thousand = 0;
	for(int a = 0; a < 1000; a++)
	{	in_stream.get(garbage_byte);
		if(in_stream.eof() == false) {thousand++;}
	}
	in_stream.close();
	
	if(thousand < 1000) {cout << "\n\nBad \"Seeds\" file. (Must be 1,000 Bytes.)\n\n"; return 0;}
	
	//Boolean sieve of Eratosthenes. Zeros are mapped to prime elements. Laughably, bool[] & char[] both consume 1 Byte.
	static bool sieve[1000000000] = {1, 1};
	for(int prime = 2; prime < 31623; prime++) //..........31,623 is sqrt(1,000,000,000). There are 50,847,534 primes. 
	{	for(; sieve[prime] == 1;) {prime++;} //..........Moves up the list if number already marked off.
		for(int a = prime + prime; a < 1000000000; a += prime) {sieve[a] = 1;} //..........Marks multiples (composites.)
	}
	
	
	
	
	
	//Begins.
	cout << "\nGenerating " << prime_length << "-digit primes...\n";
	char random_digits[311] = {'\0'};
	static bool proximity_sieve[1000000000] = {0}; //..........Billion-element segment "in front of random n", to be marked by p<1B.
	for(;;)
	{	//Creates random n (random_digits[]) based on file "Seeds" which is overwritten per.
		in_stream.open("Seeds"); //..........Loads temp_seeds[].
		int temp_seeds[1000];
		for(int a = 0; a < 1000; a++)
		{	in_stream.get(garbage_byte);
			int normal_byte = garbage_byte;
			if(normal_byte < 0) {normal_byte += 256;}
			
			temp_seeds[a] = normal_byte;
		}
		in_stream.close();
		
		//..........Makes proper seeds[] out of temp_seeds[].
		int seeds[1000];
		for(int a = 0; a < 1000; a++)
		{	srand(temp_seeds[a]);
			
			if(temp_seeds[a] % 2 == 0)
			{	//..........Constructively fills seeds[] LEFT to right based on temp_seeds[].
				for(int b = 0; b < 1000; b++)
				{	seeds[b] += rand();
					seeds[b] %= 95;
				}
			}
			else
			{	//..........Constructively fills seeds[] RIGHT to left based on temp_seeds[].
				for(int b = 999; b >= 0; b--)
				{	seeds[b] += rand();
					seeds[b] %= 95;
				}
			}
		}
		
		//..........Generates code. (6,570-bit seed per generated char.)
		long long secondary_seeds[100];
		int table[1001];
		for(int a = 0; a < prime_length; a++)
		{	for(int b = 0; b < 1001; b++) {table[b] = 0;}
			
			//..........Uses seeds[].
			for(int b = 0; b < 1000; b++)
			{	srand(seeds[b]);
				
				if(seeds[b] % 2 == 0)
				{	//..........Constructively fills table[] LEFT to right based on seeds[].
					for(int c = 0; c < 1001; c++)
					{	table[c] += rand();
						table[c] %= 95;
					}
				}
				else
				{	//..........Constructively fills table[] RIGHT to left based on seeds[].
					for(int c = 1000; c >= 0; c--)
					{	table[c] += rand();
						table[c] %= 95;
					}
				}
			}
			
			//..........Creates secondary_seeds[] based on seeds[].
			for(int b = 0; b < 100; b++) {secondary_seeds[b] = 36;}
			int seeds_read_bookmark = 0;
			for(int b = 0; b < 100; b++) //..........Creates 100 (mostly 10-digit) seeds.
			{	for(int c = 0; c < 10; c++)
				{	secondary_seeds[b] *= (seeds[seeds_read_bookmark] + 1);
					secondary_seeds[b] %= 10000000000;
					if(secondary_seeds[b] == 0) {secondary_seeds[b]++;}
					
					seeds_read_bookmark++;
				}
			}
			
			//..........Uses secondary_seeds[].
			for(int b = 0; b < 100; b++)
			{	srand(secondary_seeds[b]);
				
				if(secondary_seeds[b] % 2 == 0)
				{	//..........Constructively fills table[] LEFT to right based on secondary_seeds[].
					for(int c = 0; c < 1001; c++)
					{	table[c] += rand();
						table[c] %= 95;
					}
				}
				else
				{	//..........Constructively fills table[] RIGHT to left based on secondary_seeds[].
					for(int c = 1000; c >= 0; c--)
					{	table[c] += rand();
						table[c] %= 95;
					}
				}
			}
			
			//..........Copies first 1,000 char from table[] to seeds[].
			for(int b = 0; b < 1000; b++) {seeds[b] = table[b];}
			
			//..........Append-writes 1,001st character from table[] to random_digits[]. (One char at-a-time.)
			random_digits[a]  = table[1000];
			random_digits[a] %= 10;
			random_digits[a] += 48;
		}
		
		//..........Ensures first digit is not zero.
		if(random_digits[0] == '0') {random_digits[0] = '1';}
		
		//..........Overwrites file "Seeds" with new seeds.
		out_stream.open("Seeds");
		for(int a = 0; a < 1000; a++) {out_stream.put(seeds[a] + 32);}
		out_stream.close();
		
		
		
		
		
		/*#######*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*##########
		#####'`                                                                  `'#####
		###'                Here's your chance to brand each n with                 '###
		##                 your message, before that n is made prime.                 ##
		#,                                                                            ,#
		#'                    Example:  random_digits[100] = '5';                     '#
		##                   Just don't disturb the last 20 digits                    ##
		###,                       such as random_digits[279].                      ,###
		#####,.                                                                  .,#####
		##########*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#######*/
		
		
		
		
		
		//Operates on proximity_sieve[].
		mpz_t dividend ; mpz_init (dividend );
		mpz_t divisor  ; mpz_init (divisor  );
		mpz_t remainder; mpz_init (remainder);
		
		mpz_set_str(dividend, random_digits, 10);
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
		mpz_t n     ; mpz_init(n     );
		mpz_t addend; mpz_init(addend);
		mpz_t sum   ; mpz_init(sum   );
		
		char char_sum[1000];
		mpz_set_str(n, random_digits,  10);
		mpz_set_si (addend, prime_element);
		mpz_set_str(sum, char_sum,     10);
		
		//..........Sum operation.
		mpz_add    (sum, n, addend       );
		mpz_get_str(char_sum, 10, sum    );
		
		//..........Writes just the prime.
		out_stream.open("prime_values", ios::app);
		for(int a = 0; char_sum[a] != '\0'; a++) {out_stream.put(char_sum[a]);}
		out_stream.close();
		
		//..........Append-writes info on hitting the target set by minimum_preceding_composites.
		out_stream.open("prime_values", ios::app);
		if(largest_negative_gap >= minimum_preceding_composites)
		{	out_stream << " (" << largest_negative_gap << " preceding composites)";
		}
		else
		{	out_stream << " (UNLIKELY PRIME! Missed the target of having " << minimum_preceding_composites
			           << "+ preceding composites as set by minimum_preceding_composites. This prime has only "
			           << largest_negative_gap << ")";
		}
		out_stream << "\n";
		out_stream.close();
		
		//Resets proximity_sieve[].
		for(int a = 0; a < 1000000000; a++) {proximity_sieve[a] = 0;}
	}
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
