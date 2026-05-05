#pragma once
#include <string>
#include <vector>
using namespace std;

class Player
{
public:
	Player(string Fname, string Lname, int id);
	Player();

	~Player();

	void SetScore( char h);

	void SetName(string fname, string lname);

	void AddOpponent(Player* opponent);
	void RemoveOpponents(Player* opponent);

	double GetWR() const;
	double GetOWR() const;

	/*double GetOOWR() const;*/
	int GetID();

	int GetWins() const;
	int GetLosses() const;
	int GetTies() const;

	string GetName() const;

private:
	string Fname;
	string Lname;
	int wins;
	int losses;
	int ties;
	int ID;

	vector<Player*> opponents;
};

