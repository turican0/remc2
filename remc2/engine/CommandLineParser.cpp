#include "CommandLineParser.h"

#include <algorithm>

CommandLineParser CommandLineParams;

CommandLineParser::CommandLineParser (int &argc, char **argv) {
    this->Init(argc, argv);
}

void CommandLineParser::Init(int &argc, char **argv) {
    this->m_params.clear();
    for (int i=1; i < argc; ++i) {
        this->m_params.push_back(argv[i]);
    }
    this->InterpretParams();
}

void CommandLineParser::InterpretParams() {
    // supported params
    // --regression_tests - default: false

    const auto &p = this->m_params;

    auto location_regression_tests = std::find(begin(p), end(p), "--regression_tests");

    m_regression_tests = (location_regression_tests != end(p));
}
