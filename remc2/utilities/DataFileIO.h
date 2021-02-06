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
		uint32 l1; // +0
		uint16 l2; // +4
		uint32 l3; // +6
		uint16 bit_depth; // +A
	} huftable_t;

	typedef struct vars_s {
		uint16 max_matches;
		uint16 enc_key;
		uint32 pack_block_size;
		uint16 dict_size;
		uint32 method;
		uint32 pus_mode;
		uint32 input_size;
		uint32 file_size;

		// inner
		uint32 bytes_left;
		uint32 packed_size;
		uint32 processed_size;
		uint32 v7;
		uint32 pack_block_pos;
		uint16 pack_token, bit_count, v11;
		uint16 last_min_offset;
		uint32 v17;
		uint32 pack_block_left_size;
		uint16 match_count;
		uint16 match_offset;
		uint32 v20, v21;
		uint32 bit_buffer;

		uint32 unpacked_size;
		uint32 rnc_data_size;
		uint16 unpacked_crc, unpacked_crc_real;
		uint16 packed_crc;
		uint32 leeway;
		uint32 chunks_count;

		uint8* mem1;
		uint8* pack_block_start;
		uint8* pack_block_max;
		uint8* pack_block_end;
		uint16* mem2;
		uint16* mem3;
		uint16* mem4;
		uint16* mem5;

		uint8* decoded;
		uint8* window;

		size_t read_start_offset, write_start_offset;
		uint8* input, * output, * temp;
		size_t input_offset, output_offset, temp_offset;

		uint8 tmp_crc_data[2048];
		huftable_t raw_table[16];
		huftable_t pos_table[16];
		huftable_t len_table[16];
	} vars_t;

	static uint16 crc_table[256];

public:
	static int ReadFileAndDecompress(const char* path, uint8_t** data); //Reads the file path passed in into "data" array of 1 byte elements
	static int Decompress(uint8_t* a1, uint8_t* a2);
	static vars_t* init_vars();
	static int UnpackData(vars_t* v);
	static int Unpack(vars_t* v);
	static FILE* CreateOrOpenFile(char* pathname, int __pmode);
	static FILE* CreateFile(char* path, uint32_t flags);
	static FILE* Open(char* path, int pmode, uint32_t flags);
	static int32 Close(FILE* file);
	static int32 Seek(FILE* file, x_DWORD position, char type);
	static size_t Read(FILE* file, uint8_t* data, uint32_t length);
	static long FileLengthBytes(FILE* file);
	static uint8 read_byte(uint8* buf, size_t* offset);
	static uint16 read_word_be(uint8* buf, size_t* offset);
	static uint32 read_dword_be(uint8* buf, size_t* offset);
	static int unpack_data_m1(vars_t* v);
	static int unpack_data_m2(vars_t* v);
	static uint16 crc_block(uint8* buf, size_t offset, int size);
	static int input_bits(vars_t* v, short count);
	static uint32 input_bits_m1(vars_t* v, short count);
	static uint32 input_bits_m2(vars_t* v, short count);
	static void decode_match_count(vars_t* v);
	static void decode_match_offset(vars_t* v);
	static void write_decoded_byte(vars_t* v, uint8 b);
	static void write_buf(uint8* dest, size_t* offset, uint8* source, int size);
	static uint8 read_source_byte(vars_t* v);
	static void read_buf(uint8* dest, uint8* source, size_t* offset, int size);
	static void ror_w(uint16* x);
	static uint32 inverse_bits(uint32 value, int count);
	static void proc_20(huftable_t* data, int count);
	static void make_huftable(vars_t* v, huftable_t* data, int count);
	static uint32 decode_table_data(vars_t* v, huftable_t* data);
	static void clear_table(huftable_t* data, int count);


private:
	// Disallow creating an instance of this object
	DataFileIO();
};

#endif