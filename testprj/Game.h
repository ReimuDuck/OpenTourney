#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"

class Game
{
public:
    Game() : rounds(0), roundNumber(0){}

    std::string GetPairing() ;
    std::string GetStandings();

	void SortPlayers();
    void SetPairings();
    void setScore(Player* w, Player* l);
    void SetRounds(int r);
    std::string PlayRound();
    void AddPlayer(Player* p);
	Player* GetPlayer(int id) const;

    void removeLatestPlayer();
	int getPlayersSize() { 
        SortPlayers();
        return sortedPlayers.size(); 
    }

private:
    int rounds;
	int roundNumber;

    std::vector<Player*> sortedPlayers;
    std::unordered_map<int, Player*> players;
    std::vector<std::pair<Player*, Player*>> pairings;
};