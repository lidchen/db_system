// logger.h
#pragma once
#include <iostream>

enum class LogCategory {
    EVENT_SUBSCRIBED = 1 << 0,
    EVENT_UNSUBSCRIBED = 1 << 1,
    EVENT_PUBLISH = 1 << 2,
    EVENT_TRIGGER = 1 << 3,
    EVENT_GUI = 1 << 4,
    DB = 1 << 5
};

inline const char* LogCategoryToString(LogCategory cat) {
    using enum LogCategory;
    switch (cat) {
        case EVENT_SUBSCRIBED:
            return "EVENT_SUBSCRIBED";
        case EVENT_UNSUBSCRIBED:
            return "EVENT_UNSUBSCRIBED";
        case EVENT_PUBLISH:
            return "EVENT_PUBLISH";
        case EVENT_TRIGGER:
            return "EVENT_TRIGGER";
        case EVENT_GUI:
            return "EVENT_GUI";
        case DB:
            return "DB";
        default:
            return "UNKNOWN";
    }
}

const inline unsigned int EnabledCategories =
#ifdef DEBUG_ON
        static_cast<unsigned int>(LogCategory::EVENT_SUBSCRIBED) |
        static_cast<unsigned int>(LogCategory::EVENT_UNSUBSCRIBED) |
        static_cast<unsigned int>(LogCategory::EVENT_PUBLISH) |
        static_cast<unsigned int>(LogCategory::EVENT_TRIGGER) |
        static_cast<unsigned int>(LogCategory::EVENT_GUI) |
        static_cast<unsigned int>(LogCategory::DB)
#else
        0
#endif
        ;

#define LOG(cat, msg)                                                   \
    do {                                                                \
        if (EnabledCategories & static_cast<unsigned int>(cat)) {       \
            std::cerr << "[" << LogCategoryToString(cat) << "] " << msg \
                      << std::endl;                                     \
        }                                                               \
    } while (0)