#ifndef HORIZONTAL_PANEL_H_
#define HORIZONTAL_PANEL_H_

#include "wx/wx.h"

class HorizontalPanel : public wxPanel {
public:
    HorizontalPanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
        {}

    template<typename... Args>
    void add_children(Args... children) 
    {
        static_assert((std::is_convertible_v<Args, wxWindow*> && ...), "All args should be wxWindow*");
        if (!sizer_) {
            sizer_ = new wxBoxSizer(wxHORIZONTAL);
            SetSizer(sizer_);
        }
        (sizer_->Add(children, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10), ...);
        Layout();
    }
private:
    wxBoxSizer* sizer_ = nullptr;
};

#endif // HORIZONTAL_PANEL_H_