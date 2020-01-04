#ifndef RSA_H
#define RSA_H

#include <gmpxx.h>

mpz_class rsa(const mpz_class &data, const mpz_class &exp, const mpz_class &mod);

struct rsa_keys {
    mpz_class n, e, d; // public key: (n, e); private key: (n, d)
};

rsa_keys rsa_keygen(gmp_randclass &mprand);

#endif // RSA_H
