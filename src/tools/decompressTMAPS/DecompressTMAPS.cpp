// DecompressTMAPS.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>

//#define level1 //night
//#define level2 //day
#define level2 //cave

#ifdef level1
char palfilename[] = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-n.pal";
char tmapsdatfilename[] = "c:\\prenos\\Magic2\\mc2-orig\\data\\tmaps1-0.dat";
char tmapstabfilename[] = "c:\\prenos\\Magic2\\mc2-orig\\data\\tmaps1-0.tab";
char tmapsstr[] = "TMAPS2-1-";
#endif
#ifdef level2
char palfilename[] = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-block.pal";
char tmapsdatfilename[] = "c:\\prenos\\Magic2\\mc2-orig\\data\\tmaps0-0.dat";
char tmapstabfilename[] = "c:\\prenos\\Magic2\\mc2-orig\\data\\tmaps0-0.tab";
char tmapsstr[]="TMAPS2-0-";
#endif
#ifdef level4
char palfilename[] = "c:\\prenos\\remc2\\tools\\palletelight\\Debug\\out-c.pal";
char tmapsdatfilename[] = "c:\\prenos\\Magic2\\mc2-orig\\data\\tmaps2-0.dat";
char tmapstabfilename[] = "c:\\prenos\\Magic2\\mc2-orig\\data\\tmaps2-0.tab";
char tmapsstr[] = "TMAPS2-2-";
#endif

#include "png.h"
#pragma comment(lib, "zlib.lib") // must be before libpng!
#ifndef _WIN64
#pragma comment(lib, "libpng15.lib") // must be after zlib!
#else
#endif

#define x_BYTE int8
#define x_WORD int16
#define x_DWORD int32
#define x_LONG int32

#define SIZEOF_UNSIGNED_INT 4

#define SIZEOF_UNSIGNED_CHAR 1

#define SIZEOF_UNSIGNED_SHORT 2

#if SIZEOF_UNSIGNED_SHORT != 2
#  error "sizeof (unsigned short) != 2"
#else
typedef unsigned short Bit16u;
typedef   signed short Bit16s;
#endif

#if SIZEOF_UNSIGNED_INT == 4
typedef unsigned int Bit32u;
typedef   signed int Bit32s;
#elif SIZEOF_UNSIGNED_LONG == 4
typedef unsigned long Bit32u;
typedef   signed long Bit32s;
#else
//#  error "can't find sizeof(type) of 4 bytes!"
#endif

#if SIZEOF_UNSIGNED_CHAR != 1
#  error "sizeof (unsigned char) != 1"
#else
typedef unsigned char Bit8u;
typedef   signed char Bit8s;
#endif

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;

#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64

#define LOWORD(x)   (*((_WORD*)&(x)))   // low word
#define HIWORD(x)   (*((_WORD*)&(x)+1))
#define LOBYTE(x)   (*((_BYTE*)&(x)))   // low byte
#define HIBYTE(x)   (*((_BYTE*)&(x)+1))

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

#define RNC_SIGN 0x524E43 // RNC
#define RNC_HEADER_SIZE 0x12
#define MAX_BUF_SIZE 0x90000

uint8 read_byte(uint8 * buf, size_t * offset)
{
	return buf[(*offset)++];
}

uint16 read_word_be(uint8* buf, size_t* offset)
{
	uint8 b1 = read_byte(buf, offset);
	uint8 b2 = read_byte(buf, offset);

	return (b1 << 8) | b2;
}

uint32 read_dword_be(uint8* buf, size_t* offset)
{
	uint16 w1 = read_word_be(buf, offset);
	uint16 w2 = read_word_be(buf, offset);

	return (w1 << 16) | w2;
}

static const uint16 crc_table[] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

uint16 crc_block(uint8* buf, size_t offset, int size)
{
	uint16 crc = 0;

	while (size--)
	{
		crc ^= read_byte(buf, &offset);
		crc = (crc >> 8) ^ crc_table[crc & 0xFF];
	}

	return crc;
}

void read_buf(uint8* dest, uint8* source, size_t* offset, int size)
{
	memmove(dest, &source[*offset], size);
	*offset += size;
}

uint8 read_source_byte(vars_t* v)
{
	if (v->pack_block_start == &v->mem1[0xFFFD])
	{
		int left_size = v->file_size - v->input_offset;

		int size_to_read;
		if (left_size <= 0xFFFD)
			size_to_read = left_size;
		else
			size_to_read = 0xFFFD;

		v->pack_block_start = v->mem1;

		read_buf(v->pack_block_start, v->input, &v->input_offset, size_to_read);

		if (left_size - size_to_read > 2)
			left_size = 2;

		read_buf(&v->mem1[size_to_read], v->input, &v->input_offset, left_size);
		v->input_offset -= left_size;
	}

	return *v->pack_block_start++;
}

uint32 input_bits_m2(vars_t* v, short count)
{
	uint32 bits = 0;

	while (count--)
	{
		if (!v->bit_count)
		{
			v->bit_buffer = read_source_byte(v);
			v->bit_count = 8;
		}

		bits <<= 1;

		if (v->bit_buffer & 0x80)
			bits |= 1;

		v->bit_buffer <<= 1;
		v->bit_count--;
	}

	return bits;
}



uint32 input_bits_m1(vars_t* v, short count)
{
	uint32 bits = 0;
	uint32 prev_bits = 1;

	while (count--)
	{
		if (!v->bit_count)
		{
			uint8 b1 = read_source_byte(v);
			uint8 b2 = read_source_byte(v);
			v->bit_buffer = (v->pack_block_start[1] << 24) | (v->pack_block_start[0] << 16) | (b2 << 8) | b1;

			v->bit_count = 16;
		}

		if (v->bit_buffer & 1)
			bits |= prev_bits;

		v->bit_buffer >>= 1;
		prev_bits <<= 1;
		v->bit_count--;
	}

	return bits;
}

int input_bits(vars_t* v, short count)
{
	if (v->method != 2)
		return input_bits_m1(v, count);
	else
		return input_bits_m2(v, count);
}

void clear_table(huftable_t* data, int count)
{
	for (int i = 0; i < count; ++i)
	{
		data[i].l1 = 0;
		data[i].l2 = 0xFFFF;
		data[i].l3 = 0;
		data[i].bit_depth = 0;
	}
}

uint32 inverse_bits(uint32 value, int count)
{
	int i = 0;
	while (count--)
	{
		i <<= 1;

		if (value & 1)
			i |= 1;

		value >>= 1;
	}

	return i;
}

void proc_20(huftable_t* data, int count)
{
	int val = 0;
	uint32 div = 0x80000000;
	int bits_count = 1;

	while (bits_count <= 16)
	{
		int i = 0;

		while (1)
		{
			if (i >= count)
			{
				bits_count++;
				div >>= 1;
				break;
			}

			if (data[i].bit_depth == bits_count)
			{
				data[i].l3 = inverse_bits(val / div, bits_count);
				val += div;
			}

			i++;
		}
	}
}

void make_huftable(vars_t* v, huftable_t* data, int count)
{
	clear_table(data, count);

	int leaf_nodes = input_bits_m1(v, 5);

	if (leaf_nodes)
	{
		if (leaf_nodes > 16)
			leaf_nodes = 16;

		for (int i = 0; i < leaf_nodes; ++i)
			data[i].bit_depth = input_bits_m1(v, 4);

		proc_20(data, leaf_nodes);
	}
}

uint32 decode_table_data(vars_t* v, huftable_t* data)
{
	uint32 i = 0;

	while (1)
	{
		if (data[i].bit_depth && (data[i].l3 == (v->bit_buffer & ((1 << data[i].bit_depth) - 1))))
		{
			input_bits_m1(v, data[i].bit_depth);

			if (i < 2)
				return i;

			return input_bits_m1(v, i - 1) | (1 << (i - 1));
		}

		i++;
	}
}

void write_buf(uint8* dest, size_t* offset, uint8* source, int size)
{
	memmove(&dest[*offset], source, size);
	*offset += size;
}

void write_decoded_byte(vars_t* v, uint8 b)
{
	if (&v->decoded[0xFFFF] == v->window)
	{
		write_buf(v->output, &v->output_offset, &v->decoded[v->dict_size], 0xFFFF - v->dict_size);
		memmove(v->decoded, &v->window[-v->dict_size], v->dict_size);
		v->window = &v->decoded[v->dict_size];
	}

	*v->window++ = b;
	v->unpacked_crc_real = crc_table[(v->unpacked_crc_real ^ b) & 0xFF] ^ (v->unpacked_crc_real >> 8);
}

void ror_w(uint16* x)
{
	if (*x & 1)
		* x = 0x8000 | (*x >> 1);
	else
		*x >>= 1;
}

int unpack_data_m1(vars_t* v)
{
	while (v->processed_size < v->input_size)
	{
		make_huftable(v, v->raw_table, _countof(v->raw_table));
		make_huftable(v, v->len_table, _countof(v->len_table));
		make_huftable(v, v->pos_table, _countof(v->pos_table));

		int subchunks = input_bits_m1(v, 16);

		while (subchunks--)
		{
			uint32 data_length = decode_table_data(v, v->raw_table);
			v->processed_size += data_length;

			if (data_length)
			{
				while (data_length--)
					write_decoded_byte(v, (v->enc_key ^ read_source_byte(v)) & 0xFF);

				ror_w(&v->enc_key);

				v->bit_buffer = (((v->pack_block_start[2] << 16) | (v->pack_block_start[1] << 8) | v->pack_block_start[0]) << v->bit_count) | (v->bit_buffer & ((1 << v->bit_count) - 1));
			}

			if (subchunks)
			{
				v->match_offset = decode_table_data(v, v->len_table) + 1;
				v->match_count = decode_table_data(v, v->pos_table) + 2;
				v->processed_size += v->match_count;

				while (v->match_count--)
					write_decoded_byte(v, v->window[-v->match_offset]);
			}
		}
	}

	write_buf(v->output, &v->output_offset, &v->decoded[v->dict_size], v->window - &v->decoded[v->dict_size]);
	return 0;
}

void decode_match_count(vars_t* v)
{
	v->match_count = input_bits_m2(v, 1) + 4;

	if (input_bits_m2(v, 1))
		v->match_count = ((v->match_count - 1) << 1) + input_bits_m2(v, 1);
}

void decode_match_offset(vars_t* v)
{
	v->match_offset = 0;
	if (input_bits_m2(v, 1))
	{
		v->match_offset = input_bits_m2(v, 1);

		if (input_bits_m2(v, 1))
		{
			v->match_offset = ((v->match_offset << 1) | input_bits_m2(v, 1)) | 4;

			if (!input_bits_m2(v, 1))
				v->match_offset = (v->match_offset << 1) | input_bits_m2(v, 1);
		}
		else if (!v->match_offset)
			v->match_offset = input_bits_m2(v, 1) + 2;
	}

	v->match_offset = ((v->match_offset << 8) | read_source_byte(v)) + 1;
}

int unpack_data_m2(vars_t* v)
{
	while (v->processed_size < v->input_size)
	{
		while (1)
		{
			if (!input_bits_m2(v, 1))
			{
				write_decoded_byte(v, (v->enc_key ^ read_source_byte(v)) & 0xFF);

				ror_w(&v->enc_key);

				v->processed_size++;
			}
			else
			{
				if (input_bits_m2(v, 1))
				{
					if (input_bits_m2(v, 1))
					{
						if (input_bits_m2(v, 1))
						{
							v->match_count = read_source_byte(v) + 8;

							if (v->match_count == 8)
							{
								input_bits_m2(v, 1);
								break;
							}
						}
						else
							v->match_count = 3;

						decode_match_offset(v);
					}
					else
					{
						v->match_count = 2;
						v->match_offset = read_source_byte(v) + 1;
					}

					v->processed_size += v->match_count;

					while (v->match_count--)
						write_decoded_byte(v, v->window[-v->match_offset]);
				}
				else
				{
					decode_match_count(v);

					if (v->match_count != 9)
					{
						decode_match_offset(v);
						v->processed_size += v->match_count;

						while (v->match_count--)
							write_decoded_byte(v, v->window[-v->match_offset]);
					}
					else
					{
						uint32 data_length = (input_bits_m2(v, 4) << 2) + 12;
						v->processed_size += data_length;

						while (data_length--)
							write_decoded_byte(v, (v->enc_key ^ read_source_byte(v)) & 0xFF);

						ror_w(&v->enc_key);
					}
				}
			}
		}
	}

	write_buf(v->output, &v->output_offset, &v->decoded[v->dict_size], v->window - &v->decoded[v->dict_size]);
	return 0;
}

int do_unpack_data(vars_t* v)
{
	int start_pos = v->input_offset;

	uint32 sign = read_dword_be(v->input, &v->input_offset);
	if ((sign >> 8) != RNC_SIGN)
		return 6;

	v->method = sign & 3;
	v->input_size = read_dword_be(v->input, &v->input_offset);
	v->packed_size = read_dword_be(v->input, &v->input_offset);
	if (v->file_size < v->packed_size)
		return 7;
	v->unpacked_crc = read_word_be(v->input, &v->input_offset);
	v->packed_crc = read_word_be(v->input, &v->input_offset);

	/*v->leeway = */read_byte(v->input, &v->input_offset);
	/*v->chunks_count = */read_byte(v->input, &v->input_offset);

	if (crc_block(v->input, v->input_offset, v->packed_size) != v->packed_crc)
		return 4;

	v->mem1 = (uint8*)malloc(0xFFFF);
	v->decoded = (uint8*)malloc(0xFFFF);
	v->pack_block_start = &v->mem1[0xFFFD];
	v->window = &v->decoded[v->dict_size];

	v->unpacked_crc_real = 0;
	v->bit_count = 0;
	v->bit_buffer = 0;
	v->processed_size = 0;

	uint16 specified_key = v->enc_key;

	int error_code = 0;
	if (input_bits(v, 1) && !v->pus_mode)
		error_code = 9;

	if (!error_code)
	{
		if (input_bits(v, 1) && !v->enc_key) // key is needed, but not specified as argument
			error_code = 10;
	}

	if (!error_code)
	{
		switch (v->method)
		{
		case 1: error_code = unpack_data_m1(v); break;
		case 2: error_code = unpack_data_m2(v); break;
		}
	}

	v->enc_key = specified_key;

	free(v->mem1);
	free(v->decoded);

	v->input_offset = start_pos + v->packed_size + RNC_HEADER_SIZE;

	if (error_code)
		return error_code;

	if (v->unpacked_crc != v->unpacked_crc_real)
		return 5;

	return 0;
}

int do_unpack(vars_t* v)
{
	v->packed_size = v->file_size;

	if (v->file_size < RNC_HEADER_SIZE)
		return 6;

	int result = do_unpack_data(v); // data
	if (result == 0)return v->input_size;
	return -result;
}

vars_t* init_vars()
{
	vars_t* v = (vars_t*)malloc(sizeof(vars_t));
	v->enc_key = 0;
	v->max_matches = 0x1000;
	v->unpacked_crc_real = 0;
	v->pack_block_size = 0x3000;
	v->dict_size = 0xFFFF;
	v->method = 1;
	v->pus_mode = 0;

	v->read_start_offset = 0;
	v->write_start_offset = 0;
	v->input_offset = 0;
	v->output_offset = 0;
	v->temp_offset = 0;

	memset(v->tmp_crc_data, 0, sizeof(v->tmp_crc_data));
	memset(v->raw_table, 0, sizeof(v->raw_table));
	memset(v->pos_table, 0, sizeof(v->pos_table));
	memset(v->len_table, 0, sizeof(v->len_table));

	return v;
}


int sub_9894C_decompress(Bit8u* a1, Bit8u* a2) {
	vars_t* v = init_vars();
	if (v->method == 1)
	{
		if (v->dict_size > 0x8000)
			v->dict_size = 0x8000;
		v->max_matches = 0x1000;
	}
	else if (v->method == 2)
	{
		if (v->dict_size > 0x1000)
			v->dict_size = 0x1000;
		v->max_matches = 0xFF;
	}

	v->file_size = MAX_BUF_SIZE;
	v->input = (uint8*)malloc(MAX_BUF_SIZE);

	Bit32u signature = a1[0] + (a1[1] << 8) + (a1[2] << 16);
	if (signature == 0x434e52)
	{
		Bit32u inputsize = a1[11] + (a1[10] << 8) + (a1[9] << 16) + (a1[8] << 24);
		memmove(v->input, a1, inputsize + 0x12);
	}
	else
	{
		v->input[0] = 0;
	}
	//v->input = a1;

	v->output = a2;
	v->temp = (uint8*)malloc(MAX_BUF_SIZE);

	int error_code = 0;
	/*switch (v->pus_mode)
	{
	case 0: error_code = do_pack(v); break;
	case 1: error_code = do_unpack(v); break;
	case 2: error_code = do_search(v); break;
	}*/
	error_code = do_unpack(v);

	if (v->pus_mode == 2)
	{
		free(v->input);
		free(v->output);
		free(v->temp);
		free(v->input);
		free(v);

		return error_code;
	}

	/*if (error_code)
	 {
		switch (error_code) {
		case 4: printf("Corrupted input data.\n"); break;
		case 5: printf("CRC check failed.\n"); break;
		case 6:
		case 7:
			printf("Wrong RNC header.\n"); break;
		case 10: printf("No RNC archives were found.\n"); break;
		default: printf("Cannot process file. Error code: %x\n", error_code); break;
		}
	}*/

	free(v->input);
	//free(v->output);
	free(v->temp);
	free(v);
	if (error_code == -6)return 0;
	//if (error_code == 0)return 0;
	return error_code;
}


signed int sub_5C3D0_file_decompress(Bit8u* input, Bit8u* output)//23d3d0
{
	//char v3; // [esp+0h] [ebp-8h]
	//char v4; // [esp+1h] [ebp-7h]
	//char v5; // [esp+2h] [ebp-6h]
	//char v6; // [esp+3h] [ebp-5h]
	//char v7; // [esp+4h] [ebp-4h]

	char RNSSING[5] = "RNC\x1";
	//v3 = 82;
	//v4 = 78;
	//v6 = 1;
	//v5 = 67;
	//v7 = 0;
	if (strncmp((const char*)input, RNSSING, 4))
		return 0;
	sub_9894C_decompress(input, output);
	return 1;
}

const int bytesPerPixel = 4; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

unsigned char* createBitmapFileHeader(int height, int width, int pitch, int paddingSize) {
	int fileSize = fileHeaderSize + infoHeaderSize + (/*bytesPerPixel*width*/pitch + paddingSize) * height;

	static unsigned char fileHeader[] = {
		0,0, /// signature
		0,0,0,0, /// image file size in bytes
		0,0,0,0, /// reserved
		0,0,0,0, /// start of pixel array
	};

	fileHeader[0] = (unsigned char)('B');
	fileHeader[1] = (unsigned char)('M');
	fileHeader[2] = (unsigned char)(fileSize);
	fileHeader[3] = (unsigned char)(fileSize >> 8);
	fileHeader[4] = (unsigned char)(fileSize >> 16);
	fileHeader[5] = (unsigned char)(fileSize >> 24);
	fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

	return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width) {
	static unsigned char infoHeader[] = {
		0,0,0,0, /// header size
		0,0,0,0, /// image width
		0,0,0,0, /// image height
		0,0, /// number of color planes
		0,0, /// bits per pixel
		0,0,0,0, /// compression
		0,0,0,0, /// image size
		0,0,0,0, /// horizontal resolution
		0,0,0,0, /// vertical resolution
		0,0,0,0, /// colors in color table
		0,0,0,0, /// important color count
	};

	infoHeader[0] = (unsigned char)(infoHeaderSize);
	infoHeader[4] = (unsigned char)(width);
	infoHeader[5] = (unsigned char)(width >> 8);
	infoHeader[6] = (unsigned char)(width >> 16);
	infoHeader[7] = (unsigned char)(width >> 24);
	infoHeader[8] = (unsigned char)(height);
	infoHeader[9] = (unsigned char)(height >> 8);
	infoHeader[10] = (unsigned char)(height >> 16);
	infoHeader[11] = (unsigned char)(height >> 24);
	infoHeader[12] = (unsigned char)(1);
	infoHeader[14] = (unsigned char)(bytesPerPixel * 8);

	return infoHeader;
}

void writeImageBMP(char* imageFileName, int width, int height, Bit8u* image)
{
	int pitch = bytesPerPixel * width;
	unsigned char padding[3] = { 0, 0, 0 };
	int paddingSize = (4 - (/*width*bytesPerPixel*/ pitch) % 4) % 4;

	unsigned char* fileHeader = createBitmapFileHeader(height, width, pitch, paddingSize);
	unsigned char* infoHeader = createBitmapInfoHeader(height, width);

	FILE* imageFile;
	fopen_s(&imageFile, imageFileName, "wb");

	fwrite(fileHeader, 1, fileHeaderSize, imageFile);
	fwrite(infoHeader, 1, infoHeaderSize, imageFile);

	int i;
	for (i = 0; i < height; i++) {
		fwrite(image + (i * pitch /*width*bytesPerPixel*/), bytesPerPixel, width, imageFile);
		fwrite(padding, 1, paddingSize, imageFile);
	}

	fclose(imageFile);
	//free(fileHeader);
	//free(infoHeader);
}

inline void setRGBA(png_byte* ptr, Bit8u* val)
{
	ptr[0] = val[0];
	ptr[1] = val[1];
	ptr[2] = val[2];
	ptr[3] = val[3];
}

void writeImagePNG(char* filename, int width, int height, Bit8u* buffer, char* title)
{
		int code = 0;
		FILE* fp = NULL;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		png_bytep row = NULL;

		// Open file for writing (binary mode)
		fopen_s(&fp,filename, "wb");
		if (fp == NULL) {
			fprintf(stderr, "Could not open file %s for writing\n", filename);
			code = 1;
			goto finalise;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fprintf(stderr, "Could not allocate write struct\n");
			code = 1;
			goto finalise;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			fprintf(stderr, "Could not allocate info struct\n");
			code = 1;
			goto finalise;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			code = 1;
			goto finalise;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		// Set title
		if (title != NULL) {
			png_text title_text;
			title_text.compression = PNG_TEXT_COMPRESSION_NONE;
			title_text.key = (png_charp)"Title";
			title_text.text = title;
			png_set_text(png_ptr, info_ptr, &title_text, 1);
		}

		png_write_info(png_ptr, info_ptr);

		// Allocate memory for one row (3 bytes per pixel - RGB)
		row = (png_bytep)malloc(4 * width * sizeof(png_byte));

		// Write image data
		int x, y;
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				setRGBA(&(row[x * 4]), buffer + (y * width + x) * 4);
			}
			png_write_row(png_ptr, row);
		}

		// End write
		png_write_end(png_ptr, NULL);

	finalise:
		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		if (row != NULL) free(row);

		//return code;
}

void write_posistruct_to_png(Bit8u* buffer, int width, int height, char* filename,char* title,bool alpha) {
	Bit8u pallettebuffer[768];
	FILE* palfile;
	//fopen_s(&palfile, "c:\\prenos\\remc2\\testpal.pal", "rb");
	fopen_s(&palfile, palfilename, "rb");
	fread(pallettebuffer, 768, 1, palfile);
	fclose(palfile);

	Bit8u buffer2[100000 * 4];
	for (int i = 0; i < width * height; i++)
	{
		/*buffer2[i * 4 + 0] = buffer[i];
		buffer2[i * 4 + 1] = buffer[i];
		buffer2[i * 4 + 2] = buffer[i];*/
		buffer2[i * 4 + 0] = pallettebuffer[buffer[i] * 3];
		buffer2[i * 4 + 1] = pallettebuffer[buffer[i] * 3 + 1];
		buffer2[i * 4 + 2] = pallettebuffer[buffer[i] * 3+2];

		if (buffer[i] != 0xff)
		{
			if (alpha)
			{
				buffer2[i * 4 + 3] = 255;
				if ((buffer2[i * 4 + 0] == 0) && (buffer2[i * 4 + 1] == 0) && (buffer2[i * 4 + 2] == 0))
				{
					buffer2[i * 4 + 0] = 0;
					buffer2[i * 4 + 1] = 0;
					buffer2[i * 4 + 2] = 0;
				}
				else
				{
					buffer2[i * 4 + 0] = 255;
					buffer2[i * 4 + 1] = 255;
					buffer2[i * 4 + 2] = 255;
				}
			}
			else
			{
				if ((buffer2[i * 4 + 0] == 0) && (buffer2[i * 4 + 1] == 0) && (buffer2[i * 4 + 2] == 0))
					buffer2[i * 4 + 3] = 255;
				else
					buffer2[i * 4 + 3] = 255;
			}

		}
	}
	//writeImage(filename, width, height, buffer2);
	writeImagePNG(filename, width, height, buffer2,title);
}

void write_posistruct_to_bmp(Bit8u* buffer, int width, int height, char* filename) {
	Bit8u pallettebuffer[768];
	FILE* palfile;
	//fopen_s(&palfile, "c:\\prenos\\remc2\\testpal.pal", "rb");
	fopen_s(&palfile, palfilename, "rb");
	fread(pallettebuffer, 768, 1, palfile);
	fclose(palfile);

	Bit8u buffer2[100000 * 4];
	for (int i = 0; i < width * height; i++)
	{
		/*buffer2[i * 4 + 0] = buffer[i];
		buffer2[i * 4 + 1] = buffer[i];
		buffer2[i * 4 + 2] = buffer[i];*/
		buffer2[i * 4 + 0] = pallettebuffer[buffer[(width * height) - 1 - i] * 3+2];
		buffer2[i * 4 + 1] = pallettebuffer[buffer[(width * height) - 1 - i] * 3 + 1];
		buffer2[i * 4 + 2] = pallettebuffer[buffer[(width * height) - 1 - i] * 3];

		if (buffer[(width * height) - 1 - i] != 0xff)
		{
			if((buffer2[i * 4 + 0]==0)&& (buffer2[i * 4 + 1] == 0)&& (buffer2[i * 4 + 2] == 0))
				buffer2[i * 4 + 3] = 0;
			else
				buffer2[i * 4 + 3] = 255;
		}
	}
	//writeImage(filename, width, height, buffer2);
	writeImageBMP(filename, width, height, buffer2);
}

#pragma pack (1)
typedef struct {//size 26
	Bit32s dword_0;
	Bit32s dword_4;
	Bit8u* dword_8_data;
	Bit32u* dword_12x;//Bit32u*
	Bit32u* dword_16x;//Bit32u*
	Bit16s word_20;
	Bit16s word_22;
	Bit16s word_24;
	Bit8u data[];
	//Bit32u dword_26;
} type_x_DWORD_E9C28_str;

typedef struct {//lenght 28
	Bit32s dword_0;
	Bit32s dword_4;
	Bit32s dword_8;
	Bit16s word_12;
	Bit16u word_14;
	Bit16s word_16;
	Bit16s word_18;
	Bit16s word_20;
	Bit16s word_22;
	Bit16s word_24;
	Bit16s word_26;
}
type_animations1;

typedef struct {
	Bit16s word_0;
	type_animations1* dword_2;
}
type_E9C08;
#pragma pack (16)

__int16 x_WORD_E130C = 0; // weak
Bit8u* TMAPS00TAB_BEGIN_BUFFER;// ?? must set
char x_BYTE_E2A20 = 0; // weak
int x_DWORD_E1304 = 0; // weak
int x_DWORD_E1308 = 0; // weak
type_E9C08* x_DWORD_E9C08x;  // weak ?? must set
int x_DWORD_F66F0[504]; // idb//2c76f0 ?? must set
__int16 x_WORD_E130E = 0; // weak
__int16 x_WORD_E1310 = 0; // weak
__int16 x_WORD_E1312ar[2] = { 0,0 }; // weak
__int16 x_WORD_E1316 = 0; // weak
char x_BYTE_E29E8 = 1; // weak
Bit8u x_DWORD_17ECA0[4608]; // weak
type_x_DWORD_E9C28_str* x_DWORD_E9C28_str;

type_animations1* sub_724F0(type_E9C08* a1x, __int16 a2)
{
	//type_animations1* resultx; // eax
	int resulty;
	//int v3; // edx
	Bit16s v3x;
	//HIWORD(v3) = HIWORD(a1);
	//resultx = a1x->dword_2;
	resulty = 0;
	//LOWORD(v3) = *(x_WORD *)a1;
	v3x = a1x->word_0;
	if (!a1x->word_0)
		return 0;
	while (!a1x->dword_2[resulty].dword_4 || a2 != a1x->dword_2[resulty].word_26)
	{
		--v3x;
		//result += 28;
		//resultx++;
		resulty++;
		if (!v3x)
			return 0;
	}
	return &(a1x->dword_2[resulty]);
}

void sub_765FC(__int16 a1, __int16 a2)
{
	x_WORD_E130E = a1;
	x_WORD_E1310 = a2;
}

void qmemcpy(void* a, void* b, size_t c) {
	memcpy(a, b, c);
};

int sub_7677C()
{
	int result; // eax

	result = (unsigned __int16)x_WORD_E1312ar[0] - 6;
	x_DWORD_E1308 += result;
	return result;
}

__int16 sub_7678D()//animate sprite
{
	x_WORD* v0; // edi
	x_WORD* v1; // esi
	__int16 result; // ax
	x_BYTE* v3; // edi
	x_BYTE* v4; // esi
	char v5; // dl
	x_WORD* v6; // [esp-4h] [ebp-14h]
	x_WORD* v7; // [esp+0h] [ebp-10h]
	__int16 i; // [esp+6h] [ebp-Ah]
	__int16 v9; // [esp+8h] [ebp-8h]
	int v10; // [esp+Ch] [ebp-4h]

	v10 = (unsigned __int16)x_WORD_E130E;
	v0 = (x_WORD*)x_DWORD_E1304;
	v1 = (x_WORD*)(x_DWORD_E1308 + 2);
	v9 = *(x_WORD*)x_DWORD_E1308;
	do
	{
		while (1)
		{
			result = *v1;
			++v1;
			v7 = v0;
			if (result >= 0)
				goto LABEL_10;
			if (!(result & 0x4000))
				break;
			v0 = (x_WORD*)((char*)v0 + v10 * -result);
		}
		v6 = v0;
		v3 = (char*)v0 + v10 - 1;
		if (x_BYTE_E2A20 == 1)
		{
			if ((x_BYTE)result)
				* v3 = result;
		}
		else
		{
			*v3 = result;
		}
		v0 = v6;
	LABEL_10:
		for (i = result; i; i--)
		{
			while (1)
			{
				LOBYTE(result) = *(x_BYTE*)v1;
				v4 = (char*)v1 + 1;
				result = (unsigned __int8)result;
				v0 = (x_WORD*)((char*)v0 + (unsigned __int8)result);
				v5 = *v4;
				v1 = (x_WORD*)(v4 + 1);
				if (v5 > 0)
					break;
				result = *v1;
				++v1;
				do
				{
					*v0 = result;
					++v0;
					++v5;
				} while (v5);
				if (!--i)
					goto LABEL_18;
			}
			do
			{
				*v0 = *v1;
				++v1;
				++v0;
				--v5;
			} while (v5);
		}
	LABEL_18:
		v0 = (x_WORD*)((char*)v7 + v10);
		--v9;
	} while (v9);
	x_DWORD_E1308 = (int)v1;
	return result;
}

int sub_76840()
{
	x_BYTE* v0; // edi
	char* v1; // esi
	__int16 v2; // bx
	__int16 v3; // cx
	unsigned __int8 v4; // dl
	__int16 v5; // ax
	char v6; // dh
	char v7; // dh
	int result; // eax
	int v9; // [esp-8h] [ebp-Ch]
	int v10; // [esp-4h] [ebp-8h]
	__int16 v11; // [esp+2h] [ebp-2h]

	v11 = x_WORD_E130E;
	v0 = (x_BYTE*)x_DWORD_E1304;
	v1 = (char*)x_DWORD_E1308;
	v10 = (unsigned __int16)x_WORD_E1312ar[0] - 6;
	v9 = x_DWORD_E1308;
	v2 = x_WORD_E1310;
	do
	{
		++v1;
		v3 = v11;
		do
		{
			v4 = *v1++;
			if ((char)v4 > 0)
			{
				v3 -= v4;
			}
			else
			{
				HIBYTE(v5) = -1;
				LOBYTE(v5) = v4;
				v3 += v5;
			}
			if ((char)v4 > 0)
			{
				v7 = *v1++;
				do
				{
					if (x_BYTE_E2A20 == 1)
					{
						if (v7)
							* v0 = v7;
					}
					else
					{
						*v0 = v7;
					}
					++v0;
					--v4;
				} while (v4);
			}
			else
			{
				do
				{
					v6 = *v1++;
					if (x_BYTE_E2A20 == 1)
					{
						if (v6)
							* v0 = v6;
					}
					else
					{
						*v0 = v6;
					}
					++v0;
					++v4;
				} while (v4);
			}
		} while (v3);
		--v2;
	} while (v2);
	result = v10;
	x_DWORD_E1308 = v10 + v9;
	return result;
}

void sub_766A4()//animate sprite
{
	__int16 result; // ax

	qmemcpy(&x_WORD_E1316, (void*)x_DWORD_E1308, sizeof(x_WORD_E1316));
	x_DWORD_E1308 += 2;
	x_DWORD_E1308 += 8;
	while (x_WORD_E1316)
	{
		--x_WORD_E1316;
		qmemcpy(x_WORD_E1312ar, (void*)x_DWORD_E1308, 4u);
		x_DWORD_E1308 += 4;
		qmemcpy(&x_WORD_E130C, (void*)x_DWORD_E1308, sizeof(x_WORD_E130C));
		x_DWORD_E1308 += 2;
		switch (x_WORD_E130C)
		{
		case 7:
			result = sub_7678D();
			break;
		case 4:
			result = sub_7677C();
			break;
		case 15:
			result = sub_76840();
			break;
		default:
			result = sub_7677C();
			break;
		}
	}
	//return result;
}

__int16 sub_76752()
{
	__int16 result; // ax

	x_WORD_E130E = *(x_WORD*)(x_DWORD_E1308 + 2);
	result = *(x_WORD*)(x_DWORD_E1308 + 4);
	x_WORD_E1310 = *(x_WORD*)(x_DWORD_E1308 + 4);
	x_DWORD_E1308 += 6;
	return result;
}

int sub_76619(int a1, int a2)//animate sprite
{
	x_DWORD_E1308 = a1;
	x_DWORD_E1304 = a2;
	while (1)
	{
		qmemcpy(x_WORD_E1312ar, (void*)x_DWORD_E1308, 4u);
		x_DWORD_E1308 += 4;
		qmemcpy(&x_WORD_E130C, (void*)x_DWORD_E1308, sizeof(x_WORD_E130C));
		x_DWORD_E1308 += 2;
		if (x_WORD_E130C != -20718)
			break;
		sub_76752();
	}
	if (x_WORD_E130C != -3590)
		return 0;
	sub_766A4();
	return x_DWORD_E1308;
}

int sub_72350(type_animations1* a1x)//253350 //animates sprite
{
	int v1; // esi
	int v2; // edi
	int v3; // eax
	__int16 v4; // cx
	int result; // eax

	if (a1x->word_22 > a1x->word_16)
	{
		a1x->word_22 = 1;
		a1x->dword_8 = a1x->word_14;
	}
	v1 = *(x_DWORD*)a1x->dword_4;
	v2 = a1x->dword_8;
	sub_765FC(a1x->word_18, 0);
	v1 += 6;
	v3 = sub_76619(v1 + v2, v1);
	v4 = a1x->word_22;
	result = v3 - v1;
	a1x->dword_8 = result;
	a1x->word_22 = v4 + 1;
	return result;
}



int sub_715B0()//2525b0
{
	unsigned __int16 v0; // bx
	int result; // eax
	int* v2; // ecx
	char v3; // cl
	char v4; // cl
	type_animations1* v5x; // eax
	__int16 v6; // si
	unsigned __int16 i; // bx
	int* v8; // edi
	char v9; // cl
	type_animations1* v10x; // eax

	v0 = 0;
	do
	{
		result = 4 * v0;
		v2 = *(int**)((char*)x_DWORD_F66F0 + result);
		if (v2)
		{
			result = *v2;
			v3 = *(x_BYTE*)* v2;
			if (v3 & 8)
			{
				if (v3 & 0x20)
				{
					result = 5 * v0;
					v6 = *(x_WORD*)(TMAPS00TAB_BEGIN_BUFFER + 10 * v0 + 8);
					for (i = *(x_WORD*)(TMAPS00TAB_BEGIN_BUFFER + 10 * v0 + 8); i < 0x1F8u; i++)
					{
						result = 5 * i;
						if (v6 != *(x_WORD*)(TMAPS00TAB_BEGIN_BUFFER + 10 * i + 8))
							break;
						result = 4 * i;
						v8 = (int*)x_DWORD_F66F0[i];
						if (v8)
						{
							result = *v8;
							v9 = *(x_BYTE*)* v8 & 0xF7;
							*(x_BYTE*)* v8 = v9;
							if (v9 & 1)
							{
								v10x = sub_724F0(x_DWORD_E9C08x, i);
								result = sub_72350(v10x);
							}
						}
					}
					v0 = i - 1;
				}
				else
				{
					v4 = v3 & 0xF7;
					*(x_BYTE*)result = v4;
					if (v4 & 1)
					{
						v5x = sub_724F0(x_DWORD_E9C08x, v0);
						result = sub_72350(v5x);
					}
				}
			}
		}
		v0++;
	} while (v0 < 0x1F8u);
	return result;
}

void sub_83CC0(char a1)//264cc0
{
	//char result; // al

	//result = a1;
	x_BYTE_E29E8 = a1;
	//  return result;
}
void* sub_83CD0_malloc2(size_t a1)//264cd0
{
	return malloc(a1);
}
int sub_84000(int a1)//265000
{
	int result; // eax
	int v2; // edx
	int v3; // ebx

	result = a1;
	if (*(x_DWORD*)(a1 + 12))
	{
		*(x_BYTE*)(a1 + 16) = 0;
		v2 = *(x_DWORD*)(a1 + 12);
		if (*(x_BYTE*)(a1 + 17) == *(x_BYTE*)(v2 + 17) && !*(x_BYTE*)(v2 + 16))
		{
			v3 = *(x_DWORD*)(a1 + 8);
			if (v3)
				* (x_DWORD*)(v3 + 12) = v2;
			*(x_DWORD*)(*(x_DWORD*)(a1 + 12) + 8) = *(x_DWORD*)(a1 + 8);
			*(x_DWORD*)(*(x_DWORD*)(a1 + 12) + 4) += *(x_DWORD*)(a1 + 4);
			*(x_DWORD*)(a1 + 4) = 0;
		}
	}
	return result;
}
void sub_83E80_freemem4(Bit8u* a1)//264e80
{
	Bit8u* v1; // eax
	char v2; // bl
	int* i; // ebx

	if (a1)
	{
		v1 = (Bit8u*)& x_DWORD_17ECA0;
		v2 = 0;
		while (v1)
		{
			if ((int)a1 == v1[0])//fix
			{
				v2 = 1;
				*((x_BYTE*)v1 + 16) = 0;
				break;
			}
			v1 = (Bit8u*)(int*)v1[2];
		}
		if (v2 == 1)
		{
			for (i = (int*)& x_DWORD_17ECA0; i; i = (int*)i[2])
			{
				if (!*((x_BYTE*)i + 16))
					sub_84000((int)i);
			}
		}
		//sub_85350(); //fix
	}
}
type_E9C08* sub_72120(unsigned __int16 a1)//253120
{
	unsigned __int16 v1; // di
	type_E9C08* v2x; // esi
	type_E9C08* v3x; // ebx
	type_animations1* v4; // eax
	//int v5; // eax

	v1 = a1;
	sub_83CC0(9);
	v2x = (type_E9C08*)sub_83CD0_malloc2(6);
	v3x = v2x;
	if (v2x && (v4 = (type_animations1*)sub_83CD0_malloc2(28 * a1), (v2x->dword_2 = v4) != 0))
	{
		v2x->word_0 = a1;
		while (--v1 != 0xffff)
		{
			//v5 = 7 * v1;
			//*(x_DWORD *)(v2x->dword_2 + 4 * v5 + 4) = 0;
			//*(x_DWORD *)(v2x->dword_2 + 4 * v5) = 0;
			v2x->dword_2[v1].dword_4 = 0;
			v2x->dword_2[v1].dword_0 = 0;
		}
	}
	else if (v2x)
	{
		sub_83E80_freemem4((Bit8u*)v2x);
		v3x = 0;
	}
	sub_83CC0(10);
	return v3x;
}




signed int sub_71CD0(type_x_DWORD_E9C28_str* a1y)//252cd0
{
	int i; // edx

	for (i = 0; (signed __int16)i < (signed int)a1y->word_22; i++)
	{
		if (!*(x_DWORD*)(14 * (signed __int16)i + a1y->dword_8_data + 4))
			return i;
	}
	return -1;
}

Bit8u* sub_71E70(type_x_DWORD_E9C28_str* a1y, unsigned int a2, __int16 a3)//252e70
{
	signed __int16 v3; // si
	signed __int16 v4; // ax
	signed __int16 v5; // dx
	int v6; // ecx
	int v7; // eax
	Bit8u* result; // eax

	v3 = -1;
	if (a2 < a1y->dword_4)
	{
		v4 = sub_71CD0(a1y);
		v5 = v4;
		v6 = v4;
		v3 = v4;
		if (v4 > -1)
		{
			v7 = 14 * v4;
			*(x_WORD*)(a1y->dword_8_data + v7 + 10) = v5;
			*(x_DWORD*)(a1y->dword_8_data + v7 + 4) = a2;
			*(x_DWORD*)(a1y->dword_8_data + v7) = a1y->dword_0 + (int)a1y->dword_16x - a1y->dword_4;
			*(x_WORD*)(a1y->dword_8_data + v7 + 12) = a3;
			a1y->dword_4 -= a2;
			*(x_WORD*)(a1y->dword_8_data + v7 + 8) = a1y->word_20;
			*(x_DWORD*)(a1y->dword_12x + (unsigned __int16)(a1y->word_20)++) = (Bit32u)a1y->dword_8_data + 14 * v6;
		}
	}
	if (v3 <= -1)
		result = 0;
	else
		result = 14 * v3 + a1y->dword_8_data;
	return result;
}


type_x_DWORD_E9C28_str* sub_71B40(int a1, unsigned __int16 a2, type_x_DWORD_E9C28_str* a3y)//252b40
{
	unsigned __int16 v3; // di
	int v4; // eax
	int v5; // eax
	int v6; // edx
	type_x_DWORD_E9C28_str* v7y; // esi
	//int v8; // eax
	type_x_DWORD_E9C28_str* v10y; // ebx
	Bit8u* v11x; // eax
	Bit8u* v12x; // eax
	Bit8u* v13x; // eax
	int v14; // [esp+0h] [ebp-Ch]

	v3 = a2;
	if (a3y)
	{
		v4 = 14 * a2;
		v14 = v4;
		v5 = 4 * a2 + v4 + 26;
		v6 = a1 - v5;
		if (a1 == v5)
			return 0;
		a3y->word_20 = 0;
		a3y->word_24 = 2;
		a3y->dword_0 = v6;
		a3y->dword_4 = v6;
		v7y = a3y;
		a3y->word_22 = a2;
		a3y->dword_8_data = a3y->data;
		//v8 = v14 + a3x + 26;
		a3y->dword_12x = (Bit32u*)& a3y->data[v14];// (Bit32u)(v14 + (Bit8u*)a3y + 26);//must fix for 64 bit version - data
		a3y->dword_16x = (Bit32u*)& a3y->data[v14 + 4 * a2];//(Bit32u)(4 * a2 + (v14 + (Bit8u*)a3y + 26));//must fix for 64 bit version - data
		while (--v3 != 0xffff)
			* (x_DWORD*)(a3y->dword_8_data + 14 * v3 + 4) = 0;
	}
	else
	{
		v10y = (type_x_DWORD_E9C28_str*)sub_83CD0_malloc2(26);
		v7y = v10y;
		if (!v10y
			|| (v11x = (Bit8u*)sub_83CD0_malloc2(a1), (v10y->dword_16x = (Bit32u*)v11x) == 0)
			|| (v12x = (Bit8u*)sub_83CD0_malloc2(14 * a2), (v10y->dword_8_data = (Bit8u*)v12x) == 0)
			|| (v13x = (Bit8u*)sub_83CD0_malloc2(4 * a2), (v10y->dword_12x = (Bit32u*)v13x) == 0))
		{
			if (v10y)
			{
				if (v10y->dword_16x)
				{
					if (v10y->dword_8_data)
						sub_83E80_freemem4((Bit8u*)v10y->dword_8_data);
					sub_83E80_freemem4((Bit8u*)v10y->dword_16x);
				}
				sub_83E80_freemem4((Bit8u*)v10y);
			}
			exit(1);
		}
		v10y->word_20 = 0;
		v10y->word_24 = 1;
		v10y->word_22 = a2;
		v10y->dword_0 = a1;
		v10y->dword_4 = a1;
		while (--v3 != 0xffff)
			* (x_DWORD*)(v10y->dword_8_data + 14 * v3 + 4) = 0;
	}
	return v7y;
}


type_animations1* sub_721C0_initTmap(type_E9C08* a1x, int* a2, __int16 a3)//2531c0
{
	signed __int16 v3; // cx
	signed __int16 v4; // si
	signed __int16 i; // bx
	//x_DWORD *v6; // edx
	type_animations1* v6x;
	int v7; // ebx
	int v8; // ecx
	__int16 v9; // ST08_2
	//int v10; // edx
	signed __int16 v12; // [esp+Ch] [ebp-4h]

	v3 = -1;
	v4 = -1;
	if (!(a1x->word_0))
		return 0;
	for (i = 0; i < a1x->word_0; i++)
	{
		v6x = &a1x->dword_2[i];
		if (v6x->dword_4)
		{
			if (!v6x->dword_0)
				v4 = i;
		}
		else
		{
			v3 = i;
		}
	}
	v12 = v3 <= 0 ? v4 : v3;
	if (v12 <= -1)
		return 0;
	v7 = *a2;
	v8 = *(unsigned __int16*)(*a2 + 4) * *(unsigned __int16*)(*a2 + 2);
	v9 = *(x_WORD*)(v8 + *a2 + 6);
	//v10 = 28 * v12;
	a1x->dword_2[v12].dword_4 = (x_DWORD)a2;
	a1x->dword_2[v12].word_12 = 6;
	a1x->dword_2[v12].word_14 = v8 + 6;
	a1x->dword_2[v12].word_16 = v9;
	a1x->dword_2[v12].word_18 = *(x_WORD*)(v7 + 2);
	a1x->dword_2[v12].word_20 = *(x_WORD*)(v7 + 4);
	a1x->dword_2[v12].dword_8 = v8 + 6;
	a1x->dword_2[v12].word_22 = 1;
	a1x->dword_2[v12].dword_0 = 1;
	a1x->dword_2[v12].word_24 = v12;
	a1x->dword_2[v12].word_26 = a3;
	//return v10 + a1x->dword_2;
	return &a1x->dword_2[v12];
}


#define write_data
//#define write_rnc
//#define write_bmp
#define write_png
//#define write_alphapng



int main()
{
	FILE* fptrTMAPSdata;
	fopen_s(&fptrTMAPSdata, tmapsdatfilename, "rb");
	fseek(fptrTMAPSdata, 0L, SEEK_END);
	long sz = ftell(fptrTMAPSdata);
	fseek(fptrTMAPSdata, 0L, SEEK_SET);
	Bit8u* contentTMAPSdat = (Bit8u*)malloc(sz * sizeof(char*));
	fread(contentTMAPSdat, sz, 1, fptrTMAPSdata);
	fclose(fptrTMAPSdata);

	FILE* fptrTMAPStab;
	fopen_s(&fptrTMAPStab, tmapstabfilename, "rb");
	fseek(fptrTMAPStab, 0L, SEEK_END);
	long sztab = ftell(fptrTMAPStab);
	fseek(fptrTMAPStab, 0L, SEEK_SET);
	Bit8u* contentTMAPStab = (Bit8u*)malloc(sztab * sizeof(char*));
	fread(contentTMAPStab, sztab, 1, fptrTMAPStab);
	fclose(fptrTMAPStab);

	Bit8u buffer[100000];
	Bit8u prevbuffer[100000];

	int indextab = 0;
	int index = 0;

	int dword_0xE6_heapsize_230 = 0x400000;
	Bit8u* pointer_0xE2_heapbuffer_226 = (Bit8u*)sub_83CD0_malloc2(dword_0xE6_heapsize_230);
	x_DWORD_E9C28_str = sub_71B40(dword_0xE6_heapsize_230, 0x1F8u, (type_x_DWORD_E9C28_str*)pointer_0xE2_heapbuffer_226);
	TMAPS00TAB_BEGIN_BUFFER = contentTMAPStab;
	x_DWORD_E9C08x = sub_72120(0x1F8u);

	char outname[512];
	char title[512];
	while (index < 504)
	{
		if (index == 77)
		{
			index++;
			index--;
		}
		//int size = *(Bit32u*)&contentTMAPStab[indextab];
		int shift = *(Bit32u*)& contentTMAPStab[indextab + 4];
		//if (shift > 500)shift = shift - 500;
		Bit8u* stmpdat = &contentTMAPSdat[shift];

		while ((*(Bit32u*)stmpdat) != 0x1434e52) { shift++; stmpdat = &contentTMAPSdat[shift + 1]; }

		Bit32u size = stmpdat[11] + (stmpdat[10] << 8) + (stmpdat[9] << 16) + (stmpdat[8] << 24) + 12;
		Bit32u unpacksize = stmpdat[7] + (stmpdat[6] << 8) + (stmpdat[5] << 16) + (stmpdat[4] << 24);

#ifdef write_rnc
		FILE* fptw;
		char filename[300];
		sprintf_s(filename, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i.rnc", tmapsstr, index);
		fopen_s(&fptw, filename, "wb");
		fwrite(&contentTMAPSdat[shift], size, 1, fptw);
		fclose(fptw);
#endif

		int decompsize = *(Bit32u*)& contentTMAPSdat[shift + 6];
		sub_5C3D0_file_decompress(&contentTMAPSdat[shift], buffer);



		Bit8u* index2 = 10 * index + TMAPS00TAB_BEGIN_BUFFER;
		x_DWORD_F66F0[index] = (int)sub_71E70(x_DWORD_E9C28_str, (unsigned __int16)(4 * ((unsigned int)(*(x_DWORD*)index2 + 13) >> 2)), index);
		int index6 = x_DWORD_F66F0[index];
		Bit8u** subpointer = (Bit8u * *)x_DWORD_F66F0[index];
		*subpointer = (Bit8u*)malloc(unpacksize);
		memcpy(*subpointer, buffer, unpacksize);

		if (**(x_BYTE * *)index6 & 1)
			/*index = */sub_721C0_initTmap(x_DWORD_E9C08x, (int*)index6, index);
		/*
				if (**(x_BYTE **)index6 & 1)
			index = sub_721C0_initTmap((unsigned __int16*)x_DWORD_E9C08, (int*)index6, index);
		*/



		int width = *(Bit16u*)& buffer[2];
		int height = *(Bit16u*)& buffer[4];

#ifdef write_data
		FILE* fptw2;
		char filenamedata[300];
		sprintf_s(filenamedata, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-00.data", tmapsstr, index);
		fopen_s(&fptw2, filenamedata, "wb");
		fwrite(buffer, unpacksize, 1, fptw2);
		fclose(fptw2);
#endif


#ifdef write_bmp
		sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-00.bmp", tmapsstr, index);
		write_posistruct_to_bmp(buffer + 6, width, height, outname);//test write
#endif

#ifdef write_png
		sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-00.png", tmapsstr, index);

		sprintf_s(title, "%s%03i", tmapsstr, index);
		write_posistruct_to_png(buffer + 6, width, height, outname, title, 0);//test write
#endif
#ifdef write_alphapng
		sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-alpha-00.png", tmapsstr, index);
		sprintf_s(title, "%s%03i", tmapsstr, index);
		write_posistruct_to_png(buffer + 6, width, height, outname, title, 1);//test write
#endif

#ifdef level4
		if (index < 452)
#endif
			indextab += 10;
		index++;
	}

	for(int mainindex=0; mainindex<40; mainindex++)
	{
		

		index = 0;
		while (index < 504) {
			Bit8u* subpointer = *(Bit8u * *)x_DWORD_F66F0[index];
			subpointer[0] |= 8;
			index++;
		}

		sub_715B0();
		//image2
		indextab = 0;
		index = 0;
		while (index < 504)
		{

			Bit8u* subpointer = *(Bit8u * *)x_DWORD_F66F0[index];
			//memcpy(buffer, subpointer, unpacksize);

			//int shift = *(Bit32u*)&contentTMAPStab[indextab + 4];
			Bit8u* stmpdat = &subpointer[0/*shift*/];
			//Bit32u size = stmpdat[11] + (stmpdat[10] << 8) + (stmpdat[9] << 16) + (stmpdat[8] << 24) + 12;
			//Bit32u unpacksize = stmpdat[7] + (stmpdat[6] << 8) + (stmpdat[5] << 16) + (stmpdat[4] << 24);

			int width = *(Bit16u*)& stmpdat[2];
			int height = *(Bit16u*)& stmpdat[4];

			memcpy(buffer, stmpdat, width * height + 6);
			/*
			FILE* fptw;
			char filename[300];
			sprintf_s(filename, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-0-%03i.rnc", index);
			fopen_s(&fptw, filename, "wb");
			fwrite(&contentTMAPSdat[shift], size, 1, fptw);
			fclose(fptw);

			int decompsize = *(Bit32u*)&contentTMAPSdat[shift + 6];
			sub_5C3D0_file_decompress(&contentTMAPSdat[shift], buffer);



			Bit8u* index2 = 10 * index + TMAPS00TAB_BEGIN_BUFFER;
			x_DWORD_F66F0[index] = (int)sub_71E70(x_DWORD_E9C28_str, (unsigned __int16)(4 * ((unsigned int)(*(x_DWORD*)index2 + 13) >> 2)), index);
			Bit8u* subpointer = *(Bit8u * *)x_DWORD_F66F0[index];
			subpointer = (Bit8u*)malloc(unpacksize);
			memcpy(subpointer, buffer, unpacksize);
			*/

	#ifdef write_data

			FILE* fptw2_prev;
			char filenamedata[300];
			sprintf_s(filenamedata, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-00.data", tmapsstr, index);
			fopen_s(&fptw2_prev, filenamedata, "rb");
			fread(prevbuffer, width * height + 6, 1, fptw2_prev);
			fclose(fptw2_prev);

			bool same = true;
			for (int kk = 0; kk < width * height + 6; kk++)
			{
				if (buffer[kk] != prevbuffer[kk])
					same = false;
			}
			if (same)
			{
				index++; continue;
			}

			//FILE* fptw2_prev;
			//char filenamedata[300];
			sprintf_s(filenamedata, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-%02i.data", tmapsstr, index, mainindex);
			fopen_s(&fptw2_prev, filenamedata, "rb");
			if(fptw2_prev==NULL)
			{
				index++; continue;
			}
			fread(prevbuffer, width* height + 6, 1, fptw2_prev);
			fclose(fptw2_prev);

			same = true;
			for (int kk = 0; kk < width * height + 6; kk++)
			{
				if (buffer[kk] != prevbuffer[kk])
					same = false;
			}
			if (same)
			{
				index++; continue;
			}


			FILE* fptw2;
			sprintf_s(filenamedata, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-%02i.data", tmapsstr, index, mainindex+1);
			fopen_s(&fptw2, filenamedata, "wb");
			fwrite(buffer, width * height + 6, 1, fptw2);
			fclose(fptw2);
	#endif

	#ifdef write_bmp
			char outname[512];
			sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-%02i.bmp", tmapsstr, index, mainindex + 1);
			write_posistruct_to_bmp(buffer + 6, width, height, outname);//test write
	#endif

	#ifdef write_png
			sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-%02i.png", tmapsstr, index, mainindex+1);
			sprintf_s(title, "%s%03i", tmapsstr, index);
			write_posistruct_to_png(buffer + 6, width, height, outname, title, 0);//test write
	#endif
	#ifdef write_alphapng
			sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\%s%03i-alpha-%02i.png", tmapsstr, index, mainindex + 1);
			sprintf_s(title, "%s%03i", tmapsstr, index);
			write_posistruct_to_png(buffer + 6, width, height, outname, title, 1);//test write
	#endif

	#ifdef level4
			if (index < 452)
	#endif
				indextab += 10;
			index++;
		}



}

	/*
	sub_715B0();

	indextab = 0;
	index = 0;
	while (index < 504)
	{

		Bit8u* subpointer = *(Bit8u * *)x_DWORD_F66F0[index];
		//memcpy(buffer, subpointer, unpacksize);

		//int shift = *(Bit32u*)&contentTMAPStab[indextab + 4];
		Bit8u* stmpdat = &subpointer[0];
		//Bit32u size = stmpdat[11] + (stmpdat[10] << 8) + (stmpdat[9] << 16) + (stmpdat[8] << 24) + 12;
		//Bit32u unpacksize = stmpdat[7] + (stmpdat[6] << 8) + (stmpdat[5] << 16) + (stmpdat[4] << 24);

		int width = *(Bit16u*)& stmpdat[2];
		int height = *(Bit16u*)& stmpdat[4];

		memcpy(buffer, stmpdat, width * height + 6);

#ifdef write_data
		FILE* fptw2;
		char filenamedata[300];
		sprintf_s(filenamedata, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-0-%03i-3.data", index);
		fopen_s(&fptw2, filenamedata, "wb");
		fwrite(buffer, width * height + 6, 1, fptw2);
		fclose(fptw2);
#endif

#ifdef write_bmp
		char outname[512];
		sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-0-%03i-3.bmp", index);
		write_posistruct_to_bmp(buffer + 6, width, height, outname);//test write
#endif

#ifdef write_png
		sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-0-%03i-3.png", index);
		sprintf_s(title, "TMAPS2-0-%03i", index);
		write_posistruct_to_png(buffer + 6, width, height, outname, title, 0);//test write
#ifdef write_alphapng
#endif
		sprintf_s(outname, "c:\\prenos\\remc2\\tools\\decompressTMAPS\\out\\TMAPS2-0-%03i-alpha-3.png", index);
		sprintf_s(title, "TMAPS2-0-%03i", index);
		write_posistruct_to_png(buffer + 6, width, height, outname, title, 1);//test write
#endif

		indextab += 10;
		index++;
	}
	*/
	return 0;
}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
