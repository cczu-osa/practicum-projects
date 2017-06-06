#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

cudaError_t aes_on_cuda(const uint8_t *input, const uint8_t *keys, uint8_t *output, const size_t size, bool isEncrypt);

int hexToDec(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	else if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	}
	return -1;
}

inline size_t compute_block_size(size_t data_size) {
	return (data_size / 16 + 1) * 16;
}

size_t pkcs7_padding(uint8_t **data, size_t data_size) {
	// Compute the new text length
	size_t new_size = compute_block_size(data_size);
	// And recoad the diff between them.
	uint8_t diff = new_size - data_size;
	// Reallocate memory
	*data = (uint8_t *)realloc(*data, new_size);
	for (size_t index = data_size; index != new_size; ++index) {
		// And filling the extra uint8_ts with diff
		(*data)[index] = diff;
	}
	return new_size;
}

void encrypt_entry(std::string input_file, std::string output_file, uint8_t key[]) {
	// Open the file. if any errors were found, print error and quit.
	FILE *fp = fopen(input_file.c_str(), "rb+");
	if (fp == NULL) {
		printf("Error on opening file: %s.\n", input_file.c_str());
	}

	// Find out the length of file
	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Declare two buffer pointer and allocate memory for plaintext
	uint8_t *data_in, *data_out;
	data_in = (uint8_t *)malloc(file_size * sizeof(uint8_t));

	// Read all file content to RAM.
	fread(data_in, file_size, 1, fp);

	// use PKCS#7 padding mode
	size_t full_block_size = pkcs7_padding(&data_in, file_size);

	// Allocate memory for chipertext, as original plaintext has been padded, the size is not the same.
	data_out = (uint8_t *)malloc(full_block_size * sizeof(uint8_t));

	// Record when it begin.
	clock_t begin = clock();

	// Copy memory and launch CUDA kernel
	cudaError_t cudaStatus = aes_on_cuda(data_in, key, data_out, full_block_size, true);
	if (cudaStatus != cudaSuccess) {
		printf("Something bad happened!\n");
	}


	// Record when it come to the end.
	clock_t end = clock();

	// And print the duration
	printf("Time cost: %ums\n", end - begin);

	// Finally, write chipertext to filesystem.
	FILE *out_fp = fopen(output_file.c_str(), "wb+");
	if (!out_fp) {
		// Can't open file.
		puts("fuck you");
	}
	else {
		fwrite(data_out, full_block_size, 1, out_fp);
	}

	// Release all using memory
	free(data_in);
	free(data_out);

	// Close all opened files.
	fclose(fp);
	fclose(out_fp);
}

void decrypt_entry(std::string input_file, std::string output_file, uint8_t key[]) {
	puts("Mode: Decrypt");
	// Open the file. if any errors were found, print error and quit.
	FILE *fp = fopen(input_file.c_str(), "rb+");
	if (fp == NULL) {
		printf("Error on opening file: %s.\n", input_file.c_str());
	}

	// Find out the length of file
	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Declare two buffer pointer and allocate memory for plaintext
	uint8_t *data_in, *data_out;
	data_in = (uint8_t *)malloc(file_size * sizeof(uint8_t));

	// Read all file content to RAM.
	fread(data_in, file_size, 1, fp);

	// Allocate memory for chipertext, as original plaintext has been padded, the size is not the same.
	data_out = (uint8_t *)malloc(file_size * sizeof(uint8_t));

	// Record when it begin.
	clock_t begin = clock();

	// Copy memory and launch CUDA kernel
	cudaError_t cudaStatus = aes_on_cuda(data_in, key, data_out, file_size, false);
	if (cudaStatus != cudaSuccess) {
		printf("Something bad happened!\n");
	}

	// Record when it come to the end.
	clock_t end = clock();

	// And print the duration
	printf("Time cost: %ums\n", end - begin);

	// Find out padding value used by PKCS#7
	size_t diff = data_out[file_size - 1];

	// And excluding them from being part of our file
	size_t real_file_size = file_size - diff;

	// Finally, write chipertext to filesystem.
	FILE *out_fp = fopen(output_file.c_str(), "wb+");
	if (!out_fp) {
		// Can't open file.
		puts("fuck you");
	}
	else {
		fwrite(data_out, real_file_size, 1, out_fp);
	}

	// Release all using memory
	free(data_in);
	free(data_out);

	// Close all opened files.
	fclose(fp);
	fclose(out_fp);
}


int main(int argc, char *argv[])
{
	if (argc == 1) {
		// we need more parameters, print help message and quit.
		printf("useage: %s -m (encrypt|decrypt) -k 0123456789abcdeffedcba9876543210 -i /path/to/input -o /path/to/output -t <thread count>\n", strrchr(argv[0], '\\') + 1);
		return 1;
	}

	bool isEnctypt = false;
	std::string inputFile, outputFile;
	uint8_t key[16];
	int i, threadCount;
	for (i = 1; i < argc - 1; ++i) {
		if (std::string(argv[i]) == "-m") {
			i++;
			if (std::string(argv[i]) == "encrypt") {
				isEnctypt = true;
			}
			else if (std::string(argv[i]) == "decrypt") {
				isEnctypt = false;
			}
			else {
				printf("Mode must be encrypt or decrypt.");
				return 2;
			}
		}
		else if (std::string(argv[i]) == "-k") {
			// parse input key
			i++;
			int j;
			for (j = 0; j != 16; j += 2) {
				key[j / 2] = (hexToDec(argv[i][j]) << 8) & (hexToDec(argv[i][j]));
			}
		}
		else if (std::string(argv[i]) == "-i") {
			// parse input file
			i++;
			inputFile = std::string(argv[i]);
		}
		else if (std::string(argv[i]) == "-o") {
			// parse input file
			i++;
			outputFile = std::string(argv[i]);
		}
		else if (std::string(argv[i]) == "-t") {
			// parse thread count
			i++;
			sscanf(argv[i], "%d", &threadCount);
		}
	}

	if (isEnctypt) {
		encrypt_entry(inputFile, outputFile, key);
	}
	else {
		decrypt_entry(inputFile, outputFile, key);
	}

    cudaDeviceReset();
	
    return 0;
}

// SBox and InverseSBox
#if 1

const uint8_t round_const[] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
};

__device__  __host__ uint8_t sbox(uint8_t in) {
	const static uint8_t __sbox[] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
	};
	return __sbox[in];

}

__device__ uint8_t inverse_sbox(uint8_t in) {
	const static uint8_t __inverse_sbox[] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
	};
	return __inverse_sbox[in];

}

#endif

// AES core functions block
#if 1
__host__ uint8_t *key_schedule(const uint8_t *input_key) {
	uint8_t *output = (uint8_t *)malloc((10 + 1) * 16 * sizeof(uint8_t));
	uint8_t temp[4], *cursor_4_before, *cursor_16_before, *cursor_current;
	size_t counter, i, j;

	// Copy the first 16 uint8_ts
	memcpy(output, input_key, 16 * sizeof(uint8_t));

	// Initialize cursors.
	cursor_current = output + 16;
	cursor_4_before = cursor_current - 4;
	cursor_16_before = cursor_current - 16;

	// Rock and roll.

	for (counter = 0; counter != 10; counter++) {
		// RotWord
		temp[3] = cursor_4_before[0];
		temp[0] = cursor_4_before[1];
		temp[1] = cursor_4_before[2];
		temp[2] = cursor_4_before[3];

		// SubBytes and add
		for (i = 0; i != 4; ++i) {
			cursor_current[i] = sbox(temp[i]) ^ cursor_16_before[i];
			if (i == 0) {
				cursor_current[i] ^= round_const[counter];
			}
		}

		// Forward all cursors
		cursor_current += 4, cursor_4_before += 4, cursor_16_before += 4;

		// Three step remaining is simple.
		for (i = 0; i != 3; ++i) {
			for (j = 0; j != 4; ++j) {
				cursor_current[j] = cursor_4_before[j] ^ cursor_16_before[j];
			}
			cursor_current += 4, cursor_4_before += 4, cursor_16_before += 4;
		}
	}

	return output;
}

__device__ void sub_bytes(uint8_t *input) {
	int i = 15;
	do {
		input[i] = sbox(input[i]);
	} while (--i >= 0);
}

__device__ void inverse_sub_bytes(uint8_t *input) {
	int i = 15;
	do {
		input[i] = inverse_sbox(input[i]);
	} while (--i >= 0);
}

__device__ void shift_rows(uint8_t *input) {
	int index, inner_index;
#define OFFSET(i, j) ((j) * 4 + (i))
	for (index = 0; index != 4; ++index) {
		uint8_t temp;
		inner_index = index;
		while (inner_index-- > 0) {
			temp = input[OFFSET(index, 0)];
			input[OFFSET(index, 0)] = input[OFFSET(index, 1)];
			input[OFFSET(index, 1)] = input[OFFSET(index, 2)];
			input[OFFSET(index, 2)] = input[OFFSET(index, 3)];
			input[OFFSET(index, 3)] = temp;
		}
	}
#undef OFFSET
}

__device__ void inverse_shift_rows(uint8_t *input) {
	int index, inner_index;
#define OFFSET(i, j) ((j) * 4 + (i))
	for (index = 0; index != 4; ++index) {
		uint8_t temp;
		inner_index = index;
		while (inner_index-- > 0) {
			temp = input[OFFSET(index, 3)];
			input[OFFSET(index, 3)] = input[OFFSET(index, 2)];
			input[OFFSET(index, 2)] = input[OFFSET(index, 1)];
			input[OFFSET(index, 1)] = input[OFFSET(index, 0)];
			input[OFFSET(index, 0)] = temp;
		}
	}
#undef OFFSET
}

__device__ uint8_t mul2(uint8_t a) {
	static uint8_t cache[256] = {
		0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E,
		0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E,
		0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E,
		0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E,
		0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E,
		0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE, 0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE,
		0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE, 0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
		0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF0, 0xF2, 0xF4, 0xF6, 0xF8, 0xFA, 0xFC, 0xFE,
		0x1B, 0x19, 0x1F, 0x1D, 0x13, 0x11, 0x17, 0x15, 0x0B, 0x09, 0x0F, 0x0D, 0x03, 0x01, 0x07, 0x05,
		0x3B, 0x39, 0x3F, 0x3D, 0x33, 0x31, 0x37, 0x35, 0x2B, 0x29, 0x2F, 0x2D, 0x23, 0x21, 0x27, 0x25,
		0x5B, 0x59, 0x5F, 0x5D, 0x53, 0x51, 0x57, 0x55, 0x4B, 0x49, 0x4F, 0x4D, 0x43, 0x41, 0x47, 0x45,
		0x7B, 0x79, 0x7F, 0x7D, 0x73, 0x71, 0x77, 0x75, 0x6B, 0x69, 0x6F, 0x6D, 0x63, 0x61, 0x67, 0x65,
		0x9B, 0x99, 0x9F, 0x9D, 0x93, 0x91, 0x97, 0x95, 0x8B, 0x89, 0x8F, 0x8D, 0x83, 0x81, 0x87, 0x85,
		0xBB, 0xB9, 0xBF, 0xBD, 0xB3, 0xB1, 0xB7, 0xB5, 0xAB, 0xA9, 0xAF, 0xAD, 0xA3, 0xA1, 0xA7, 0xA5,
		0xDB, 0xD9, 0xDF, 0xDD, 0xD3, 0xD1, 0xD7, 0xD5, 0xCB, 0xC9, 0xCF, 0xCD, 0xC3, 0xC1, 0xC7, 0xC5,
		0xFB, 0xF9, 0xFF, 0xFD, 0xF3, 0xF1, 0xF7, 0xF5, 0xEB, 0xE9, 0xEF, 0xED, 0xE3, 0xE1, 0xE7, 0xE5,
	};
	return cache[a];
}

__device__ void mix_columns(uint8_t *input) {
	int8_t i = 0;
	uint8_t temp[4], tmp;

	memcpy(temp, input, 4);

	for (i = 0; i < 16; i += 4) {
		temp[0] = input[i + 0];
		temp[1] = input[i + 1];
		temp[2] = input[i + 2];
		temp[3] = input[i + 3];
		tmp = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
		input[i + 0] = mul2(temp[0] ^ temp[1]) ^ temp[0] ^ tmp;
		input[i + 1] = mul2(temp[1] ^ temp[2]) ^ temp[1] ^ tmp;
		input[i + 2] = mul2(temp[2] ^ temp[3]) ^ temp[2] ^ tmp;
		input[i + 3] = mul2(temp[3] ^ temp[0]) ^ temp[3] ^ tmp;
	}
}

__device__ void inverse_mix_columns(uint8_t *input) {
	size_t i;
	uint8_t temp[4], tmp;

	for (i = 0; i != 16; i += 4) {
		temp[0] = input[i + 0];
		temp[1] = input[i + 1];
		temp[2] = input[i + 2];
		temp[3] = input[i + 3];
		tmp = temp[0] ^ temp[1] ^ temp[2] ^ temp[3];
		input[i + 0] = mul2(temp[0] ^ temp[1]) ^ temp[0] ^ tmp;
		input[i + 1] = mul2(temp[1] ^ temp[2]) ^ temp[1] ^ tmp;
		input[i + 2] = mul2(temp[2] ^ temp[3]) ^ temp[2] ^ tmp;
		input[i + 3] = mul2(temp[3] ^ temp[0]) ^ temp[3] ^ tmp;

		uint8_t u, v;
		u = mul2(mul2(temp[0] ^ temp[2]));
		v = mul2(mul2(temp[1] ^ temp[3]));
		tmp = mul2(v ^ u);

		input[i + 0] ^= tmp ^ u;
		input[i + 2] ^= tmp ^ u;
		input[i + 1] ^= tmp ^ v;
		input[i + 3] ^= tmp ^ v;
	}
}

#endif

__global__ void aes_128_single_block(uint8_t *input, uint8_t *round_keys, uint8_t *output, size_t size, size_t offset, size_t blockSize) {
	size_t index = blockDim.x * blockIdx.x + threadIdx.x;
	if (offset * blockSize + index >= size) {
		return;
	}
	size_t i, j;
	uint8_t *result, *data, *cursor_round_key;

	// forward pointers
	data = input + (offset * blockSize + index) * 16;
	result = output + (offset * blockSize + index) * 16;

	// initialize cursors.
	cursor_round_key = round_keys;

	// Copy plain text.
	for (i = 0; i != 16; ++i) {
		result[i] = data[i] ^ cursor_round_key[i];
	}
	cursor_round_key += 0x10;

	// Run rounds excluding the last round.
	for (i = 0; i != 9; ++i) {
		sub_bytes(result);
		shift_rows(result);
		mix_columns(result);
		// Add round
		for (j = 0; j != 16; ++j) {
			result[j] ^= cursor_round_key[j];
		}
		cursor_round_key += 0x10;
	}

	// Now the last round.
	sub_bytes(result);
	shift_rows(result);
	for (i = 0; i != 16; ++i) {
		result[i] = result[i] ^ cursor_round_key[i];
	}
}

__global__ void aes_decrypt_single_block(uint8_t *input, uint8_t *round_keys, uint8_t *output, size_t size, size_t offset, size_t blockSize) {
	size_t index = blockDim.x * blockIdx.x + threadIdx.x;
	if (offset * blockSize + index >= size) {
		return;
	}
	size_t i, j;
	uint8_t *result, *cursor_round_key, *data;

	// forward pointers
	result = output + (offset * blockSize + index) * 16;
	data = input + (offset * blockSize + index) * 16;
	cursor_round_key = round_keys + 160;

	// The last round
	for (i = 0; i != 16; ++i) {
		result[i] = data[i] ^ cursor_round_key[i];
	}

	inverse_shift_rows(result);
	inverse_sub_bytes(result);
	cursor_round_key -= 16;

	// Run rounds excluding the last round.
	for (i = 0; i != 9; ++i) {
		for (j = 0; j != 16; ++j) {
			result[j] = result[j] ^ cursor_round_key[j];
		}
		inverse_mix_columns(result);
		inverse_shift_rows(result);
		inverse_sub_bytes(result);
		cursor_round_key -= 16;
	}

	// Now the last addRoundKey.
	for (i = 0; i != 16; ++i) {
		result[i] = result[i] ^ cursor_round_key[i];
	}
}


cudaError_t aes_on_cuda(const uint8_t *input, const uint8_t *keys, uint8_t *output, const size_t size, bool isEncrypt) {
	uint8_t *round_keys = key_schedule(keys);
	uint8_t *gpu_input = nullptr;
	uint8_t *gpu_round_keys = nullptr;
	uint8_t *gpu_output = nullptr;
	cudaError_t cudaStatus;

#define ASSERT(s) \
	if (cudaStatus != cudaSuccess) { \
		fprintf(stderr, s"\n"); \
		goto Error; \
	}


	// Select which GPU to work with.
	cudaStatus = cudaSetDevice(0);
	ASSERT("cudaSetDevice");

	// Prepare GPU memory block
	cudaStatus = cudaMalloc((void**)&gpu_input, size * sizeof(uint8_t));
	ASSERT("cudaMalloc");
	cudaStatus = cudaMalloc((void**)&gpu_round_keys, (11 * 16) * sizeof(uint8_t));
	ASSERT("cudaMalloc");
	cudaStatus = cudaMalloc((void**)&gpu_output, size * sizeof(uint8_t));
	ASSERT("cudaMalloc");

	// Copy data from memory to GPU memory.
	cudaStatus = cudaMemcpy(gpu_input, input, size * sizeof(uint8_t), cudaMemcpyHostToDevice);
	ASSERT("cudaMemcpy: gpu_input");
	cudaStatus = cudaMemcpy(gpu_round_keys, round_keys, (11 * 16) * sizeof(uint8_t), cudaMemcpyHostToDevice);
	ASSERT("cudaMemcpy: gpu_round_keys");

	size_t offset = 0;
	const size_t threadsPerBlock = 512;
	const size_t blockSize = 16;
	while (offset * threadsPerBlock * blockSize * 16 < size) {
		if (offset % 100 == 0) {
			printf("progress: %lf\r", offset * (double)threadsPerBlock * blockSize * 1600 / size);
		}
		if (isEncrypt) {
			aes_128_single_block <<<blockSize, threadsPerBlock >>>(gpu_input, gpu_round_keys, gpu_output, size / 16, offset, threadsPerBlock * blockSize);
		}
		else {
			aes_decrypt_single_block <<<blockSize, threadsPerBlock >>>(gpu_input, gpu_round_keys, gpu_output, size / 16, offset, threadsPerBlock * blockSize);
		}
		offset++;
	}

	cudaStatus = cudaGetLastError();
	ASSERT("cudaGetLastError");
	cudaStatus = cudaDeviceSynchronize();
	ASSERT("cudaDeviceSynchronize");
	cudaStatus = cudaMemcpy(output, gpu_output, size * sizeof(uint8_t), cudaMemcpyDeviceToHost);
	ASSERT("cudaMemcpy");

Error:
	cudaFree(gpu_input);
	cudaFree(gpu_round_keys);
	cudaFree(gpu_output);
	free(round_keys);

	return cudaStatus;

#undef ASSERT
}