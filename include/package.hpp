#pragma once

#include <string>

class PackageManager {
public:
    PackageManager();

    void add(const std::string& packageName);
    void run();
    void build();
};