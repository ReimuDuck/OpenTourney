#include "mainframe.h"
#include <wx/wx.h>


mainframe::mainframe(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	wxButton* button = new wxButton(panel, wxID_ANY, "Button", wxPoint(300, 275), wxSize(200, 50));
	wxSlider* slider = new wxSlider(panel, wxID_ANY,0,0,100, wxPoint(300, 200), wxSize(200, -1));
	wxTextCtrl* txt = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 375), wxSize(200, -1));

	button->Bind(wxEVT_BUTTON, &mainframe::OnButtonClicked, this);
	slider->Bind(wxEVT_SLIDER, &mainframe::OnSliderChanged, this);
	txt->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);

	

	CreateStatusBar();


}

void mainframe::OnButtonClicked(wxCommandEvent& evt) {
	wxLogStatus("Button Clicked");
}
void mainframe::OnSliderChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Slider Value %d", evt.GetInt());
	wxLogStatus(str);
}
void mainframe::OnTextChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Text: %s", evt.GetString());
	wxLogStatus(str);
}