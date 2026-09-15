#pragma once

#include <filesystem>
#include <string>

namespace Io {
    bool readTomlValue(const std::filesystem::path& TomlPath, const std::string& Key, std::string& Value);
    bool createProjectDirectory(const std::string& ProjectName);
    bool createDirectories(const std::filesystem::path& Path);
    bool writeCMakeLists(const std::filesystem::path& CmakePath, const std::string& ProjectName);
    std::filesystem::path findExecutable(const std::filesystem::path& SearchPath, const std::string& ProjectName);
}
