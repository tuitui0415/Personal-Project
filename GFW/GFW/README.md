# The Great Firewall of Santa Cruz Bloom Filters, Binary Trees and Hash Tables

This makes a word filter, filt the word king don't want or change the oldspeak to newspeak.

## Building

Use Makefile to build progrmas:

to make banhammer, using 

```
$ make 
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
$ ./banhammer
```
• -h prints out the program usage. Refer to the reference program in the resources repository for what to print.

• -t size specifies that the hash table will have size entries(thedefaultwillbe216).

• -f size specifies that the Bloom filter will have size entries(the default will be 2^20).

• -s will enable the printing of statistics to stdout.


------------------------------------------------------------------
