#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include "sha2.hpp"

int main(int argc, char **argv) {

    if (argc != 3) {
        std::cerr << argv[0] << "%s [filename] [0 <= Rounds <= 63]" << std::endl;
        exit(EXIT_FAILURE);
    }
    const char *infile = argv[1];
    const int rounds = atoi(argv[2]);

    struct stat statbuf;
    if (stat(infile, &statbuf)) {
        perror(infile);
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(infile, "r");
    if (!f) {
        perror("data");
        exit(EXIT_FAILURE);
    }

    uint8_t *buffer = (uint8_t*) malloc(statbuf.st_size);
    if (!buffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    size_t r = fread(buffer, 1, statbuf.st_size, f);
    if (r != statbuf.st_size) {
        perror("short read");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "SHA2 over %ld bytes of %s\n", r, infile);

    uint8_t dgst1[32], dgst2[32];
    for(int iter = 0; iter <= rounds; ++iter) {
        
        TSha256 ctx(buffer, r, iter);
        ctx.Сalc(dgst1);

        if(buffer[statbuf.st_size - 1] % 2 == 0) {
            ++buffer[statbuf.st_size - 1];
        }
        else {
            --buffer[statbuf.st_size - 1];
        }

        TSha256 cty(buffer, r, iter);
        cty.Сalc(dgst2);

        /*
        fprintf(stderr, "Before: ");
        for (int i = 0; i < 32; i++) {
            fprintf(stderr, "%02x", dgst1[i]);
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "After: ");
        for (int i = 0; i < 32; i++) {
            fprintf(stderr, "%02x", dgst2[i]);
        }
        */

        size_t diffbits = 0;
        for (int i = 0; i < 32; i++) {

            uint8_t mask = 1;
            for(int j = 0; j < 8; j++) {
                if((mask & dgst1[i]) != (mask & dgst2[i])) {
                    ++diffbits;
                }
                mask = mask << 1;
            }
        }
        std::cout << "| Rounds = " << iter << " | Diff = " << diffbits << " |" << std::endl;
    }
    
    free(buffer);
    return 0;
}
