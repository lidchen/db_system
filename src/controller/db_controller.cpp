#include "db_controller.h"

DbController::DbController(std::shared_ptr<DbConnection> db_connection)
    : db_connection_(db_connection) {
    set_model(std::make_unique<DbModel>(db_connection_));
    set_view(std::make_unique<DbView>());
    init();
}
void DbController::init() {
    event_broker_.subscribe([this](const Event& e) {
        if (e.type == EventType::DB_CHANGE) {
            std::cerr << "db_change catched at db_controller\n";
            std::cerr << "payload: " + e.payload << "\n";
            // this->refresh_db_model();
        }
    });
}

void DbController::test_db_view_publish_event() {
    event_broker_.publish(Event{EventType::DB_CHANGE, "hello"});
}