#include "labeled_text_ctrl.h"

LabeledTextCtrl::LabeledTextCtrl(wxWindow* parent, wxWindowID id
                                , const wxString& label, const wxString& value
                                , const wxPoint& pos, const wxSize& size, long style) 
                                : wxPanel(parent, id, pos, size)
{
    setup(label, value);
}

LabeledTextCtrl::LabeledTextCtrl(wxWindow* parent, wxWindowID id
                                , const wxString& label, const wxString& value
                                , const wxPoint& pos, const wxSize& size) 
                                : wxPanel(parent, id, pos, size)
{
    setup(label, value);
}

void LabeledTextCtrl::setup(const wxString& label, const wxString& value) {
    label_ = new wxStaticText(this, wxID_ANY, label);
    text_ctrl_ = new wxTextCtrl(this, wxID_ANY
                                , value, wxDefaultPosition, wxDefaultSize
                                , wxTE_PROCESS_ENTER);
    auto* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(label_, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    sizer->Add(text_ctrl_, 1, wxEXPAND);
    this->SetSizer(sizer);

    // Bind Enter key event
    text_ctrl_->Bind(wxEVT_TEXT_ENTER, &LabeledTextCtrl::on_text_enter, this);
}
void LabeledTextCtrl::on_text_enter(wxCommandEvent& event) {
    this->MoveAfterInTabOrder(this);
}
wxString LabeledTextCtrl::get_value() const {
    return text_ctrl_->GetValue();
}
