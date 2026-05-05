#include "mainframe.h"
#include "Player.h"
#include "Game.h"
#include <wx/wx.h>
#include <sstream>
#include <string>
#include <vector>


mainframe::mainframe(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title), player(nullptr),
	game()
{
	createControls();
}



void mainframe::createControls() {
	// initializes the panel and menu bar, and shows the players in the tournament
	wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	panel = new wxPanel(this, wxID_ANY);
	panel->SetFont(font);

	menuBar = new wxMenuBar();
	tourneyMenu = new wxMenu();
	wxMenu* fileMenu = new wxMenu();

	//todo: add save and load functionality
	fileMenu->Append(wxID_EXIT, "Save", "Save to CSV");
	menuBar->Append(fileMenu, "File");

	tourneyMenu->Append(wxID_ANY, "Add Player", "Add a new competitor");
	menuBar->Append(tourneyMenu, "Options");

	tourneyMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &mainframe::onAddPlayer, this, wxID_ANY);

	statusBar = CreateStatusBar();

	showPlayers();
	SetMenuBar(menuBar);
}

void mainframe::onAddPlayer(wxCommandEvent& evt)
{	
	// clear the panel and create new controls for adding a player
	sizer->Clear(true);
	sizer = new wxBoxSizer(wxHORIZONTAL);
	panel->SetSizer(sizer);

	sizer->Add(new wxStaticText(panel, wxID_ANY, "First Name:"), 0, wxALL, 3);

	wxTextCtrl* txt = new wxTextCtrl(panel, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(150, 30));
	sizer->Add(txt, 0,wxALL, 5);
	// just to show the text in the status bar when the user types something in the text box, not necessary for functionality
	txt->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);
	fName = txt;

	sizer->Add(new wxStaticText(panel, wxID_ANY, "Last Name:"), 0, wxALL, 3);

	wxTextCtrl* txt2 = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, 30));
	sizer->Add(txt2, 0,wxALL, 5);
	txt2->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);
	lName = txt2;

	sizer->Add(new wxStaticText(panel, wxID_ANY, "ID:"), 0, wxALL, 3);

	wxTextCtrl* txt3 = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, 30));
	sizer->Add(txt3, 4, wxALL, 5);
	txt3->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);
	ID = txt3;

	wxButton* addButton = new wxButton(panel, wxID_ANY, "Add Player");
	sizer->Add(addButton, 0, wxALL, 5);

	addButton->Bind(wxEVT_BUTTON, &mainframe::OnAddClicked, this);
	panel->Layout();
}
// could be removed later
void mainframe::OnTextChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Text: %s", evt.GetString());
	wxLogStatus(str);
}

void mainframe::showPlayers()
{	
	// clear the panel and create new controls for showing the players in the tournament
	wxBoxSizer* Hsizer = new wxBoxSizer(wxHORIZONTAL);
	panel->DestroyChildren();

	sizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(sizer);

	// headers for player list
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "Name:"), 0, wxALL, 3);
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "ID:"), 0, wxALL, 3);

	sizer->Add(Hsizer, 0, wxEXPAND);
	
	// get the standings from the game and display them in the panel
	std::string standings = game.GetStandings();
	std::stringstream ss(standings);
	std::string line;
	while (std::getline(ss, line)) {
		wxBoxSizer* PlayerListSizer = new wxBoxSizer(wxHORIZONTAL);

		wxString wxLine = wxString::FromUTF8(line.c_str());
		PlayerListSizer->Add(new wxStaticText(panel, wxID_ANY, wxLine), 0, wxALIGN_CENTER_VERTICAL);
		sizer->Add(PlayerListSizer, 0, wxBOTTOM, 3);
	}
	if (game.getPlayersSize() != 0) {
		wxButton* addButton = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(30, 30));
		sizer->Add(addButton, 0, wxALL);
		addButton->Bind(wxEVT_BUTTON, &mainframe::removePlayer, this);
	}

	
	panel->Layout();
}
void mainframe::OnAddClicked(wxCommandEvent& evt) {
	// get the values from the text boxes and add a new player to the tournament
	long idLong;
	if(fName->GetValue().IsEmpty() || lName->GetValue().IsEmpty() || ID->GetValue().IsEmpty()) {
		wxLogStatus("Please fill in all fields.");
		return;
	}
	wxString fname = fName->GetValue();
	wxString lname = lName->GetValue();
	wxString idStr = ID->GetValue();
	idStr.ToLong(&idLong);
	sizer->Clear(true);

	player = new Player(fname.ToStdString(), lname.ToStdString(), idLong);
	game.AddPlayer(player);
	Player* p = game.GetPlayer(idLong);

	wxLogStatus("Player Added: %s, ID: %d", p->GetName(), p->GetID());
	showPlayers();
}
void mainframe::removePlayer(wxCommandEvent& evt)
{
	game.removeLatestPlayer();
	showPlayers();
}