#include <wx/wx.h>

#include "main_frame.h"
#include "src/model/db_connection.h"
#include "src/view/welcome_panel.h"
#include "src/view/dialog/wx_database_manager_dialog.h"

MainFrame::MainFrame(const wxString& name)
    : wxFrame(nullptr, wxID_ANY, name, wxDefaultPosition, wxSize(700, 500)) {
    db_connection_ = std::make_shared<DbConnection>();

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

    // file->AppendSubMenu(database_manager, wxT("Manage Database"));
    file->AppendSubMenu(import_from_csv, wxT("Import From CSV"));
    menubar->Append(file, wxT("File"));
    menubar->Append(database_manager, wxT("Manage Database"));
    SetMenuBar(menubar);

    Bind(wxEVT_MENU,
         &MainFrame::on_db_manager,
         this,
         open_db_mananger->GetId());
    // Bind(wxEVT_MENU, &MainFrame::on_tb_manager, this, open_tb_manager->GetId());

    // Open WelcomePanel at default
    show_welcome_panel();
}
void MainFrame::on_db_manager(wxCommandEvent& event) {
    wxDatabaseManagerDialog* db_manager_dialog =
            new wxDatabaseManagerDialog(this);
    db_manager_dialog->ShowModal();
    db_manager_dialog->Destroy();
}

// void MainFrame::on_tb_manager(wxCommandEvent& event) {
//     DatabaseManager& db_manager = DatabaseManager::getInstance();
//     DatabaseTableManager* shared_tb_manager = db_manager.get_tb_manager();
//     if (!shared_tb_manager) {
//         throw(DatabaseException("No db selected"));
//     }
//     wxTableManagerDialog* tb_manager_dialog = new wxTableManagerDialog(this);
//     if (tb_manager_dialog->ShowModal() == wxID_OK) {
//         db_manager.notify_tb_changed();
//         refresh_current_panel();
//     }
//     tb_manager_dialog->Destroy();
// }

// void MainFrame::show_db_editor_panel() {
//     DatabaseManager& db_manager = DatabaseManager::getInstance();
//     DatabaseTableManager* shared_tb_manager = db_manager.get_tb_manager();
//     if (current_panel_) current_panel_->Destroy();
//     try {
//         current_panel_ = new DatabaseEditorPanel(this);
//         db_manager.register_tb_observer([this]() {
//             if (auto* editor_panel =
//                         dynamic_cast<DatabaseEditorPanel*>(current_panel_)) {
//                 editor_panel->refresh_tb_list();
//             }
//         });
//         SetSize(wxSize(1024, 768));
//     } catch (DatabaseException& e) {
//         wxLogError(e.what());
//     }
// }

void MainFrame::show_database_manager() {
    wxDatabaseManagerDialog* db_manager_dlg = new wxDatabaseManagerDialog(this);
    db_manager_dlg->ShowModal();
    db_manager_dlg->Destroy();
}
// void MainFrame::show_table_manager() {
//     wxTableManagerDialog* tb_manager_dlg = new wxTableManagerDialog(this);
//     tb_manager_dlg->ShowModal();
//     tb_manager_dlg->Destroy();
// }
void MainFrame::show_welcome_panel() {
    if (current_panel_) current_panel_->Destroy();
    current_panel_ = new WelcomePanel(this, db_connection_);
}
// void MainFrame::refresh_current_panel() {
//     if (auto* editor_panel =
//                 dynamic_cast<DatabaseEditorPanel*>(current_panel_)) {
//         editor_panel->refresh_tb_list();
//     }
// }