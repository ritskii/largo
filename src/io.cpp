#include "io.hpp"

#include <fstream>
#include <iostream>
#include <memory>

namespace Io {
bool readTomlValue(const std::filesystem::path& TomlPath, const std::string& Key, std::string& Value) {
    std::unique_ptr<std::ifstream> File = std::make_unique<std::ifstream>(TomlPath);
    if (!File->is_open()) {
        return false;
    }

    std::string Line;
    while (std::getline(*File, Line)) {
        const std::size_t EqualPosition = Line.find('=');
        if (EqualPosition == std::string::npos) {
            continue;
        }

        const std::string TrimmedKey = Line.substr(0, EqualPosition);
        const std::size_t KeyStart = TrimmedKey.find_first_not_of(" \t");
        const std::size_t KeyEnd = TrimmedKey.find_last_not_of(" \t");
        if (KeyStart == std::string::npos || KeyEnd == std::string::npos) {
            continue;
        }

        const std::string CleanedKey = TrimmedKey.substr(KeyStart, KeyEnd - KeyStart + 1);
        if (CleanedKey != Key) {
            continue;
        }

        const std::size_t ValueStart = Line.find_first_not_of(" \t", EqualPosition + 1);
        if (ValueStart == std::string::npos) {
            return false;
        }

        const std::string RawValue = Line.substr(ValueStart);
        const std::size_t FirstQuote = RawValue.find('"');
        const std::size_t LastQuote = RawValue.rfind('"');
        if (FirstQuote == std::string::npos || LastQuote == std::string::npos || LastQuote <= FirstQuote) {
            return false;
        }

        Value = RawValue.substr(FirstQuote + 1, LastQuote - FirstQuote - 1);
        return true;
    }

    return false;
}

bool createProjectDirectory(const std::string& ProjectName) {
    namespace fs = std::filesystem;

    const fs::path ProjectPath = fs::current_path() / ProjectName;
    const fs::path SourcePath = ProjectPath / "src";
    const fs::path IncludePath = ProjectPath / "include";
    const fs::path MainPath = SourcePath / "main.cpp";
    const fs::path TomlPath = ProjectPath / "largo.toml";

    if (fs::exists(ProjectPath)) {
        std::cout << "Error: project directory already exists.\n";
        return false;
    }

    std::error_code Error;
    fs::create_directory(ProjectPath, Error);
    if (Error) {
        std::cout << "Error: failed to create project directory.\n";
        return false;
    }

    fs::create_directory(SourcePath, Error);
    if (Error) {
        std::cout << "Error: failed to create src directory.\n";
        return false;
    }

    fs::create_directory(IncludePath, Error);
    if (Error) {
        std::cout << "Error: failed to create include directory.\n";
        return false;
    }

    std::ofstream MainFile(MainPath);
    if (!MainFile.is_open()) {
        std::cout << "Error: failed to create src/main.cpp.\n";
        return false;
    }

    MainFile << "#include <iostream>\n\n";
    MainFile << "int main() {\n";
    MainFile << "    std::cout << \"Hello world\\n\";\n";
    MainFile << "    return 0;\n";
    MainFile << "}\n";
    MainFile.close();

    std::ofstream TomlFile(TomlPath);
    if (!TomlFile.is_open()) {
        std::cout << "Error: failed to create largo.toml.\n";
        return false;
    }

    TomlFile << "name = \"" << ProjectName << "\"\n";
    TomlFile << "version = \"0.1.0\"\n";
    TomlFile.close();

    return true;
}

bool createDirectories(const std::filesystem::path& Path) {
    namespace fs = std::filesystem;

    std::error_code Error;
    fs::create_directories(Path, Error);
    return !Error;
}

bool writeCMakeLists(const std::filesystem::path& CmakePath, const std::string& ProjectName) {
    std::unique_ptr<std::ofstream> CmakeFile = std::make_unique<std::ofstream>(CmakePath);
    if (!CmakeFile->is_open()) {
        std::cout << "Error: failed to create CMakeLists.txt.\n";
        return false;
    }

    *CmakeFile << "cmake_minimum_required(VERSION 3.16)\n";
    *CmakeFile << "project(" << ProjectName << " LANGUAGES CXX)\n";
    *CmakeFile << "set(CMAKE_CXX_STANDARD 17)\n";
    *CmakeFile << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
    *CmakeFile << "set(CMAKE_CXX_EXTENSIONS OFF)\n\n";
    *CmakeFile << "file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS\n";
    *CmakeFile << "    \"${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp\"\n";
    *CmakeFile << ")\n\n";
    *CmakeFile << "add_executable(" << ProjectName << " ${APP_SOURCES})\n\n";
    *CmakeFile << "target_include_directories(" << ProjectName << " PRIVATE\n";
    *CmakeFile << "    \"${CMAKE_CURRENT_SOURCE_DIR}/../include\"\n";
    *CmakeFile << ")\n";
    CmakeFile->close();

    return true;
}

std::filesystem::path findExecutable(const std::filesystem::path& SearchPath, const std::string& ProjectName) {
    namespace fs = std::filesystem;

    fs::path ExecutablePath;
    for (fs::recursive_directory_iterator Iterator(SearchPath, fs::directory_options::skip_permission_denied), End; Iterator != End; ++Iterator) {
        if (Iterator->is_directory() && Iterator->path().filename() == "CMakeFiles") {
            Iterator.disable_recursion_pending();
            continue;
        }

        const fs::path Candidate = Iterator->path();
        if (!fs::is_regular_file(Candidate)) {
            continue;
        }

        const std::string FileName = Candidate.filename().string();
        if (FileName == ProjectName + ".exe" || FileName == ProjectName) {
            ExecutablePath = Candidate;
            break;
        }
    }

    return ExecutablePath;
}
}
