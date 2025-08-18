#ifndef BASIC_SEARCH_PANEL_H_
#define BASIC_SEARCH_PANEL_H_

#include "base_search_panel.h"
#include "wx/wx.h"
#include "wx/checkbox.h"
#include "gui/gui_components/cb_select_popup.h"
#include "core/database/database_table_manager.h"


class BasicSearchPanel : public BaseSearchPanel {
public:
    BasicSearchPanel(wxWindow* parent);
    void update() override;
    std::string build_search_sql() override;
    wxButton* get_search_btn() const { return search_btn_; }
    wxTextCtrl* get_searched_value_tc() const { return searched_value_tc_; }
private:
    wxButton* search_btn_;
    wxTextCtrl* searched_value_tc_;
private:
    CbSelectPopup* col_select_popup_;
    wxCheckBox* cb_fuzzy_;
    wxCheckBox* cb_case_insensitive_;
};

#endif // BASIC_SEARCH_PANEL_H_