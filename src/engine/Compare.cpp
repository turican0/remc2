#include "Compare.h"

std::string regressionsDataPath = "c:/prenos/remc2-development/x64/Debug/regressions/";


uint32_t HashFromStr(char* name, int plus) {
	int index = 0;
	uint32_t result = 0;
	while (name[index])
	{
		result += name[index];
		result <<= 3;
		index++;
	}
	result += plus;
	return result;
}

char bufferf[512];
void SaveCompare(char* name, int value, int len, uint8_t* sequence)
{	
	uint32_t locIndex=getcompindex(HashFromStr(name,0));

	if (len == -1)
	{
		len = 4;
		sequence = (uint8_t*)&value;
	}
	sprintf(bufferf, "%s%s.dat", regressionsDataPath.c_str(), name);
	FILE* file;
	if(locIndex==0)
		file = fopen(bufferf, "wb");
	else
		file = fopen(bufferf, "ab");
	fwrite(sequence, 1, len, file);
	fclose(file);
};
void CompareWith(char* name, int value, int len, uint8_t* sequence)
{
	uint32_t locIndex = getcompindex(HashFromStr(name, 0));
	uint8_t compBuffer[100000];
	if (len == -1)
	{
		len = 4;
		sequence = (uint8_t*)&value;
	}
	sprintf(bufferf, "%s%s.dat", regressionsDataPath.c_str(), name);
	FILE* file = fopen(bufferf, "rb");
	fseek(file, len * locIndex, SEEK_SET);
	fread(compBuffer, 1, len, file);
	fclose(file);
	for (int i = 0; i < len; i++)
	{
		if (compBuffer[i] != sequence[i])
		{
			allert_error();
		}
	}
};