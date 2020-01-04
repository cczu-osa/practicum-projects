# AES-128-CBC

c implement

## Usage

1. call `aes128cbc_chiperlen(plain_text_len)` to calculate chiper length.
2. alloc enough space and call `aes128cbc_encrypt(...)` to encrypt.
3. alloc enough space and call `aes128cbc_decrypt(...)` to decrypt.