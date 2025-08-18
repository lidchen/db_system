#include <wx/wx.h>

#include <catch2/catch_session.hpp>

#include "src/controller/db_controller.h"
#include "src/view/db_view.h"
#include "src/view/main_frame.h"

#ifdef UI_ON
class MainApp : public wxApp {
   public:
    virtual bool OnInit() {
        try {
            // wxFrame* main_frame = new MainFrame("db_system");
            // main_frame->Show();
            // DbView db_view;
            // db_view.show();

        } catch (const DbException& e) {
            std::cerr << e.what() << "\n";
        }
        return true;
    }
};
IMPLEMENT_APP(MainApp);
#endif

#ifndef UI_ON
int main() {
#ifdef RUN_TEST
    Catch::Session session;
    return session.run();
#endif

    auto db_connection = std::make_shared<DbConnection>();
    auto db_controller = DbController(db_connection);
    auto db_controller2 = DbController(db_connection);
    db_controller.test_db_view_publish_event();
    db_controller2.test_db_view_publish_event();
}
#endif