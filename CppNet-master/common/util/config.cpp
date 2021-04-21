// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <fstream>
#include <cstring>

#include "config.h"
#include "common/log/log.h"

namespace cppnet {

void Config::SetFilePath(const std::string& path) {
    _file = path;
}

bool Config::ReLoadFile() {
    return LoadFile(_file);
}

bool Config::LoadFile(const std::string& path) {
    SetFilePath(path);
    std::fstream file(path);
    if (!file) {
        LOG_ERROR("load config file failed, can't open file.");
        return false;
    }
    std::string line;
    std::string key;
    std::string value;
    std::map<std::string, std::string> temp_map;
    while (!file.eof()) {
        char buf[1024] = { 0 };
        file.getline(buf, 1024);
        if (strlen(buf) < 3) {
            continue;
        }
        line = buf;
        _Trim(line);
        if (line[0] == '#') {
            continue;
        }

        key = line.substr(0, line.find_first_of("="));
        value = line.substr(line.find_first_of("=") + 1);
        _Trim(key);
        _Trim(value);
        LOG_INFO("load config key : %s, value : %s", key.c_str(), value.c_str());
        temp_map[key] = value;
    }

    std::unique_lock<std::mutex> lock(_mutex);
    _config_map = temp_map;
    return true;
}

int Config::GetIntValue(const std::string& key) {
    try {
        std::unique_lock<std::mutex> lock(_mutex);
        auto iter = _config_map.find(key);
        if (iter != _config_map.end()) {
            return atoi(iter->second.c_str());
        }
        return -1;
    
    } catch (...) {
        LOG_ERROR("get config int value failed.");
        return -1;
    }
}

std::string Config::GetStringValue(const std::string& key) {
    try {
        std::unique_lock<std::mutex> lock(_mutex);
        auto iter = _config_map.find(key);
        if (iter != _config_map.end()) {
            return iter->second;
        }
        return "";

    } catch (...) {
        LOG_ERROR("get config string value failed.");
        return "";
    }
}

double Config::GetDoubleValue(const std::string& key) {
    try {
        std::unique_lock<std::mutex> lock(_mutex);
        auto iter = _config_map.find(key);
        if (iter != _config_map.end()) {
            return atof(iter->second.c_str());
        }
        return -1;

    } catch (...) {
        LOG_ERROR("get config double value failed.");
        return -1;
    }
}

bool Config::GetBoolValue(const std::string& key) {
    try {
        std::unique_lock<std::mutex> lock(_mutex);
        auto iter = _config_map.find(key);
        if (iter != _config_map.end()) {
            return iter->second == "true" || iter->second == "1";
        }
        return false;

    } catch (...) {
        LOG_ERROR("get config double value failed.");
        return false;
    }
}

void Config::_Trim(std::string& line) {
    if (line.empty()) {
        return;
    }
    line.erase(0, line.find_first_not_of(" "));
    line.erase(line.find_last_not_of(" ") + 1);
}
 
}