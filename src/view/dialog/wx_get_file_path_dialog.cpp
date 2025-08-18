#include "wx_get_file_path_dialog.h"
wxGetFilePathDialog::wxGetFilePathDialog(wxWindow* parent, Callback cb)
    : wxFileDialog(parent, "Select File", "", "", "*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST), callback_(cb)
{
    // ShowModal();
}

int wxGetFilePathDialog::ShowModal() {
    int result = wxFileDialog::ShowModal();
    if (result == wxID_OK && callback_) {
        callback_(GetPath());
    }
    return result;
}

wxString wxGetFilePathDialog::get_path() const {
    return GetPath();
}

// std::string wxGetFilePathDialog::on_set(wxCommandEvent& event) {
//     if (this->ShowModal() == wxID_OK) {
//         wxString path = this->GetPath();
//         return path.ToStdString();
//     }
//     return "";
// }