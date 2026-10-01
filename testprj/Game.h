#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"

class Game
{
public:
    Game() : rounds(0), roundNumber(0) {}
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    std::vector<std::string> GetPairing();
    std::string GetStandings();
    std::string CVVStandings();
    int getRoundNumber() const { return roundNumber; }
    int getRounds() const { return rounds; }
    bool getTopCut() const { return topCut; }
    void SortPlayers();
    void setPairingsTopCut();
    void SetPairings();
    void createPairings(); // this describes the logic inside set pairings

    const std::vector<std::pair<Player*, Player*>>& GetPairings() const { return pairings; }

    // Records the result for this pairing. Returns false and changes nothing if
    // the pairing has already been scored this round, or if it is a bye 
    bool setScore(Player* w, Player* l, char t);

    // Scoring state for the current round's pairings.
    bool IsPairingScored(std::size_t index) const;
    char GetPairingResult(std::size_t index) const; // 0 = unscored, 'W'/'L' from player 1's side, 'T', 'B' = bye
    bool AllPairingsScored() const;

    void SetRounds(int r);
    // flip flop switch
    void setPaired() { paired = !paired; }
    bool getPaired() const { return paired; }
    void PlayTopCut();
    void PlayRound();
    void AddPlayer(Player* p);
    void BeegQueue(Player* p, int purpose);
    bool isBeegPlayer(Player* p);
    void ResolveBeeg();
    Player* GetPlayer(int id) const;
    void FillListTest();

    void removeLatestPlayer();
    void removeFirstPlayer();

    int getPlayersSize() const { return static_cast<int>(players.size()); }

private:
    // Index of the pairing containing both players regardless of order, or -1.
    int FindPairingIndex(const Player* a, const Player* b) const;
    // Drops a player from the active field but keeps the object alive, because
    // surviving players still reference it in their opponent lists for OWR/OOWR.
    void RetirePlayer(Player* p);
    void ResetPairings();

    int rounds;
    int roundNumber;
    bool topCut = false;
    bool paired = false;

    std::vector<Player*> sortedPlayers;
    std::unordered_map<int, Player*> players;
    std::vector<std::pair<Player*, Player*>> pairings;
    std::vector<char> pairingResults;  // parallel to pairings
    std::vector<Player*> eliminated;   
    std::vector<std::pair<Player*, int>> queue;
};