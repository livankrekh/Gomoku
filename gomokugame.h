#ifndef GOMOKUGAME_H
#define GOMOKUGAME_H

#include <QObject>
#include <vector>
#include <cmath>
#include <iostream>

#define GOBAN_SIZE 19

struct Three
{
public:
    Three(int x, int y, int x_end, int y_end, int dir_X, int dir_Y);
    ~Three();

    int         x1;
    int         y1;
    int         x2;
    int         y2;
    int         dirX;
    int         dirY;
    int         space;
    bool        active;
};

class GomokuGame : public QObject
{
    Q_OBJECT
public:
    explicit GomokuGame();
    ~GomokuGame();

    Q_INVOKABLE bool setMove(int x, int y, int player);
    Q_INVOKABLE bool checkPairRule(int x, int y, int player);
    Q_INVOKABLE bool checkWin(int x, int y, int player);
    Q_INVOKABLE bool checkTwoThrees(int x, int y, int player);

    bool checkVal(int x, int y, int val);
    bool checkAlignment(int x, int y, int dirX, int dirY, int len, int player);
    void activateThree(int x, int y, int player);
    void deactivateThree(int x, int y, int player);

signals:
    void matrixChanged(int x, int y, int val);

private:
    std::vector<std::vector<int>>   matrix;
    std::vector<Three>              threes;
    int                             current_player;
};

#endif // GOMOKUGAME_H
