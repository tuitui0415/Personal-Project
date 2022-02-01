#include "io.h"
#include "node.h"
#include "pq.h"
#include "defines.h"
#include "huffman.h"
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define OPTIONS "hi:o:v"
//static int writebuf[BLOCK];
//static int wp=0;
int main(int argc, char **argv) {
    int opt = 0;
    int infile = 0;
    int outfile = 1;
    //char *inputfile = NULL;
    //char *outputfile =NULL;
    int len = 0;
    //int num_node=0;
    int index = 0;
    int count = 0;
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
        case 'v': break;
        case 'h':
            printf("help messag!\n");
            return EXIT_SUCCESS;
            break;
        default:
            printf("help messag!\n");
            fprintf(stderr, "help messag! no arguments\n");
            break;
        }
        opt = getopt(argc, argv, OPTIONS);
    }
    /*   FILE *fp = fopen("a.txt","r");
    if (!fp)
    {return -1;
    }
    fseek(fp,0L,SEEK_END);
    int sizeofinfile= ftell(fp);
    fclose(fp);*/

    /*   infile = open("a.txt",O_RDONLY);


    if (infile == -1){
        printf("cannot open file!\n");
        return 1;

    }
    */
    //len = filelength(infile);
    //int sizeofinfile = 4096;
    //char *buf=(char*)malloc (sizeofinfile* sizeof(char ));

    int sizeofile = 0;
    uint8_t cc;
    while (read(infile, &cc, 1) > 0) {
        sizeofile += 1;
    }
    lseek(infile, 0, SEEK_SET);

    uint8_t *buf = (uint8_t *) malloc(sizeofile * sizeof(uint8_t));
    memset(buf, 0, sizeof(*buf));
    len = read_bytes(infile, buf, 4096);

    /*for(int i=0;i<len-10;i++){
        printf("%c",buf[i]);
    }
    printf("\n len=%d \n",len);
    */
    lseek(infile, 0, SEEK_SET);

    /* 
    int toutfile = open("b.txt",O_WRONLY);

    if (toutfile == -1){
        printf("cannot open testfile!\n");
        return 1;

    }
    write_bytes(toutfile,buf,4096);
    close(toutfile);
*/

    uint32_t Tsymbol_array[ALPHABET] = { 0 };
    int frequency_array[ALPHABET] = { 0 };

    for (int i = 0; i < len; i++) {
        if (i == 0) {
            Tsymbol_array[0] = buf[0];
            count++;
            for (int j = 1; j < len; j++) {

                // int cmp = strcmp(buf[0],buf[j]);
                if (buf[0] == buf[j]) {

                    count += 1;
                }
            }
            index += 1;
            frequency_array[0] = count;

        } else {
            //count = 0;
            for (int k = 0; k < i; k++) {
                if (buf[i] == buf[k])
                    goto L1;
                //index++;
            }
            index++;
            Tsymbol_array[index - 1] = buf[i];
            count = 1;
            for (int j = i + 1; j < len; j++) {
                if (buf[i] == buf[j]) {
                    count = count + 1;
                }
            }

            frequency_array[index - 1] = count;
        L1:;
        }
    }

    uint64_t hist[ALPHABET] = { 0 };
    for (int x = 0; x < index; x++) {
        hist[Tsymbol_array[x]] = frequency_array[x];
    }

    free(buf);

    /*
     * for (int l = 0; l < index; l++) {
        printf("frequency= %ld , symbol = %02x,   %c\n", hist[Tsymbol_array[l]], Tsymbol_array[l],
            Tsymbol_array[l]);
    }
	*/
    Node *tree = build_tree(hist);
    //printf("tree frequency=%d, tree symbol=%c\n", tree->right->right->left->frequency,tree->right->right->left->symbol);
    Code code_table[ALPHABET] = { 0 };
    build_codes(tree, code_table);

    //printf("%ld",code_table['b']);

    Header head;
    head.magic = MAGIC;
    struct stat srcstats;
    struct stat dststats;
    fstat(infile, &srcstats);
    fstat(outfile, &dststats);
    fchmod(outfile, srcstats.st_mode);
    head.permissions = dststats.st_mode;
    head.tree_size = (index * 3 - 1);
    head.file_size = srcstats.st_size;
    //outfile=open("out.txt",O_WRONLY);
    write(outfile, &head, sizeof(head));

    if (outfile == -1) {
        printf("cannot open out.txt!\n");
        return 1;
    }
    dump_tree(outfile, tree);
    uint8_t *outbuf = (uint8_t *) malloc(srcstats.st_size * sizeof(uint8_t));
    len = read_bytes(infile, outbuf, 4096);
    for (uint64_t i = 0; i < head.file_size; i++) {

        uint8_t temp = outbuf[i];
        write_code(outfile, &code_table[temp]);
    }
    flush_codes(outfile);

    close(infile);
    close(outfile);

    //free(buf);
    //free (outbuf);
}
