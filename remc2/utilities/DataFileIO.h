#pragma once
#ifndef DataFileIO_H
#define DataFileIO_H

#include <cstdio>
#include <cstdint>
#include "../portability/port_filesystem.h"
#include "../portability/port_outputs.h"

#define RNC_SIGN 0x524E43 // RNC
#define RNC_HEADER_SIZE 0x12
#define MAX_BUF_SIZE 0x90000

class DataFileIO
{
private:

	typedef struct huftable_s {
		uint32_t l1; // +0
		uint16_t l2; // +4
		uint32_t l3; // +6
		uint16_t bit_depth; // +A
	} huftable_t;

	typedef struct vars_s {
		uint16_t max_matches;
		uint16_t enc_key;
		uint32_t pack_block_size;
		uint16_t dict_size;
		uint32_t method;
		uint32_t pus_mode;
		uint32_t input_size;
		uint32_t file_size;

		// inner
		uint32_t bytes_left;
		uint32_t packed_size;
		uint32_t processed_size;
		uint32_t v7;
		uint32_t pack_block_pos;
		uint16_t pack_token, bit_count, v11;
		uint16_t last_min_offset;
		uint32_t v17;
		uint32_t pack_block_left_size;
		uint16_t match_count;
		uint16_t match_offset;
		uint32_t v20, v21;
		uint32_t bit_buffer;

		uint32_t unpacked_size;
		uint32_t rnc_data_size;
		uint16_t unpacked_crc, unpacked_crc_real;
		uint16_t packed_crc;
		uint32_t leeway;
		uint32_t chunks_count;

		uint8_t* mem1;
		uint8_t* pack_block_start;
		uint8_t* pack_block_max;
		uint8_t* pack_block_end;
		uint16_t* mem2;
		uint16_t* mem3;
		uint16_t* mem4;
		uint16_t* mem5;

		uint8_t* decoded;
		uint8_t* window;

		size_t read_start_offset, write_start_offset;
		uint8_t* input, * output, * temp;
		size_t input_offset, output_offset, temp_offset;

		uint8_t tmp_crc_data[2048];
		huftable_t raw_table[16];
		huftable_t pos_table[16];
		huftable_t len_table[16];
	} vars_t;

	static uint16_t crc_table[256];

public:
	static int ReadFileAndDecompress(const char* path, uint8_t** data); //Reads the file path passed in into "data" array of 1 byte elements
	static int Decompress(uint8_t* a1, uint8_t* a2);
	static vars_t* init_vars();
	static int UnpackData(vars_t* v);
	static int Unpack(vars_t* v);
	static FILE* CreateOrOpenFile(char* pathname, int __pmode);
	static FILE* CreateFile(char* path, uint32_t flags);
	static FILE* Open(char* path, int pmode, uint32_t flags);
	static int32_t Close(FILE* file);
	static int32_t Seek(FILE* file, x_DWORD position, char type);
	static size_t Read(FILE* file, uint8_t* data, uint32_t length);
	static long FileLengthBytes(FILE* file);
	static uint8_t read_byte(uint8_t* buf, size_t* offset);
	static uint16_t read_word_be(uint8_t* buf, size_t* offset);
	static uint32_t read_dword_be(uint8_t* buf, size_t* offset);
	static int unpack_data_m1(vars_t* v);
	static int unpack_data_m2(vars_t* v);
	static uint16_t crc_block(uint8_t* buf, size_t offset, int size);
	static int input_bits(vars_t* v, short count);
	static uint32_t input_bits_m1(vars_t* v, short count);
	static uint32_t input_bits_m2(vars_t* v, short count);
	static void decode_match_count(vars_t* v);
	static void decode_match_offset(vars_t* v);
	static void write_decoded_byte(vars_t* v, uint8_t b);
	static void write_buf(uint8_t* dest, size_t* offset, uint8_t* source, int size);
	static uint8_t read_source_byte(vars_t* v);
	static void read_buf(uint8_t* dest, uint8_t* source, size_t* offset, int size);
	static void ror_w(uint16_t* x);
	static uint32_t inverse_bits(uint32_t value, int count);
	static void proc_20(huftable_t* data, int count);
	static void make_huftable(vars_t* v, huftable_t* data, int count);
	static uint32_t decode_table_data(vars_t* v, huftable_t* data);
	static void clear_table(huftable_t* data, int count);


private:
	// Disallow creating an instance of this object
	DataFileIO();
};

#endif