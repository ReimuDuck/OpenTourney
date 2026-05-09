#include "Game.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"
#include <algorithm>
#include <random>
#include <iomanip>
//-------------------------------------------------------------------------------------------------------------
void Game::SortPlayers() {
    if(players.empty()){
        return;
	}
    sortedPlayers.clear();

	// Copy players from the unordered_map to the vector
    for (const auto& pair : players) {
        sortedPlayers.push_back(pair.second);
    }
	// Sort the vector based on WR OWR then OOWR
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [this](Player* a, Player* b) {
        if (roundNumber == 2) {
            if (a->GetWR() != b->GetWR()) return a->GetWR() > b->GetWR();
            return a->GetID() < b->GetID(); // tie-breaker
        }
        else if(roundNumber==3) {
            if (a->GetWR() != b->GetWR()) return a->GetWR() > b->GetWR();
            if (a->GetOWR() != b->GetOWR()) return a->GetOWR() > b->GetOWR();
            return a->GetID() < b->GetID(); 
        }
        else {
            if (a->GetWR() != b->GetWR()) return a->GetWR() > b->GetWR();
            if (a->GetOWR() != b->GetOWR()) return a->GetOWR() > b->GetOWR();
            if (a->GetOOWR() != b->GetOOWR()) return a->GetOOWR() > b->GetOOWR();
            return a->GetID() < b->GetID();
        }
    });
}
//-------------------------------------------------------------------------------------------------------------
void Game::AddPlayer(Player* p){
	players[p->GetID()] = p;
}
//-------------------------------------------------------------------------------------------------------------
// to be removed
void Game::FillListTest() {
	Player* defaultPlayer = new Player("Default", "Player", 0);
    for (int i = 1; i <= 10; i++) {
        Player* p = new Player("Player" + std::to_string(i), "Test", i);
        AddPlayer(p);
	}
}
//-------------------------------------------------------------------------------------------------------------
Player* Game::GetPlayer(int id) const
{
    auto it = players.find(id);
    if (it != players.end()) {
        return it->second;
    }
    // Return nullptr if player with the given ID is not found
	return nullptr; 
}
//-------------------------------------------------------------------------------------------------------------

void Game::removeLatestPlayer() {
    if (sortedPlayers.empty()) {
        return;
    }

    Player* last = sortedPlayers.back();
    if (!last) {
        return;
    }

    auto it = players.find(last->GetID());
    // If not found, avoid dereferencing the end iterator
    if (it == players.end()) {
        sortedPlayers.erase(std::remove(sortedPlayers.begin(), sortedPlayers.end(), last), sortedPlayers.end());
        return;
    }

    // Remove mapping then remove any stale copies from sortedPlayers, then delete
    Player* p = it->second;
    players.erase(it);

    sortedPlayers.erase(std::remove(sortedPlayers.begin(), sortedPlayers.end(), p), sortedPlayers.end());

    delete p;
}
//------------------------------------------------------------------------------------------------------------- TO REMOVE
void Game::SetRounds(int r) {
    rounds = r;
}
//-------------------------------------------------------------------------------------------------------------
void Game::PlayRound() {
	roundNumber++;
    SetPairings();
    GetPairing();
    return;
}
//-------------------------------------------------------------------------------------------------------------
void Game::SetPairings() {
    if (players.empty()) {
        return;
    }
    pairings.clear();
	SortPlayers();
    switch (roundNumber) {
        
    case 1:{
        std::random_device rd;
        std::mt19937 g(rd());
        // Shuffle the sortedPlayers vector to create random pairings for the first round
        std::shuffle(sortedPlayers.begin(), sortedPlayers.end(), g);
        for (int i = 0; i < sortedPlayers.size(); i += 2) {
            if (i + 1 < sortedPlayers.size()) {
                pairings.emplace_back(sortedPlayers[i], sortedPlayers[i + 1]);
				sortedPlayers[i]->AddOpponent(sortedPlayers[i + 1]);
				sortedPlayers[i + 1]->AddOpponent(sortedPlayers[i]);
            }
            else {
                pairings.emplace_back(sortedPlayers[i], nullptr);
                sortedPlayers[i]->SetScore('T'); // bye counts as a tie
            }
        }
        break;

     }
    default:
        SortPlayers();
		// Create pairings based on wr for subsequent rounds
        for (int i = 0; i < sortedPlayers.size(); i += 2) {
            if (i + 1 < sortedPlayers.size()) {
                pairings.emplace_back(sortedPlayers[i], sortedPlayers[i + 1]);
                sortedPlayers[i]->AddOpponent(sortedPlayers[i + 1]);
                sortedPlayers[i + 1]->AddOpponent(sortedPlayers[i]);
            }
            else {
                pairings.emplace_back(sortedPlayers[i], nullptr);
				sortedPlayers[i]->SetScore('T');
            }
        }
        break;
    }
}
//-------------------------------------------------------------------------------------------------------------
void Game::setScore(Player* w, Player* l,char t) {
    // if tie
    if(t == 't'){
        w->SetScore('T');
        l->SetScore('T');
        return;
	}
	// if bye set tie for player with bye
    if ( !l) {
        w->SetScore('T');
        return;
	}

	w->SetScore('W');
	l->SetScore('L');
}
//------------------------------------------------------------------------------------------------------------- Rename later
vector<string> Game::GetPairing(){
    vector<string> results;
    std::string result1;
    std::string result2;
    if(pairings.empty()){
        results.push_back("No pairings to display");
        return results;
	}
	// Generate pairings based on the current round
    for (const auto& pair : pairings) {
        result1 += pair.first->GetName();
		result1 += " ------- " + std::to_string(pair.first->GetWins()) +"W/"
            + std::to_string(pair.first->GetLosses()) + "L/" 
            + std::to_string(pair.first->GetTies()) + "T";
		result1 += " - " + std::to_string(pair.first->GetWR()) + "%";
        result1 += " vs ";
        if (pair.second) {
            result2 += pair.second->GetName();
            result2 += " ------- " + std::to_string(pair.second->GetWins()) + "W/"
                + std::to_string(pair.second->GetLosses()) + "L/"
                + std::to_string(pair.second->GetTies()) + "T";
            result2 += " - " + std::to_string(pair.second->GetWR()) + "%\n";
        }
        else {
			result2 += "      BYE\n";
        }
        results.push_back(result1 + result2);
		result1.clear();
		result2.clear();
    }

    return results;
}
//-------------------------------------------------------------------------------------------------------------
std::string Game::GetStandings() {
    SortPlayers();
    if (sortedPlayers.empty()) {
        return "No players to display";
    }
    
    std::string result;
    for (int i = 0; i < sortedPlayers.size(); i++) {
		result +=  std::to_string(i + 1) + ". " + sortedPlayers[i]->GetName() + " - " + std::to_string(sortedPlayers[i]->GetID()) + " - " + std::to_string(sortedPlayers[i]->GetWR())+ " " + std::to_string(sortedPlayers[i]->GetOWR()) + " " + std::to_string(sortedPlayers[i]->GetOOWR()) + "%\n";
    }
    return result;
}
//-------------------------------------------------------------------------------------------------------------