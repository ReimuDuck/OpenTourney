#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"

class Game
{
public:
    Game() : rounds(0){}

    std::string GetPairing() const;
    std::string GetStandings() const;

    void SetRounds(int r);
    void AddPlayer(Player* p);
    Player* GetPlayer(int id) const;

private:
    int rounds;

    std::vector<Player*> sortedPlayers;
    std::unordered_map<int, Player*> players;
    std::vector<std::pair<Player*, Player*>> pairings;
};