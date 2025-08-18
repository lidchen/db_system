#include "wx_search_panel.h"
#include "gui/gui_components/search_panel/basic_search_panel.h"
#include "gui/gui_components/search_panel/advance_search_panel.h"

wxSearchPanel::wxSearchPanel(wxPanel* parent) 
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
    nb_ = new wxNotebook(this, wxID_ANY, wxPoint(-1, -1), wxDefaultSize, wxNB_TOP);

    basic_search_ = new BasicSearchPanel(nb_);
    advance_search_ = new AdvanceSearchPanel(nb_);
    nb_->AddPage(basic_search_, wxT("basic_search"));
    nb_->AddPage(advance_search_, wxT("advance_search"));

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(nb_, 1, wxEXPAND);
    SetSizer(main_sizer);

    bind_search_event();
    refresh();

    // Size it to fit
    auto* page = nb_->GetCurrentPage();
    if (page) {
        // Get the best size of the new page
        wxSize best = page->GetBestSize();

        // Add space for tab bar
        int tab_height = nb_->GetSize().y - nb_->GetClientSize().y;

        // Set the nb_ size
        nb_->SetSizeHints(best.x, best.y + tab_height);
        nb_->SetMinSize(wxSize(best.x, best.y + tab_height));
        nb_->GetParent()->Layout();
    }

    nb_->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, [=, this](wxBookCtrlEvent& event) {
        auto* page = nb_->GetCurrentPage();
        if (page) {
            // Get the best size of the new page
            wxSize best = page->GetBestSize();

            // Add space for tab bar
            int tab_height = nb_->GetSize().y - nb_->GetClientSize().y;

            // Set the nb_ size
            nb_->SetSizeHints(best.x, best.y + tab_height);
            nb_->SetMinSize(wxSize(best.x, best.y + tab_height));
            nb_->GetParent()->Layout();
            nb_->GetParent()->GetParent()->Layout();
        }
        event.Skip();
    });
}

void wxSearchPanel::on_toggle(wxCommandEvent& event) {
    wxCheckBox* cb = dynamic_cast<wxCheckBox*>(event.GetEventObject());
    if (cb) {
    }
}

std::string wxSearchPanel::build_search_sql() {
    if (nb_->GetCurrentPage() == basic_search_) {
        return basic_search_->build_search_sql();
    }
    if (nb_->GetCurrentPage() == advance_search_) {
        return advance_search_->build_search_sql();
    }
    return "";
}

void wxSearchPanel::refresh() {
    basic_search_->update();
    advance_search_->update();
}

void wxSearchPanel::bind_search_event() {
    auto* basic_search = dynamic_cast<BasicSearchPanel*>(basic_search_);
    if (basic_search) {
        basic_search->get_search_btn()->Bind(wxEVT_BUTTON, &wxSearchPanel::on_search, this);
        basic_search->get_searched_value_tc()->Bind(wxEVT_TEXT_ENTER, &wxSearchPanel::on_search, this);
    }

    auto* advance_search_panel = dynamic_cast<AdvanceSearchPanel*>(advance_search_);
    if (advance_search_panel) {
        advance_search_panel->get_search_btn()->Bind(wxEVT_BUTTON, &wxSearchPanel::on_search, this);
    }
}

void wxSearchPanel::on_search(wxCommandEvent& event) {
    std::cout << "CHILD\n";
    wxCommandEvent search_evt(EVT_SEARCH_QUERY);
    search_evt.SetEventObject(this);
    wxPostEvent(GetParent(), search_evt);
}
