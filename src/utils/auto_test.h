#pragma once
#include "ui/event/ui_event_hub.h"


class AutoTest {
public:
    explicit AutoTest(std::shared_ptr<UIEventHub> event_hub) : event_hub_(event_hub) {}

    void input_test(Events event) {
        event_hub_->send(event, std::string("test"));
        event_hub_->send(event, "test");
        event_hub_->send(event, "1");
        event_hub_->send(event, "/");
        event_hub_->send(event, "'");
        // ... and as much as you want
    }
private:
    std::shared_ptr<UIEventHub> event_hub_;
};