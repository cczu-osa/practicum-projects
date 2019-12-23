import os

from RSA.baseRSA import encrypt, decrypt
from common.fileIO import encode, decode, cut_text


def RSA_encrypt(file1, pubkey_path):
    with open(pubkey_path, 'r') as file_input:
        pubkey = file_input.read().split(' ')
    source_length = len(str(pubkey[0])) - 1
    output_length = len(str(pubkey[0]))

    '''需要被加密的信息转化成数字，长度小于秘钥n的长度，如果信息长度大于n的长度，那么分段进行加密，分段解密即可。'''
    with open(file1, 'rb') as file_in:
        output = cut_text(str(encode(file_in)), source_length)
    c = ""
    for m in output:
        c += str(encrypt(int(m), pubkey)).zfill(output_length)
    with open(file1, 'w') as file_write:
        file_write.write(c)


def RSA_decrypt(file1, selfkey_path):
    '''信息解密'''
    # 读私钥
    with open(selfkey_path, 'r') as file_input:
        selfkey = file_input.read().split(' ')
    source_length = len(str(selfkey[0])) - 1
    output_length = len(str(selfkey[0]))
    # 将加密文件信息读入
    with open(file1, 'r') as file_write:
        c = file_write.read()
        # 分割文件
    output2 = cut_text(c, output_length)

    # 初始化解密文件参数
    result = ""
    len_output2 = len(output2)
    pos = 1
    # 对文件进行分段解密
    for d in output2:
        if pos == len_output2:
            result += str(decrypt(int(d), selfkey))
        else:
            result += str(decrypt(int(d), selfkey)).zfill(source_length)
        pos += 1
    os.remove(file1)
    with open(file1, 'wb') as file_out:
        decode(int(result), file_out)
