#ifndef fcaseopen_h
#define fcaseopen_h

#include <stdio.h>
#include <string>
#include <vector>

FILE* fcaseopen(char const* path, char const* mode);

static bool our_iequals(const std::string& a, const std::string& b);

std::vector<std::string> GetTokensFromPath(const std::string &path);
#if !defined(_WIN32)
std::string casepath(const std::string &path);
#endif

#endif