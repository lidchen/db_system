#include "db_view.h"

#include "src/model/db_model.h"
#include "src/view/db_view.h"
#include "src/view/dialog/wx_get_file_path_dialog.h"
#include "src/view/gui_components/horizontal_panel.h"
#include "src/view/gui_components/listbox.h"
#include "src/utils/logger.h"

DbView::DbView(EvtPtr event_broker)
    : wxBaseDialog(nullptr, "Manage Database"), event_broker_(event_broker) {
    // DB DIR HANDLE
    auto* db_dir_panel = new HorizontalPanel(content_panel_);
    auto* db_dir_st =
            new wxStaticText(db_dir_panel, wxID_ANY, wxT("Current db path: "));
    db_dir_st_path_ = new wxTextCtrl(db_dir_panel,
                                     wxID_ANY,
                                     "",
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_READONLY | wxBORDER_NONE);

    auto* db_dir_btn = new wxButton(content_panel_, wxID_ANY, "set dir");
    db_dir_panel->add_children(db_dir_st, db_dir_st_path_);

    // LIST BOX SHOW DB IN DB_DIR
    db_listbox_ = new Listbox(content_panel_);
    auto* new_btn = new wxButton(db_listbox_->btn_panel_, wxID_ANY, "New");
    auto* rename_btn =
            new wxButton(db_listbox_->btn_panel_, wxID_ANY, "Rename");
    auto* delete_btn =
            new wxButton(db_listbox_->btn_panel_, wxID_ANY, "Delete");
    db_listbox_->add_btn(new_btn);
    db_listbox_->add_btn(rename_btn);
    db_listbox_->add_btn(delete_btn);

    auto content_sizer = new wxBoxSizer(wxVERTICAL);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_dir_btn);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_dir_panel);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_listbox_, 1, wxEXPAND | wxALL, 0);

    content_panel_->SetSizer(content_sizer);

    // new_btn->Bind(wxEVT_BUTTON, &DbView::OnNew, this);
    // delete_btn->Bind(wxEVT_BUTTON, &DbView::OnDelete, this);
    // db_dir_btn->Bind(wxEVT_BUTTON, &DbView::OnSetDir, this);
    // Bind(wxEVT_BUTTON, &DbView::OnOk, this, wxID_OK);
    // Bind(wxEVT_BUTTON, &DbView::OnCancel, this, wxID_CANCEL);
    // db_listbox_->Bind(wxEVT_LISTBOX_DCLICK,
    //                   &DbView::OnDbClickSelect,
    //                   this);
    InitEvents();
}

void DbView::Show() {
    wxDialog::ShowModal();
}

void DbView::Refresh() {
    wxDialog::Refresh();
}

void DbView::InitEvents() {
    // ListDatabase
    event_broker_->Subscribe<ResDbList>([this](const ResDbList& event){
        ListDatabase(event.GetPayload()); 
        LOG(LogCategory::EVENT_SUBSCRIBED, "DbView ResDbList");
    });
}

// call this after set dir
void DbView::PublishReqDbList() {
    event_broker_->Publish(ReqDbList{});
    LOG(LogCategory::EVENT_PUBLISH, "DbView ReqDbList");
}

void DbView::ListDatabase(const std::vector<std::string>& db_names) {
    db_listbox_->clear_list();
    for (const auto& db_name : db_names) {
        db_listbox_->append_to_list(db_name);
    }
}

/*
bool DbView::SetSelectedDatabase() {
    auto selected_db_name = db_listbox_->get_selected_value();
    if (!selected_db_name.empty()) {
        db_model_.connect(selected_db_name);
        return true;
    } else {
        wxLogError("Please select a database");
        return false;
    }
}

void DbView::OnSetDir(const wxCommandEvent& event) {
    auto cb = [this](std::filesystem::path path) {
        db_dir_st_path_->SetLabel(path.string());
        content_panel_->GetSizer()->Layout();
        // Call on_list_database after setting the directory
        db_model_.set_db_dir(path);
        ListDatabase();
    };
    // auto* dir_dialog = new wxGetFolderPathDialog(nullptr, cb);
    // dir_dialog->ShowModal();
}

// Bind this with new button
void DbView::OnNew(const wxCommandEvent& event) {
    using enum DbException::DbErrorType;
    wxTextEntryDialog dlg(
            this, wxT("Enter new database name:"), wxT("Create Database"));
    if (dlg.ShowModal() == wxID_OK) {
        auto db_raw_input_name = dlg.GetValue().ToStdString();
        std::string db_formatted_name =
                db_model_.format_file_name(db_raw_input_name);

        try {
            db_model_.create_db(db_formatted_name);
        } catch (const DbException& e) {
            wxMessageBox(e.what());
            return;
        }

        // ListDatabase();
        db_listbox_->append_to_list(db_formatted_name);
    }
}
void DbView::OnDelete(const wxCommandEvent& event) {
    std::string selected_db_name = db_listbox_->get_selected_value();
    if (wxMessageBox(wxString::Format("Are you sure you want to delete "
                                      "database '%s'?",
                                      selected_db_name),
                     "Confirm Delete",
                     wxYES_NO | wxICON_QUESTION,
                     this) != wxYES)
        return;

    try {
        db_model_.delete_db(selected_db_name);
        db_listbox_->remove_selection();
    } catch (const DbException& e) {
        wxLogError(e.what());
    }
}
void DbView::OnDbClickSelect(const wxCommandEvent& event) {
    if (SetSelectedDatabase()) {
        EndModal(wxID_OK);
    }
}
// void DbView::on_ok(wxCommandEvent& event) {
//     if (set_selected_database()) {
//         EndModal(wxID_OK);
//     }
// }

// void DbView::on_cancel(wxCommandEvent& event) {
//     EndModal(wxID_CANCEL);
// }

*/