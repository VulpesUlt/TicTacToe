#include <iostream>
#include <chrono>
#include <random>
#include "Header.h"



	void initGame(Game* game) // Создает пустое игровое поле, определяет фигуры игроков и очередность ходов
	{
		game->turn = 0;
		game->state = GAME_IN_PROGRESS;
		game->field = new tile* [game->size];
		for (int i = 0; i < game->size; i++)
		{
			game->field[i] = new tile[game->size];
			for (int j = 0; j < game->size; j++)
				game->field[i][j] = EMPTY;
		}
		if (getRand())
		{
			game->human = CROSS;
			game->ai = ROUND;
		}
		else
		{
			game->human = ROUND;
			game->ai = CROSS;
		}
		game->isPlayersTurn = getRand();
	} 

	gamestate whoWon(Game game) // Проверяет не победил ли кто-то из игроков
	{
		int numPlayer = 0;
		int numAi = 0;
		for (int i = 0; i < game.size; i++) //Проверка строк
		{
			numPlayer = 0;
			numAi = 0;
			for (int j = 0; j < game.size; j++)
			{
				if (game.field[i][j] == game.human)
				{
					numPlayer++;
					numAi = 0;
					if (numPlayer == game.toWin)
						return PLAYER_WON;
				}
				else if (game.field[i][j] == game.ai)
				{
					numAi++;
					numPlayer = 0;
					if (numAi == game.toWin)
						return AI_WON;
				}
				else
				{
					numPlayer = 0;
					numAi = 0;
				}
			}
		}

		for (int i = 0; i < game.size; i++) //Проверка столбцов
		{
			numPlayer = 0;
			numAi = 0;
			for (int j = 0; j < game.size; j++)
			{
				if (game.field[j][i] == game.human)
				{
					numPlayer++;
					numAi = 0;
					if (numPlayer == game.toWin)
						return PLAYER_WON;
				}
				else if (game.field[j][i] == game.ai)
				{
					numAi++;
					numPlayer = 0;
					if (numAi == game.toWin)
						return AI_WON;
				}
				else
				{
					numPlayer = 0;
					numAi = 0;
				}
			}
		}

		for (int i = 3 - game.size; i < game.size - 2; i++) //Проверка дигоналей
		{
			numPlayer = 0;
			numAi = 0;
			for (int j = 0; j < game.size; j++)
			{
				if (i + j >= 0 && i + j <= game.size - 1)
				{
					if (game.field[i + j][j] == game.human)
					{
						numPlayer++;
						numAi = 0;
						if(numPlayer == game.toWin)
							return PLAYER_WON;
					}
					else if (game.field[i + j][j] == game.ai)
					{
						numAi++;
						numPlayer = 0;
						if (numAi == game.toWin)
							return AI_WON;
					}
				}
			}
		}

		for (int i = 3 - game.size; i < game.size - 2; i++) //Проверка дигоналей
		{
			numPlayer = 0;
			numAi = 0;
			for (int j = 0; j < game.size; j++)
			{
				if (game.size - j - 1 + i >= 0 && game.size - j - 1 + i <= game.size - 1)
				{
					if (game.field[game.size - j - 1 + i][j] == game.human)
					{
						numPlayer++;
						numAi = 0;
						if (numPlayer == game.toWin)
							return PLAYER_WON;
					}
					else if (game.field[game.size - j - 1 + i][j] == game.ai)
					{
						numAi++;
						numPlayer = 0;
						if (numAi == game.toWin)
							return AI_WON;
					}

				}
			}
		}
		
		if (game.turn > game.size * game.size - 2) // Проверка на ничью
			return TIE;
		
		return GAME_IN_PROGRESS;
	}

	coord getCoordHuman(Game game) // Ввод координаты клетуи игроком
	{
		coord c{ 0, 0 };
		std::cout << "Введите координаты клетки (x y): ";
		do
		{
			do
			{
				std::cin >> c.x >> c.y;
				c.x--;
				c.y--;
			} while (c.x < 0 || c.y < 0 || c.x > game.size - 1 || c.y > game.size - 1);
		} while (game.field[c.y][c.x] != EMPTY);
		std::cin.clear();
		return c;
	}

	void printGame(Game game) // Вывод игры в консоль
	{
		std::cout << "TicTacToe" << std::endl << std::endl;
		std::cout << "Фигура игрока:" << (char)game.human << "  " << "Фигура ИИ:" << (char)game.ai << std::endl << std::endl;
		std::cout << " ";
		for (int i = 0; i < game.size; i++)
			std::cout << " " << i + 1;
		std::cout << std::endl;
		for (int i = 0; i < game.size; i++)
		{
			std::cout << i + 1 << "|";
			for (int j = 0; j < game.size; j++)
			{
					std::cout << (char)game.field[i][j] << "|";
			}
			std::cout << std::endl;
		}
		if (game.state == GAME_IN_PROGRESS)
			std::cout << std::endl << "Номер хода: " << game.turn + 1 << std::endl;
	}

	bool getRand() // Подбрасывает монетку
	{
		const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		static std::mt19937 generator(seed);
		std::uniform_int_distribution <int> dis(0, 999);
		if (dis(generator) < 499)
			return true;
		else
			return false;
	}

	int getRand(int size) // Возвращает случайное число от 0 до заданного
	{
		const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		static std::mt19937 generator(seed);
		std::uniform_int_distribution <int> dis(0, size - 1);
		return dis(generator);
	}

	void endGame(Game* game) // Сообщает победителя, удаляет игровое поле
	{
		std::cout << std::endl;
		switch (game->state)
		{
		case AI_WON:
			std::cout << "Машины победили!";
			break;
		case PLAYER_WON:
			std::cout << "Человечество смогло выстоять!";
			break;
		case TIE:
			std::cout << "Победила дружба!";
			break;
		case GAME_IN_PROGRESS:
			std::cout << "Что-то явно пошло не так...";
			break;
		}
		
		std::cout << std::endl;
		for (int i = 0; i < game->size; i++)
			delete[] game->field[i];
		delete[] game->field;
		game->field = nullptr;
	}

	coord getCoordAi (Game game)
	{
		coord c{0, 0};
		int numPlayer{ 0 }, numAi{ 0 };
		int** potential = new int* [game.size];
		for (int i = 0; i < game.size; i++)
			potential[i] = new int[game.size];
		for (int i = 0; i < game.size; i++)
			for (int j = 0; j < game.size; j++)
				potential[i][j] = 0;

		if (game.turn == game.toWin - 2 )
			do
			{
				c.x = getRand(game.size);
				c.y = getRand(game.size);
			} while (game.field[c.y][c.x] != EMPTY);
		else
		{
			for (int i = 0; i < game.size; i++) //Проверка строк
			{
				numPlayer = 0;
				numAi = 0;
				for (int j = 0; j < game.size - 1; j++)
				{
					if (game.field[i][j] == game.human)
					{

						if (game.field[i][j] == game.field[i][j + 1])
							numPlayer++;
						else if (j + 2 <= game.size - 1 && game.field[i][j + 1] == EMPTY && game.field[i][j + 2] == game.human) // Проверка наличия одной дырки в строке
						{
							for (int k = j + 2; k < game.size - 1; k++)
								if (game.field[i][k] == game.human)
								{
									numPlayer++;
									if (numPlayer == game.toWin - 2)
										potential[i][j + 1] = 100;
								}
								else
									break;
						}
						else
						{
							numPlayer++;
							if (game.field[i][j + 1] == EMPTY && numPlayer == game.toWin - 1)
								potential[i][j + 1] = 100;
							else if (game.field[i][j + 1] == EMPTY )
								potential[i][j + 1] += numPlayer;
						}
						numAi = 0;
					}
					else if (game.field[i][j] == game.ai)
					{

						if (game.field[i][j] == game.field[i][j + 1])
							numAi++;
						else
						{
							numAi++;
							if (game.field[i][j + 1] == EMPTY && numAi == game.toWin - 1)
								potential[i][j + 1] = 200;
							else if (game.field[i][j + 1] == EMPTY)
								potential[i][j + 1] += numAi;
						}
						numPlayer = 0;
					}
					else
					{
						numPlayer = 0;
						numAi = 0;
					}
				}
			}
			
			for (int i = 0; i < game.size; i++) //Обратная проверка строк
			{
				numPlayer = 0;
				numAi = 0;
				for (int j = game.size - 1; j > 1; j--)
				{
					if (game.field[i][j] == game.human)
					{

						if (game.field[i][j] == game.field[i][j - 1])
							numPlayer++;
						else
						{
							numPlayer++;
							if (game.field[i][j - 1] == EMPTY && numPlayer == game.toWin - 1)
								potential[i][j - 1] = 100;
							else if (game.field[i][j - 1] == EMPTY)
								potential[i][j - 1] += numPlayer;
						}
						numAi = 0;
					}
					else if (game.field[i][j] == game.ai)
					{

						if (game.field[i][j] == game.field[i][j - 1])
							numAi++;
						else
						{
							numAi++;
							if (game.field[i][j - 1] == EMPTY && numAi == game.toWin - 1)
								potential[i][j - 1] = 200;
							else if (game.field[i][j - 1] == EMPTY)
								potential[i][j - 1] += numAi;
						}
						numPlayer = 0;
					}
					else
					{
						numPlayer = 0;
						numAi = 0;
					}
				}
			}
			
			for (int i = 0; i < game.size; i++) //Проверка столбцов
			{
				numPlayer = 0;
				numAi = 0;
				for (int j = 0; j < game.size - 1; j++)
				{
					if (game.field[j][i] == game.human)
					{

						if (game.field[j][i] == game.field[j + 1][i])
							numPlayer++;
						else if (j + 2 <= game.size - 1 && game.field[j + 1][i] == EMPTY && game.field[j + 2][i] == game.human) // Проверка наличия одной дырки в строке
						{
							for (int k = j + 2; k < game.size - 1; k++)
								if (game.field[k][i] == game.human)
								{
									numPlayer++;
									if (numPlayer == game.toWin - 2)
										potential[j + 1][i] = 100;
								}
								else
									break;
						}
						else
						{
							numPlayer++;
							if (game.field[j + 1][i] == EMPTY && numPlayer == game.toWin - 1)
								potential[j + 1][i] = 100;
							else if (game.field[j + 1][i] == EMPTY)
								potential[j + 1][i] += numPlayer;
						}
						numAi = 0;
					}
					else if (game.field[j][i] == game.ai)
					{
						if (game.field[j][i] == game.field[j + 1][i])
							numAi++;
						else
						{
							numAi++;
							if (game.field[j + 1][i] == EMPTY && numAi == game.toWin - 1)
								potential[j + 1][i] = 200;
							else if (game.field[j + 1][i] == EMPTY)
								potential[j + 1][i] += numAi;
						}
						numPlayer = 0;
					}
					else
					{
						numPlayer = 0;
						numAi = 0;
					}
				}
			}
			
			for (int i = 0; i < game.size; i++) //Обратная проверка столбцов
			{
				numPlayer = 0;
				numAi = 0;
				for (int j = game.size - 1; j > 0; j--)
				{
					if (game.field[j][i] == game.human)
					{

						if (game.field[j][i] == game.field[j - 1][i])
							numPlayer++;
						else
						{
							numPlayer++;
							if (game.field[j - 1][i] == EMPTY && numPlayer == game.toWin - 1)
								potential[j - 1][i] = 100;
							else if (game.field[j - 1][i] == EMPTY)
								potential[j - 1][i] += numPlayer;
						}
						numAi = 0;
					}
					else if (game.field[j][i] == game.ai)
					{
						if (game.field[j][i] == game.field[j - 1][i])
							numAi++;
						else
						{
							numAi++;
							if (game.field[j - 1][i] == EMPTY && numAi == game.toWin - 1)
								potential[j - 1][i] = 200;
							else if (game.field[j - 1][i] == EMPTY)
								potential[j - 1][i] += numAi;
						}
						numPlayer = 0;
					}
					else
					{
						numPlayer = 0;
						numAi = 0;
					}
				}
			}
		
			for (int i = 0; i < game.size; i++)
				for (int j = 0; j < game.size; j++)
					if (potential[i][j] >= potential[c.y][c.x] && game.field[i][j] == EMPTY)
					{
						c.y = i;
						c.x = j;
					}
		}

		
		
		for (int i = 0; i < game.size; i++)
			delete[] potential[i];
		delete[] potential;
		
		return c;
	}