#include <wx/wx.h>
#include "welcome_panel.h"
#include "src/view/dialog/wx_database_manager_dialog.h"
#include "src/view/main_frame.h"

WelcomePanel::WelcomePanel(wxWindow* parent, std::shared_ptr<DbConnection> db_connection) 
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
    , db_connection_(db_connection)
{
    // Show PNG image in the main page
    auto* content_panel = new wxPanel(this);
    auto* welcome_panel = new wxPanel(this);
    auto* v_sizer = new wxBoxSizer(wxVERTICAL);
    auto* h_sizer = new wxBoxSizer(wxHORIZONTAL);

    // Left: Button panel (50%)
    auto* btn_panel = new wxPanel(content_panel);
    auto* btn_sizer = new wxBoxSizer(wxVERTICAL);

    auto* open_database_btn = new wxButton(btn_panel, wxID_ANY, wxT("Open Database"), wxDefaultPosition, wxSize(120, 30));
    auto* btn_2 = new wxButton(btn_panel, wxID_ANY, wxT("just nothing"), wxDefaultPosition, wxSize(120, 30));
    auto* btn_3 = new wxButton(btn_panel, wxID_ANY, wxT("just nothing p2"), wxDefaultPosition, wxSize(120, 30));
    btn_sizer->AddStretchSpacer(1);
    btn_sizer->Add(open_database_btn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
    btn_sizer->Add(btn_2, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
    btn_sizer->Add(btn_3, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
    btn_sizer->AddStretchSpacer(1);
    btn_sizer->AddStretchSpacer(1);
    btn_panel->SetSizer(btn_sizer);

    // Right: Logo panel (50%)
    auto* logo_panel = new wxPanel(content_panel);
    auto* logo_sizer = new wxBoxSizer(wxVERTICAL);

    wxImage::AddHandler(new wxPNGHandler());
    wxString img_path = wxGetCwd() + wxFILE_SEP_PATH + "resources" + wxFILE_SEP_PATH + "img" + wxFILE_SEP_PATH;
    wxString logo_path = img_path + "logo.png";
    wxString welcome_path = img_path + "welcome.png";
    wxImage welcome(welcome_path, wxBITMAP_TYPE_PNG);
    wxImage logo(logo_path, wxBITMAP_TYPE_PNG);
    if (welcome.IsOk()) {
        // Scale the welcome image to fit the panel size dynamically
        welcome_panel->Bind(wxEVT_SIZE, [welcome, welcome_panel](wxSizeEvent& event) mutable {
            wxSize panel_size = welcome_panel->GetClientSize();
            if (panel_size.GetWidth() > 0 && panel_size.GetHeight() > 0) {
                wxImage scaled = welcome.Scale(panel_size.GetWidth(), panel_size.GetHeight(), wxIMAGE_QUALITY_HIGH);
                wxBitmap bitmap(scaled);
                // Remove any existing children
                for (auto child : welcome_panel->GetChildren()) {
                    child->Destroy();
                }
                new wxStaticBitmap(welcome_panel, wxID_ANY, bitmap, wxPoint(0, 0), panel_size);
                welcome_panel->Layout();
            }
            event.Skip();
        });
        welcome_panel->SetMinSize(wxSize(-1, 150));
    }
    if (logo.IsOk()) {
        wxBitmap bitmap(logo);
        wxStaticBitmap* static_bitmap = new wxStaticBitmap(logo_panel, wxID_ANY, bitmap);
        logo_sizer->AddStretchSpacer(1);
        logo_sizer->Add(static_bitmap, 0, wxALIGN_CENTER | wxTOP, 20);
        logo_sizer->AddStretchSpacer(1);
    }
    logo_panel->SetSizer(logo_sizer);

    h_sizer->Add(btn_panel, 1, wxEXPAND | wxALL, 20);
    h_sizer->Add(logo_panel, 1, wxEXPAND | wxALL, 20);

    v_sizer->Add(welcome_panel, 1, wxEXPAND);
    v_sizer->Add(content_panel, 1, wxEXPAND);

    SetSizer(v_sizer);
    content_panel->SetSizer(h_sizer);

    open_database_btn->Bind(wxEVT_BUTTON, &WelcomePanel::on_db_manager, this);
}

void WelcomePanel::on_db_manager(wxCommandEvent& event) {
    wxDatabaseManagerDialog* db_manager_dialog = new wxDatabaseManagerDialog(this);
    if (db_manager_dialog->ShowModal() == wxID_OK) {
        MainFrame* main_frame = wxDynamicCast(GetParent(), MainFrame);
        // main_frame->show_db_editor_panel();

        // This line cause segmentation fault
        // db_manager_dialog->Destroy();
    } else {
    // if cancel, do nothing, stay at welcome
    }
}