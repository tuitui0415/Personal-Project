# Assignment 6 - Public Key Cryptography

This makes a key generator, a encryptor and a decryptor which using RSA algorithm to encrypt files and decrypt files.

## Building

Use Makefile to build progrmas:

to make all the programs, using 

```
$ make 
```

to make only keygen:

```
$ make keygen
```

to make only decrypt:

```
$ make decrypt
```

to make only encrypt

```
$ make encrypt
```

to remove all files that are compiler generated:

```
$ make clean
```

to format all source code ,including the header files:

```
$ make format
```
## Running

Run the program with:

```
$ ./keygen 
```
• -b: specifies the minimum bits needed for the public modulus n.

• -i: specifies the number of Miller-Rabin iterations for testing primes (default: 50).

• -n pbfile: specifies the public key file (default: rsa.pub).

• -d pvfile: specifies the private key file (default: rsa.priv).

• -s: specifies the random seed for the random state initialization (default: the seconds since the UNIX epoch, given by time(NULL)).

• -v: enables verbose output.

• -h: displays program synopsis and usage.

------------------------------------------------------------------
```
$ ./encrypt
```

• -i: specifies the input file to encrypt (default: stdin).

• -o: specifies the output file to encrypt (default: stdout).

• -n: specifies the file containing the public key (default: rsa.pub).

• -v: enables verbose output.

• -h: displays program synopsis and usage.

------------------------------------------------------------------
```
$ ./decrypt
```

• -i: specifies the input file to decrypt (default: stdin).

• -o: specifies the output file to decrypt (default: stdout).

• -n: specifies the file containing the private key (default: rsa.priv).

• -v: enables verbose output.

• -h: displays program synopsis and usage.







