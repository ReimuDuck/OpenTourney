#pragma once
#include <wx/wx.h>
#include <vector>
#include <wx/overlay.h>
#include "Player.h"
#include "Game.h"

class mainframe : public wxFrame
{
public:
	mainframe(const wxString& title);
private:
	Player* player;
	Game game;
	
	wxStatusBar* statusBar;
	wxPanel* panel;
	wxBoxSizer* sizer;
	wxMenuBar* menuBar;
	wxMenu* tourneyMenu;
	wxOverlay* overlay;
	wxFont font;
	wxCheckBox* checkScore;

	void removePlayer(wxCommandEvent& evt);
	void createControls();
	void onAddPlayer(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);
	void showPlayers();
	void startTourney(wxCommandEvent& evt);


	std::vector<wxTextCtrl*> textBoxes;
	wxTextCtrl* fName;
	wxTextCtrl* lName;
	wxTextCtrl* ID;

	void OnAddClicked(wxCommandEvent& evt);
};

