#ifndef CB_SELECT_POPUP
#define CB_SELECT_POPUP

#include <vector>
#include "wx/wx.h"
#include "wx/popupwin.h"

class CbSelectPopup : public wxPopupTransientWindow {
public:
    CbSelectPopup(wxWindow* parent, std::vector<std::string> cb_names);
    CbSelectPopup(wxWindow* parent);
    void set_selection(const std::vector<bool> &selection);
    bool is_all_selected();
    void select_all();
    void deselect_all();
    void select_and_deselect_all();
    void update_checkboxes(const std::vector<std::string> &cb_names);
    std::vector<bool> get_selection();
private:
    wxBoxSizer* sizer_;
    std::vector<wxCheckBox*> cb_container_;
};

#endif // CB_SELECT_POPUP
