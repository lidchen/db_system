#pragma once

#include "utils/logger.h"
#include "ui/event/ui_event_hub.h"
#include "controller/database/db_controller.h"

struct AppContext
{
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<DBController> db_controller_;
    std::shared_ptr<UIEventHub> event_hub_;

    AppContext() {
        logger_ = std::make_shared<Logger>();
        event_hub_ = std::make_shared<UIEventHub>();
        db_controller_ = std::make_shared<DBController>(logger_, event_hub_);
    }
};
