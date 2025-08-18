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
   public:
    explicit DbController(std::shared_ptr<DbConnection> db_connection);
    DbController(const DbController&) = delete;
    DbController& operator=(const DbController&) = delete;
    ~DbController() = default;

    void test_db_view_publish_event();
    void refresh_db_model();
    void refresh_db_view();

   private:

    void init();
    std::shared_ptr<DbConnection> db_connection_;
    EventBroker& event_broker_ = EventBroker::get_instance();

   protected:
    // Provide protected setter functions in IController to allow derived
    // classes to set base pointers
    using IController::set_model;
    using IController::set_view;
};