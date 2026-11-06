#pragma once

#include <cstdint>
#include <string>

// MC1 install directory C:\CARPET.CD (in the port <MC1 data>/CARPET.CD) and the files copied there from the CD

std::string Mc1InstallPath();
int16_t Access_3EC80_3EFC0(const char* path);
uint8_t CreateInstallDirectory_3EC90_3EFD0(char drive, const char* basePath, const char* subDirectory);
bool FileMissing_3EF20_3F260(const char* targetPath, const char* sourcePath);
bool PathExists_3EF30_3F270(const char* path);
uint8_t CopyDataFiles_3EF50_3F290(const char* sourceDirectory, const char* targetDirectory, const char* name);
void SetupInstallFiles_3F1D0_3F510();
