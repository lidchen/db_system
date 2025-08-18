#pragma once

#include <string>

enum class EventType {
    DB_CHANGE,
    TB_CHANGE,
    CONTENT_CHANGE
};

inline std::string to_string(EventType event) {
    using enum EventType;
    switch (event) {
        case DB_CHANGE: return "DB_CHANGE";
        case TB_CHANGE: return "TB_CHANGE";
        case CONTENT_CHANGE: return "CONTENT_CHANGE";
        default: return "Unknown";
    }
}
