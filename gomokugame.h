#ifndef GOMOKUGAME_H
#define GOMOKUGAME_H

#include <QObject>
#include <cmath>
#include <iostream>
# include <vector>
# include <iostream>
# include <stdlib.h>
# include <string>
# include <fstream>

#define GOBAN_SIZE 19

struct all_variants {
    int num, _x, _y;
};

struct numS {
    int num_1;
    int num_2;
    int num_3;
    int num_4;
    int num_5;
};

typedef struct			node
{
    std::vector<std::vector<int> >	map_in_node;
    std::vector<std::vector<int> >	cross_map;
    std::vector<std::vector<int> >	cross_map_not_you;
    std::vector<all_variants>	variants;
    bool					win;
    size_t 					heuristics;
    int 					now_player;
    int 					other_player;
    int 					level_depth;
    int 					size;
    int						x;
    int						y;
    struct node*			parent;
    std::vector<struct node* >	nodes;
}	node;

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
    bool checkRules(int x, int y, int player);
    void activateThree(int x, int y, int player);
    void deactivateThree(int x, int y, int player);
    void make_childs(node *parent, int MAX_DEPTH ,int MAX_WIDTH, int START_PLAYER);

    all_variants _find_MF(int START_PLAYER, int MAX_DEPTH, int MAX_WIDTH, std::vector<std::vector<int>> MAP);

signals:
    void matrixChanged(int x, int y, int val);

private:
    std::vector<std::vector<int>>   matrix;
    std::vector<Three>              threes;
    int                             current_player;
};

#endif // GOMOKUGAME_H
