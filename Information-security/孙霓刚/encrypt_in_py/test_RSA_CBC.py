from RSA import RSA_CBC,gen_RSA_key

'''生成密钥'''
# gen_RSA_key.gen_key_files()

print("文件原文：————————————————————————")
with open('1.txt', 'r') as f:
    print(f.read())

RSA_CBC.RSA_encrypt('1.txt', 'pub.key')

print("加密后文件保存密文：————————————————————————")
with open('1.txt', 'r') as f:
    print(f.read())

RSA_CBC.RSA_decrypt('1.txt', 'self.key')

print("解密后的文件内容：————————————————————————")
with open('1.txt', 'r') as f:
    print(f.read())
