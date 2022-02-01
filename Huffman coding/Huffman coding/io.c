#include "code.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
static uint8_t buffer[BLOCK];
static int INDEX = 0;
static int end = -1;

/*
bool read_byte(int infile, uint8_t *x){
    static uint32_t index_rb=0;
    if (index_rb == 0){
        int bytes = read(infile, buffer, BLOCK);
        if(bytes<BLOCK){
            end = bytes +1;
        }
    }
    x = buffer[index_rb];
    index_rb+=1;
    if (index_rb == BLOCK){
        index_rb = 0;
    }
    return index_rb !=end;
}
*/

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    /*
    uint32_t INDEX_wb=0;
    uint32_t len=0;
    int leftsize= 0;
    int bufsize=strlen(buf);
    leftsize=bufsize;
    if(bufsize<nbytes){
        read(infile,buf,bufsize);
    }
    else{
        do{
        //write(outfile,buf,nbytes);
            len=read(infile,buf+INDEX_wb,nbytes);
            leftsize=leftsize-len;
            INDEX_wb += len;
        }while(leftsize>=nbytes);
        INDEX_wb += read(infile,buf+INDEX_wb,leftsize);
        }
    return INDEX_wb;
    */
    uint64_t readed_length = 0;
    uint64_t len = 0;
    do {
        len = read(infile, buf + readed_length, nbytes);
        readed_length += len;
    } while (len > 0);

    return readed_length;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    uint32_t INDEX_wb = 0;
    //uint32_t len = 0;
    //int leftsize = 0;
    //int bufsize = 0;
    /*do {
        bufsize++;
    } while (buf[bufsize] != '\0');
*/
    //int bufsize=sizeof(buf);
    // leftsize = bufsize;
    if (nbytes < 512) {
        write(outfile, buf, nbytes);
    } else {
        //do {
        //write(outfile,buf,nbytes);
        //len = write(outfile, buf + INDEX_wb, nbytes);
        //leftsize = leftsize - len;
        //INDEX_wb += write;
        //leftsize >= nbytes);
        INDEX_wb += write(outfile, buf + INDEX_wb, nbytes);
    }
    return INDEX_wb;
}

bool read_bit(int infile, uint8_t *bit) {

    if (INDEX == 0) {
        int bytes = read(infile, buffer, BLOCK);
        if (bytes == 0) {
            return false;
        }
        if (bytes < BLOCK) {
            end = bytes * 8 + 1;
        }
    }
    *bit = (buffer[INDEX / 8] >> (INDEX % 8)) & 0x1;
    INDEX += 1;
    if (INDEX == BLOCK * 8) {
        INDEX = 0;
    }
    if (INDEX == end - 1) {
        INDEX = 0;
    }
    return INDEX != end;
}

void write_code(int outfile, Code *c) {

    //static int end = -1;

    for (uint32_t i = 0; i < code_size(c); i += 1) {
        // Code *bit = code_get_bit(c,i);
        if (code_get_bit(c, i)) {
            buffer[INDEX / 8] |= (0x1 << INDEX % 8);
            //code_set_bit(c,INDEX);
        } else {
            buffer[INDEX / 8] &= ~(0x1 << INDEX % 8);
            //code_clr_bit(c,INDEX);
        }
        INDEX += 1;
        //end=INDEX+1;
        if (INDEX == BLOCK) {
            write_bytes(outfile, buffer, BLOCK / 8);
            INDEX = 0;
        }
    }
}

void flush_codes(int outfile) {
    if (INDEX > 0) {
        write_bytes(outfile, buffer, (INDEX - 1) / 8 + 1);
    }
}
