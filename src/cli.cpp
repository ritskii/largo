#include "cli.hpp"

#include <cstdlib>
#include <iostream>

bool CliParser::buildProject() {
    namespace fs = std::filesystem;

    const fs::path ProjectPath = fs::current_path();
    const fs::path TomlPath = ProjectPath / "largo.toml";
    const fs::path BuildPath = ProjectPath / "build";
    const fs::path CmakePath = BuildPath / "CMakeLists.txt";

    std::string ProjectName;
    std::string ProjectVersion;
    if (!FileIo.readTomlValue(TomlPath, "name", ProjectName)) {
        std::cout << "Error: failed to read project name from largo.toml.\n";
        return false;
    }

    if (!FileIo.readTomlValue(TomlPath, "version", ProjectVersion)) {
        std::cout << "Error: failed to read project version from largo.toml.\n";
        return false;
    }

    if (!FileIo.createDirectories(BuildPath)) {
        std::cout << "Error: failed to create build directory.\n";
        return false;
    }

    if (!FileIo.writeCMakeLists(CmakePath, ProjectName)) {
        return false;
    }

    std::cout << "Generated CMake project for " << ProjectName << " version " << ProjectVersion << "\n";
    return true;
}

bool CliParser::runProject() {
    namespace fs = std::filesystem;

    const fs::path ProjectPath = fs::current_path();
    const fs::path TomlPath = ProjectPath / "largo.toml";
    const fs::path BuildPath = ProjectPath / "build";
    const fs::path OutputPath = BuildPath / "out";

    std::string ProjectName;
    if (!FileIo.readTomlValue(TomlPath, "name", ProjectName)) {
        std::cout << "Error: failed to read project name from largo.toml.\n";
        return false;
    }

    if (!buildProject()) {
        return false;
    }

    if (!FileIo.createDirectories(OutputPath)) {
        std::cout << "Error: failed to create output directory.\n";
        return false;
    }

    const std::string ConfigureCommand = "cmake -S \"" + BuildPath.string() + "\" -B \"" + OutputPath.string() + "\"";
    if (std::system(ConfigureCommand.c_str()) != 0) {
        std::cout << "Error: failed to configure project with CMake.\n";
        return false;
    }

    const std::string BuildCommand = "cmake --build \"" + OutputPath.string() + "\" --config Debug";
    if (std::system(BuildCommand.c_str()) != 0) {
        std::cout << "Error: failed to build project with CMake.\n";
        return false;
    }

    const fs::path ExecutablePath = FileIo.findExecutable(OutputPath, ProjectName);
    if (ExecutablePath.empty()) {
        std::cout << "Error: failed to locate built executable.\n";
        return false;
    }

    const std::string ExecutionCommand = "\"" + ExecutablePath.string() + "\"";
    if (std::system(ExecutionCommand.c_str()) != 0) {
        std::cout << "Error: failed to execute built project.\n";
        return false;
    }

    return true;
}

int CliParser::parse(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: largo <new|build|run> [<string>]\n";
        return 1;
    }

    const std::string FirstArgument = argv[1];
    const bool HasCommandName = FirstArgument == "largo";
    const int CommandIndex = HasCommandName ? 2 : 1;
    const int ValueIndex = HasCommandName ? 3 : 2;

    if (argc <= CommandIndex) {
        std::cout << "Error: missing command. Usage: largo <new|build|run> [<string>]\n";
        return 1;
    }

    const std::string Command = argv[CommandIndex];

    if (Command == "build") {
        std::cout << "Executing command: largo build\n";
        if (!buildProject()) {
            return 1;
        }
        std::cout << "Accept\n";
        return 0;
    }

    if (Command == "run") {
        std::cout << "Executing command: largo run\n";
        if (!runProject()) {
            return 1;
        }
        std::cout << "Accept\n";
        return 0;
    }

    if (Command == "new") {
        if (argc <= ValueIndex) {
            std::cout << "Error: 'largo new' requires <string>.\n";
            return 1;
        }

        const std::string StringValue = argv[ValueIndex];

        if (StringValue.empty()) {
            std::cout << "Error: 'largo new' requires a valid <string> value.\n";
            return 1;
        }

        std::cout << "Executing command: largo new " << StringValue << "\n";
        if (!FileIo.createProjectDirectory(StringValue)) {
            return 1;
        }
        std::cout << "Accept\n";
        return 0;
    }

    std::cout << "Error: unknown command. Usage: largo <new|build|run> [<string>]\n";
    return 1;
}
