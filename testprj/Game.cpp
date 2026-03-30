#include "Game.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"


void Game::AddPlayer(Player* p){
	players[p->GetID()] = p;
}


void Game::SetRounds(int r) {
    rounds = r;
}

std::string Game::GetPairing() const {
    pairings.clear();
    std::string result;

    for (const auto& pair : pairings) {
        result += pair.first->GetName();
        result += " vs ";
        result += pair.second->GetName();
        result += "\n";
    }

    return result;
}
std::string Game::GetStandings() const {

}

Player* Game::GetPlayer(int id) const {

}


//class Game
//{
//public:
//    void SetPairing();
//
//    std::string GetPairing() const;
//    std::string GetStandings() const;
//
//    void AddPlayer(Player* p);
//    Player* GetPlayer(int id) const;
//
//private:
//    int rounds;
//
//    std::unordered_map<int, Player*> players;
//    std::vector<std::pair<Player*, Player*>> pairings;
//};
