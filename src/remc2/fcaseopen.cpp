#include <boost/algorithm/string/predicate.hpp>
#include "fcaseopen.h"

#if !defined(_WIN32)
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <filesystem>
#include <boost/algorithm/string.hpp>


std::vector<std::string> GetTokensFromPath(const std::filesystem::path &path) {
    size_t pos = 0;
    size_t start = 0;
    std::vector<std::string> tokens;

    while ((pos = path.native().find('/', start)) != std::string::npos) {
        if (pos != start)
            tokens.push_back(path.native().substr(start, pos - start));
        start = pos + 1;
    }
    tokens.push_back(path.native().substr(start, path.native().length() - start));

    return tokens;
}

std::string casepath(const std::filesystem::path& path)
{
    // returns either a path that has upper/lower case fixed and points to an / existing file or returns the input path

    // split original path
    std::vector<std::string> tokens = GetTokensFromPath(path);

    // iterate through directories and check if we find case-insensitive matches to the path
    // NOTE: the first match will be taken
    std::string result {""};
    if (path.native()[0] == '/')
        result = "/";

    for (int i = 0; i < tokens.size(); ++i) {
        std::string token = tokens[i];
        std::string current = result + token;

        if (!std::filesystem::exists(current)) {
            if (!std::filesystem::exists(result))
                return path;

            for (const auto &entry: std::filesystem::directory_iterator(result)) {
                std::string test = GetTokensFromPath(entry.path().native()).back();
                if (boost::iequals(token, test)) {
                    current = result + test;
                    break;
                }
            }
        }

        result = current + ((i != tokens.size()-1) ? "/" : "");
    }

    if (!std::filesystem::exists(result))
        result = path;

    return result;
}
#endif

FILE* fcaseopen(const std::filesystem::path& path, char const* mode)
{
    FILE* f = fopen(path.c_str(), mode);
#if !defined(_WIN32)
    if (!f)
    {
        std::string r = casepath(path);
        f = fopen(r.c_str(), mode);
    }
#endif
    return f;
}
