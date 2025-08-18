#pragma once
#include <any>
#include <functional>
#include <unordered_map>
#include <mutex>

#include "event_type.h"

struct Event {
    EventType type;
    std::string payload;
};

class EventBroker {
public:
    static EventBroker& get_instance();
    using Callback = std::function<void(const Event&)>;
    using CallbackId = size_t;

    // Subscribe returns a unique id for the callback
    CallbackId subscribe(const Callback& cb);

    // Unsubscribe by id
    void unsubscribe(CallbackId id);

    void publish(const Event& e);

private:
    EventBroker() = default;
    EventBroker(const EventBroker&) = delete;
    EventBroker& operator= (const EventBroker&) = delete;
    std::unordered_map<CallbackId, Callback> subscribers_;
    std::mutex mtx_;
    CallbackId next_id_ = 0;
};

inline EventBroker& EventBroker::get_instance() {
    static EventBroker instance;
    return instance;
}