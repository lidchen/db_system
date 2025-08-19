#pragma once
#include "src/controller/i_controller.h"
#include "src/event/event_broker.h"
#include "src/model/db_connection.h"
#include "src/model/db_model.h"
#include "src/view/db_view.h"

/**
 * @brief
 *
 * connnect between db_model, db_view
 *
 */
class DbController : public IController<DbModel, DbView> {
    using Callback = EventBroker::Callback;

   public:
    explicit DbController(std::shared_ptr<DbConnection> db_connection);
    DbController(const DbController&) = delete;
    DbController& operator=(const DbController&) = delete;

    void ShowView();
    DbView& GetDbView();
    DbModel& GetDbModel();
    void RefreshDbModel();
    void RefreshDbView();

   private:
    void InitEvents();
    std::shared_ptr<EventBroker> mvc_event_broker_ =
            std::make_shared<EventBroker>();
    std::shared_ptr<DbConnection> db_connection_;
    std::unique_ptr<DbModel> db_model_ =
            std::make_unique<DbModel>(db_connection_, mvc_event_broker_);
    std::unique_ptr<DbView> db_view_ =
            std::make_unique<DbView>(mvc_event_broker_);
};