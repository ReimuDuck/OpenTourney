#include "Game.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"
#include <algorithm>
#include <random>

void Game::SortPlayers() {
    sortedPlayers.clear();

	// Copy players from the unordered_map to the vector
    for (const auto& pair : players) {
        sortedPlayers.push_back(pair.second);
    }
	// Sort the vector based on WR or OWR
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [this](Player* a, Player* b) {
        if (roundNumber == 1) {
            return a->GetWR() >= b->GetWR();
        }
        else {
            return a->GetOWR() >= b->GetOWR();
        }
        });
}
void Game::AddPlayer(Player* p){
	players[p->GetID()] = p;
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
    std::string result;
    for (int i = 0; i < sortedPlayers.size(); i++) {
		result += i + ". " + sortedPlayers[i]->GetName() + " - " + std::to_string(sortedPlayers[i]->GetWR()) + "\n";
    }
    return result;
}
