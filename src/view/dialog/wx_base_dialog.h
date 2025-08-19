#ifndef WX_BASE_SUB_PANEL_H_
#define WX_BASE_SUB_PANEL_H_

#include "wx/wx.h"

class wxBaseDialog : public wxDialog {
public:
    wxBaseDialog(wxWindow *parent, const wxString& name);
    wxPanel* GetContentPanel() {return content_panel_;}
    wxSizer* GetContentSizer() {return content_sizer_;}
private:
    wxPanel* content_panel_;
    wxSizer* content_sizer_;
};

#endif // WX_BASE_SUB_PANEL_H_
