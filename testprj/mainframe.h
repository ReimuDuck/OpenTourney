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
	wxMenu* fileMenu;
	wxOverlay* overlay;
	wxFont font;
	wxCheckBox* checkScore;

	void removePlayer(wxCommandEvent& evt);
	std::string checkFileExists();
	void cvvCreate(wxCommandEvent& evt);
	void createControls();
	void onAddPlayer(wxCommandEvent& evt);
	void OnNextRound(wxCommandEvent& evt);
	void OnTextChanged(wxCommandEvent& evt);
	void showPlayers();
	void startAndNextRound(wxCommandEvent& evt);
	void OnAddScore(wxCommandEvent& evt, int p1, int p2);

	wxTextCtrl* roundNumber;
	wxTextCtrl* fName;
	wxTextCtrl* lName;
	wxTextCtrl* ID;

	void OnAddClicked(wxCommandEvent& evt);
};

