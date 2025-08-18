#include "advance_search_panel.h"
#include "wx/grid.h"

AdvanceSearchPanel::AdvanceSearchPanel(wxWindow* parent) 
    : BaseSearchPanel(parent)
{
    on_off_choices.Add("ON");
    on_off_choices.Add("OFF");

    config_grid_ = new SmartGrid(this);
    search_btn_ = new wxButton(this, wxID_ANY, "Search");
    wxBoxSizer* advance_sizer = new wxBoxSizer(wxVERTICAL);
    advance_sizer->Add(config_grid_, 1, wxEXPAND, 0);
    advance_sizer->Add(search_btn_, 0, wxALIGN_RIGHT | wxALL, 5);
    SetSizer(advance_sizer);

    auto col_names = tb_manager_->get_current_table_schema().get_col_names();
    config_grid_->CreateGrid(3, col_names.size());
    config_grid_->SetRowLabelValue(0, "Value");
    config_grid_->SetRowLabelValue(1, "Fuzzy Match");
    config_grid_->SetRowLabelValue(2, "Case Insensitive");
    config_grid_->EnableDragRowSize(false);

    config_grid_->format();
}

wxButton* AdvanceSearchPanel::get_search_btn() const {
    return search_btn_;
}
void AdvanceSearchPanel::update() {
    // Col Label
    auto col_names = tb_manager_->get_current_table_schema().get_col_names();

    config_grid_->set_row_label(col_names);
    config_grid_->auto_assign_row_editors(0, tb_manager_->get_current_table_schema());
    config_grid_->auto_assign_row_editors(1, ColumnType::BOOLEAN);
    config_grid_->auto_assign_row_editors(2, ColumnType::BOOLEAN);
    config_grid_->format();
}

std::string AdvanceSearchPanel::build_search_sql() {
    DatabaseStmtBuilder builder;
    auto where_builder = builder.select("*").from(tb_manager_->get_current_table_name()).where();
    for (size_t col = 0; col < config_grid_->GetNumberCols(); ++col) {
        std::string value = config_grid_->GetCellValue(0, col).ToStdString();
        bool fuzzy_match = config_grid_->GetCellValue(1, col) == "1";
        bool case_insensitive = config_grid_->GetCellValue(2, col) == "1";
        
        if (!value.empty()) {
            where_builder.column(config_grid_->GetColLabelValue(col).ToStdString());
            // FUZZY SEARCH
            if (fuzzy_match) {
                where_builder.like(value);
            } else {
                where_builder.equals(value);
            }
            // CASE SENSITIVE
            if (!case_insensitive) {
                where_builder.case_sensitive();
            } 
        }
    }
    auto sql = where_builder.build_with_and();
    std::cout << sql << "\n";
    return sql;
}
