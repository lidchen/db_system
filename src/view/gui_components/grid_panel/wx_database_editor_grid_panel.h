#ifndef WX_DATABASE_EDITOR_GRID_PANEL_H_
#define WX_DATABASE_EDITOR_GRID_PANEL_H_

#include "wx_editable_grid_panel.h"

#include <vector>
#include <string>

#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"
#include "core/database/schema/table_schema.h"

class wxDatabaseEditorGridPanel : public wxEditableGridPanel {
public:
    wxDatabaseEditorGridPanel(wxWindow* parent, DatabaseTableManager* tb_mananger);
    void init_grid_cols() override;
    void update_grid() override;
    void update_grid_with_sql(const std::string& sql);
private:
    void append_new_row() override;
    void commit_cell(int row, int col, const std::string& new_value, const std::string& prev_value) override;
    void commit_row(int row) override;
    void commit_row_delete(int row) override;
    std::string get_pk_value(int row);
    Database* db_;
    // passed from dlg's smart ptr
    DatabaseTableManager* tb_manager_;
    void set_table();
    void on_select_table(wxCommandEvent& event);

    // Helper local variable
    // For preformance, I dont want to get this every update
    TableSchema schema_;
    std::vector<std::string> col_names_;
    int col_num_;
    std::string pk_name_;
    std::string table_name_;
    int pk_index_;
};


#endif // WX_DATABASE_EDITOR_GRID_PANEL_H_