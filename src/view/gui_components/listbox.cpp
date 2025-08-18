#include "listbox.h"
#include "src/model/db_exception.h"

Listbox::Listbox(wxWindow* parent) 
    : wxPanel(parent, wxID_ANY) 
{
    list_panel_ = new wxListBox(this, wxID_ANY);
    btn_panel_ = new wxPanel(this, wxID_ANY);

    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    h_sizer->Add(list_panel_, 3, wxEXPAND | wxLEFT | wxRIGHT, 20);
    h_sizer->Add(btn_panel_, 1, wxEXPAND | wxRIGHT, 20);
    SetSizer(h_sizer);

    btn_sizer_ = new wxBoxSizer(wxVERTICAL);
    btn_panel_->SetSizer(btn_sizer_);
}
void Listbox::add_btn(wxButton* btn) {
    // Set a fixed size for the button
    btn->SetMinSize(wxSize(60, 60));
    btn_sizer_->Add(btn, 0, wxTOP | wxLEFT | wxRIGHT, 10);
    btn_panel_->Layout();
}
void Listbox::append_to_list(const wxString& info) {
    assert(info.Len() > 0);
    list_panel_->Append(info);
    list_panel_->Layout();
}
void Listbox::clear_list() {
    list_panel_->Clear();
}
void Listbox::remove_selection() {
    int sel = list_panel_->GetSelection();
    if (sel != wxNOT_FOUND) {
        list_panel_->Delete(sel);
    }
}
int Listbox::get_selection() {
    return list_panel_->GetSelection();
}
void Listbox::rename_selection(std::string new_value) {
    int pos = list_panel_->GetSelection();
    list_panel_->Delete(pos);
    list_panel_->Insert(new_value, pos);
}
std::string Listbox::get_selected_value() {
    int sel = list_panel_->GetSelection();
    if (sel == wxNOT_FOUND) {
        throw(DbException("Please select a value", DbException::DbErrorType::NOT_FOUND));
    }
    return list_panel_->GetString(sel).ToStdString();
}
// void Listbox::on_dbl_click(wxCommandEvent& event) {
//     int sel = list_panel_->GetSelection();
// }