#pragma once
#include <any>
#include <functional>
#include <unordered_map>

#include "events.h"
#include "utils/result.h"
class EventBroker {
   public:
    std::unordered_map<Events,
                       std::vector<std::function<void(const std::any& data)>>>
            cb_container_;
    void register_cb(Events event,
                     const std::function<void(const std::any& data)>& cb) {
        cb_container_[event].push_back(cb);
    }

    Result<bool> send(const Events event, const std::any& data) {
        if (cb_container_.find(event) != cb_container_.end()) {
            for (auto fun : cb_container_[event]) fun(data);
            return Result(true, "");
        } else {
            return Result(false,
                          "Event " + to_string(event) +
                                  " dont trigger callback function");
        }
    }
};