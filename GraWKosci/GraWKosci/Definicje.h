#pragma once

#include <iostream>
#include<string>
#include <windows.h>
#include <ctime>
#include <sstream> 
#include <vector>
#include <thread>
#include <random>
#include <map>

class Player {
	std::string Name;
	std::string DateOfJoin;
	int ID;
	int Points = 0;
	int PointsInDraw = 0;
	bool HasPlace = false;
	void setDateOfJoin();
	void getRandomRollsAndCollors(std::vector<int>& rolls, std::vector<int>& colors, std::mt19937& rng);
public:
	Player(std::string name, int id);
	int rollDice(int numberOfDice);
	void updatePoints(int number);
	void updateDrawPoints(int number);
	std::string getName();
	int getID();
	int getPoints();
	int getDrawPoints();
	void setHasPlace(bool zmienna);
	bool getHasPlace();
	std::string getDateOfJoin();
};

class Round {
	int Number;
public:
	Round(int number);
	void RoundLayout();
};

void menuLayout();
void optionsLayout();
void checkDrawLayout();
void scoreboardLayout();
bool showMenu();
void showOptions();
void showScoreboard(std::vector<Player*>& players);
void startGame();
void setGame(int& numbersOfPlayers, int& numbersOfDice, int& numbersOfRounds);
void writeCharacter(int number, const char* character, int color);
void writeName(std::string name, int number, int colorName, int mainColor);
void writeRow(int number, int color);
void alert(std::string text, int color);
bool checkNumber(std::string str);
bool checkName(std::string name);
void setNamesOfPlayers(int& numbersOfPlayers, std::vector<Player*>& vectorOfPlayers);
std::string correctName(std::string name);
void getInputUntilDone(int& Number, std::string text, std::string alertText, int minimal);
void loading();
void startRound(int numbersOfRounds, int numbersOfDice, std::vector<Player*>& players, std::vector<Round>& rounds);
void getRoundsInstances(std::vector<Round>& rounds, int numberOfRounds);
bool comparePlayersByPoints(Player* p1, Player* p2);
void sortPlayersByPoints(std::vector<Player*>& players);
void checkDraw(std::vector<Player*>& sortedPlayers, int numberOfDice);
void drawRound(std::vector<Player*>& drawPlayers, int numberOfDice);
bool checkIfEaqulPoints(Player* p1, Player* p2);
void applySort(std::vector<Player*>& sortedPlayers, std::vector<Player*>& drawPlayers, int index);