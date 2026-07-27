#pragma once

#include "io.hpp"

#include <string>

class CliParser {
public:
    int parse(int argc, char* argv[]);

private:
    bool buildProject();
    bool runProject();

    Io FileIo;
};
