#pragma once

#include <string>

enum class Events {
    DB_CHANGE,
    TB_CHANGE,
    CONTENT_CHANGE
};

inline std::string to_string(Events event) {
    switch (event) {
        case Events::DB_CHANGE: return "DB_CHANGE";
        case Events::TB_CHANGE: return "TB_CHANGE";
        case Events::CONTENT_CHANGE: return "CONTENT_CHANGE";
        default: return "Unknown";
    }
}
