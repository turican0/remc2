#include "fcaseopen.h"

static bool our_iequals(const std::string& a, const std::string& b) {
	if (a.size() != b.size())
		return false;
	for (size_t i = 0; i < a.size(); ++i)
		if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
			return false;
	return true;
}

#if defined(__linux__) || defined(__APPLE__)
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <filesystem>

#if !defined(__ANDROID__)
std::vector<std::string> GetTokensFromPath(const std::string &path) {
    size_t pos = 0;
    size_t start = 0;
    std::vector<std::string> tokens;

    while ((pos = path.find('/', start)) != std::string::npos) {
        if (pos != start)
            tokens.push_back(path.substr(start, pos - start));
        start = pos + 1;
    }
    tokens.push_back(path.substr(start, path.length() - start));

    return tokens;
}

std::string casepath(const std::string &path)
{
    // returns either a path that has upper/lower case fixed and points to an / existing file or returns the input path

    // split original path
    std::vector<std::string> tokens = GetTokensFromPath(path);

    // iterate through directories and check if we find case-insensitive matches to the path
    // NOTE: the first match will be taken
    std::string result {""};
    if (path[0] == '/')
        result = "/";
    else if (path.size() >= 2 && path[0] == '.' && path[1] == '/')
        result = "./";  // use preffix ./

    // skip token "." when path begin ./
    int start_i = 0;
    if (!tokens.empty() && tokens[0] == ".")
        start_i = 1;

    for (int i = start_i; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        std::string current = result + token;

        if (!std::filesystem::exists(current)) {
            std::string dir = result;
            if (dir.empty())
                dir = ".";
            else if (dir.size() > 1 && dir.back() == '/')
                dir.pop_back();

            if (!std::filesystem::exists(dir))
                return path;

            bool found = false;
            for (const auto &entry : std::filesystem::directory_iterator(dir)) {
                std::string test = entry.path().filename().string();
                if (our_iequals(token, test)) {
                    current = result + test;
                    found = true;
                    break;
                }
            }
            if (!found)
                return path;
        }

        result = current + ((i != tokens.size()-1) ? "/" : "");
    }

    return result;
}
#endif//!defined(__ANDROID__)
#endif

FILE* fcaseopen(char const* path, char const* mode)
{
    FILE* f = fopen(path, mode);
#if defined(__linux__) || defined(__APPLE__)
    if (!f)
    {
        std::string r = casepath(path);
        f = fopen(r.c_str(), mode);
    }
#endif
    return f;
}