#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <gmp.h>
#include "ss.h"

#define OPTIONS "i:o:n:vh"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "Decrypts a message from the specified input file and outputs the decrypted\n"
        "message to specified output file.\n"
        "\n"
        "USAGE\n"
        "   %s [-i:o:n:vh] [-i infile_path] [-o outfile_path] [-n pvfile_path]\n"
        "\n"
        "OPTIONS\n"
        "   -i infile_path   specifies the input file to encrypt.\n"
        "   -o outfile_path  specifies the output file for the encrypted message.\n"
        "   -n pvfile_path   sets the file path where the private key is saved.\n"
        "   -v               enables verbose output, analytical is printed during decryption.\n"
        "   -h               displays usage help.\n",
        exec);
}

int main(int argc, char **argv) {
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    FILE *pvfile = fopen("ss.priv", "r");
    FILE *infile = stdin;
    FILE *outfile = stdout;

    bool verbose = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pvfile = fopen(optarg, "r"); break;
        case 'v': verbose = true; break;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    if (pvfile == NULL) {
        fprintf(stderr, "File ss.priv does not exist!\n");
        return 1;
    }

    ss_read_priv(pq, d, pvfile);

    if (verbose) {
        gmp_printf("pq (%lu bits) = %Zu\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%lu bits) = %Zu\n", mpz_sizeinbase(d, 2), d);
    }

    ss_decrypt_file(infile, outfile, pq, d);

    fclose(pvfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(pq, d, NULL);

    return 0;
}
