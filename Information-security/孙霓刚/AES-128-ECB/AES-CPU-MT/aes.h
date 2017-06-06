#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <memory>

// #define DEBUG

namespace aes_cpu {
	using byte = uint8_t;
	using word = uint16_t;
	using dword = uint32_t;

	class aes {
	private:
		const static byte round_const[];
		const static byte sbox[];
		const static byte inverse_sbox[];
#ifdef DEBUG
		static void print_key(uint8_t *data) {
			int i;
			for (i = 0; i != 16; ++i) {
				printf("%02x", data[i]);
			}
			printf("\n\n");
		}
#endif
		static void sub_bytes(byte *input);
		static void inverse_sub_bytes(byte *input);

		static void shift_rows(byte *input);
		static void inverse_shift_rows(byte *input);

		static void mix_columns(byte *input);
		static void inverse_mix_columns(byte *input);

		static byte mul2(byte n);
	public:

		static byte * key_schedule(const byte *input_key);
		static void aes_128_single_block(const byte *input, byte * const round_keys, byte *output);
		static void aes_decrypt_single_block(const byte *input, byte * const round_keys, byte *output);
	};
}
