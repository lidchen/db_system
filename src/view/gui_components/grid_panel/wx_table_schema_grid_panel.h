#ifndef WX_TABLE_SCHEMA_GRID_PANEL_H_
#define WX_TABLE_SCHEMA_GRID_PANEL_H_

#include "wx_editable_grid_panel.h"
#include "core/database/database_table_manager.h"

enum class TableSchemaGridMode {
    CREATE_TABLE,
    ALTER_TABLE
};

class wxTableSchemaGridPanel : public wxEditableGridPanel {
public:
    wxTableSchemaGridPanel(wxWindow* parent , DatabaseTableManager* tb_manager
        , TableSchemaGridMode mode = TableSchemaGridMode::ALTER_TABLE);
    std::vector<ColumnDefinition> generate_col_defs(); // For CREATE_MODE
    void init_grid_cols() override;
    void update_grid() override;
private:
    TableSchemaGridMode mode_;
    DatabaseTableManager* tb_manager_;
    void append_new_row() override;
    void commit_cell(int row, int col, const std::string& new_value, const std::string& prev_value) override;
    void commit_row(int row) override;
    void commit_row_delete(int row) override;

    bool is_create_mode() const {
        return mode_ == TableSchemaGridMode::CREATE_TABLE;
    }
    ColumnDefinition get_col_def_from_row(int row_index);
    wxArrayString type_choices;
    wxArrayString constraint_choices;
};

#endif // WX_TABLE_SCHEMA_GRID_PANEL_H_
