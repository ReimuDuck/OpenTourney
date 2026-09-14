#include "mainframe.h"
#include "Player.h"
#include "Game.h"
#include <wx/wx.h>
#include <sstream>
#include <fstream> 
#include <string>
#include <vector>
//KNOWN BUGS/TODO:
// add load functionality
//
//


namespace {
	constexpr int ID_P1_WIN = 1001;
	constexpr int ID_TIE = 1002;
	constexpr int ID_P1_LOSS = 1003;

	wxString ResultLabel(char result) {
		switch (result) {
		case 'W': return "P1 WIN";
		case 'L': return "P1 LOSS";
		case 'T': return "TIE";
		case 'B': return "BYE";
		default:  return "";
		}
	}
}

mainframe::mainframe(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title), player(nullptr),
	game()
{
	createControls();
}


//-------------------------------------------------------------------------------------------------------------
void mainframe::createControls() {
	// initializes the panel and menu bar, and shows the players in the tournament
	wxFont Hfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	font = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	panel = new wxPanel(this, wxID_ANY);
	panel->SetFont(Hfont);

	sizer = nullptr;
	fName = nullptr;
	lName = nullptr;
	ID = nullptr;
	roundNumber = nullptr;

	menuBar = new wxMenuBar();
	tourneyMenu = new wxMenu();
	fileMenu = new wxMenu();

	//todo: add load functionality

	fileMenu->Append(wxID_SAVE, "Save", "Save to CSV");
	menuBar->Append(fileMenu, "File");
	fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &mainframe::cvvCreate, this, wxID_SAVE);

	// Commented out unsure if I want to keep it
	/*tourneyMenu->Append(wxID_ANY, "Add Player", "Add a new competitor");
	menuBar->Append(tourneyMenu, "Options");*/


	delete tourneyMenu;
	tourneyMenu = nullptr;

	statusBar = CreateStatusBar();
	// uncomment for testing purposes
	game.FillListTest();
	showPlayers();
	SetMenuBar(menuBar);
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::onAddPlayer(wxCommandEvent& evt)
{
	// clear the panel and create new controls for adding a player
	panel->DestroyChildren();
	fName = lName = ID = nullptr;
	roundNumber = nullptr;

	sizer = new wxBoxSizer(wxHORIZONTAL);
	panel->SetSizer(sizer);
	sizer->Add(new wxStaticText(panel, wxID_ANY, "First Name:"), 0, wxALL, 3);

	// text boxes for name, id, and a button to add the player to the tournament

	wxTextCtrl* txt = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, 30));
	txt->SetFont(font);
	sizer->Add(txt, 0, wxALL, 5);
	// just to show the text in the status bar when the user types something in the text box
	txt->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);
	fName = txt;

	sizer->Add(new wxStaticText(panel, wxID_ANY, "Last Name:"), 0, wxALL, 3);

	wxTextCtrl* txt2 = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, 30));
	txt2->SetFont(font);
	sizer->Add(txt2, 0, wxALL, 5);
	txt2->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);
	lName = txt2;

	sizer->Add(new wxStaticText(panel, wxID_ANY, "ID:"), 0, wxALL, 3);

	wxTextCtrl* txt3 = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 30));
	txt3->SetFont(font);
	sizer->Add(txt3, 4, wxALL, 5);
	txt3->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);
	ID = txt3;

	wxButton* addButton = new wxButton(panel, wxID_ANY, "Add Player");
	sizer->Add(addButton, 0, wxALL, 5);

	// adds to the tournament when the button is clicked, and shows the updated player list
	addButton->Bind(wxEVT_BUTTON, &mainframe::OnAddClicked, this);
	panel->Layout();
}
//-------------------------------------------------------------------------------------------------------------
// could be removed later
void mainframe::OnTextChanged(wxCommandEvent& evt) {
	wxString str = wxString::Format("Text: %s", evt.GetString());
	wxLogStatus(str);
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::removePlayer(wxCommandEvent& evt)
{
	game.removeLatestPlayer();

	CallAfter([this]() { showPlayers(); });
}
//-------------------------------------------------------------------------------------------------------------
std::string mainframe::checkFileExists() {
	// Every path now returns a value, and the search is bounded.
	for (int i = 0; i < 10000; ++i) {
		std::string line = "standings" + std::to_string(i) + ".csv";
		std::ifstream file(line);
		if (!file.good()) {
			return line;
		}
	}
	return "standings.csv";
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::cvvCreate(wxCommandEvent& evt)
{
	std::string standingFile = checkFileExists();
	std::ofstream standingsFile(standingFile);
	if (!standingsFile) {
		wxLogStatus("Could not open %s for writing.", standingFile.c_str());
		return;
	}

	std::string standings = game.CVVStandings();
	std::stringstream ss(standings);
	std::string line;
	// header has to match what CVVStandings actually emits
	standingsFile << "NAME, ID, WR OWR OOWR" << std::endl;
	while (std::getline(ss, line)) {
		standingsFile << line << std::endl;
	}
	standingsFile.close();
	wxLogStatus("Standings written to %s", standingFile.c_str());
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::topCut()
{
	// clear the panel and create new controls for showing the players in the tournament
	panel->DestroyChildren();
	fName = lName = ID = nullptr;
	roundNumber = nullptr;

	wxBoxSizer* Hsizer = new wxBoxSizer(wxHORIZONTAL);

	sizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(sizer);

	// headers for player list
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "Name:"), 0, wxALIGN_CENTER | wxALL, 3);
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "ID:"), 0, wxALIGN_CENTER | wxALL, 3);

	sizer->Add(Hsizer, 0, wxEXPAND);
	sizer->AddSpacer(10);
	// get the standings from the game and display them in the panel
	std::string standings = game.GetStandings();
	std::stringstream ss(standings);
	std::string line;
	// for each player in the standings, create a new static text control and add it to the panel
	while (std::getline(ss, line)) {
		wxBoxSizer* PlayerListSizer = new wxBoxSizer(wxHORIZONTAL);

		wxString wxLine = wxString::FromUTF8(line.c_str());
		wxStaticText* name = new wxStaticText(panel, wxID_ANY, wxLine);
		name->SetFont(font);
		PlayerListSizer->Add(name, 0, wxALIGN_CENTER_VERTICAL);
		sizer->Add(PlayerListSizer, 0, wxBOTTOM, 3);
	}
	// BUTTON TO START TOP CUT - 30x30 cannot fit the label, let it size itself
	wxButton* addButton = new wxButton(panel, wxID_ANY, "Top Cut");
	sizer->Add(addButton, 0, wxALL, 5);
	addButton->Bind(wxEVT_BUTTON, &mainframe::OnTopCut, this);


	panel->Layout();
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::showPlayers()
{
	// clear the panel and create new controls for showing the players in the tournament
	panel->DestroyChildren();
	fName = lName = ID = nullptr;
	roundNumber = nullptr;

	wxBoxSizer* Hsizer = new wxBoxSizer(wxHORIZONTAL);

	sizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(sizer);

	// headers for player list
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "Name:"), 0, wxALIGN_CENTER | wxALL, 3);
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "ID:"), 0, wxALIGN_CENTER | wxALL, 3);

	sizer->Add(Hsizer, 0, wxEXPAND);
	sizer->AddSpacer(10);
	// get the standings from the game and display them in the panel
	std::string standings = game.GetStandings();
	std::stringstream ss(standings);
	std::string line;
	// for each player in the standings, create a new static text control and add it to the panel
	while (std::getline(ss, line)) {
		wxBoxSizer* PlayerListSizer = new wxBoxSizer(wxHORIZONTAL);

		wxString wxLine = wxString::FromUTF8(line.c_str());
		wxStaticText* name = new wxStaticText(panel, wxID_ANY, wxLine);
		name->SetFont(font);
		PlayerListSizer->Add(name, 0, wxALIGN_CENTER_VERTICAL);
		sizer->Add(PlayerListSizer, 0, wxBOTTOM, 3);
	}


	const bool tourneyStarted = (game.getRoundNumber() != 0) || game.getTopCut();

	// button to add a new player
	if (!tourneyStarted) {
		wxButton* addButton = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(30, 30));
		sizer->Add(addButton, 0, wxALL);
		addButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
			CallAfter([this]() {
				wxCommandEvent e(wxEVT_BUTTON, wxID_ANY);
				onAddPlayer(e);
				});
			});
	}


	// sets button to remove the most recently added player, only shows if there is at least 2 players in the tournament or if the tournament has not started yet
	if (game.getPlayersSize() > 1 && !tourneyStarted) {
		wxBoxSizer* Fsizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* remButton = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(30, 30));
		sizer->Add(remButton, 0, wxALL);
		remButton->Bind(wxEVT_BUTTON, &mainframe::removePlayer, this);


		// text box to set the number of rounds in the tournament
		sizer->Add(new wxStaticText(panel, wxID_ANY, "Round Num"), 0, wxALL, 3);

		wxTextCtrl* rnum = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, 30));
		rnum->SetFont(font);
		sizer->Add(rnum, 0, wxALL, 5);
		rnum->Bind(wxEVT_TEXT, &mainframe::OnTextChanged, this);

		roundNumber = rnum;

		sizer->AddSpacer(20);
		// sets button to start the tournament
		wxButton* addTButton = new wxButton(panel, wxID_ANY, "Start Tourney");
		Fsizer->Add(addTButton, 1, wxALIGN_CENTER_VERTICAL, 4);
		addTButton->Bind(wxEVT_BUTTON, &mainframe::startAndNextRound, this);
		sizer->Add(Fsizer, 1, wxEXPAND, 0);
	}

	panel->Layout();
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::OnAddClicked(wxCommandEvent& evt) {
	// get the values from the text boxes and add a new player to the tournament
	if (!fName || !lName || !ID) {
		return;
	}
	if (fName->GetValue().IsEmpty() || lName->GetValue().IsEmpty() || ID->GetValue().IsEmpty()) {
		wxLogStatus("Please fill in all fields.");
		return;
	}
	wxString fnametemp = fName->GetValue();
	wxString lnametemp = lName->GetValue();
	wxString idtemp = ID->GetValue();

	long idLong = 0;
	if (!idtemp.ToLong(&idLong)) {
		wxLogStatus("ID must be a whole number.");
		return;
	}
	if (game.GetPlayer(static_cast<int>(idLong))) {
		wxLogStatus("A player with ID %ld already exists.", idLong);
		return;
	}

	Player* newPlayer = new Player(fnametemp.ToStdString(), lnametemp.ToStdString(), static_cast<int>(idLong));
	game.AddPlayer(newPlayer);

	Player* p = game.GetPlayer(static_cast<int>(idLong));
	if (!p) {
		// AddPlayer rejected it, so nothing took ownership
		delete newPlayer;
		wxLogStatus("Could not add player.");
		return;
	}

	wxLogStatus("Player Added: %s, ID: %d", wxString::FromUTF8(p->GetName().c_str()), p->GetID());
	CallAfter([this]() { showPlayers(); });
}
//-------------------------------------------------------------------------------------------------------------

void mainframe::startAndNextRound(wxCommandEvent& evt)
{
	// if the round number text box is still alive and the tournament has not
	// started yet, set the number of rounds from it
	if (roundNumber && game.getRoundNumber() == 0) {
		long roundINT = 0;
		if (!roundNumber->GetValue().ToLong(&roundINT) || roundINT < 1) {
			wxLogStatus("Enter a valid number of rounds.");
			return;
		}
		game.SetRounds((int)roundINT);
	}
	if (game.getRounds() < 1) {
		wxLogStatus("Set the number of rounds before starting.");
		return;
	}
	if (game.getPlayersSize() < 2) {
		wxLogStatus("Not enough players to start the tournament.");
		return;
	}
	if (game.getRoundNumber() == 0)
		game.PlayRound();

	const bool swissOver = !game.getTopCut() && game.getRoundNumber() > game.getRounds();
	const bool cutOver = game.getTopCut() && game.GetPairings().empty();

	if (swissOver || cutOver) { // 3 is min for player count and round count
		if (!game.getTopCut() && game.getRounds() >= 3
			&& game.getPlayersSize() >= 3) {
			wxLogStatus("Swiss rounds complete - top cut available");
			topCut();
		}
		else {
			wxLogStatus("Tournament concluded showing standings");
			showPlayers();
		}
		cvvCreate(evt);
		return;
	}

	// clear the panel and create new controls for showing the players in the tournament
	panel->DestroyChildren();
	fName = lName = ID = nullptr;
	roundNumber = nullptr;

	wxBoxSizer* Hsizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* Fsizer = new wxBoxSizer(wxHORIZONTAL);

	sizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(sizer);

	// to ensure that it is not auto-generated every time past the first 
	if (game.getRoundNumber() == 0)
		game.PlayRound();


	// headers for pairings and round number
	wxString roundText = wxString::Format("Round %d", game.getRoundNumber());

	Hsizer->Add(new wxStaticText(panel, wxID_ANY, roundText), 1, wxALIGN_CENTER | wxALL, 3);
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "Player 1, WR VS Player 2, WR"), 1, wxALIGN_CENTER | wxALL, 3);
	Hsizer->Add(new wxStaticText(panel, wxID_ANY, "Set score:"), 1, wxALIGN_CENTER | wxALL, 3);

	sizer->Add(Hsizer, 0, wxEXPAND);
	sizer->AddSpacer(10);
	// get the pairings from the game and display them in the panel, along with buttons to set the score for each pairing
	std::vector<std::string> pairings = game.GetPairing();
	const std::vector<std::pair<Player*, Player*>>& paired = game.GetPairings();

	for (std::size_t j = 0; j < paired.size() && j < pairings.size(); ++j) {
		if (!paired[j].first) {
			continue;
		}

		wxBoxSizer* PlayerListSizer = new wxBoxSizer(wxHORIZONTAL);

		wxString wxLine = wxString::FromUTF8(pairings[j].c_str());
		wxStaticText* name = new wxStaticText(panel, wxID_ANY, wxLine);

		name->SetFont(font);

		PlayerListSizer->Add(name, 0, wxALIGN_CENTER_VERTICAL);

		// get the player ids for the pairing, and bind the buttons to the OnAddScore function with the player ids as parameters
		const int i = paired[j].first->GetID();
		int k = -1;
		if (paired[j].second)
			k = paired[j].second->GetID();

		if (game.IsPairingScored(j)) {
			// already decided this round - show the result instead of live
			// buttons, so the same match cannot be scored twice
			wxStaticText* recorded = new wxStaticText(panel, wxID_ANY, ResultLabel(game.GetPairingResult(j)));
			recorded->SetFont(font);
			PlayerListSizer->Add(recorded, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
		}
		else if (paired[j].second) {
			wxButton* addButton = new wxButton(panel, ID_P1_WIN, "P1 WIN", wxDefaultPosition, wxSize(100, 30));
			PlayerListSizer->Add(addButton, 0, wxALL);
			addButton->Bind(wxEVT_BUTTON, [this, i, k](wxCommandEvent& event) {

				this->OnAddScore(event, i, k);
				});
			wxButton* add1Button = new wxButton(panel, ID_TIE, "TIE", wxDefaultPosition, wxSize(100, 30));
			PlayerListSizer->Add(add1Button, 0, wxALL);
			add1Button->Bind(wxEVT_BUTTON, [this, i, k](wxCommandEvent& event) {
				this->OnAddScore(event, i, k);
				});
			wxButton* add2Button = new wxButton(panel, ID_P1_LOSS, "P1 LOSS", wxDefaultPosition, wxSize(100, 30));
			PlayerListSizer->Add(add2Button, 0, wxALL);
			add2Button->Bind(wxEVT_BUTTON, [this, i, k](wxCommandEvent& event) {
				this->OnAddScore(event, i, k);
				});

		}
		sizer->AddSpacer(20);

		sizer->Add(PlayerListSizer, 0, wxBOTTOM, 3);
	}

	wxButton* addTButton = new wxButton(panel, wxID_ANY, "Next round");
	addTButton->Enable(game.AllPairingsScored());
	Fsizer->Add(addTButton, 1, wxALIGN_CENTER_VERTICAL, 4);
	addTButton->Bind(wxEVT_BUTTON, &mainframe::OnNextRound, this);
	sizer->Add(Fsizer, 1, wxEXPAND, 0);
	panel->Layout();
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::OnAddScore(wxCommandEvent& evt, int p1, int p2)
{
	Player* first = game.GetPlayer(p1);
	Player* second = (p2 != -1) ? game.GetPlayer(p2) : nullptr;

	if (!first || !second) {
		return;
	}

	bool recorded = false;
	if (evt.GetId() == ID_P1_WIN) {
		recorded = game.setScore(first, second, 'w');
	}
	else if (evt.GetId() == ID_P1_LOSS) {
		recorded = game.setScore(second, first, 'l');
	}
	else if (evt.GetId() == ID_TIE) {
		recorded = game.setScore(first, second, 't');
	}

	if (!recorded) {
		wxLogStatus("That pairing already has a result for this round.");
	}

	CallAfter([this]() {
		wxCommandEvent e(wxEVT_BUTTON, wxID_ANY);
		startAndNextRound(e);
		});
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::OnNextRound(wxCommandEvent& evt) {
	if (game.getTopCut()) {
		game.PlayTopCut();
	}
	else if (game.getRoundNumber() < game.getRounds()) {
		game.PlayRound();
	}
	else {
		wxLogStatus("Swiss rounds complete");
		CallAfter([this]() {
			wxCommandEvent e(wxEVT_BUTTON, wxID_ANY);
			if (game.getRounds() >= 3 && game.getPlayersSize() >= 3)
				topCut();
			else
				showPlayers();
			cvvCreate(e);
			});
		return;
	}

	CallAfter([this]() {
		wxCommandEvent e(wxEVT_BUTTON, wxID_ANY);
		startAndNextRound(e);
		});
}
//-------------------------------------------------------------------------------------------------------------
void mainframe::OnTopCut(wxCommandEvent& evt) {
	if (game.getRounds() < 3) {
		wxLogStatus("A top cut needs at least %d rounds of swiss.", 3);
		return;
	}
	if (game.getPlayersSize() < 3) {
		wxLogStatus("Not enough players left for a top cut.");
		return;
	}
	game.PlayTopCut();
	CallAfter([this]() {
		wxCommandEvent e(wxEVT_BUTTON, wxID_ANY);
		startAndNextRound(e);
		});
}
//-------------------------------------------------------------------------------------------------------------