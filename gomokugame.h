#ifndef GOMOKUGAME_H
#define GOMOKUGAME_H

#include <QObject>
#include <vector>

#define GOBAN_SIZE 19

class GomokuGame : public QObject
{
    Q_OBJECT
public:
    explicit GomokuGame(QObject *parent = 0);
    ~GomokuGame();

    Q_INVOKABLE bool setMove(int x, int y, int player);
    Q_INVOKABLE int  getMove(int x, int y);

private:
    std::vector<std::vector<int>>   matrix;
    int                             current_player;
};

#endif // GOMOKUGAME_H
