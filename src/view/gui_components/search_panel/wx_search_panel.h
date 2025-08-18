#ifndef WX_SEARCH_PANEL_H_
#define WX_SEARCH_PANEL_H_
#include "wx/wx.h"
#include "wx/notebook.h"
#include "gui/gui_components/search_panel/base_search_panel.h"
#include "gui/gui_components/search_panel/advance_search_panel.h"

class wxSearchPanel : public wxPanel{
public:
    wxSearchPanel(wxPanel* parent);
    void refresh();
    void bind_search_event();
    void on_search(wxCommandEvent& event);
    std::string build_search_sql();
    wxButton* search_btn_;
    wxTextCtrl* searched_value_tc_;
private:
    void setup_advance_search();
    void setup_basic_search();
    void on_toggle(wxCommandEvent& event);
    wxNotebook *nb_;
    BaseSearchPanel* basic_search_;
    AdvanceSearchPanel* advance_search_;
};

#endif // WX_SEARCH_PANEL_H_
