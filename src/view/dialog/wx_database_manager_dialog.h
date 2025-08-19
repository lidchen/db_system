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
    wxPanel* content_panel_ = GetContentPanel();
    wxSizer* content_sizer_ = GetContentSizer();

    wxTextCtrl* db_dir_st_path_;
    Listbox* db_listbox_;
    void ListDatabase();
    bool SetSelectedDatabase();
    void OnLoadDatabase(const wxCommandEvent& event);
    void OnDbClickSelect(const wxCommandEvent& event);
    void OnSetDir(const wxCommandEvent& event);
    void OnNew(const wxCommandEvent& event);
    void OnDelete(const wxCommandEvent& event);
    void OnOk(const wxCommandEvent& event);
    void OnCancel(const wxCommandEvent& event);
};

#endif  // WX_CREATA_DATABASE_DIALOG
