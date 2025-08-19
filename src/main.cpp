#include "src/controller/db_controller.h"
#include "src/event/event_broker.h"
#include "src/view/db_view.h"
#include "src/view/main_frame.h"

#include <wx/wx.h>

#include <catch2/catch_session.hpp>
// Define UI_ON macro to enable or disable UI code.
// To enable UI, define UI_ON as 1 before including this file or in your build
// system. Example: g++ ... -DUI_ON=1 ...
#define UI_ON 1

#if UI_ON == 1
class MainApp : public wxApp {
   public:
    virtual bool OnInit() {
        try {
            wxFrame* main_frame = new MainFrame("db_system");
        } catch (const DbException& e) {
            std::cerr << e.what() << "\n";
        }
        return true;
    }
};

wxIMPLEMENT_APP(MainApp);

#else
int main() {
#ifdef RUN_TEST
    Catch::Session session;
    return session.run();
#endif
    auto db_connection = std::make_shared<DbConnection>();
    auto db_controller = DbController(db_connection);
    auto db_controller2 = DbController(db_connection);
    db_controller.TestDbViewPublishEvent();
    return 0;
}
#endif