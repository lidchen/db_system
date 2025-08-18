#include "wx_database_editor_grid_panel.h"

wxDatabaseEditorGridPanel::wxDatabaseEditorGridPanel(wxWindow* parent, DatabaseTableManager* tb_manager)
    : tb_manager_(tb_manager)
    , wxEditableGridPanel(parent)
{
    grid_->CreateGrid(0, 0);
    init_grid_cols();
    update_grid();

    Layout();
}

void wxDatabaseEditorGridPanel::set_table() {
    auto& db_manager = DatabaseManager::getInstance();
    db_ = db_manager.get_current_database();

    // helper local variables
    schema_ = tb_manager_->get_current_table_schema();
    table_name_ = tb_manager_->get_current_table_name();
    col_names_ = schema_.get_col_names();
    col_num_ = col_names_.size();
    pk_name_ = schema_.get_pk_name();
    pk_index_ = schema_.get_pk_index();

assert(pk_index_ != -1 
    && (schema_.get_col_names()[pk_index_] == pk_name_));
}

void wxDatabaseEditorGridPanel::init_grid_cols() {
    set_table();
    init_state();
    remove_existing_grid_rows();
    remove_existing_grid_cols();

    grid_->AppendCols(col_num_);

    grid_->SetColLabelSize(25);
    grid_->SetRowLabelSize(10);
    // grid_->HideRowLabels();
    grid_->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    grid_->SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    grid_->DisableDragRowSize();
    // grid_->DisableDragColSize();
    // Set column labels
    for (int col = 0; col < col_num_; ++col) {
        grid_->SetColLabelValue(col, col_names_[col]);
    }
}

void wxDatabaseEditorGridPanel::update_grid() {
    update_grid_with_sql("");
}

void wxDatabaseEditorGridPanel::update_grid_with_sql(const std::string& sql) {
    std::string sql_ = sql;
    remove_existing_grid_rows();
    // update grid using query result
    if (sql.empty()) 
        sql_ = "SELECT * FROM '" + tb_manager_->get_current_table_name() + "'";

    grid_->BeginBatch();
    try {
        auto row_callback = [this, count = 0](const std::vector<std::string> col_names, const std::vector<std::string>& row_value) mutable {
            int new_row = grid_->GetNumberRows();
            grid_->AppendRows(1);
            for (int col = 0; col < col_num_; ++col) {
                grid_->SetCellValue(new_row, col, row_value[col]);
            }
            // Yield to the event loop to keep UI responsive
            if (++count % 500 == 0) {
                wxSafeYield();
            }
        };
        db_->execute_sql(sql_, row_callback);
    } catch (const DatabaseException& e) {
        wxLogError("Failed update grid: %s", e.what());
    }
    grid_->EndBatch();

    // Set Grid Editor
    auto col_defs = tb_manager_->get_current_table_schema().col_defs_;
    for (size_t col = 0; col < grid_->GetNumberCols(); ++col) {
        grid_->auto_assign_col_editors(col, col_defs[col].type_); 
    }
    grid_->AutoSizeColumns();
    grid_->AutoSizeRows();
}

void wxDatabaseEditorGridPanel::append_new_row() {
    actual_rows_++;
    grid_->AppendRows(1);
    grid_->auto_assign_row_editors(actual_rows_ - 1, schema_);
}

std::string wxDatabaseEditorGridPanel::get_pk_value(int row) {
    // Get the value of the primary key in the given row
    wxString pk_value = grid_->GetCellValue(row, pk_index_);
    return pk_value.ToStdString();
}

void wxDatabaseEditorGridPanel::commit_cell(int row, int col, const std::string& new_value, const std::string& prev_value) {
    const std::string& col_name = col_names_[col];
    std::string pk_value;
    // Change pk, prev_cell_value is prepared for this situation
    if (col == pk_index_) {
        pk_value = prev_value;
    } else {
        pk_value = get_pk_value(row);
    }
    DatabaseStmtBuilder builder;
    std::string sql = builder.update(table_name_)
        .set(col_name, new_value)
        .where()
        .column(pk_name_).equals(pk_value)
        .build_with_and();
    std::cout << sql << "\n";
    db_->execute_sql(sql); 
}

void wxDatabaseEditorGridPanel::commit_row(int row) {
    std::vector<std::string> values;
    for (int col = 0; col < col_num_; ++col) {
        wxString cell_value = grid_->GetCellValue(row, col);
        values.push_back(cell_value.ToStdString());
        // builder.value(col_names_[col], cell_value.ToStdString());
    }
    DatabaseStmtBuilder builder;
    std::string sql = builder
        .insert_into(table_name_)
        .values(values)
        .build();
    db_->execute_sql(sql);
}

void wxDatabaseEditorGridPanel::commit_row_delete(int row) {
    const std::string& pk_value = get_pk_value(row);
    DatabaseStmtBuilder builder;
    std::string sql = builder.delete_from(table_name_)
        .where()
        .column(pk_name_).equals(pk_value)
        .build_with_and();
    db_->execute_sql(sql);    
}