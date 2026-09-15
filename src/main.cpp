#include "package.hpp"
#include "cli.hpp"
#include "io.hpp"

int main(int argc, char* argv[]) {
    CliBuilder manager;
    manager.option("run")
        .description("Runs the project");
    manager.option("build")
        .description("Builds the project");
    manager.option("new")
        .argument()
        .description("Creates a new project");
    manager.option("add")
        .argument()
        .description("Adds a new package to the project");
    manager.parse(argc, argv);

    PackageManager package;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "run") {
            package.run();
        } else if (arg == "build") {
            package.build();
        } else if (arg == "new") {
            const Option& opt = manager.m_options.at(arg);
            std::string argument;
            if (opt.has_argument() && i + 1 < argc) {
                argument = argv[i + 1];
                ++i;
            }
            if (!Io::createProjectDirectory(argument)) {
                return 1;
            }
        } else if (arg == "add") {
            const Option& opt = manager.m_options.at(arg);
            std::string argument;
            if (opt.has_argument() && i + 1 < argc) {
                argument = argv[i + 1];
                ++i;    
                package.add(argument);
            }
        }
    }

    return 0;
}