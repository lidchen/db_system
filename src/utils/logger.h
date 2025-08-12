#pragma once
#include <iostream>
#include <string>

class Logger {
public:
    void log_info(const std::string& msg) const {
        std::cout << "[INFO]" << msg << "\n";
    }
    void log_error(const std::string& msg) const {
        std::cout << "[ERROR]" << msg << "\n";
    }
};