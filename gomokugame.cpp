#include "gomokugame.h"
#include <iostream>

GomokuGame::GomokuGame(QObject *parent) : QObject()
{
    for (int i = 0; i < GOBAN_SIZE; i++) {
        std::vector<int> tmp;
        tmp.resize(GOBAN_SIZE, 0);

        matrix.push_back(tmp);
    }

    current_player = 1;
}

GomokuGame::~GomokuGame()
{
}

bool GomokuGame::setMove(int x, int y, int player)
{
    if (x + 1 > GOBAN_SIZE || y + 1 > GOBAN_SIZE || x < 0 || y < 0) {
        return false;
    }

    if (matrix[y][x] != 0) {
        return false;
    }

    matrix[y][x] = player;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return true;
}

int GomokuGame::getMove(int x, int y)
{
    if (x + 1 > GOBAN_SIZE || y + 1 > GOBAN_SIZE || x < 0 || y < 0) {
        return -2;
    }

    return matrix[y][x];
}
