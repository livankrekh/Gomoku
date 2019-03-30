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
# include <ctime>

#define GOBAN_SIZE 19

struct all_variants {
    size_t num, _x, _y;
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
    Three(int x, int y, int x_2, int y_2, int x_3, int y_3);
    ~Three();

    int         x1;
    int         y1;
    int         x2;
    int         y2;
    int         x3;
    int         y3;
    bool        active;
};

class GomokuGame : public QObject
{
    Q_OBJECT
public:
    explicit GomokuGame();
    ~GomokuGame();

    Q_PROPERTY(bool alphaBetaProoning READ getProoning WRITE setProoning)

    Q_INVOKABLE bool setMove(int x, int y, int player);
    Q_INVOKABLE bool checkPairRule(int x, int y, int player);
    Q_INVOKABLE int checkWin(int x, int y, int player);
    Q_INVOKABLE bool checkTwoThrees(int x, int y, int player);
    Q_INVOKABLE bool moveAI(int player);

    bool getProoning();
    void setProoning(const bool val);

    int countAlignment(int x, int y, int val, int dirX, int dirY);

    bool checkVal(int x, int y, int val);
    bool checkAlignment(int x, int y, int dirX, int dirY, int len, int player);
    bool checkRules(int x, int y, int player);
    int checkAlertRule(int x, int y, int val, int dirX, int dirY);
    void activateThree(int x, int y, int player);
    void deactivateThree(int x, int y, int player);

    // AI part

    int minimax(node *parent, int MAX_DEPTH ,int MAX_WIDTH, int AI_PLAYER, int alpha, int beta, bool maximizingPlayer, bool USE_OPTIMIZATION);
    all_variants _find_where_go(int AI_PLAYER, int MAX_DEPTH, int MAX_WIDTH, std::vector<std::vector<int> > map, bool USE_OPTIMIZATION);

signals:
    void matrixChanged(int x, int y, int val);
    void reactionChanged(double time);
    void winnerChecked(int player);

private:
    std::vector<std::vector<int>>   matrix;
    std::vector<Three>              threes;
    int                             current_player;
    int                             twoRuleCount[2];
    bool                            prooning;
};

#endif // GOMOKUGAME_H
