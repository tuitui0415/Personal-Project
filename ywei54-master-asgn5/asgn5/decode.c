#include "io.h"
#include "node.h"
#include "pq.h"
#include "defines.h"
#include "huffman.h"
#include "stack.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define OPTIONS "i:o:v"

int main(int argc, char **argv) {
    bool printV = false;
    int opt = 0;
    int infile = 0;
    int outfile = 1;
    //char *inputfile = NULL;
    //char *outputfile =NULL;
    int len = 0;
    //int num_node=0;
    //int index=0;
    //int count=0;
    opt = getopt(argc, argv, OPTIONS);
    while (opt != -1) {

        switch (opt) {
        case 'i':
            infile = open(optarg, O_RDONLY);
            if (infile <= 0) {
                fprintf(stderr, "Erro:failed to open infile.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT);
            if (outfile == 0) {
                fprintf(stderr, "Erro:failed to open outfile.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'v': printV = true; break;
        case 'h':
            printf("help messag!\n");
            return EXIT_SUCCESS;
            break;
        default:
            printf("help messag!\n");
            fprintf(stderr, "help messag! no arguments\n");
            return EXIT_FAILURE;
        }
        opt = getopt(argc, argv, OPTIONS);
    }

    /* write the head file to the file
     *
     */

    uint32_t *buf_magic = (uint32_t *) malloc(sizeof(uint32_t));
    //memset(&head,0,sizeof(head));
    //uint32_t check_magic = 0;
    /*
    * set magic number
    */
    len = read(infile, buf_magic, 4);
    if (len > 0) {

        printf("len = %d,\nread magic successful\n", len);
    }
    if (*buf_magic == MAGIC) {
        printf("match successful\n");
    }
    uint16_t *buf_permission = (uint16_t *) malloc(sizeof(uint16_t));
    /*set permission number
 */
    if (read(infile, buf_permission, 2)) {
        printf("read permission successful\n");
    }
    struct stat srcstat;
    srcstat.st_mode = *buf_permission;
    fchmod(outfile, srcstat.st_mode);
    //fchmod(outfile,buf_permission);

    uint16_t *buf_treesize = (uint16_t *) malloc(sizeof(uint16_t));

    if (read(infile, buf_treesize, 2)) {
        printf("read treesize successful\n");
    }
    /*
 * set filesize using  header
 */
    uint64_t *buf_filesize = (uint64_t *) malloc(sizeof(uint64_t));
    if (read(infile, buf_filesize, 8)) {
        printf("read filesize successful\n");
    }
    /*
     * get the root of the huffmantree
     */
    uint8_t tree[*buf_treesize];
    read(infile, tree, *buf_treesize);
    Node *root = rebuild_tree(*buf_treesize, tree);

    uint8_t *bit_read = (uint8_t *) malloc(sizeof(uint8_t));
    Node *current_node = root;
    /*
     *traverse the whole file, write use the root to find the symbol
     and write to the outfile
     */
    int bianliang = 0;
    while (read_bit(infile, bit_read)) {
        bianliang = *bit_read;
        if (bianliang == 1) {
            current_node = current_node->right;
        } else {
            current_node = current_node->left;
        }
        if (current_node->left == 0 && current_node->right == 0) {
            uint8_t *fuhao = (uint8_t *) malloc(sizeof(uint8_t));
            fuhao[0] = current_node->symbol;
            write(outfile, fuhao, 1);
            current_node = root;
        }
    }
    /*    struct stat statbuf;
    fstat_code(outfile,&statbuf);
    outsize  = statbu.st_size;

    double saveSpace = 100 *(1-head.file_size*1.0/outsize);
    *
     * close files
     *
    if(printV){
    printf("space saving:%f\n , saveSpace");
    }*/

    close(infile);
    close(outfile);
    free(buf_magic);
    free(buf_permission);
    free(buf_treesize);
    free(buf_filesize);
    free(bit_read);
}
