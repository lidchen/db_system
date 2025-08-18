#include <filesystem>

#include "src/model/db_model.h"
#include "src/view/dialog/wx_get_file_path_dialog.h"
#include "src/view/gui_components/horizontal_panel.h"
#include "src/view/gui_components/listbox.h"
#include "wx_database_manager_dialog.h"

wxDatabaseManagerDialog::wxDatabaseManagerDialog(wxWindow* parent)
    : wxBaseDialog(parent, "Manage Database") {
    ID_database_name_ = wxWindow::NewControlId();

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
    // db_dir_btn->Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_set_dir, this);
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

    wxBoxSizer* content_sizer = new wxBoxSizer(wxVERTICAL);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_dir_btn);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_dir_panel);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_listbox_, 1, wxEXPAND | wxALL, 0);

    content_panel_->SetSizer(content_sizer);

    // new_btn->Bind(
    //         wxEVT_BUTTON, &wxDatabaseManagerDialog::on_create_database, this);
    // delete_btn->Bind(wxEVT_BUTTON,
    //                  &wxDatabaseManagerDialog::on_delete_selected_database,
    //                  this);

    // Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_ok, this, wxID_OK);
    // Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_cancel, this, wxID_CANCEL);
    // db_listbox_->Bind(wxEVT_LISTBOX_DCLICK,
    //                   &wxDatabaseManagerDialog::on_db_click_select,
    //                   this);
}
void wxDatabaseManagerDialog::test() {
    std::cerr << "test from dbdlg\n";
}

/*
// call this after set dir
void wxDatabaseManagerDialog::list_database() {
    db_listbox_->clear_list();
    db_model_.scan_db_dir();
    auto db_names = db_model_.get_db_names();
    for (const auto& db_name : db_names) {
        db_listbox_->append_to_list(db_name);
    }
}

bool wxDatabaseManagerDialog::set_selected_database() {
    auto selected_db_name = db_listbox_->get_selected_value();
    if (!selected_db_name.empty()) {
        db_model_.connect(selected_db_name);
        return true;
    } else {
        wxLogError("Please select a database");
        return false;
    }
}

void wxDatabaseManagerDialog::on_set_dir(wxCommandEvent& event) {
    auto cb = [this](std::filesystem::path path) {
        db_dir_st_path_->SetLabel(path.string());
        db_model_.set_db_dir(path);
        content_panel_->GetSizer()->Layout();
        // Call on_list_database after setting the directory
        list_database();
    };
    // auto* dir_dialog = new wxGetFolderPathDialog(nullptr, cb);
    // dir_dialog->ShowModal();
}

// Bind this with new button
void wxDatabaseManagerDialog::on_create_database(wxCommandEvent& event) {
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

        // list_database();
        db_listbox_->append_to_list(db_formatted_name);
    }
}
void wxDatabaseManagerDialog::on_delete_selected_database(
        wxCommandEvent& event) {
    std::string selected_db_name = db_listbox_->get_selected_value();
    if (wxMessageBox(wxString::Format(
                             "Are you sure you want to delete database '%s'?",
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
void wxDatabaseManagerDialog::on_db_click_select(wxCommandEvent& event) {
    if (set_selected_database()) {
        EndModal(wxID_OK);
    }
}
void wxDatabaseManagerDialog::on_ok(wxCommandEvent& event) {
    if (set_selected_database()) {
        EndModal(wxID_OK);
    }
}

void wxDatabaseManagerDialog::on_cancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}
*/