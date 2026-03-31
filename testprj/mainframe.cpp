#include "mainframe.h"
#include "Player.h"
#include "Game.h"
#include <wx/wx.h>


mainframe::mainframe(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title)
{
    panel = new wxPanel(this);

    // Main vertical sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Sizer for text boxes (grows)
    sizer = new wxBoxSizer(wxVERTICAL);

    // Sizer for buttons (fixed at bottom)
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxTextCtrl* txt = new wxTextCtrl(panel, wxID_ANY);
	wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Enter Competitors: ");
    wxButton* button = new wxButton(panel, wxID_ANY, "Submit");
    wxButton* addButton = new wxButton(panel, wxID_ANY, "+");

    addButton->Bind(wxEVT_BUTTON, &mainframe::OnAddClicked, this);
    button->Bind(wxEVT_BUTTON, &mainframe::OnButtonClicked, this);

    sizer->Add(label, 0, wxEXPAND | wxALL, 5);
    // Add first textbox
    sizer->Add(txt, 0, wxEXPAND | wxALL, 5);
    textBoxes.push_back(txt);

    // Add buttons to button sizer
    buttonSizer->Add(addButton, 0, wxALL, 5);
    buttonSizer->AddStretchSpacer(); // pushes Submit to the right
    buttonSizer->Add(button, 0, wxALL, 5);

    // Add both sections to main sizer
    mainSizer->Add(sizer, 1, wxEXPAND);  // <-- grows
    mainSizer->Add(buttonSizer, 0, wxEXPAND); // <-- stays at bottom

    panel->SetSizer(mainSizer);
}

void mainframe::OnButtonClicked(wxCommandEvent& evt) {
	wxLogStatus("Button Clicked");
}

void mainframe::OnTextChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Text: %s", evt.GetString());
	wxLogStatus(str);
}
void mainframe::OnAddClicked(wxCommandEvent& evt) {
    wxTextCtrl* txt = new wxTextCtrl(panel, wxID_ANY);

    sizer->Add(txt, 0, wxEXPAND | wxALL, 5);

    textBoxes.push_back(txt);

    panel->Layout();  // IMPORTANT: refresh layout

    wxLogStatus("Added new text box");
}