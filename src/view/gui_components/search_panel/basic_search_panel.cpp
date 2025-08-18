#include "basic_search_panel.h"
#include "gui/gui_components/horizontal_panel.h"
#include "core/database/database_manager.h"

BasicSearchPanel::BasicSearchPanel(wxWindow* parent)
    : BaseSearchPanel(parent)
{
    wxBoxSizer* basic_sizer = new wxBoxSizer(wxVERTICAL);
    HorizontalPanel* search_bar = new HorizontalPanel(this);
    searched_value_tc_ = new wxTextCtrl(search_bar, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    search_btn_ = new wxButton(search_bar, wxID_ANY, "Search");
    search_bar->add_children(searched_value_tc_, search_btn_);

    wxPanel* toggle_panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* toggle_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* col_select_btn = new wxButton(toggle_panel, wxID_ANY, "Select Column");

    col_select_popup_ = new CbSelectPopup(wxGetTopLevelParent(toggle_panel));

    cb_fuzzy_ = new wxCheckBox
        (toggle_panel, wxID_ANY, "Fuzzy Search", wxDefaultPosition, wxDefaultSize);
    cb_case_insensitive_ = new wxCheckBox
        (toggle_panel, wxID_ANY, "Case Insensitive", wxDefaultPosition, wxDefaultSize);

    toggle_panel_sizer->Add(cb_fuzzy_);
    toggle_panel_sizer->Add(cb_case_insensitive_);
    toggle_panel_sizer->Add(col_select_btn);
    toggle_panel->SetSizer(toggle_panel_sizer);
    
    basic_sizer->Add(search_bar, 1, wxALL, 10);
    basic_sizer->Add(toggle_panel);
    this->SetSizer(basic_sizer);

    // COL SELECTION POPUP FOR BASIC SEARCH
    col_select_btn->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent&) {
        auto col_names = tb_manager_->get_current_table_schema().get_col_names();
        // CbSelectPopup* col_select_popup = new CbSelectPopup(wxGetTopLevelParent(toggle_panel), col_names);

        // Get the button's position and size
        wxPoint btn_pos = col_select_btn->GetPosition();
        wxSize btn_size = col_select_btn->GetSize();
        // The point just below the button in its parent's coordinates
        wxPoint below_btn(btn_pos.x, btn_pos.y + btn_size.GetHeight());
        // Convert that point to screen coordinates
        wxPoint screen_pos = col_select_btn->GetParent()->ClientToScreen(below_btn);
        // Position the popup there
        col_select_popup_->Position(screen_pos, wxSize(0, 0));
        col_select_popup_->Popup();
    });

    // BIND
    // cb_fuzzy_->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &wxSearchPanel::on_toggle, this);
}

void BasicSearchPanel::update() {
    auto col_names = tb_manager_->get_current_table_schema().get_col_names();

    // BASIC SEARCH
    col_select_popup_->update_checkboxes(col_names);
    // DEFAULT COL SELECTION: SELECT ALL
    col_select_popup_->select_all();

    cb_fuzzy_->SetValue(true);
    cb_case_insensitive_->SetValue(true);
}

std::string BasicSearchPanel::build_search_sql() {
    auto selection_info = col_select_popup_->get_selection();
    std::string search_value = searched_value_tc_->GetValue().ToStdString();
    bool fuzzy_on = cb_fuzzy_->GetValue();
    bool case_insensitive = cb_case_insensitive_->GetValue();

    DatabaseStmtBuilder builder;
    auto where_builder = builder.select("*")
        .from(tb_manager_->get_current_table_name())
        .where();

    auto col_names = tb_manager_->get_current_table_schema().get_col_names();
    for (size_t i = 0; i < col_names.size(); ++i) {
        if (selection_info[i]) {
            where_builder.column(col_names[i]);
            // FUZZY SEARCH
            if (fuzzy_on) {
                where_builder.like(search_value);
            } else {
                where_builder.equals(search_value);
            }
            // CASE SENSITIVE
            if (!case_insensitive) {
                where_builder.case_sensitive();
            } 
        }
    }
    auto sql = where_builder.build_with_or();
    std::cout << sql << "\n";
    return sql;
}
