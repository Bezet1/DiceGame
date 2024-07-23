# Dice Game Project

## Overview

This project is a console-based dice game implemented in C++ for Windows. The game allows multiple players to compete over several rounds by rolling dice, accumulating points, and determining a winner based on the highest score. The game includes features such as a menu interface, player management, and automated game rounds.

## Features

- **Player Management**: Create players with unique IDs and names. Track their join date, points, and status.
- **Game Rounds**: Conduct multiple rounds where players roll dice and accumulate points.
- **Scoreboard**: Display a scoreboard showing player rankings based on their points.
- **Tie Breaker**: Handle tie situations by conducting draw rounds.

## Class Descriptions

### Player Class

The `Player` class manages the information and actions related to each player in the game. It includes methods for rolling dice, updating points, and retrieving player details such as name, ID, and points.

### Round Class

The `Round` class represents a single round of the game. It manages the layout and execution of each round, ensuring players can roll dice and accumulate points correctly.

## Functions

### Menu and Options

- `void menuLayout()`: Display the menu layout.
- `void optionsLayout()`: Display the options layout.
- `bool showMenu()`: Show the menu and return a boolean indicating if the game should start.
- `void showOptions()`: Show the game options.

### Game Setup and Start

- `void setGame(int& numbersOfPlayers, int& numbersOfDice, int& numbersOfRounds)`: Set up the game with the specified number of players, dice, and rounds.
- `void startGame()`: Start the game.

### Player and Score Management

- `void showScoreboard(std::vector<Player*>& players)`: Display the scoreboard.
- `bool comparePlayersByPoints(Player* p1, Player* p2)`: Compare two players by their points.
- `void sortPlayersByPoints(std::vector<Player*>& players)`: Sort players by their points.
- `void checkDraw(std::vector<Player*>& sortedPlayers, int numberOfDice)`: Check for draw situations.
- `void drawRound(std::vector<Player*>& drawPlayers, int numberOfDice)`: Conduct a draw round.
- `bool checkIfEaqulPoints(Player* p1, Player* p2)`: Check if two players have equal points.
- `void applySort(std::vector<Player*>& sortedPlayers, std::vector<Player*>& drawPlayers, int index)`: Apply sorting to players based on their points.

### Utility Functions

- `void writeCharacter(int number, const char* character, int color)`: Write a character to the console with color.
- `void writeName(std::string name, int number, int colorName, int mainColor)`: Write a player's name to the console with color.
- `void writeRow(int number, int color)`: Write a row to the console with color.
- `void alert(std::string text, int color)`: Display an alert message with color.
- `bool checkNumber(std::string str)`: Check if a string is a valid number.
- `bool checkName(std::string name)`: Check if a name is valid.
- `void setNamesOfPlayers(int& numbersOfPlayers, std::vector<Player*>& vectorOfPlayers)`: Set the names of the players.
- `std::string correctName(std::string name)`: Correct the name format.
- `void getInputUntilDone(int& Number, std::string text, std::string alertText, int minimal)`: Get input until a valid number is entered.
- `void loading()`: Display a loading message.
- `void startRound(int numbersOfRounds, int numbersOfDice, std::vector<Player*>& players, std::vector<Round>& rounds)`: Start the specified number of rounds.
- `void getRoundsInstances(std::vector<Round>& rounds, int numberOfRounds)`: Get instances of the rounds.
