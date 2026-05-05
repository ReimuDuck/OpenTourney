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


	void createControls();
	void onAddPlayer(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);
	void showPlayers();


	std::vector<wxTextCtrl*> textBoxes;
	wxTextCtrl* fName;
	wxTextCtrl* lName;
	wxTextCtrl* ID;

	void OnAddClicked(wxCommandEvent& evt);
};

