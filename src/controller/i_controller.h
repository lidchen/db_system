#pragma once
#include "src/event/event_broker.h"
#include "src/model/i_model.h"
#include "src/utils/logger.h"
#include "src/view/i_view.h"

#include <memory>
#include <unordered_map>

template <typename ModelT, typename ViewT>
class IController {
    using Callback = EventBroker::Callback;

   private:
    EventBroker& event_broker_ = gEventBroker::GetInstance();

   public:
    template <typename T>
    void SubscribeGlobalEvent(std::function<void(const T&)> cb) {
        // Wrap the user callback to accept Event&
        event_broker_.Subscribe<T>(cb);
        LOG(LogCategory::EVENT_SUBSCRIBED, "Global Event");
    }

    // Helper to publish events
    template <typename T>
    void PublishGlobalEvent(const T& event) {
        event_broker_.Publish(event);
        LOG(LogCategory::EVENT_PUBLISH, "Global Event");
    }

   public:
    IController() = default;
    virtual ~IController() {
        // for (auto id : cb_ids_) {
        //     event_broker_.Unsubscribe(id);
        //     LOG(LogCategory::EVENT_UNSUBSCRIBED, "Global Event: " +
        //     std::to_string(id));
        // }
    }
};