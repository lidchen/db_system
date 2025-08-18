#include <assert.h>
#include "smart_grid.h"
#include "gui/event/evt_cell_commit.h"

SmartGrid::SmartGrid(wxWindow* parent)
    : wxGrid(parent, wxID_ANY)
{
    // Bind(wxEVT_GRID_CELL_LEFT_CLICK, [=, this](wxGridEvent& event) {
    //     int row = event.GetRow();
    //     int col = event.GetCol();

    //     wxGridCellEditor* editor = GetCellEditor(row, col);
    //     if (dynamic_cast<wxGridCellBoolEditor*>(editor) != nullptr) {
    //         wxString val = GetCellValue(row, col);
    //         SetCellValue(row, col, val == "1" ? "0" : "1");
    //         event.Skip(false);  // skip prevents the default double-click behavior
    //     } else {
    //         event.Skip();  // let other cells behave normally
    //     }
    // });

    Bind(wxEVT_GRID_CELL_CHANGED, &SmartGrid::on_grid_cell_change, this);
    Bind(wxEVT_GRID_CELL_LEFT_CLICK, &SmartGrid::on_grid_cell_lclick, this);

// TODO
    // Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent& event) {
    //     if (event.GetKeyCode() == WXK_BACK) {
    //         if (IsCellEditControlEnabled()) event.Skip();
    //         int row = GetGridCursorRow();
    //         int col = GetGridCursorCol();
    //         if (row >= 0 && col >= 0) {
    //             SetCellValue(row, col, "");
    //         }
    //         return;
    //         // event.Skip();
    //     }
    //     event.Skip();
    // });
}

void SmartGrid::cal_row_label_width() {
    row_label_width_ = 0;
    wxClientDC dc(this);
    wxFont font = GetLabelFont();
    dc.SetFont(font);
    wxArrayString labels;
    for (size_t row = 0; row < GetNumberRows(); ++row) {
        auto row_value = GetRowLabelValue(row);
        labels.Add(row_value);
    }
    for (const auto& label : labels) {
        int w, h;
        dc.GetTextExtent(label, &w, &h);
        row_label_width_ = row_label_width_ > w ? row_label_width_ : w;
    }
    row_label_width_ += 20;
    // SetRowLabelSize(max_w);
}

void SmartGrid::cal_cell_width() {
    if (!row_label_width_) {cal_row_label_width();}
    int panel_width = GetClientSize().GetWidth();
    int max_col_width = (panel_width - row_label_width_) / 4;
    int avg_col_width = (panel_width - row_label_width_) / (GetNumberCols());
    cell_width_ = avg_col_width < max_col_width ? avg_col_width : max_col_width;
}

void SmartGrid::set_row_label(const std::vector<std::string>& col_names) {
    // Clear Grid Label
    ClearGrid();
    int cols = GetNumberCols();
    if (cols > 0) { DeleteCols(0, cols); }

    AppendCols(col_names.size());
    for (size_t col = 0; col < col_names.size(); ++col) {
        SetColLabelValue(col, col_names[col]);
    }
}

// Make sure set col label before this
void SmartGrid::auto_assign_row_editors(int row, const TableSchema& schema) {
    auto col_defs = schema.col_defs_;
    assert(col_defs.size() == GetNumberCols());
    for (size_t col = 0; col < GetNumberCols(); ++col) {
        ColumnType type = col_defs[col].type_;
        assign_type(row, col, type);
    }
}

void SmartGrid::auto_assign_row_editors(int row, ColumnType type) {
    for (size_t col = 0; col < GetNumberCols(); ++col) {
        assign_type(row, col, type);
    }
}

void SmartGrid::auto_assign_col_editors(int col, ColumnType type) {
    for (size_t row = 0; row < GetNumberRows(); ++row) {
        assign_type(row, col, type);
    }
}

void SmartGrid::assign_type(int row, int col, ColumnType type) {
    switch (type) {
        case ColumnType::TEXT: {
            SetCellEditor(row, col, new wxGridCellTextEditor());
            break;
        }
        case ColumnType::BOOLEAN: {
            SetCellEditor(row, col, new wxGridCellBoolEditor());
            SetCellRenderer(row, col, new wxGridCellBoolRenderer());
            SetCellValue(row, col, GetCellValue(row, col) == "1" ? "1" : "");
            break;
        }
        case ColumnType::INTEGER: {
            SetCellEditor(row, col, new wxGridCellNumberEditor());
            break;
        }
        case ColumnType::FLOAT: {
            SetCellEditor(row, col, new wxGridCellFloatEditor());
            break;
        }
        case ColumnType::DATE: {
            SetCellEditor(row, col, new wxGridCellDateEditor());
            break;
        }
        default: {
            SetCellEditor(row, col, new wxGridCellTextEditor());
            break;
        }
    }
}
void SmartGrid::format_grid() {
    if (!row_label_width_) { cal_row_label_width(); }
    if (!cell_width_) { cal_cell_width(); }

    // ROW LABEL
    SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
    SetRowLabelSize(row_label_width_);

    // COL && COL LABEL
    for (size_t col = 0; col < GetNumberCols(); ++col) {
        SetColSize(col, cell_width_);
    }
}

void SmartGrid::format() {
    cal_row_label_width();
    cal_cell_width();
    format_grid();
}

void SmartGrid::on_grid_cell_change(wxGridEvent& event) {
    std::cout << "smart_grid_cell_triggered\n";
    int row = event.GetRow();
    int col = event.GetCol();
    std::string value = GetCellValue(row, col).ToStdString();
    std::string old_value = event.GetString().ToStdString();
    wxPostEvent(GetParent(), EvtCellCommit(row, col, value, old_value));
}

void SmartGrid::on_grid_cell_lclick(wxGridEvent& event) {
    int row = event.GetRow();
    int col = event.GetCol();
    std::string value;

    auto cell_editor = dynamic_cast<wxGridCellBoolEditor*>(GetCellEditor(row, col));
    if (cell_editor) {
        std::cout << "smart_grid_lclick_triggered: ";
        std::cout << event.GetEventType() << "\n";
        std::string value = GetCellValue(row, col).ToStdString();
        value = (value == "1") ? "0" : "1";
        SetCellValue(row, col, value);
        EvtCellCommit* commit_evt = new EvtCellCommit(row, col, value);
        // wxPostEvent(GetParent(), commit_evt);
        wxQueueEvent(GetParent(), commit_evt);
    }
    event.Skip();
}