#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t Byte;

size_t aes128cbc_chiperlen(size_t plainlen);

// needs a random function to generate IV
// write exactly aes128cbc_chiperlen(len) bytes
void aes128cbc_encrypt(Byte *chiper, const Byte *plain, size_t len, const Byte *key, Byte(*randgen)());

// write len-16 bytes, return plain text real length, or SIZE_MAX on error
size_t aes128cbc_decrypt(Byte *plain, const Byte *chiper, size_t len, const Byte *key);

#endif // AES_H
