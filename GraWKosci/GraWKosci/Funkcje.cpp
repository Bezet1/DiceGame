#include "Definicje.h"

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

std::string Player::getName() {
	return this->Name;
}

int Player::getID() {
	return this->ID;
}

int Player::getPoints() {
	return this->Points;
}

int Player::getDrawPoints() {
	return this->PointsInDraw;
}

void Player::setHasPlace(bool zmienna) {
	this->HasPlace = zmienna;
}

bool Player::getHasPlace() {
	return this->HasPlace;
}

std::string Player::getDateOfJoin() {
	return this->DateOfJoin;
}

void Player::updatePoints(int number) {
	this->Points += number;
}

void Player::updateDrawPoints(int number) {
	this->PointsInDraw += number;
}

void Player::setDateOfJoin() {
	
	time_t localTime;
	tm tm_obj;
	auto ptr = &tm_obj;
	time(&localTime);
	localtime_s(ptr, &localTime);

	std::stringstream ss;

	ss << ptr->tm_mday << ":" << ptr->tm_mon + 1 << ":" << 1900 + ptr->tm_year;

	DateOfJoin = ss.str();
}

void Player::getRandomRollsAndCollors(std::vector<int>& rolls, std::vector<int>& colors, std::mt19937& rng) {
	for (int i = 0; i < rolls.size(); ++i) {
		rolls[i] = std::uniform_int_distribution<int> (1, 6)(rng);
		colors[i] = std::uniform_int_distribution<int>(5, 15)(rng);
	}
}

int Player::rollDice(int numberOfDice) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::vector<int> rolls(numberOfDice);
	std::vector<int> colors(numberOfDice);
	std::vector<int> lastRolls;
	std::string input;

	std::cout << std::endl;
	writeCharacter(10, " ", 7);
	SetConsoleTextAttribute(h, 3);
	std::cout << Name;
	SetConsoleTextAttribute(h, 7);
	std::cout<< "'s turn (press r to roll)" << std::endl;

	while (std::getline(std::cin, input)) {
		if (input == "r" || input == "R") {
			break;
		}
		else {
			alert("Invalid input", 4);
		}
	}

	std::cout << "Rolling the dice..." << std::endl;
	for (int i = 0; i < 10; i++) {
		getRandomRollsAndCollors(rolls, colors, rng);
		lastRolls = rolls;

		SetConsoleTextAttribute(h, 7); 
		std::cout << "\rRolling: ";

		for (int j = 0; j < numberOfDice; j++) {
			SetConsoleTextAttribute(h, colors[j]);
			std::cout << std::string(i + 1, '#') << std::string(10 - i - 1, ' ') << " " << rolls[j] << " " << std::flush;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(3));//300
	}

	std::cout << std::endl;

	SetConsoleTextAttribute(h, 3);
	std::cout << Name;
	SetConsoleTextAttribute(h, 7);
	std::cout << " rolled a ";
	SetConsoleTextAttribute(h, 6);
	
	int sum = 0;
	for (auto singleRoll : lastRolls) {
		sum += singleRoll;
	}
	
	std::cout << sum << std::endl;
	SetConsoleTextAttribute(h, 7);

	return sum;

}

Player::Player(std::string name, int id) {
	this->Name = name;
	this->ID = id;
	Player::setDateOfJoin();
}


bool showMenu() {
	menuLayout();

	std::string input;
	while (std::getline(std::cin, input)) {
		if (input == "o" || input == "O") {
			showOptions();
			break;
		}
		else if (input == "r" || input == "R") {
			showRanking();
			break;
		}
		else if (input == "s" || input == "S") {
			startGame();
			break;
		}
		else if (input == "e" || input == "E") {
			return false;
		}
		else {
			alert("Invalid input", 4);
		}
	}
	return true;
}

void showOptions() {
	optionsLayout();

	std::string input;
	while (std::getline(std::cin, input)) {
		if (input == "b" || input == "B") {
			showMenu();
			break;
		}
		else {
			alert("Invalid input", 4);
		}
	}
}

void showRanking() {
	rankingLayout();

	std::string input;
	while (std::getline(std::cin, input)) {
		if (input == "b" || input == "B") {
			showMenu();
			break;
		}
		else {
			alert("Invalid input", 4);
		}
	}
}

void startGame() {
	//jesli sie uda to players do wywalenia TODO
	std::vector<Player*> player_ptrs;

	int numberOfPlayers, numberOfDice, numberOfRounds;

	setGame(numberOfPlayers, numberOfDice, numberOfRounds);

	setNamesOfPlayers(numberOfPlayers, player_ptrs);
	
	loading();

	startRound(numberOfRounds, numberOfDice, player_ptrs);

	sortPlayersByPoints(player_ptrs);

	checkDraw(player_ptrs, numberOfDice);

	showScoreboard(player_ptrs);
}

void startRound(int numberOfRounds, int numberOfDice, std::vector<Player*>& players) {

	for (int i = 1; i < numberOfRounds + 1; i++) {
		roundLayout(i);

		for (Player* singlePlayer : players) {
			int sum = singlePlayer->rollDice(numberOfDice);
			singlePlayer->updatePoints(sum);
		}

		std::string input;
		std::cout << "\nPress n to start next round" << std::endl;
		while (std::getline(std::cin, input)) {
			if (input == "n" || input == "N") {
				break;
			}
			else {
				alert("Invalid input", 4);
			}
		}
	}

}

void applySort(std::vector<Player*>& sortedPlayers, std::vector<Player*>& drawPlayers, int index) {
	int counter = index;
	for (int i = 0; i < drawPlayers.size(); i++) {
		sortedPlayers[counter] = drawPlayers[i];
		counter++;
	}
}

//zmienic tak aby checkDraw zaczynal od pierwszyej osoby ktora nie ma miejsca
void checkDraw(std::vector<Player*>& sortedPlayers, int numberOfDice) {

	std::vector<Player*> drawPlayers;
	drawPlayers.push_back(sortedPlayers[0]);

	for (int i = 1; i < sortedPlayers.size(); i++) {
		if (checkIfEaqulPoints(drawPlayers[0], sortedPlayers[i])) {

			drawPlayers.push_back(sortedPlayers[i]);

		}
		else if (drawPlayers.size() > 1) {

			drawRound(drawPlayers, numberOfDice);

			applySort(sortedPlayers, drawPlayers, i - drawPlayers.size());

			drawPlayers.clear();
			drawPlayers.push_back(sortedPlayers[i]);
			
		}
		else {

			drawPlayers.clear();
			drawPlayers.push_back(sortedPlayers[i]);
			
		}
	}

	if (drawPlayers.size() > 1) {
		drawRound(drawPlayers, numberOfDice);
		applySort(sortedPlayers, drawPlayers, sortedPlayers.size() - drawPlayers.size());

	}

}



bool checkIfEaqulPoints(Player* p1, Player* p2) {
	if (p1->getPoints() == p2->getPoints()) {
		if (p1->getDrawPoints() == p2->getDrawPoints()) {
			return true;
		}
		else {
			p1->setHasPlace(true);
			return false;
		}
	}
	else {
		p1->setHasPlace(true);
		return false;
	}
}

void drawRound(std::vector<Player*>& drawPlayers, int numberOfDice) {
	checkDrawLayout();

	for (Player* singlePlayer : drawPlayers) {
		std::cout << singlePlayer->getName() << " ";
	}
	std::cout << std::endl;

	for (int i = 0; i < drawPlayers.size(); i++) {
		int sum = drawPlayers[i]->rollDice(numberOfDice);
		drawPlayers[i]->updateDrawPoints(sum);
	}

	std::string input;
	std::cout << "\nPress n to start next round" << std::endl;
	while (std::getline(std::cin, input)) {
		if (input == "n" || input == "N") {
			break;
		}
		else {
			alert("Invalid input", 4);
		}
	}

	sortPlayersByPoints(drawPlayers);

	checkDraw(drawPlayers, numberOfDice);

}

void showScoreboard(std::vector<Player*>& players) {
	scoreboardLayout();
	
	//sortPlayersByPoints(players);
	
	std::cout << std::endl;
	writeCharacter(25, " ", 7);

	SetConsoleTextAttribute(h, 14);
	std::cout << players[0]->getName() << " WINS!" << std::endl << std::endl;
	SetConsoleTextAttribute(h, 7);

	int counter = 1;
	for (Player* singlePlayer : players) {
		SetConsoleTextAttribute(h, 7);
		std::cout << counter << ". ";
		SetConsoleTextAttribute(h, 3);
		std::cout << singlePlayer->getName();
		SetConsoleTextAttribute(h, 7);
		std::cout << ": " << singlePlayer->getPoints() << std::endl;
		counter++;
	}

	SetConsoleTextAttribute(h, 7);
	std::cout << std::endl << "\nMenu - m" << "\nExit - e" << std::endl;

	std::string input;
	while (std::getline(std::cin, input)) {
		if (input == "m" || input == "M") {
			showMenu();
			break;
		}
		else if (input == "e" || input == "E") {
			break;
		}
		else {
			alert("Invalid input", 4);
		}
	}
}

void setGame(int& numbersOfPlayers, int& numbersOfDice, int& numbersOfRounds) {

	int mainColor = 5;
	int titleColor = 2;
	int namesColor = 6;

	system("cls");

	std::cout << std::endl;
	writeCharacter(25, " ", 7);

	SetConsoleTextAttribute(h, titleColor);
	std::cout << "Game settings" << std::endl << std::endl << std::endl;

	getInputUntilDone(numbersOfPlayers, "Number of Players: ", "There must be at least 2 players", 2);

	getInputUntilDone(numbersOfDice, "Number of Dice: ", "There must be at least 1 dice", 1);

	getInputUntilDone(numbersOfRounds, "Number of Rounds: ", "There must be at least 1 Round", 1);

	SetConsoleTextAttribute(h, 7);
}

void setNamesOfPlayers(int& numbersOfPlayers, std::vector<Player*> &players) {

	int mainColor = 5;
	int titleColor = 2;
	int namesColor = 6;

	system("cls");

	std::cout << std::endl;
	writeCharacter(25, " ", 7);

	SetConsoleTextAttribute(h, titleColor);
	std::cout << "Set players' names" << std::endl << std::endl << std::endl;

	for (int i = 0; i < numbersOfPlayers; i++) {
		std::string name;
		SetConsoleTextAttribute(h, 7);
		std::cout << "Name of player "<< i + 1 <<": ";
		SetConsoleTextAttribute(h, 3);

		while (std::getline(std::cin, name)) {
			std::string correctedName = correctName(name);
			if(!checkName(correctedName)){
				alert("Name must contain 3 - 15 characters", 4);
				std::cout << "Name of player " << i + 1 << ": ";
				SetConsoleTextAttribute(h, 3);
			}
			else {
				Player* newPlayer = new Player(correctedName, i);
				players.push_back(newPlayer);
				break;

			}
		}

		
	}

	SetConsoleTextAttribute(h, 7);
}

void getInputUntilDone(int& Number, std::string text, std::string alertText, int minimal) {

	std::string input;

	SetConsoleTextAttribute(h, 7);
	std::cout << text;
	SetConsoleTextAttribute(h, 3);

	while (std::getline(std::cin, input)) {
		if (checkNumber(input)) {
			if (stoi(input) < minimal) {
				alert(alertText, 4);
				std::cout << "Try again: ";
				SetConsoleTextAttribute(h, 3);
			}
			else {
				Number = stoi(input);
				break;
			}

		}
		else {
			alert("Input provided is not a valid number", 4);
			std::cout << "Try again: ";
			SetConsoleTextAttribute(h, 3);
		}
	}
}

void loading() {
	system("cls");

	std::cout << std::endl << std::endl;
	writeCharacter(25, " ", 7);
	std::cout << "Loading";

	for (int i = 0; i < 1; ++i) {
		std::cout << ".";

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	system("cls");

}

bool comparePlayersByPoints(Player* p1, Player* p2) {
	if (p1->getHasPlace()) {
		return false;
	}
	else if (p1->getPoints() == p2->getPoints()) {
		return p1->getDrawPoints() > p2->getDrawPoints();
	}
	else {
		return p1->getPoints() > p2->getPoints();
	}
}

void sortPlayersByPoints(std::vector<Player*>& players) {
	std::sort(players.begin(), players.end(), comparePlayersByPoints);
}

bool checkName(std::string name) {
	if (name.length() > 15 || name.length() < 3) {
		return false;
	}
	else return true;
}


bool checkNumber(std::string str) {
	for (int i = 0; i < str.length(); i++)
		if (isdigit(str[i]) == false)
			return false;
	return true;
}

std::string correctName(std::string name) {

	//usuwanie przypadkowych spacji z poczatku i konca nazwy
	while (!name.empty() && std::isspace(name.front())) {
		name.erase(0, 1);
	}

	while (!name.empty() && std::isspace(name.back())) {
		name.pop_back();
	}

	return name;
}

void menuLayout() {

	int mainColor = 5;
	int titleColor = 2;
	int namesColor = 6;
	int exitColor = 7;

	system("cls");
	
	writeCharacter(71, "#", mainColor);
	std::cout << std::endl;
	writeRow(1, mainColor);
	writeName("Dice Game", 30, titleColor, mainColor);
	writeRow(3, mainColor);
	writeName("Start - s", 30, namesColor, mainColor);
	writeRow(1, mainColor);
	writeName("Ranking - r", 29, namesColor, mainColor);
	writeRow(1, mainColor);
	writeName("Options - o", 29, namesColor, mainColor);
	writeRow(2, mainColor);
	writeName("Exit - e ", 30, exitColor, mainColor);
	writeRow(1, mainColor);
	writeCharacter(71, "#", mainColor);
	std::cout << std::endl;

	SetConsoleTextAttribute(h, 7);

}

void optionsLayout() {

	int mainColor = 8;
	int titleColor = 2;
	int namesColor = 6;
	int exitColor = 7;

	system("cls");

	SetConsoleTextAttribute(h, mainColor);

	writeCharacter(71, "#", mainColor);
	std::cout << std::endl;
	writeRow(1, mainColor);
	writeName("OPCJE", 32, titleColor, mainColor);
	writeRow(3, mainColor);
	writeName("Roll the dice - r", 26, namesColor, mainColor);
	writeRow(1, mainColor);
	writeName("Next player - n", 27, namesColor, mainColor);
	writeRow(2, mainColor);
	writeName(" Back - b", 30, exitColor, mainColor);
	writeRow(1, mainColor);
	writeCharacter(71, "#", mainColor);
	std::cout << std::endl;
}

void rankingLayout() {
	int mainColor = 8;
	int titleColor = 2;
	int namesColor = 6;
	int exitColor = 7;

	system("cls");

	SetConsoleTextAttribute(h, mainColor);

	writeCharacter(71, "#", mainColor);
	std::cout << std::endl;
	writeRow(1, mainColor);
	writeName("RANKING", 31, titleColor, mainColor);
	writeRow(10, mainColor);
	writeName(" Back - b", 30, exitColor, mainColor);
	writeRow(1, mainColor);

	writeCharacter(71, "#", mainColor);
	std::cout << std::endl;
}

void roundLayout(int numberOfRound) {
	int titleColor = 2;

	system("cls");

	writeCharacter(25, " ", 7);
	SetConsoleTextAttribute(h, titleColor);
	std::cout << "Round " << numberOfRound << std::endl;
	SetConsoleTextAttribute(h, 7);
}

void checkDrawLayout() {
	int titleColor = 2;

	system("cls");
	std::cout << std::endl;
	writeCharacter(25, " ", 7);

	SetConsoleTextAttribute(h, titleColor);
	std::cout << "Draw of players: ";
	SetConsoleTextAttribute(h, 7);
}

void scoreboardLayout() {
	int titleColor = 2;
	int namesColor = 6;

	system("cls");

	writeCharacter(25, " ", 7);
	SetConsoleTextAttribute(h, titleColor);
	std::cout << "Scoreboard " << std::endl;
	SetConsoleTextAttribute(h, 7);
}


void writeRow(int number, int color) {
	for (int i = 0; i < number; i++) {
		
		SetConsoleTextAttribute(h, color);
		std::cout << "#";

		writeCharacter(69, " ", color);

		SetConsoleTextAttribute(h, color);
		std::cout << "#" << std::endl;
	}
}

void writeName(std::string name, int number, int colorName, int mainColor) {
	
	SetConsoleTextAttribute(h, mainColor);
	std::cout << "#";

	writeCharacter(number, " ", 7);

	SetConsoleTextAttribute(h, colorName);
	std::cout << name;
	SetConsoleTextAttribute(h, mainColor);

	writeCharacter(number, " ", 7);

	SetConsoleTextAttribute(h, mainColor);
	std::cout << "#" << std::endl;
	SetConsoleTextAttribute(h, 7);
}


void writeCharacter(int number, const char* character, int color) {

	SetConsoleTextAttribute(h, color);

	for (int i = 0; i < number; i++) {
		std::cout << character;
	}

	SetConsoleTextAttribute(h, 7);
}

void alert(std::string text, int color) {
	SetConsoleTextAttribute(h, color);
	std::cout << text << std::endl;
	SetConsoleTextAttribute(h, 7);
}
