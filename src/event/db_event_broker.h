#pragma once

#include "event_broker.h"

class DbEventBroker : public EventBroker {
    public:
        DbEventBroker() : EventBroker() {}
}