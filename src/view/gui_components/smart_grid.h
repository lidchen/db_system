#ifndef SMART_GRID_H_
#define SMART_GRID_H_
#include <vector>
#include <string>

#include "wx/wx.h"
#include "wx/grid.h"
#include "core/database/schema/table_schema.h"


class SmartGrid : public wxGrid {
public:
    SmartGrid(wxWindow* parent);
    void format_grid();
    void format();
    void auto_assign_row_editors(int row, const TableSchema& schema);
    void auto_assign_row_editors(int row, ColumnType type);
    void auto_assign_col_editors(int col, ColumnType type);
    void set_row_label(const std::vector<std::string> &col_names);

private:
    void assign_type(int row, int col, ColumnType type);
    void cal_row_label_width();
    void cal_cell_width();
    void on_grid_cell_change(wxGridEvent& event);
    void on_grid_cell_lclick(wxGridEvent& event);
    int cell_width_;
    int row_label_width_;
};

#endif // SMART_GRID_H_