#pragma once

#include <string>
#include <vector>

class CommandLineParser {
    public:
        CommandLineParser () {};
        CommandLineParser (int &argc, char **argv);

        void Init(int &argc, char **argv);

        bool DoRegressionTest() const {return m_regression_tests;};

    private:
        void InterpretParams();

        std::vector<std::string> m_params;

        bool m_regression_tests {false};
};

extern CommandLineParser CommandLineParams;
