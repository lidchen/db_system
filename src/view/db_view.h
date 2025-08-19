#pragma once
#include <filesystem>
#include "src/event/event_broker.h"
#include "src/view/dialog/wx_base_dialog.h"
#include "src/view/dialog/wx_database_manager_dialog.h"
#include "src/view/i_view.h"

class DbView : public IView, public wxBaseDialog {
    using EvtPtr = std::shared_ptr<EventBroker>;

   public:
    explicit DbView(EvtPtr evt_ptr);
    void Show() override;
    void Refresh() override;
    void PublishReqDbList();

    // std::vector<std::string> PublishScanDbDir();
    // void PublishConnect(const std::string& db_name);
    // void PublishSetDir(std::filesystem::path path);
    // std::string PublishFormatFileName(const std::string& raw_input_name);
    // void PublishDelete(const std::string& selected_db_name);

   private:
    EvtPtr event_broker_;
    wxPanel* content_panel_ = GetContentPanel();
    wxSizer* content_sizer_ = GetContentSizer();
    wxTextCtrl* db_dir_st_path_;
    Listbox* db_listbox_;
    void InitEvents();
    void ListDatabase(const std::vector<std::string>& db_names);
    bool SetSelectedDatabase();
    void OnLoadDatabase(const wxCommandEvent& event);
    void OnDbClickSelect(const wxCommandEvent& event);
    void OnSetDir(const wxCommandEvent& event);
    void OnNew(const wxCommandEvent& event);
    void OnDelete(const wxCommandEvent& event);
    void OnOk(const wxCommandEvent& event);
    void OnCancel(const wxCommandEvent& event);
};