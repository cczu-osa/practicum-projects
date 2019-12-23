import random


def AKS(a, n):  # AKS素性检验
    if pow(17 - a, n, n) == pow(17, n, n) - (a % n):
        return 1
    else:
        return 0


def big_rand():  # 生成大素数
    flag = 0
    l, u = 2 ** 16, 2 ** 32
    while not flag:
        n = random.randrange(l, u)
        if any([n % x == 0 for x in [2, 3, 5, 7, 13]]):
            continue
        flag = AKS(2, n)
    return n


def get_e(e_n):  # 与欧拉数e_n互质的e
    flag = 1
    while flag:
        e = random.randrange(e_n)
        if coprime(e, e_n) == (1, 0):
            flag = 0
    return e


def euclid(a, b):  # 扩展欧几里得算法求逆元
    lx = [1, 0, b]
    ly = [0, 1, a]
    while ly[2] != 1:
        if ly[2] == 0:
            return 0
        q = lx[2] // ly[2]
        lt = [lx[i] - ly[i] * q for i in range(3)]
        lx = ly
        ly = lt
    return ly[1] % b


def coprime(a, b):  # 判断互质
    if a < b:
        a, b = b, a
    while b != 0:
        t = a % b
        a = b
        b = t
    return (a, b)


def gen_key():  # 生成公钥和秘钥A
    p = big_rand()
    q = big_rand()
    n = p * q
    e_n = n - p - q + 1
    e = get_e(e_n)
    d = euclid(e, e_n)
    return (n, e), (n, d)


def gen_key_files():
    pubkey, selfkey = gen_key()
    # d不能为负数，如果为负数则重新生成
    while selfkey[1] <= 0:
        pubkey, selfkey = gen_key()
    else:
        with open('pub.key', 'w') as pubkey_file:
            pubkey_file.write(str(pubkey[0]) + " " + str(pubkey[1]))
        with open('self.key', 'w') as selfkey_file:
            selfkey_file.write(str(selfkey[0]) + " " + str(selfkey[1]))
