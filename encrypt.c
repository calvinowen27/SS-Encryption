#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include "ss.h"

#define OPTIONS "i:o:n:vh"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "Encrypts a message from the specified input file and outputs the encrypted\n"
        "message to specified output file.\n"
        "\n"
        "USAGE\n"
        "   %s [-i:o:n:vh] [-i infile_path] [-o outfile_path] [-n pbfile_path]\n"
        "\n"
        "OPTIONS\n"
        "   -i infile_path   specifies the input file to encrypt.\n"
        "   -o outfile_path  specifies the output file for the encrypted message.\n"
        "   -n pbfile_path   sets the file path where the public key is saved.\n"
        "   -v               enables verbose output, analytical is printed during encryption.\n"
        "   -h               displays usage help.\n",
        exec);
}

int main(int argc, char **argv) {
    mpz_t n;
    mpz_inits(n, NULL);

    char *username = malloc(100);

    FILE *pbfile = fopen("ss.pub", "r");
    FILE *infile = stdin;
    FILE *outfile = stdout;

    bool verbose = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pbfile = fopen(optarg, "r"); break;
        case 'v': verbose = true; break;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    if (pbfile == NULL) {
        fprintf(stderr, "File ss.pub does not exist!\n");
        return 1;
    }

    ss_read_pub(n, username, pbfile);

    if (verbose) {
        printf("user = %s\n", username);
        gmp_printf("n (%lu bits) = %Zu\n", mpz_sizeinbase(n, 2), n);
    }

    ss_encrypt_file(infile, outfile, n);

    free(username);
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, NULL);

    return 0;
}
