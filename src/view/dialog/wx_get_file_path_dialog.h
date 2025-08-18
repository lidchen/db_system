#ifndef WX_GET_FILE_PATH_DIALOG
#define WX_GET_FILE_PATH_DIALOG

#include <functional>
#include "wx/wx.h"
#include "wx/dir.h"
#include "wx/filedlg.h"

class wxGetFilePathDialog : public wxFileDialog {
public:
    using Callback = std::function<void(const wxString&)>;
    wxGetFilePathDialog(wxWindow* parent, Callback cb);
    int ShowModal() override;
    wxString get_path() const;
private:
    Callback callback_;
};

#endif // WX_GET_FILE_PATH_DIALOG
