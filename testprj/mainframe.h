#pragma once
#include <wx/wx.h>

class mainframe : public wxFrame
{
public:
	mainframe(const wxString& title);
private:
	void OnButtonClicked(wxCommandEvent& evt);
	void OnSliderChanged(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);

};

