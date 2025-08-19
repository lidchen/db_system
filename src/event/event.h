#pragma once
#include "event_type.h"

#include <any>
#include <string>
#include <vector>

class Event {
   private:
    bool handled_ = false;
    std::any payload_;

   public:
    Event() = default;
    explicit Event(const std::any& payload) : payload_(payload) {}
    template <typename T>
    const T& GetPayload() const {
        return std::any_cast<const T&>(payload_);
    }
    // Mark the event as handled so it won't be propagated further
    void MarkHandled() { handled_ = true; }

    bool IsHandled() const { return handled_; }
};

struct gReqDatabaseUpdate : Event {};
struct gReqTableUpdate : Event {};
struct gReqContentUpdate : Event {};

// Do scan database then publish response event
struct ReqDbList : Event {};
struct ResDbList : Event {
    using result = std::vector<std::string>;
    explicit ResDbList(const result& result) : Event(result) {}
    const result& GetPayload() const { return this->Event::GetPayload<result>(); }
};

struct ReqFormatFileName : Event {
    std::string raw_input;
};
struct ResFormatFileName : Event {
    std::string formatted_name;
};