#include "wx_table_schema_grid_panel.h"
#include "core/database/database_manager.h"

wxTableSchemaGridPanel::wxTableSchemaGridPanel(wxWindow* parent
    , DatabaseTableManager* tb_manager, TableSchemaGridMode mode)
    : tb_manager_(tb_manager) 
    , mode_(mode)
    , wxEditableGridPanel(parent)
{
    type_choices = ColumnDefinition::get_col_types();
    constraint_choices = ColumnDefinition::get_col_constraints();

    init_grid_cols();
    update_grid();

    Layout();
}
void wxTableSchemaGridPanel::init_grid_cols() {
    init_state();
    remove_existing_grid_cols();
    remove_existing_grid_rows();
    grid_->CreateGrid(0, 0);
    int row;
    if (is_create_mode()) {
        row = 0;
    } else {
        row = tb_manager_->get_current_table_schema().get_row_num();
    }
    grid_->AppendRows(row);
    grid_->AppendCols(3);

    // grid_->HideRowLabels();
    grid_->SetRowLabelSize(10);
    grid_->SetColLabelValue(0, "name");
    grid_->SetColLabelValue(1, "type");
    grid_->SetColLabelValue(2, "constraints");

    grid_->SetColSize(0, 160);
    grid_->SetColSize(1, 160);
    grid_->SetColSize(2, 300);
    // grid_->DisableCellEditControl();

    grid_->DisableDragRowSize();
    grid_->DisableDragColSize();
}

void wxTableSchemaGridPanel::update_grid() {
    if (!is_create_mode()) {
        remove_existing_grid_rows();
        TableSchema schema = tb_manager_->get_current_table_schema();
        int row = schema.get_row_num();
        grid_->AppendRows(row);

        int current_row = 0;
        for (const ColumnDefinition& col_def : schema.col_defs_) {
            grid_->SetCellValue(wxGridCellCoords(current_row, 0), col_def.name_); 
            grid_->SetCellValue(wxGridCellCoords(current_row, 1), col_def.type_to_string()); 
            grid_->SetCellValue(wxGridCellCoords(current_row, 2), col_def.constraints_to_string()); 
            current_row++;
        }
        actual_rows_ = grid_->GetNumberRows();

        for (int row = 0; row < actual_rows_;++row) {
            for (int col = 0; col < 3; ++col) {
                grid_->SetReadOnly(row, col, true);
            }
        }
    }
}

void wxTableSchemaGridPanel::append_new_row() {
    // do change actual_rows
    actual_rows_++;
    int last_row_index = actual_rows_ - 1;
    grid_->AppendRows(1);
    grid_->EnableCellEditControl(false);
    grid_->SetReadOnly(last_row_index, 0, false);
    grid_->SetReadOnly(last_row_index, 1, false);
    grid_->SetReadOnly(last_row_index, 2, false);
    grid_->SetCellEditor(last_row_index, 0, new wxGridCellTextEditor());
    grid_->SetCellEditor(last_row_index, 1, new wxGridCellChoiceEditor(type_choices));
    grid_->SetCellEditor(last_row_index, 2, new wxGridCellChoiceEditor(constraint_choices));
}
void wxTableSchemaGridPanel::commit_cell(int row, int col, const std::string& new_value, const std::string& prev_value) {
    // NO, YOU CANT CHANGE COLUMN IN SQLITE
}
void wxTableSchemaGridPanel::commit_row(int row) {
    if (!is_create_mode()) {
        tb_manager_->add_column(get_col_def_from_row(row));

        int last_row_index = actual_rows_ - 1;
        grid_->EnableCellEditControl(true);
        grid_->SetReadOnly(last_row_index, 0, true);
        grid_->SetReadOnly(last_row_index, 1, true);
        grid_->SetReadOnly(last_row_index, 2, true);
        
    } else {
        // In create mode, check for duplicate column names in the grid
        std::string new_col_name = grid_->GetCellValue(row, 0).ToStdString();
        for (int i = 0; i < actual_rows_; ++i) {
            if (i != row) {
                std::string existing_col_name = grid_->GetCellValue(i, 0).ToStdString();
                if (!new_col_name.empty() && new_col_name == existing_col_name) {
                    throw DatabaseException("Duplicate column name: " + new_col_name);
                    // Optionally, clear the duplicate or set focus
                    grid_->SetGridCursor(row, 0);
                    grid_->SetFocus();
                    return;
                }
            }
        }
        // No duplicate found, nothing else to do in create mode
    }
}

void wxTableSchemaGridPanel::commit_row_delete(int row) {
    // Get selected rows
    // This could for mult row deletion
    wxArrayInt selectedRows = grid_->GetSelectedRows();
    if (selectedRows.IsEmpty()) {
        wxLogWarning("Please select a row to delete.");
        return;
    }
    int res = wxMessageBox(
        wxString::Format("Are you sure you want to delete column '%s'?", grid_->GetCellValue(selectedRows[0], 0)),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION, this);
    if (res != wxYES) return;

    // Highlight the selected row
    // int first_selected_row = selectedRows[0];
    // grid_->SelectRow(first_selected_row, true); // true to deselect others
    // grid_->SetGridCursor(first_selected_row, 0);
    // grid_->SetFocus();
    // Only allow deleting one row at a time for safety
    if(!is_create_mode()) {
        std::string col_name = grid_->GetCellValue(row, 0).ToStdString();
        try {
            tb_manager_->drop_column(col_name);
            actual_rows_--;
            update_grid();
        } catch (DatabaseException& e) {
            wxLogError("Failed to delete column: %s", e.what());
        }
    } else {
        grid_->DeleteRows(selectedRows[0], 1);
    }
}
std::vector<ColumnDefinition> wxTableSchemaGridPanel::generate_col_defs() {
    std::vector<ColumnDefinition> col_defs;
    for (size_t row = 0; row < actual_rows_; ++row) {
        col_defs.push_back(get_col_def_from_row(row));
    }
    return col_defs;
}

ColumnDefinition wxTableSchemaGridPanel::get_col_def_from_row(int row_index) {
    std::string name = grid_->GetCellValue(row_index, 0).ToStdString(); 
    std::string type = grid_->GetCellValue(row_index, 1).ToStdString(); 
    std::string constraints = grid_->GetCellValue(row_index, 2).ToStdString(); 
    return ColumnDefinition(name, type, constraints);
}