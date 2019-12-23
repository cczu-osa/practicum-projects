import os

from RSA.baseRSA import encrypt, decrypt
from common.fileIO import encode, decode, cut_text


def RSA_encrypt(file1, pubkey_path):
    with open(pubkey_path, 'r') as file_input:
        pubkey = file_input.read().split(' ')
    source_length = len(str(pubkey[0]))
    output_length = len(str(pubkey[0]))

    '''需要被加密的信息转化成数字，长度小于秘钥n的长度，如果信息长度大于n的长度，那么分段进行加密，分段解密即可。'''
    with open(file1, 'rb') as file_in:
        output = cut_text(str(encode(file_in)), source_length)
    c = []
    pos = 0
    for m in output:
        m = int(m)
        if pos == 0 or pos == len(output) - 1:
            c.append(str(encrypt(m, pubkey)).zfill(output_length))
        else:
            c.append(str((encrypt(m ^ int(c[pos - 1]), pubkey))).zfill(output_length))
        pos += 1
    print(c)
    c = "".join(c)
    os.remove(file1)
    with open(file1, 'w') as file_write:
        file_write.write(c)


def RSA_decrypt(file1, selfkey_path):
    '''信息解密'''
    # 读私钥
    with open(selfkey_path, 'r') as file_input:
        selfkey = file_input.read().split(' ')
    source_length = len(str(selfkey[0]))
    output_length = len(str(selfkey[0]))
    # 将加密文件信息读入
    with open(file1, 'r') as file_write:
        c = file_write.read()
        # 分割文件
    output = cut_text(c, output_length)
    # 初始化解密文件参数
    result = []
    len_output = len(output)
    pos = 0
    # 对文件进行分段解密
    for d in output:
        if pos == 0:
            result.append(str(decrypt(int(d), selfkey)).zfill(source_length))
        elif pos == len_output - 1:
            result.append(str(decrypt(int(d), selfkey)))
        elif len_output == 1:
            result.append(str(decrypt(int(d), selfkey)))
        else:
            result.append(str(decrypt(int(d), selfkey) ^ int(output[pos - 1])).zfill(source_length))
        pos += 1
    print(result)
    result = "".join(result)
    os.remove(file1)
    with open(file1, 'wb') as file_out:
        decode(int(result), file_out)
