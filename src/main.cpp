#include "cli.hpp"

int main(int argc, char* argv[]) {
    CliParser cliParser;
    return cliParser.parse(argc, argv);
}
