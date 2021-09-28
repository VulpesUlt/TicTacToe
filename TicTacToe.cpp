// TicTacToe.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Header.h"
#include <string>






int main()
{
    std::string nextGame;
    Game game;
    coord c;
    setlocale(LC_ALL, "Russian");
    do
    {
        system("cls");
        std::cout << "Введите размер поля:";
        do
        {
            std::cin >> game.size;
        } while (game.size < 3 || game.size > 9); // Можно больше, но это ломает вывод
        std::cout << "Введите колл-во фигур для победы:";
        do
        {
            std::cin >> game.toWin;
        } while (game.toWin > game.size || game.toWin < 3);
        initGame(&game);

        do {
            if (game.isPlayersTurn)
            {
                system("cls");
                printGame(game);
                c = getCoordHuman(game);
                game.field[c.y][c.x] = game.human;
                game.isPlayersTurn = false;
}
            else
            {

                c = getCoordAi(game);
                game.field[c.y][c.x] = game.ai;
                game.isPlayersTurn = true;

            }
            game.state = whoWon(game);
            game.turn++;
        } while (game.state == GAME_IN_PROGRESS);
        system("cls");
        printGame(game);
        endGame(&game);

        std::cout << "Хотите сыграть ещё раз? (y/n): ";
        do
        {
            std::cin >> nextGame;
        } while (nextGame != "n" && nextGame != "y");
    } while (nextGame != "n");
}

