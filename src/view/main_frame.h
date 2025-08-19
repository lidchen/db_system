#pragma once
#include <wx/wx.h>
#include "src/model/db_connection.h"

class MainFrame : public wxFrame
{
public:
 explicit MainFrame(const wxString& name);
 void InitUI();
 void show_welcome_panel();
 void show_db_editor_panel();
 void show_database_manager();
 void show_table_manager();
 void refresh_current_panel();
private:
    std::shared_ptr<DbConnection> db_connection_;
    wxPanel* current_panel_ = nullptr;
    wxMenuBar *menubar;
    wxMenu* file;
    wxMenu* import_from_csv;
    wxMenu* database_manager;
    void on_db_manager(const wxCommandEvent& event);
    void on_quit(wxCommandEvent &event);
};