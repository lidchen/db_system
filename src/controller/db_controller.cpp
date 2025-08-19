#include "db_controller.h"

DbController::DbController(std::shared_ptr<DbConnection> db_connection)
    : db_connection_(db_connection)
{
    InitEvents();
}

void DbController::ShowView() {
    if (db_view_) {
        db_view_->Show();
    }
}

DbView& DbController::GetDbView() {
    return *db_view_;
}

DbModel& DbController::GetDbModel() {
    return *db_model_;
}

void DbController::InitEvents() {
    // SubscribeGlobalEvent([this](const Event& e) {
    //     if (e.type == EventType::DB_CHANGE) {
    //         std::cerr << "db_change catched at db_controller\n";
    //         std::cerr << "payload: " << e.GetPayload<std::string>() << "\n";
    //         // this->refresh_db_model();
    //     }
    // });
}