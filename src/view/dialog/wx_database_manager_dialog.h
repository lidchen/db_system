#ifndef WX_DATABASE_MANAGER_PANEL_H_
#define WX_DATABASE_MANAGER_PANEL_H_
#include "src/model/db_connection.h"
#include "src/model/db_model.h"
#include "src/view/gui_components/listbox.h"
#include "wx/wx.h"
#include "wx_base_dialog.h"

class wxDatabaseManagerDialog : public wxBaseDialog {
   public:
    explicit wxDatabaseManagerDialog(wxWindow* parent);

   private:
    wxTextCtrl* db_dir_st_path_;
    Listbox* db_listbox_;
    int ID_database_name_;
    void test();

    void list_database();
    bool set_selected_database();
    void on_load_database(wxCommandEvent& event);
    void on_set_dir(wxCommandEvent& event);
    void on_create_database(wxCommandEvent& event);
    void on_delete_selected_database(wxCommandEvent& event);
    void on_db_click_select(wxCommandEvent& event);
    // void on_ok(wxCommandEvent& event) override;
    // void on_cancel(wxCommandEvent& event) override;
};

#endif  // WX_CREATA_DATABASE_DIALOG
