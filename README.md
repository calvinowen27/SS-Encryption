# SS Encryption

## Short Description:
	This program has 3 separate parts. Key generation, Encryption, and Decryption.
	Each is separated into their own c file, keygen.c, encryption.c, and decryption.c.
	The keygen program generates 2 keys, a public and a private key, using the ss
	algorithms and outputs them to the specified output file. The encrypt program
	uses the public key to encrypt a message that has been inputted and outputs the 
	encrypted message to the specified output file. The decrypt program uses the private
	key to decrypt a message from a specified input file and output the decrypted message
	to the specified output file.

## Build:
	To build, run '$ make' or '$ make all' on the command line within the ss encryption
	directory. This will create 3 executable files called 'keygen', 'encrypt',
	and 'decrypt' which can be run using optional parameters. Additionally, you can
	run '$ make keygen', '$ make encrypt', or '$ make decrypt' in order to create
	each executable individually.

## Cleaning:
	To clean the directory after building simply type '$ make clean' to
	remove the executables 'keygen', 'encrypt', and 'decrypt', along with
	the .o files from your directory

## Running keygen:
	Typing '$ ./keygen', followed by one or more of these valid arguments
	(-b, -i, -n, -d, -s, -v, -h) will run the key generation program with the given options.
	
	OPTIONS
           -b bits          sets the minimum number of bits for the public modulus n.
           -i iters         sets the number of iterations used in Miller-Rabin primality testing.
           -n pbfile_path   sets the file path where the public key is saved.
           -d pvfile_path   sets the file path where the private key is saved.
           -s seed          sets the seed for the random number generators used in key creation.
           -v               enables verbose output, analytical is printed during key creation.
           -h               displays usage help.

## Running encrypt:
	Typing '$ ./encrypt', followed by one or more of these valid arguments
	(-i, -o, -n, -v, -h) will run the encryption program with the given options.
	
	OPTIONS
    	   -i infile_path   specifies the input file to encrypt.
           -o outfile_path  specifies the output file for the encrypted message.
           -n pbfile_path   sets the file path where the public key is saved.
           -v               enables verbose output, analytical is printed during encryption.
           -h               displays usage help.

## Running decrypt:
	Typing '$ ./decrypt', followed by one or more of these valid arguments
	(-i, -o, -n, -v, -h) will run the decryption program with the given options.
	
	OPTIONS
           -i infile_path   specifies the input file to encrypt.
           -o outfile_path  specifies the output file for the encrypted message.
           -n pvfile_path   sets the file path where the private key is saved.
           -v               enables verbose output, analytical is printed during decryption.
           -h               displays usage help.
