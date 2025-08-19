#pragma once
#include "event.h"
#include "src/model/db_exception.h"

#include <any>
#include <functional>
#include <mutex>
#include <typeindex>
#include <unordered_map>

class EventBroker {
   public:
    using Callback = std::function<void(const Event&)>;

   private:
    std::unordered_map<std::type_index, std::vector<Callback>> subscribers_;
    std::mutex mtx_;

   public:
    EventBroker() = default;

    template <typename T>
    void Subscribe(std::function<void(const T&)> cb) {
        std::lock_guard lock(mtx_);
        auto& subscribers = subscribers_[std::type_index(typeid(T))];
        subscribers.push_back([cb](const Event& e) {
            // Could use dynamic_cast for RTTI here
            // But if we could guarantee T is derived from Event
            // Then this approach is more perfered
            // https://en.cppreference.com/w/cpp/language/dynamic_cast.html
            cb(static_cast<const T&>(e));
        });
    }

    template <typename T>
    void Publish(const T& event) {
        if (auto it = subscribers_.find(std::type_index(typeid(T)));
            it != subscribers_.end()) {
            for (auto sub : it->second) {
                sub(event);
            }
        }
    }

    template <typename Req, typename Res>
    Res Call(const Req& request) {
        if (auto it = subscribers_.find(std::type_index(typeid(Req)));
            it != subscribers_.end()) {
            auto result = it->second(request);
            return static_cast<const Res&>(*result);
        } else {
            throw(DbException("No related subscriber",
                              DbException::DbErrorType::EVENT));
        }
    }
};

// Child singleton class
class gEventBroker : public EventBroker {
   public:
    static gEventBroker& GetInstance() {
        static gEventBroker instance;
        return instance;
    }

   private:
    gEventBroker() : EventBroker() {}
    gEventBroker(const gEventBroker&) = delete;
    gEventBroker& operator=(const gEventBroker&) = delete;
};
