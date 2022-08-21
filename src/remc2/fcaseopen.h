#ifndef fcaseopen_h
#define fcaseopen_h

#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

FILE* fcaseopen(const std::filesystem::path& path, char const* mode);

std::vector<std::string> GetTokensFromPath(const std::filesystem::path& path);
#if !defined(_WIN32)
std::string casepath(const std::filesystem::path& path);
#endif

#endif
