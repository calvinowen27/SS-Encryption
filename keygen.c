#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include <string.h>
#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

#define OPTIONS "b:i:n:d:s:vh"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "Generates both public and private ss encryption keys, and saves them to their\n"
        "respective files, by default ss.pub and ss.priv\n"
        "\n"
        "USAGE\n"
        "   %s [-b:i:n:d:s:vh] [-b bits] [-i iters] [-n pbfile_path] [-d pvfile_path] [-s seed]\n"
        "\n"
        "OPTIONS\n"
        "   -b bits          sets the minimum number of bits for the public modulus n.\n"
        "   -i iters         sets the number of iterations used in Miller-Rabin primality "
        "testing.\n"
        "   -n pbfile_path   sets the file path where the public key is saved.\n"
        "   -d pvfile_path   sets the file path where the private key is saved.\n"
        "   -s seed          sets the seed for the random number generators used in key creation.\n"
        "   -v               enables verbose output, analytical is printed during key creation.\n"
        "   -h               displays usage help.\n",
        exec);
}

int main(int argc, char **argv) {
    unsigned long int seed = time(NULL);
    char *username = getlogin();

    char *pbfile_path = "ss.pub";
    char *pvfile_path = "ss.priv";

    bool verbose = false;

    uint64_t nbits = 256;
    uint64_t iters = 50;

    mpz_t p, q, n, pq, d;
    mpz_inits(p, q, n, pq, d, NULL);

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': nbits = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'i': iters = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'n':
            pbfile_path = calloc(strlen(optarg), sizeof(char));
            strcpy(pbfile_path, optarg);
            break;
        case 'd':
            pvfile_path = calloc(strlen(optarg), sizeof(char));
            strcpy(pvfile_path, optarg);
            break;
        case 's': seed = (uint64_t) strtoul(optarg, NULL, 10); break;
        case 'v': verbose = true; break;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    randstate_init(seed);

    ss_make_pub(p, q, n, nbits, iters);
    ss_make_priv(d, pq, p, q);

    if (verbose) {
        printf("user = %s\n", username);
        gmp_printf("p  (%lu bits) = %Zu\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q  (%lu bits) = %Zu\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n  (%lu bits) = %Zu\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq (%lu bits) = %Zu\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%lu bits) = %Zu\n", mpz_sizeinbase(d, 2), d);
    }

    FILE *pbfile = fopen(pbfile_path, "w");
    ss_write_pub(n, username, pbfile);

    FILE *pvfile = fopen(pvfile_path, "w");
    ss_write_priv(pq, d, pvfile);

    fclose(pbfile);
    fclose(pvfile);
    mpz_clears(p, q, n, pq, d, NULL);
    randstate_clear();

    return 0;
}
