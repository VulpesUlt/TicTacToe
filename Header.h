#pragma once

enum tile
{
	EMPTY = '_',
	CROSS = 'X',
	ROUND = 'O'
};

enum gamestate
{
	GAME_IN_PROGRESS,
	PLAYER_WON,
	AI_WON,
	TIE
};

struct Game
{
	int size = 3;
	tile** field = nullptr;
	gamestate state = GAME_IN_PROGRESS;
	tile ai;
	tile human;
	int turn = 0;
	int toWin = 3;
	bool isPlayersTurn;
};

struct coord
{
	int x;
	int y;
};

bool getRand();
int getRand(int);
void initGame(Game*);
gamestate whoWon(Game);
coord getCoordHuman(Game);
void printGame(Game);
void endGame(Game*);
coord getCoordAi(Game);