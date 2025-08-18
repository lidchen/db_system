#ifndef ADVANCE_SEARCH_PANEL_H_
#define ADVANCE_SEARCH_PANEL_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "base_search_panel.h"
#include "gui/gui_components/smart_grid.h"

class AdvanceSearchPanel : public BaseSearchPanel {
public:
    AdvanceSearchPanel(wxWindow* parent);
    void update() override;
    std::string build_search_sql() override;
    wxButton* get_search_btn() const;
private:
    int max_label_width_;
    wxButton* search_btn_;
    SmartGrid* config_grid_;
    wxArrayString on_off_choices;
};

#endif // ADVANCE_SEARCH_PANEL_H_