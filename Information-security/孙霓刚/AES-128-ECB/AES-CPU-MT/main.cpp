#include <thread>
#include <vector>
#include <ctime>
#include <string>
#include <algorithm>
#include "aes.h"

/// the real unit
void process_unit(uint8_t *data_in, uint8_t *data_out, uint8_t *key, bool isEncrypt) {
	aes_cpu::byte *rkey = aes_cpu::aes::key_schedule(key);
	if (isEncrypt) {
		aes_cpu::aes::aes_128_single_block(data_in, rkey, data_out);
	}
	else {
		aes_cpu::aes::aes_decrypt_single_block(data_in, rkey, data_out);
	}
	delete[] rkey;
}

/// Entry for our threads
void thread_entry(uint8_t *data_in, uint8_t *data_out, uint8_t *key, size_t file_size, size_t current, size_t total, bool isEncrypt) {
	for (uint32_t i = current; i < file_size / 16; i += total) {
		if (i % 10000 == 0 && current == 0) {
			printf("process: %.2lf\r", (double)i * 100 / (file_size / 16));
		}
		process_unit(data_in + 16 * i, data_out + 16 * i, key, isEncrypt);
	}
}

/// Convert a character to hex number
/// '0' - '9' -> 0 - 9
/// 'a' - 'f' -> 10 - 15
/// 'A' - 'F' -> 10 - 15
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
		// And filling the extra bytes with diff
		(*data)[index] = diff;
	}
	return new_size;
}

void encrypt_entry(std::string input_file, std::string output_file, uint8_t key[], size_t thread_count) {
	puts("Mode: Encrypt");
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

	// Create an array for all threads
	std::vector<std::thread> threads(thread_count);

	// Construct all threads, and let it go.
	for (int i = 0; i != threads.size(); ++i) {
		threads[i] = std::thread(thread_entry, data_in, data_out, key, full_block_size, i, threads.size(), true);
	}

	// Sync for all threads.
	for (int i = 0; i != threads.size(); ++i) {
		threads[i].join();
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

void decrypt_entry(std::string input_file, std::string output_file, uint8_t key[], size_t thread_count) {
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

	// Create an array for all threads
	std::vector<std::thread> threads(thread_count);

	// Construct all threads, and let it go.
	for (int i = 0; i != threads.size(); ++i) {
		threads[i] = std::thread(thread_entry, data_in, data_out, key, file_size, i, threads.size(), false);
	}

	// Sync for all threads.
	for (int i = 0; i != threads.size(); ++i) {
		threads[i].join();
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

int main(int argc, char *argv[]) {
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
		// parse run mode
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
		encrypt_entry(inputFile, outputFile, key, threadCount);
	}
	else {
		decrypt_entry(inputFile, outputFile, key, threadCount);
	}

	return 0;
}