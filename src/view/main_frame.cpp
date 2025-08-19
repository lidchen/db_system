#include "main_frame.h"
#include "src/controller/db_controller.h"
#include "src/model/db_connection.h"
#include "src/view/dialog/wx_database_manager_dialog.h"

#include <wx/wx.h>

MainFrame::MainFrame(const wxString& name)
    : wxFrame(nullptr, wxID_ANY, name, wxDefaultPosition, wxSize(700, 500)) {
    InitUI();

    db_connection_ = std::make_shared<DbConnection>();
    auto db_controller = DbController(db_connection_);
    DbView& db_view = db_controller.GetDbView();
    DbModel& db_model = db_controller.GetDbModel();

    db_model.SetDbDir("/Users/lid/Develop/work/db_system/resources/sql");
    db_view.PublishReqDbList();
    db_controller.ShowView();

    this->Close(true);
    wxFrame::Destroy();
    wxExit();
}

void MainFrame::InitUI() {
    menubar = new wxMenuBar();
    file = new wxMenu();
    import_from_csv = new wxMenu();
    database_manager = new wxMenu();
    auto* open_db_mananger = new wxMenuItem(
            database_manager, wxID_ANY, wxT("Open Database Manager"));
    auto* open_tb_manager = new wxMenuItem(
            database_manager, wxID_ANY, wxT("Open Table Manager"));

    database_manager->Append(open_db_mananger);
    database_manager->Append(open_tb_manager);

    file->AppendSubMenu(import_from_csv, wxT("Import From CSV"));
    menubar->Append(file, wxT("File"));
    menubar->Append(database_manager, wxT("Manage Database"));
    wxFrame::SetMenuBar(menubar);
}
