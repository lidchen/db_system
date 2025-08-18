#include "wx_editable_grid_panel.h"
#include <core/database/database_exception.h>
#include "cli/cli_util.h"
#include "gui/event/evt_cell_commit.h"

wxEditableGridPanel::wxEditableGridPanel(wxWindow* parent) 
    : wxPanel(parent, wxID_ANY)
{
    grid_panel_ = new wxPanel(this, wxID_ANY);
    btn_pannel_ = new wxPanel(this, wxID_ANY);
    grid_ = new SmartGrid(grid_panel_);
    add_btn_ = new wxButton(btn_pannel_, wxID_ANY, "Add");
    delete_btn_ = new wxButton(btn_pannel_, wxID_ANY, "Delete");

    h_sizer_ = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* grid_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* btn_sizer = new wxBoxSizer(wxVERTICAL);

    init_state();

    grid_sizer->Add(grid_, 1, wxEXPAND);
    grid_panel_->SetSizer(grid_sizer);
    btn_sizer->Add(add_btn_, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    btn_sizer->Add(delete_btn_, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    btn_pannel_->SetSizer(btn_sizer);
    h_sizer_->Add(grid_panel_, 1, wxEXPAND | wxALL, 10);
    h_sizer_->Add(btn_pannel_, 0, wxEXPAND | wxALL, 10);

    SetSizer(h_sizer_);

    grid_panel_->Bind(EVT_CELL_COMMIT, &wxEditableGridPanel::on_cell_change, this);
    add_btn_->Bind(wxEVT_BUTTON, &wxEditableGridPanel::on_add_or_commit_row, this);
    delete_btn_->Bind(wxEVT_BUTTON, &wxEditableGridPanel::on_delete_row, this);
}
void wxEditableGridPanel::init_state() {
    actual_rows_ = grid_->GetNumberRows();
    new_row_index_ = -1;
    set_btn_status();
}
void wxEditableGridPanel::set_btn_status() {
    if (add_btn_) {
        if (new_row_index_ == -1) {
            add_btn_->SetLabel("Add");
            add_btn_->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        } else {
            add_btn_->SetLabel("OK");
            add_btn_->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        }
    }
}

void wxEditableGridPanel::on_cell_change(EvtCellCommit& event) {
    int row = event.get_row();

    // if is editing new line, ignore cell commit
    if (row == new_row_index_) {
        return;
    }

    int col = event.get_col();
    std::string prev_cell_value = event.get_prev_value();
    std::string new_value = event.get_value();
    try {
        commit_cell(row, col, new_value, prev_cell_value);
    } catch (const DatabaseException& e) {
        wxMessageBox("Error updating database: " + wxString(e.what()));
        grid_->SetCellValue(row, col, prev_cell_value);
    }
}

void wxEditableGridPanel::on_add_or_commit_row(wxCommandEvent& event) {
    actual_rows_ = grid_->GetNumberRows();
    // CLIUtil::print<int>(actual_rows_, "actual_rows");
    // CLIUtil::print<int>(new_row_index_, "new_row_index");
    // CLIUtil::split();
    if (new_row_index_ == -1) {
        // actual_rows_ should be changed inside append_new_row();
        append_new_row();
        new_row_index_ = actual_rows_ - 1;
        grid_->SetGridCursor(new_row_index_, 0);
        grid_->SetFocus();
        grid_->SelectBlock(new_row_index_, 0, new_row_index_, grid_->GetNumberCols());
        grid_->MakeCellVisible(new_row_index_, 0);
        set_btn_status();
    } else {
        // Commit row state
        try {
            commit_row(new_row_index_);
            new_row_index_ = -1;
            set_btn_status();
        } catch (const DatabaseException& e) {
            wxLogError(e.what());
            event.Skip();
        }
    }
}

void wxEditableGridPanel::on_delete_row(wxCommandEvent& event) {
    // Get selected rows
    wxArrayInt selectedRows = grid_->GetSelectedRows();
    if (selectedRows.IsEmpty()) {
        wxLogWarning("Please select a row to delete.");
        return;
    }

    // Highlight the selected row
    int row = selectedRows[0];
    grid_->SelectRow(row, true); // true to deselect others
    grid_->SetGridCursor(row, 0);
    SetFocus();
    // Only allow deleting one row at a time for safety
    std::string col_name = grid_->GetCellValue(row, 0).ToStdString();
    // Only allow deleting one row at a time for safety
    std::string cell_value = grid_->GetCellValue(row, 0).ToStdString();

    // Confirm deletion
    int res = wxMessageBox(
        wxString::Format("Are you sure you want to delete column '%s'?", cell_value),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION, this);
    if (res != wxYES)
        return;
    try {
        commit_row_delete(row);
        grid_->DeleteRows(row);

        // Idiotic approach: update the new_row_index
        if (row > new_row_index_) new_row_index_++;
        else if (row < new_row_index_) new_row_index_--;
        else if (row == new_row_index_) {
        // If delete pending new line, init btn and new_row_index
            new_row_index_ = -1;
            set_btn_status();
        }
    } catch (DatabaseException& e) {
        wxLogError("Failed to delete column: %s", wxString(e.what()));
    }
}

void wxEditableGridPanel::remove_existing_grid_rows() {
    if (grid_->GetNumberRows() > 0) {
        grid_->DeleteRows(0, grid_->GetNumberRows());
    }
}

void wxEditableGridPanel::remove_existing_grid_cols() {
    if (grid_->GetNumberCols() > 0) {
        grid_->DeleteCols(0, grid_->GetNumberCols());
    }
}