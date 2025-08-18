#pragma once
#include "wx/wx.h"
#include "src/model/db_connection.h"

class WelcomePanel : public wxPanel {
public:
    explicit WelcomePanel(wxWindow* parent, std::shared_ptr<DbConnection> db_connection);
private:
    std::shared_ptr<DbConnection> db_connection_;
    void on_db_manager(wxCommandEvent& event);
    void on_quit(wxCommandEvent& event);
};