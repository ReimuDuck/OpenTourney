#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"

class Game
{
public:
    Game() : rounds(0){}

    std::string GetPairing() ;
    std::string GetStandings();

	void SortPlayers();
    void SetPairings();
    void ComparePlayers();
    void SetRounds(int r);
	void PlayRound();
    void AddPlayer(Player* p);

private:
    int rounds;
	int roundNumber;

    std::vector<Player*> sortedPlayers;
    std::unordered_map<int, Player*> players;
    std::vector<std::pair<Player*, Player*>> pairings;
};