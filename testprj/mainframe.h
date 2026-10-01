#pragma once
#include <wx/wx.h>
#include <vector>
#include <wx/overlay.h>
#include "Player.h"
#include "Game.h"
#include <wx/simplebook.h>


class mainframe : public wxFrame
{
public:
	mainframe(const wxString& title);
private:
	Player* player;
	Game game;
	
	wxStatusBar* statusBar;

	wxSimplebook* book;
	wxScrolledWindow* playersPage;
	wxScrolledWindow* addPage;
	wxScrolledWindow* removePage;
	wxScrolledWindow* roundPage;
	wxScrolledWindow* cutPage;

	wxScrolledWindow* makePage();
	void showPage(wxScrolledWindow* page);

	wxBoxSizer* sizer;
	wxBoxSizer* PlayerListSizer;
	wxMenuBar* menuBar;
	wxMenu* tourneyMenu;
	wxMenu* fileMenu;
	wxFont font;
	wxCheckBox* checkScore;
	void topCut();
	void removePlayer(wxCommandEvent& evt);
	std::string checkFileExists();
	void cvvCreate(wxCommandEvent& evt);
	void createControls();
	void onAddPlayer(wxCommandEvent& evt);
	void onRemovePlayer(wxCommandEvent& evt);
	void initRounds();
	void OnNextRound(wxCommandEvent& evt);
	void OnTopCut(wxCommandEvent& evt);
	void showPlayers();
	void startAndNextRound(wxCommandEvent& evt);
	void OnAddScore(wxCommandEvent& evt, int p1, int p2);

	wxTextCtrl* roundNumber;
	wxTextCtrl* fName;
	wxTextCtrl* lName;
	wxTextCtrl* ID;

	void OnRemoveClicked(wxCommandEvent& evt);
	void OnAddClicked(wxCommandEvent& evt);
};

