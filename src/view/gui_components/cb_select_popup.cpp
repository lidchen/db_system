#include "cb_select_popup.h"
CbSelectPopup::CbSelectPopup(wxWindow* parent) 
    : wxPopupTransientWindow(parent, wxBORDER_SIMPLE)
{
    sizer_ = new wxBoxSizer(wxVERTICAL);
    SetSizerAndFit(sizer_);
    // Add Select All Option
    wxButton* select_all_btn = new wxButton(this, wxID_ANY, "Select All", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    sizer_->Add(select_all_btn, 0, wxALL, 5);
    select_all_btn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { this->select_and_deselect_all(); });
}
CbSelectPopup::CbSelectPopup(wxWindow* parent, std::vector<std::string> cb_names) 
    : wxPopupTransientWindow(parent, wxBORDER_SIMPLE)
{
    sizer_ = new wxBoxSizer(wxVERTICAL);
    update_checkboxes(cb_names);
    SetSizerAndFit(sizer_);
    // Add Select All Option
    wxButton* select_all_btn = new wxButton(this, wxID_ANY, "Select All", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    sizer_->Add(select_all_btn, 0, wxALL, 5);
    select_all_btn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { this->select_and_deselect_all(); });
}
void CbSelectPopup::set_selection(const std::vector<bool>& selection) {
    for (size_t i = 0; i < cb_container_.size() && i < selection.size(); ++i) {
        cb_container_[i]->SetValue(selection[i]);
    }
}
bool CbSelectPopup::is_all_selected() {
    for (auto cb : cb_container_) {
        if (!cb->GetValue()) {
            return false;
        }
    } 
    return true;
}
void CbSelectPopup::select_all() {
    for (size_t i = 0; i < cb_container_.size(); ++i) {
        cb_container_[i]->SetValue(true);
    }
}
void CbSelectPopup::deselect_all() {
    for (size_t i = 0; i < cb_container_.size(); ++i) {
        cb_container_[i]->SetValue(false);
    }
}
void CbSelectPopup::select_and_deselect_all() {
    if (is_all_selected()) {
        deselect_all();
    } else {
        select_all();
    }
}
void CbSelectPopup::update_checkboxes(const std::vector<std::string>& cb_names) {
    if (!cb_container_.empty()) {
        for (auto cb : cb_container_) {
            cb->Destroy();
        }
        cb_container_.clear();
    }
    for (const auto& name : cb_names) {
        wxCheckBox* cb = new wxCheckBox(this, wxID_ANY, name);
        cb_container_.push_back(cb);
        sizer_->Add(cb, 0, wxALL, 5);
        Fit();
        Layout();
    }
}
std::vector<bool> CbSelectPopup::get_selection() {
    std::vector<bool> selection_info;
    for (auto const& cb : cb_container_) {
        selection_info.push_back(cb->GetValue());
    }
    return selection_info;
}