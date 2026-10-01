#include "Game.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Player.h"
#include <algorithm>
#include <random>
#include <iomanip>
//-------------------------------------------------------------------------------------------------------------
Game::~Game() {
    for (auto& pair : players) {
        delete pair.second;
    }
    for (Player* p : eliminated) {
        delete p;
    }
    players.clear();
    eliminated.clear();
    sortedPlayers.clear();
    pairings.clear();
    pairingResults.clear();
}
//-------------------------------------------------------------------------------------------------------------
void Game::SortPlayers() {
    sortedPlayers.clear();

    if (players.empty()) {
        return;
    }

    // Copy players from the unordered_map to the vector
    for (const auto& pair : players) {
        if (pair.second) {
            sortedPlayers.push_back(pair.second);
        }
    }
    // Sort the vector based on WR OWR then OOWR
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [this](Player* a, Player* b) {
        if (roundNumber == 2) {
            if (a->GetWR() != b->GetWR()) return a->GetWR() > b->GetWR();
            return a->GetID() < b->GetID(); // tie-breaker
        }
        else if (roundNumber == 3) {
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
void Game::AddPlayer(Player* p) {
    if (!p) {
        return;
    }

    auto it = players.find(p->GetID());
    if (it != players.end()) {
        return; // duplicate ID: reject
    }
    if (roundNumber != 0) {
        for (int i = 0; i < roundNumber; i++) {
            p->SetScore('L');
        }
    }
    players[p->GetID()] = p;

    sortedPlayers.clear(); // force a re-sort before the next use
}
//-------------------------------------------------------------------------------------------------------------
// purpose = 0 add, purpose = 1 remove.
void Game::BeegQueue(Player* p,int purpose) {
    for (auto& pair : queue) {
        
        if (pair.first == p && (pair.second == purpose)) return;
        //if (pair.first == p && (pair.second != purpose)) queue.at(pair); find a method to cancel out queues
    }
    queue.emplace_back(p, purpose);
    return;
}
//-------------------------------------------------------------------------------------------------------------
// get a player object inside the queue
bool Game::isBeegPlayer(Player *p) {
    if (queue.empty()) return false;
    for (auto& pair : queue) {
        if (pair.first == p) return true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------------------------
// resolve all queued instructions
void Game::ResolveBeeg() {
    if (queue.empty()) return;
    for ( auto& pair : queue) {
        if (pair.second == 0) AddPlayer(pair.first);
        if (pair.second == 1) RetirePlayer(pair.first);
    }
    queue.clear();
    return;
}
//-------------------------------------------------------------------------------------------------------------
// test
void Game::FillListTest() {

    for (int i = 1; i <= 20; ++i) {
        std::string name = "PLAYER" + std::to_string(i);
        Player* p = new Player(name, name, i);
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
void Game::RetirePlayer(Player* p) {
    if (!p) {
        return;
    }

    auto it = players.find(p->GetID());
    if (it != players.end() && it->second == p) {
        players.erase(it);
        eliminated.push_back(p);
    }

    sortedPlayers.erase(std::remove(sortedPlayers.begin(), sortedPlayers.end(), p), sortedPlayers.end());
}
//-------------------------------------------------------------------------------------------------------------
void Game::removeLatestPlayer() {
    if (sortedPlayers.empty()) {
        return;
    }

    Player* last = sortedPlayers.back();
    if (!last) {
        sortedPlayers.pop_back();
        return;
    }

    RetirePlayer(last);
}
//-------------------------------------------------------------------------------------------------------------
void Game::removeFirstPlayer() {
    if (sortedPlayers.empty()) {
        return;
    }

    Player* first = sortedPlayers.front();
    if (!first) {
        sortedPlayers.erase(sortedPlayers.begin());
        return;
    }

    RetirePlayer(first);
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
void Game::PlayTopCut() {
    if (!topCut) {
        topCut = true;
        roundNumber = 0;  
    }
    roundNumber++;
    setPairingsTopCut();
    GetPairing();
    return;
}
//-------------------------------------------------------------------------------------------------------------
void Game::ResetPairings() {
    pairings.clear();
    pairingResults.clear();
}
//-------------------------------------------------------------------------------------------------------------
void Game::createPairings() {
    if (sortedPlayers.empty()) {
        return;
    }

    // If odd, make sure whoever ends up last hasn't already had a bye
    if (sortedPlayers.size() % 2 != 0) {
        std::size_t lastIdx = sortedPlayers.size() - 1;
        if (sortedPlayers[lastIdx]->HadBye()) {
            for (std::size_t j = lastIdx; j-- > 0; ) {
                if (!sortedPlayers[j]->HadBye()) {
                    std::swap(sortedPlayers[j], sortedPlayers[lastIdx]);
                    break;
                }
            }
        }
    }
    // Create pairings based on wr for rounds
    for (std::size_t i = 0; i < sortedPlayers.size(); i += 2) {
        if (i + 1 < sortedPlayers.size()) {
            pairings.emplace_back(sortedPlayers[i], sortedPlayers[i + 1]);
            pairingResults.push_back('\0'); // awaiting a result
            sortedPlayers[i]->AddOpponent(sortedPlayers[i + 1]);
            sortedPlayers[i + 1]->AddOpponent(sortedPlayers[i]);
        }
        else {
            pairings.emplace_back(sortedPlayers[i], nullptr);
            pairingResults.push_back('B');   
            sortedPlayers[i]->SetScore('W'); // bye counts as a win
            sortedPlayers[i]->SetHadBye(true);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
void Game::SetPairings() {
    if (players.empty()) {
        return;
    }
    ResetPairings();
    SortPlayers();

    switch (roundNumber) {

    case 1: {
        std::random_device rd;
        std::mt19937 g(rd());
        // Shuffle the sortedPlayers vector to create random pairings for the first round
        std::shuffle(sortedPlayers.begin(), sortedPlayers.end(), g);
        createPairings();
        break;
    }

    default:
        createPairings();
        break;
    }
}
//-------------------------------------------------------------------------------------------------------------
void Game::setPairingsTopCut() {
    if (players.empty()) {
        return;
    }
    if (4 < players.size()) {
        while (players.size() > 4) {
            removeLatestPlayer();
        }
    }
    ResetPairings();
    SortPlayers();

    if (roundNumber == 1) {
        createPairings();
        return;
    }
    if (sortedPlayers.size() < 4) {
        return;   // cut is done
    }

    removeLatestPlayer();
    removeFirstPlayer();

    ResetPairings();
    createPairings();
}
//-------------------------------------------------------------------------------------------------------------
int Game::FindPairingIndex(const Player* a, const Player* b) const {
    for (std::size_t i = 0; i < pairings.size(); ++i) {
        const Player* first = pairings[i].first;
        const Player* second = pairings[i].second;
        if ((first == a && second == b) || (first == b && second == a)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
//-------------------------------------------------------------------------------------------------------------
bool Game::IsPairingScored(std::size_t index) const {
    if (index >= pairingResults.size()) {
        return false;
    }
    return pairingResults[index] != '\0';
}
//-------------------------------------------------------------------------------------------------------------
char Game::GetPairingResult(std::size_t index) const {
    if (index >= pairingResults.size()) {
        return '\0';
    }
    return pairingResults[index];
}
//-------------------------------------------------------------------------------------------------------------
bool Game::AllPairingsScored() const {
    if (pairingResults.empty()) {
        return false;
    }
    for (char c : pairingResults) {
        if (c == '\0') {
            return false;
        }
    }
    return true;
}
//-------------------------------------------------------------------------------------------------------------
bool Game::setScore(Player* w, Player* l, char t) {
    if (!w) {
        return false;
    }

    const int idx = FindPairingIndex(w, l);

    // Refuse to record a second result for the same pairing in a round.
    if (idx >= 0 && pairingResults[static_cast<std::size_t>(idx)] != '\0') {
        return false;
    }

    if (!l) {
        return false;
    }

    char result;
    // accept either case for a tie
    if (t == 't' || t == 'T') {
        w->SetScore('T');
        l->SetScore('T');
        result = 'T';
    }
    else {
        w->SetScore('W');
        l->SetScore('L');
        // Record the outcome from player 1 point of view
        result = (idx >= 0 && pairings[static_cast<std::size_t>(idx)].first == w) ? 'W' : 'L';
    }

    if (idx >= 0) {
        pairingResults[static_cast<std::size_t>(idx)] = result;
    }
    return true;
}
//------------------------------------------------------------------------------------------------------------- Rename later
std::vector<std::string> Game::GetPairing() {
    std::vector<std::string> results;
    std::string result1;
    std::string result2;
    if (pairings.empty()) {
        results.push_back("No pairings to display");
        return results;
    }
    // Generate pairings based on the current round
    for (const auto& pair : pairings) {
        if (!pair.first) {
            // keep the indices aligned with the pairings vector
            results.push_back("");
            continue;
        }

        result1 += pair.first->GetName() + " ";
        result1 += std::to_string(pair.first->GetID());
        result1 += " ------- " + std::to_string(pair.first->GetWins()) + "W/"
            + std::to_string(pair.first->GetLosses()) + "L/"
            + std::to_string(pair.first->GetTies()) + "T";
        result1 += " - " + std::to_string(pair.first->GetWR()) + "%";
        result1 += " vs ";
        if (pair.second) {
            result2 += pair.second->GetName() + " ";
            result2 += std::to_string(pair.second->GetID());
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
    for (std::size_t i = 0; i < sortedPlayers.size(); i++) {
        result += std::to_string(i + 1) + ". " + sortedPlayers[i]->GetName() + "       ---      " + std::to_string(sortedPlayers[i]->GetID()) + "      ---     " 
            + std::to_string(sortedPlayers[i]->GetWins()) + " - " + std::to_string(sortedPlayers[i]->GetLosses()) + " - " + std::to_string(sortedPlayers[i]->GetTies()) + "      ---       "
            + std::to_string(sortedPlayers[i]->GetWR()) + "% " + std::to_string(sortedPlayers[i]->GetOWR()) + "% " + std::to_string(sortedPlayers[i]->GetOOWR()) + "%\n";
    }
    if (!eliminated.empty() && roundNumber > 0) {
        result += "DROPPED: \n";
        for (std::size_t i = 0; i < eliminated.size(); i++) {
            result += eliminated[i]->GetName() + "      ---       " + std::to_string(eliminated[i]->GetID()) + "      ---       "
            + std::to_string(eliminated[i]->GetWins()) + " - " + std::to_string(eliminated[i]->GetLosses()) + " - " + std::to_string(eliminated[i]->GetTies()) + "      ---       "
            + std::to_string(eliminated[i]->GetWR()) + "% " + std::to_string(eliminated[i]->GetOWR()) + "% " + std::to_string(eliminated[i]->GetOOWR()) + "%\n";
        }
    }

    return result;
}
//-------------------------------------------------------------------------------------------------------------
std::string Game::CVVStandings() {
    SortPlayers();
    if (sortedPlayers.empty()) {
        return "No players to display";
    }

    std::string result;
    for (std::size_t i = 0; i < sortedPlayers.size(); i++) {
        result += sortedPlayers[i]->GetName() + " , " + std::to_string(sortedPlayers[i]->GetID()) + " , " + std::to_string(sortedPlayers[i]->GetWR()) + "% " + std::to_string(sortedPlayers[i]->GetOWR()) + "% " + std::to_string(sortedPlayers[i]->GetOOWR()) + "%\n";
    }
    if (!eliminated.empty() && roundNumber > 0) {
        result += "DROPPED: \n";
        for (std::size_t i = 0; i < eliminated.size(); i++) {
            result += eliminated[i]->GetName() + " , " + std::to_string(eliminated[i]->GetID()) + " , " + std::to_string(eliminated[i]->GetWR()) + "% " + std::to_string(eliminated[i]->GetOWR()) + "% " + std::to_string(eliminated[i]->GetOOWR()) + "%\n";
        }
    }
    return result;
}
//-------------------------------------------------------------------------------------------------------------