Describe:
These include two programs, one is for decode files and one is for encode files. We use these two files to compress file and restore the file to the origin.
To build the program:
We build different ADTs to implement functions like write/read bits/bytes and build huffman trees for create unique code for each symbol. 
To run the program:
Under the environment of ubuntu, we need to put Makefile into the files, using command "make" to generate .o objects and executable programs. Use "./encode" to run encode program "-i"for specifi inputfile "-o"for specific output file.Use "./decode" to run decode program "-i"for specifi inputfile "-o"for specific output file. You can seperately make encode & decode by using "make encode" & "make decode".
Command-line options
Use these commands to implement specific function
-h help message
-v             statistics data for compression ratio
-h             Program usage and help
-i infile      specify an inputfile
-o outfile     specify an outputfile
