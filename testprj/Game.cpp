#include "Game.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"
#include <algorithm>
#include <random>

void Game::SortPlayers() {
    if(players.empty()){
        return;
	}
    sortedPlayers.clear();

	// Copy players from the unordered_map to the vector
    for (const auto& pair : players) {
        sortedPlayers.push_back(pair.second);
    }
	// Sort the vector based on WR or OWR
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [this](Player* a, Player* b) {
        if (roundNumber == 1) {
            if (a->GetWR() != b->GetWR()) return a->GetWR() > b->GetWR();
            return a->GetID() < b->GetID(); // tie-breaker
        }
        else {
            if (a->GetOWR() != b->GetOWR()) return a->GetOWR() > b->GetOWR();
            return a->GetID() < b->GetID(); // tie-breaker
        }
    });
}
void Game::AddPlayer(Player* p){
	players[p->GetID()] = p;
}

Player* Game::GetPlayer(int id) const
{
    auto it = players.find(id);
    if (it != players.end()) {
        return it->second;
    }
    // Return nullptr if player with the given ID is not found
	return nullptr; 
}


void Game::SetRounds(int r) {
    rounds = r-1;
}

void Game::PlayRound() {
    SetPairings();
    GetPairing();
    roundNumber++;
}
void Game::SetPairings() {
    pairings.clear();
	SortPlayers();
    switch (roundNumber) {
        
    case 0:{
        std::random_device rd;
        std::mt19937 g(rd());
        // Shuffle the sortedPlayers vector to create random pairings for the first round
        std::shuffle(sortedPlayers.begin(), sortedPlayers.end(), g);
        for (int i = 0; i < sortedPlayers.size(); i += 2) {
            if (i + 1 < sortedPlayers.size()) {
                pairings.emplace_back(sortedPlayers[i], sortedPlayers[i + 1]);
            }
            else {
                pairings.emplace_back(sortedPlayers[i], nullptr);
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
            }
            else {
                pairings.emplace_back(sortedPlayers[i], nullptr);
            }
        }
        break;
    }
}
std::string Game::GetPairing(){
    std::string result;
	// Generate pairings based on the current round
    for (const auto& pair : pairings) {
        result += pair.first->GetName();
        result += " vs ";
        result += pair.second->GetName();
        result += "\n";
    }

    return result;
}
std::string Game::GetStandings() {
    SortPlayers();
    if (sortedPlayers.empty()) {
        return "No players to display";
    }
    
    std::string result;
    for (int i = 0; i < sortedPlayers.size(); i++) {
		result += "\n" + std::to_string(i + 1) + ". " + sortedPlayers[i]->GetName() + " - " + std::to_string(sortedPlayers[i]->GetID()) + " - " + std::to_string(sortedPlayers[i]->GetWR()) ;
    }
    return result;
}
