#include "rsa.hpp"

mpz_class rsa(const mpz_class &data, const mpz_class &exp, const mpz_class &mod) {
    mpz_class res;
    mpz_powm(res.get_mpz_t(), data.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return res;
}

static const int TEST_ROUND = 100;
static const int KEY_LEN = 2048;
static gmp_randclass *_mprand;

static bool _is_prime(const mpz_class &n) {
    if (n == 2 || n == 3 || n == 5 || n == 7 || n == 11) return true;
    if (n < 2 || (n % 2) == 0 || (n % 3) == 0 || (n % 5) == 0 || (n % 7) == 0 || (n % 11) == 0) return false;
    int s = 0;
    mpz_class d = n - 1;
    while ((d & 1) == 0) {s++; d >>= 1;}

    static const mpz_class two = 2;
    for (int i = 0; i < TEST_ROUND; i++) {
        mpz_class a = _mprand->get_z_range(n - 2) + 2;
        mpz_class x, y;
        mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
        for (int j = 0; j < s; j++) {
            mpz_powm(y.get_mpz_t(), x.get_mpz_t(), two.get_mpz_t(), n.get_mpz_t());
            if (y == 1 && x != 1 && x != n - 1)
                return false;
            x = y;
        }
        if (x != 1) return false;
    }
    return true;
}

static mpz_class _prime_gen(size_t bits) {
    mpz_class res = _mprand->get_z_bits(bits - 2);
    ((res <<= 1) |= 1) |= (mpz_class(1) << (bits - 1));
    while (!_is_prime(res)) res += 2;
    return res;
}

static mpz_class _extgcd(const mpz_class &a, const mpz_class &b, mpz_class &x, mpz_class &y) {
    mpz_class d = a;
    if (b != 0) {
        d = _extgcd(b, a % b, y, x);
        y -= (a / b) * x;
    } else {
        x = 1;
        y = 0;
    }
    return d;
}

static mpz_class _mod_inverse(const mpz_class &a, const mpz_class &m) {
    mpz_class x, y;
    _extgcd(a, m, x, y);
    return (m + x % m) % m;
}

rsa_keys rsa_keygen(gmp_randclass &mprand) {
    _mprand = &mprand;

    rsa_keys res;
    res.e = 65537;

    mpz_class p, q;
    p = _prime_gen(KEY_LEN / 2);
    do {
        q = _prime_gen(KEY_LEN / 2);
    } while (p == q);
    res.n = p * q;

    mpz_class phin = (p - 1) * (q - 1);
    res.d = _mod_inverse(res.e, phin);

    return res;
}
