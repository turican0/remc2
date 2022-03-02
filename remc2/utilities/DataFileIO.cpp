#include "DataFileIO.h"

#ifndef _countof
#define _countof(arr) sizeof(arr) / sizeof(arr[0])
//dirty
#endif

uint16_t DataFileIO::crc_table[256] = {
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

DataFileIO::DataFileIO() {};

int DataFileIO::ReadFileAndDecompress(const char* path, uint8_t** data)
{
	int result = 0; // eax // length of file or 0 on error
	FILE* file; // ebx
	uint32_t length; // esi
	file = CreateOrOpenFile((char*)path, 0x200);

	if (file)
	{
		length = FileLengthBytes(file);
		Read(file, *data, length);
		Close(file);
		result = (int)Decompress(*data, *data);
		if (result >= 0)
		{
			if (!result)
				result = length;
		}
		else
		{
			myprintf("ERROR decompressing %s\n");
			result = -2;
		}
	}
	return result;
}

int DataFileIO::Decompress(uint8_t* src, uint8_t* a2) {
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
	v->input = (uint8_t*)malloc(MAX_BUF_SIZE);

	uint32_t signature = src[0] + (src[1] << 8) + (src[2] << 16);
	if (signature == 0x434e52)
	{
		uint32_t inputsize = src[11] + (src[10] << 8) + (src[9] << 16) + (src[8] << 24);
		memmove(v->input, src, inputsize + 0x12);
	}
	else
	{
		v->input[0] = 0;
	}
	v->output = a2;
	v->temp = (uint8_t*)malloc(MAX_BUF_SIZE);

	int error_code = 0;
	error_code = Unpack(v);

	if (v->pus_mode == 2)
	{
		free(v->input);
		free(v->output);
		free(v->temp);
		free(v->input);
		free(v);

		return error_code;
	}

	free(v->input);
	free(v->temp);
	free(v);
	if (error_code == -6)return 0;
	return error_code;
}

DataFileIO::vars_t* DataFileIO::init_vars()
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

int DataFileIO::UnpackData(vars_t* v)
{
	int start_pos = v->input_offset;

	uint32_t sign = read_dword_be(v->input, &v->input_offset);
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

	v->mem1 = (uint8_t*)malloc(0x10000);
	v->decoded = (uint8_t*)malloc(0x10000);
	v->pack_block_start = &v->mem1[0xFFFD];
	v->window = &v->decoded[v->dict_size];

	v->unpacked_crc_real = 0;
	v->bit_count = 0;
	v->bit_buffer = 0;
	v->processed_size = 0;

	uint16_t specified_key = v->enc_key;

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

int DataFileIO::Unpack(vars_t* v)
{
	v->packed_size = v->file_size;

	if (v->file_size < RNC_HEADER_SIZE)
		return 6;

	int result = UnpackData(v); // data
	if (result == 0)return v->input_size;
	return -result;
}

FILE* DataFileIO::CreateOrOpenFile(char* pathname, int __pmode)
{
	FILE* file; // ST10_4


	if (__pmode == 0x222)
	{
		file = CreateFile(pathname, 0x1c0);
		//x_setmode(v2, 0x200);
		Close(file);
	}
	return Open(pathname, __pmode, 0x40);
}

FILE* DataFileIO::CreateFile(char* path, uint32_t flags) 
{
	return mycreate(path, flags);
}

FILE* DataFileIO::Open(char* path, int pmode, uint32_t flags) {
	return myopen(path, pmode, flags);
}

int32_t DataFileIO::Close(FILE* file) {
	return myclose(file);
}

int32_t DataFileIO::Seek(FILE* file, x_DWORD position, char type) {
	return mylseek(file, position, type);
}

size_t DataFileIO::Read(FILE* file, uint8_t* data, uint32_t length) {
	size_t result = fread(data, 1, length, file);
#ifdef _DEBUG
	debug_printf("Read fread length %d result %d\n", length, result);
#endif
	return result;
};

long DataFileIO::FileLengthBytes(FILE* file) 
{
	long size;
	myfseek(file, 0, SEEK_END); // seek to end of file
	size = myftell(file); // get current file pointer
	myfseek(file, 0, SEEK_SET); // seek back to beginning of file
	return size;
}

uint8_t DataFileIO::read_byte(uint8_t* buf, size_t* offset)
{
	return buf[(*offset)++];
}

uint16_t DataFileIO::read_word_be(uint8_t* buf, size_t* offset)
{
	uint8_t b1 = read_byte(buf, offset);
	uint8_t b2 = read_byte(buf, offset);

	return (b1 << 8) | b2;
}

uint32_t DataFileIO::read_dword_be(uint8_t* buf, size_t* offset)
{
	uint16_t w1 = read_word_be(buf, offset);
	uint16_t w2 = read_word_be(buf, offset);

	return (w1 << 16) | w2;
}

int DataFileIO::unpack_data_m1(vars_t* v)
{
	while (v->processed_size < v->input_size)
	{
		make_huftable(v, v->raw_table, _countof(v->raw_table));
		make_huftable(v, v->len_table, _countof(v->len_table));
		make_huftable(v, v->pos_table, _countof(v->pos_table));

		int subchunks = input_bits_m1(v, 16);

		while (subchunks--)
		{
			uint32_t data_length = decode_table_data(v, v->raw_table);
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

int DataFileIO::unpack_data_m2(vars_t* v)
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
						uint32_t data_length = (input_bits_m2(v, 4) << 2) + 12;
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

void DataFileIO::decode_match_count(vars_t* v)
{
	v->match_count = input_bits_m2(v, 1) + 4;

	if (input_bits_m2(v, 1))
		v->match_count = ((v->match_count - 1) << 1) + input_bits_m2(v, 1);
}

void DataFileIO::decode_match_offset(vars_t* v)
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

uint16_t DataFileIO::crc_block(uint8_t* buf, size_t offset, int size)
{
	uint16_t crc = 0;

	while (size--)
	{
		crc ^= read_byte(buf, &offset);
		crc = (crc >> 8) ^ crc_table[crc & 0xFF];
	}

	return crc;
}

uint32_t DataFileIO::input_bits_m1(vars_t* v, short count)
{
	uint32_t bits = 0;
	uint32_t prev_bits = 1;

	while (count--)
	{
		if (!v->bit_count)
		{
			uint8_t b1 = read_source_byte(v);
			uint8_t b2 = read_source_byte(v);
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

uint32_t DataFileIO::input_bits_m2(vars_t* v, short count)
{
	uint32_t bits = 0;

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

int DataFileIO::input_bits(vars_t* v, short count)
{
	if (v->method != 2)
		return input_bits_m1(v, count);
	else
		return input_bits_m2(v, count);
}

void DataFileIO::write_decoded_byte(vars_t* v, uint8_t b)
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

void DataFileIO::write_buf(uint8_t* dest, size_t* offset, uint8_t* source, int size)
{
	memmove(&dest[*offset], source, size);
	*offset += size;
}

uint8_t DataFileIO::read_source_byte(vars_t* v)
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

void DataFileIO::read_buf(uint8_t* dest, uint8_t* source, size_t* offset, int size)
{
	memmove(dest, &source[*offset], size);
	*offset += size;
}

void DataFileIO::ror_w(uint16_t* x)
{
	if (*x & 1)
		*x = 0x8000 | (*x >> 1);
	else
		*x >>= 1;
}

uint32_t  DataFileIO::inverse_bits(uint32_t value, int count)
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

void  DataFileIO::proc_20(huftable_t* data, int count)
{
	int val = 0;
	uint32_t div = 0x80000000;
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

void  DataFileIO::make_huftable(vars_t* v, huftable_t* data, int count)
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

uint32_t DataFileIO::decode_table_data(vars_t* v, huftable_t* data)
{
	uint32_t i = 0;

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

void DataFileIO::clear_table(huftable_t* data, int count)
{
	for (int i = 0; i < count; ++i)
	{
		data[i].l1 = 0;
		data[i].l2 = 0xFFFF;
		data[i].l3 = 0;
		data[i].bit_depth = 0;
	}
}