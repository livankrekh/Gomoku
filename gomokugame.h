#ifndef GOMOKUGAME_H
#define GOMOKUGAME_H

#include <QObject>
#include <vector>
#include <cmath>

#define GOBAN_SIZE 19

class GomokuGame : public QObject
{
    Q_OBJECT
public:
    explicit GomokuGame();
    ~GomokuGame();

    Q_INVOKABLE bool setMove(int x, int y, int player);
    Q_INVOKABLE bool checkPairRule(int x, int y, int player);
    Q_INVOKABLE bool checkWin(int x, int y, int player);
    Q_INVOKABLE bool checkRules(int x, int y);

    bool checkVal(int x, int y, int val);
    bool checkAlignment(int x, int y, int dirX, int dirY, int len, int player);

signals:
    void matrixChanged(int x, int y, int val);

private:
    std::vector<std::vector<int>>   matrix;
    int                             current_player;
};

#endif // GOMOKUGAME_H
