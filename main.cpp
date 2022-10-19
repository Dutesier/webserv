#include "Logger.hpp"

// Initializing instance to avoid linker errors
Logger* Logger::instance = NULL;

int main()
{
    LOG("Hello, stderr!");
    delete Logger::getLogger();
    return 0;
}
