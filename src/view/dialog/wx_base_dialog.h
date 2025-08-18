#ifndef WX_BASE_SUB_PANEL_H_
#define WX_BASE_SUB_PANEL_H_

#include "wx/wx.h"

class wxBaseDialog : public wxDialog {
public:
    // wxBaseDialog(wxWindow* parent);
protected:
    wxBaseDialog(wxWindow *parent, const wxString& name);
    virtual void on_ok(wxCommandEvent& event);
    virtual void on_cancel(wxCommandEvent& event);
    void on_escape(wxKeyEvent& event);
    wxPanel* content_panel_;
    wxSizer* content_sizer_;
};

#endif // WX_BASE_SUB_PANEL_H_
