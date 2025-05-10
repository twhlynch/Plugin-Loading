#include "example.h"

#include <iostream>

PLUGIN_API void initialize() {
    std::cout << "Plugin initialized" << std::endl;
}

PLUGIN_API void cleanup() {
    std::cout << "Plugin cleanup" << std::endl;
}