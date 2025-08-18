#include "event_broker.h"

EventBroker::CallbackId EventBroker::subscribe(const Callback& cb) {
    std::lock_guard lock(mtx_);
    next_id_++;
    CallbackId id = next_id_; 
    subscribers_[id] = cb;
    return id;
}

// Unsubscribe by id
void EventBroker::unsubscribe(EventBroker::CallbackId id) {
    std::lock_guard lock(mtx_);
    subscribers_.erase(id);
}

void EventBroker::publish(const Event& e) {
    std::lock_guard lock(mtx_);
    for (const auto& [id, cb] : subscribers_) {
        cb(e);
    }
}
