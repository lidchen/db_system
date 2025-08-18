#pragma once
#include "src/view/dialog/wx_database_manager_dialog.h"
#include "src/view/i_view.h"
#include "wx/dialog.h"

class DbView : public IView {
   public:
    void show() override {
        wxDatabaseManagerDialog* db_dlg = new wxDatabaseManagerDialog(nullptr);
        db_dlg->ShowModal();
    }
    void refresh() override { std::cerr << "refresh\n"; }
};