/// ProximitySieve - generates prime checked with p<65,536 (50 to 50k digits long.)
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net)



/* Version 1.0.0
Set testing_mode to true, and you won't have to enter randomness  for the seeds.
Applies the sieve of Eratosthenes on an  interval near the n-digit random number
--listing nearby primes, and testing only one and the same  # by all p <= 65536.
This is a sort of pretest and is weak, but if n is the product of  at least some
large primes, AND your protocol expects whole length n, then this sieve is OK.*/

#include <fstream>
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
	
	int prime_length = 500; //Range: 50 to 50000. (Technically lower limit is ~7.)
	
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
	
	//Checks if python3 is present on user's machine.
	system("python3 -c 'with open(\"python3_test\", \"w\") as file: file.write(str(2 + 2) + \"\\n\")'");
	system("clear"                                                                                    );
	in_stream.open("python3_test");
	if(in_stream.fail() == true)
	{	in_stream.close();
		system("clear");
		cout << "How do you not have python3 installed?\n\n"
		     << "Please run \"apt install python3\"\n\n";
		
		return 0;
	}
	in_stream.close();
	remove("python3_test");
	
	//Creates a mod command, onto which the n-digit random number, and prime divisors will be branded, per operation.
	char python_mod_command[50300] = {"python3 -c 'with open(\"mod_results\", \"a\") as file: file.write(str("}; //..........66-char init.
	char command_appendix  [   11] = {") + \"\\n\")'"                                                         }; //..........10-char init.
	
	//Prepares command for automatic branding.
	for(int a = 66; a < 50300; a++) {python_mod_command[a        ] = ' '                ;}
	for(int a =  0; a <    11; a++) {python_mod_command[a + 50289] = command_appendix[a];}
	
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
	cout << "\nGenerating " << prime_length << "-digit prime...\n";
	
	//Fills random_digits[50000] with random digits. Its first n digits will be used.
	unsigned char random_digits[50000] = {0};
	for(int a = 0; a < 50; a++)
	{	srand(user_seeds[a]);
		
		if((user_seeds[a] % 2) == 0)
		{	//Constructively fills random_digits[] LEFT to right based on seeds.
			for(int b = 0; b < 50000; b++)
			{	random_digits[b] += (rand() % 10);
				random_digits[b] %= 10;
			}
		}
		else
		{	//Constructively fills random_digits[] RIGHT to left based on seeds.
			for(int b = 49999; b >= 0; b--)
			{	random_digits[b] += (rand() % 10);
				random_digits[b] %= 10;
			}
		}
	}
	
	//Ensures first digit is not zero.
	if(random_digits[0] == 0) {random_digits[0]++;}
	
	//Boolean sieve of Eratosthenes. Zeros are mapped to prime elements. Laughably, bool[] & char[] both consume 1 Byte.
	bool sieve[65536] = {1, 1};
	for(int prime = 2; prime < 256; prime++) //..........256 is sqrt(65,536).
	{	for(; sieve[prime] == 1;) {prime++;} //..........Moves up the list if number already marked off.
		for(int a = prime + prime; a < 65536; a += prime) {sieve[a] = 1;} //..........Marks multiples (composites.)
	}
	
	//Brands python_mod_command[] with the n-digit random number (unchanging.)
	for(int a = 0; a < prime_length; a++) {python_mod_command[a + 66] = (random_digits[a] + 48);}
	python_mod_command[50100] = '%';
	
	//Brands python_mod_command[] with 6,542 prime divisors, one-at-a-time, and executes mod operation, each result appended to file.
	for(int a = 0; a < 65536; a++)
	{	if(sieve[a] == 0)
		{	//..........Brands.
			long long branding_arithmetic = 1000000000000000000;
			branding_arithmetic += a;
			long long temp_branding_arithmetic = branding_arithmetic;
			for(int b = 50200; b > 50183; b--) //..........Supports longer divisors!
			{	python_mod_command[b] = (temp_branding_arithmetic % 10);
				python_mod_command[b] += 48;
				
				temp_branding_arithmetic /= 10;
			}
			
			//..........Removes zeros in front of divisor.
			for(int b = 50180;; b++)
			{	if(python_mod_command[b] == '0') {python_mod_command[b] = ' ';}
				if(python_mod_command[b]  >  48) {break                      ;}
			}
			
			system(python_mod_command);
		}
	}
	
	//Fills remainders[] with mod results from file "mod_results".
	long long remainders[6542] = {0};
	char garbage_byte;
	in_stream.open("mod_results");
	for(int a = 0; a < 6542; a++)
	{	long long digits_of_individual_remainder[17];
		for(int b = 0; b < 17; b++) {digits_of_individual_remainder[b] = -1;}
		in_stream.get(garbage_byte);
		for(int b = 0; garbage_byte != '\n'; b++)
		{	digits_of_individual_remainder[b] = garbage_byte;
			digits_of_individual_remainder[b] -= 48;
			
			in_stream.get(garbage_byte);
		}
		
		for(int multiplier = 1, b = 16; b >= 0; b--)
		{	if(digits_of_individual_remainder[b] != -1)
			{	digits_of_individual_remainder[b] *= multiplier;
				remainders[a] += digits_of_individual_remainder[b];
				
				multiplier *= 10;
			}
		}
	}
	in_stream.close();
	
	//Applies proximity sieve. Element 0 represents location of the given random
	//candidate prime, on the number line. Zeros are mapped to prime elements.
	bool proximity_sieve[2000000] = {0};
	long long remainder_index = 0;
	for(int a = 0; a < 65536; a++)
	{	if(sieve[a] == 0)
		{	long long natural_prime_position = (a - remainders[remainder_index]);
			proximity_sieve[natural_prime_position] = 1;
			
			for(; natural_prime_position < 1934460;)
			{	natural_prime_position += a;
				proximity_sieve[natural_prime_position] = 1;
			}
			
			remainder_index++;
		}
	}
	
	//Adjusts python_mod_command[].
	python_mod_command[50100] = '+';
	char different_file_name[12] = {"prime_value"};
	for(int a = 23; a < 34; a++) {python_mod_command[a] = different_file_name[a - 23];}
	
	//Finds prime element.
	int prime_element;
	for(int a = 900000; a < 1500000; a++)
	{	if(proximity_sieve[a] == 0) {prime_element = a; break;}
	}
	
	//Brands prime element on python_mod_command[].
	long long prime_branding = 1000000000000000000;
	prime_branding += prime_element;
	for(int b = 50200; b > 50183; b--) //..........Supports longer additives!
	{	python_mod_command[b] = (prime_branding % 10);
		python_mod_command[b] += 48;
		
		prime_branding /= 10;
	}
	
	//Removes zeros in front of additive.
	for(int b = 50180;; b++)
	{	if(python_mod_command[b] == '0') {python_mod_command[b] = ' ';}
		if(python_mod_command[b]  >  48) {break                      ;}
	}
	
	//Append-writes prime to file.
	system(python_mod_command);
	
	//Overwrites RAM of array char python_mod_command[50300].
	for(int a = 0; a < 50300; a++) {python_mod_command[a] = '\0'; python_mod_command[a] = -1;} //Binary: 00000000, 111111111.
	
	//Overwrites RAM of array unsigned int user_seeds[50].
	for(int a = 0; a < 50; a++) {user_seeds[a] = 0; user_seeds[a] = 12345678; user_seeds[a] = 87654321; user_seeds[a] = 99999999;}
	
	//Overwrites RAM of array unsigned char random_digits[50000].
	for(int a = 0; a < 50000; a++) {random_digits[a] = 0; random_digits[a] = 255;} //Binary: 00000000, 111111111.
	
	//Overwrites RAM of array long long remainders[6542].
	for(int a = 0; a < 6542; a++) {remainders[a] = 0; remainders[a] = -9223372036854775807; remainders[a] = 9223372036854775807;}
	
	//Overwrites RAM of array bool proximity_sieve[2000000].
	for(int a = 0; a < 2000000; a++) {proximity_sieve[a] = 0; proximity_sieve[a] = 1;}
	
	//Overwrites file "mod_results".
	out_stream.open("mod_results");
	for(int a = 0; a < 100000; a++) {out_stream << '\0';} //Binary: 00000000.
	out_stream.close();
	
	out_stream.open("mod_results");
	for(int a = 0; a < 100000; a++) {out_stream.put(-1);} //Binary: 11111111.
	out_stream.close();
	
	remove("mod_results");
	
	cout << "\nDone!\n\n\n";
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
