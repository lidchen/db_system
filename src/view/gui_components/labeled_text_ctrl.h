#ifndef LABELED_TEXT_CTRL_H_
#define LABELED_TEXT_CTRL_H_

#include <wx/wx.h>

class LabeledTextCtrl : public wxPanel {
public:
    LabeledTextCtrl(wxWindow *parent, wxWindowID id, const wxString& label, const wxString& value, const wxPoint &pos, const wxSize &size, long style);
    LabeledTextCtrl(wxWindow *parent, wxWindowID id, const wxString& label, const wxString& value, const wxPoint &pos, const wxSize &size);
    wxString get_value() const;
private:
    void setup(const wxString &label, const wxString &value);
    void on_text_enter(wxCommandEvent &event);
    wxStaticText *label_;
    wxTextCtrl* text_ctrl_;
};

#endif // LABELED_TEXT_CTRL_H_