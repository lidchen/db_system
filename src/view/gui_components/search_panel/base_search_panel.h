#ifndef BASE_SEARCH_PANEL_H_
#define BASE_SEARCH_PANEL_H_

#include "wx/wx.h"
#include "gui/event/custom_event.h"
#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"

class BaseSearchPanel : public wxPanel {
public:
    BaseSearchPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
        tb_manager_ = DatabaseManager::getInstance().get_tb_manager();
    }
    virtual std::string build_search_sql() = 0;
    virtual void update() = 0;
protected:
    DatabaseTableManager* tb_manager_;
};

#endif // BASE_SEARCH_PANEL_Ht_