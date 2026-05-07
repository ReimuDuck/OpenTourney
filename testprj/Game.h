#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"

class Game
{
public:
    Game() : rounds(0), roundNumber(0){}

    vector<string> GetPairing() ;
    std::string GetStandings();

	void SortPlayers();
    void SetPairings();
	std::vector<std::pair<Player*, Player*>> GetPairings() const { return pairings; }
    void setScore(Player* w, Player* l, char t);
    void SetRounds(int r);
    void PlayRound();
    void AddPlayer(Player* p);
	Player* GetPlayer(int id) const;
    void FillListTest();

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