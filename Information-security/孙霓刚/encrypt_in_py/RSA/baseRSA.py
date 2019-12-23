# 加密函数
def encrypt(m, pubkey):
    m = int(m)
    n = int(pubkey[0])
    e = int(pubkey[1])
    c = pow(m, e, n)
    return c


# 解密函数
def decrypt(c, selfkey):
    c = int(c)
    n = int(selfkey[0])
    d = int(selfkey[1])
    m = pow(c, d, n)
    return m
