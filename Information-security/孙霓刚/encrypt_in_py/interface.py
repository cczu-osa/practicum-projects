def gen_key():
    from RSA.gen_RSA_key import gen_key_files
    gen_key_files()


def cbc_encrypt(file1, pubkey):
    from RSA.RSA_CBC import RSA_encrypt
    RSA_encrypt(file1, pubkey)


def cbc_decrypt(file1, selfkey):
    from RSA.RSA_CBC import RSA_decrypt
    RSA_decrypt(file1, selfkey)


def ecb_decrypt(file1, pubkey):
    from RSA.RSA_ECB import RSA_encrypt
    RSA_encrypt(file1, pubkey)


def ecb_decrypt(file1, selfkey):
    from RSA.RSA_ECB import RSA_decrypt
    RSA_decrypt(file1, selfkey)
