#include "gomokugame.h"

Three::Three(int x, int y, int x_end, int y_end, int dir_X, int dir_Y) : x1(x), y1(y), x2(x_end), y2(y_end), dirX(dir_X), dirY(dir_Y), space(0), active(false)
{
}

Three::~Three()
{
}

GomokuGame::GomokuGame() : QObject()
{
    for (int i = 0; i < GOBAN_SIZE; i++) {
        std::vector<int> tmp;
        tmp.resize(GOBAN_SIZE, 0);

        matrix.push_back(tmp);
    }

    current_player = -1;
    threes.push_back( Three(-1,-1,-1,-1,0,0) );
    threes.push_back( Three(-1,-1,-1,-1,0,0) );
}

GomokuGame::~GomokuGame()
{
}

void GomokuGame::deactivateThree(int x, int y, int player)
{
    Three curr = threes[player == -1 ? 0 : 1];

    if ( (x == curr.x1 && y == curr.y1) ||
         (x == curr.x2 && y == curr.y2) ) {
        threes[player == -1 ? 0 : 1].active = false;
    } else if ( (curr.x1 + curr.dirX * curr.space == x && curr.y1 + curr.dirY * curr.space == y) ) {
        threes[player == -1 ? 0 : 1].active = false;
    }
}

void GomokuGame::activateThree(int x, int y, int player)
{
    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 2, player) && !checkVal(x + 3 * dirX, y + 3 * dirY, player * -1) && !checkVal(x + -1 * dirX, y + -1 * dirY, player * -1)) {
                if (checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                    threes[player == -1 ? 0 : 1].active = false;
                } else {
                    threes[player == -1 ? 0 : 1].active = true;
                }
                threes[player == -1 ? 0 : 1].x1 = x;
                threes[player == -1 ? 0 : 1].y1 = y;
                threes[player == -1 ? 0 : 1].x2 = x + 3 * dirX;
                threes[player == -1 ? 0 : 1].y2 = y + 3 * dirY;
                threes[player == -1 ? 0 : 1].dirX = dirX;
                threes[player == -1 ? 0 : 1].dirY = dirY;
            } else if (checkVal(x + dirX, y + dirY, 0) && checkAlignment(x + dirX, y + dirY, dirX, dirY, 2, player) && !checkVal(x + 4 * dirX, y + 4 * dirY, player * -1) && !checkVal(x + -1 * dirX, y + -1 * dirY, player * -1)) {
                if (checkVal(x + 4 * dirX, y + 4 * dirY, player)) {
                    threes[player == -1 ? 0 : 1].active = false;
                } else {
                    threes[player == -1 ? 0 : 1].active = true;
                }
                threes[player == -1 ? 0 : 1].x1 = x;
                threes[player == -1 ? 0 : 1].y1 = y;
                threes[player == -1 ? 0 : 1].x2 = x + 4 * dirX;
                threes[player == -1 ? 0 : 1].y2 = y + 4 * dirY;
                threes[player == -1 ? 0 : 1].dirX = dirX;
                threes[player == -1 ? 0 : 1].dirY = dirY;
                threes[player == -1 ? 0 : 1].space = 1;
            } else if (checkVal(x + dirX, y + dirY, player) && checkVal(x + 2 * dirX, y + 2 * dirY, 0) && checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                if (checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                    threes[player == -1 ? 0 : 1].active = false;
                } else {
                    threes[player == -1 ? 0 : 1].active = true;
                }
                threes[player == -1 ? 0 : 1].x1 = x;
                threes[player == -1 ? 0 : 1].y1 = y;
                threes[player == -1 ? 0 : 1].x2 = x + 4 * dirX;
                threes[player == -1 ? 0 : 1].y2 = y + 4 * dirY;
                threes[player == -1 ? 0 : 1].dirX = dirX;
                threes[player == -1 ? 0 : 1].dirY = dirY;
                threes[player == -1 ? 0 : 1].space = 2;
            }
        }
    }
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

    activateThree(x, y, player);
    deactivateThree(x, y, player * -1);

    return true;
}

bool GomokuGame::checkVal(int x, int y, int val)
{
    if (x < 0 || y < 0 || x >= GOBAN_SIZE || y >= GOBAN_SIZE) {
        return false;
    }

    return (matrix[y][x] == val);
}

bool GomokuGame::checkWin(int x, int y, int player)
{
    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 4, player)) {
                return true;
            }
        }
    }

    return false;
}

bool GomokuGame::checkAlignment(int x, int y, int dirX, int dirY, int len, int player)
{
    for (int i = 1; i <= len; i++) {
        if (x + i * dirX < 0 || x + i * dirX >= GOBAN_SIZE || y + i * dirY < 0 || y + i * dirY >= GOBAN_SIZE) {
            return false;
        }

        if (matrix[y + i * dirY][x + i * dirX] != player) {
            return false;
        }
    }

    return true;
}

bool GomokuGame::checkTwoThrees(int x, int y, int player)
{
    int res = 0;
    int X_dir = 0;
    int Y_dir = 0;
    int x2 = -1;
    int y2 = -1;

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 2, player) && !checkVal(x + 3 * dirX, y + 3 * dirY, player * -1) && !checkVal(x + -1 * dirX, y + -1 * dirY, player * -1)) {
                res += 1;
                X_dir = dirX;
                Y_dir = dirY;
                x2 = x + 3 * dirX;
                y2 = y + 3 * dirY;
            } else if (checkVal(x + dirX, y + dirY, 0) && checkAlignment(x + dirX, y + dirY, dirX, dirY, 2, player) && !checkVal(x + 4 * dirX, y + 4 * dirY, player * -1) && !checkVal(x + -1 * dirX, y + -1 * dirY, player * -1)) {
                res += 1;
                X_dir = dirX;
                Y_dir = dirY;
                x2 = x + 4 * dirX;
                y2 = y + 4 * dirY;
            } else if (checkVal(x + dirX, y + dirY, player) && checkVal(x + 2 * dirX, y + 2 * dirY, 0) && checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                res += 1;
                X_dir = dirX;
                Y_dir = dirY;
                x2 = x + 3 * dirX;
                y2 = y + 3 * dirY;
            }
        }
    }

    if (res == 0) return false;
    if (res >= 2) return true;

    if (!threes[player == -1 ? 0 : 1].active) {
        return false;
    }

    if (threes[player == -1 ? 0 : 1].active &&
        threes[player == -1 ? 0 : 1].x1 == x &&
        threes[player == -1 ? 0 : 1].x2 == x2 &&
        threes[player == -1 ? 0 : 1].y1 == y &&
        threes[player == -1 ? 0 : 1].y2 == y2)
    {
        return false;
    } else {
        return true;
    }
}

bool GomokuGame::checkPairRule(int x, int y, int player)
{
    bool result = false;

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 2, player * -1) && checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                matrix[y + 1 * dirY][x + 1 * dirX] = 0;
                matrix[y + 2 * dirY][x + 2 * dirX] = 0;
                matrixChanged(x + 1 * dirX, y + 1 * dirY, 0);
                matrixChanged(x + 2 * dirX, y + 2 * dirY, 0);
                result = true;
            }
        }
    }

    return result;
}
