#include "package.hpp"
#include "io.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>

PackageManager::PackageManager() {
}

void PackageManager::add(const std::string& packageName) {
    std::ofstream file("largo.toml", std::ios::app);
    file << "[dependencies]\n";
    file << packageName << " = { path = \"deps/" << packageName << "\" }\n";
}

// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
// Refactor this shit
void PackageManager::run() {
    namespace fs = std::filesystem;

    const fs::path ProjectPath = fs::current_path();
    const fs::path TomlPath = ProjectPath / "largo.toml";
    const fs::path BuildPath = ProjectPath / "build";
    const fs::path OutputPath = BuildPath / "out";

    std::string ProjectName;
    if (!Io::readTomlValue(TomlPath, "name", ProjectName)) {
        std::cout << "Error: failed to read project name from largo.toml.\n";
    }

    const std::string ConfigureCommand = "cmake -S \"" + BuildPath.string() + "\" -B \"" + OutputPath.string() + "\"";
    if (std::system(ConfigureCommand.c_str()) != 0) {
        std::cout << "Error: failed to configure project with CMake.\n";
    }

    const std::string BuildCommand = "cmake --build \"" + OutputPath.string() + "\" --config Debug";
    if (std::system(BuildCommand.c_str()) != 0) {
        std::cout << "Error: failed to build project with CMake.\n";
    }

    const fs::path ExecutablePath = Io::findExecutable(OutputPath, ProjectName);
    if (ExecutablePath.empty()) {
        std::cout << "Error: failed to locate built executable.\n";
    }

    const std::string ExecutionCommand = "\"" + ExecutablePath.string() + "\"";
    if (std::system(ExecutionCommand.c_str()) != 0) {
        std::cout << "Error: failed to execute built project.\n";
    }
}

void PackageManager::build() {
    namespace fs = std::filesystem;

    const fs::path ProjectPath = fs::current_path();
    const fs::path TomlPath = ProjectPath / "largo.toml";
    const fs::path BuildPath = ProjectPath / "build";
    const fs::path CmakePath = BuildPath / "CMakeLists.txt";

    std::string ProjectName;
    std::string ProjectVersion;
    if (!Io::readTomlValue(TomlPath, "name", ProjectName)) {
        std::cout << "Error: failed to read project name from largo.toml.\n";
    }

    if (!Io::readTomlValue(TomlPath, "version", ProjectVersion)) {
        std::cout << "Error: failed to read project version from largo.toml.\n";
    }

    if (!Io::createDirectories(BuildPath)) {
        std::cout << "Error: failed to create build directory.\n";
    }

    if (!Io::writeCMakeLists(CmakePath, ProjectName)) {
    }

    std::cout << "Generated CMake project for " << ProjectName << " version " << ProjectVersion << std::endl;
}