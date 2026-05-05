#include "Player.h"
#include <string>
#include <vector>
using namespace std;

Player::Player(std::string fname, std::string lname, int id)
    : Fname(fname), Lname(lname), ID(id), wins(0), losses(0), ties(0) {

}

void Player::SetScore(char h) {
    if (h == 'W') wins++;
    else if (h == 'L') losses++;
    else if (h == 'T') ties++;
}

void Player::SetName(string fname, string lname) {
    Fname = fname;
    Lname = lname;
}

void Player::AddOpponent(Player* opponent) {
    opponents.push_back(opponent);
}

double Player::GetWR() const {
    int total = wins + losses + ties;
    if (total == 0) return 0.0;

    return static_cast<double>(wins) / total;
}
double Player::GetOWR() const {
    double total = 0.0;
    double win = 0.0;
    for(int i = 0; i < opponents.size(); i++){
        Player* p = opponents.at(i);
         total += p->GetWins() + p->GetLosses() + p->GetTies();
         win += p->GetWins();
    }
    if (total == 0) return 0.0;

    return win / total;
}
//double Player::GetOOWR() const {
//
//}
int Player::GetWins() const {
    return wins;
}
int Player::GetLosses() const {
    return losses;
}
int Player::GetTies() const {
    return ties;
}
int Player::GetID(){
    return ID;
}

string Player::GetName() const {
    string name = Fname + " " + Lname;
    return name;
}



