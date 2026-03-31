#pragma once
#include <wx/wx.h>
#include <vector>

class mainframe : public wxFrame
{
public:
	mainframe(const wxString& title);
private:
	wxPanel* panel;
	wxBoxSizer* sizer;

	void OnButtonClicked(wxCommandEvent& evt);
	void OnSliderChanged(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);
	std::vector<wxTextCtrl*> textBoxes;

	void OnAddClicked(wxCommandEvent& evt);
};

