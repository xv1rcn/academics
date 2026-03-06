#include "app.hpp"

#include <exception>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        App app;
        return app.run(argc, argv);
    } catch (const std::exception &exception) {
        std::cerr << "Program failed: " << exception.what() << std::endl;
        return 1;
    }
}
