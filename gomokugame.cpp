#include "gomokugame.h"

Three::Three(int x, int y, int x_2, int y_2, int x_3, int y_3) : x1(x), y1(y), x2(x_2), y2(y_2), x3(x_3), y3(y_3), active(false)
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
    threes.push_back( Three(-1,-1,-1,-1,-1,-1) );
    threes.push_back( Three(-1,-1,-1,-1,-1,-1) );
    twoRuleCount[0] = 0;
    twoRuleCount[1] = 0;
    prooning = true;
}

GomokuGame::~GomokuGame()
{
}

bool GomokuGame::getProoning()
{
    return prooning;
}

void GomokuGame::setProoning(const bool val)
{
    prooning = val;
}

void GomokuGame::deactivateThree(int x, int y, int player)
{
    Three null(-1,-1,-1,-1,-1,-1);
    Three current = threes[player == -1 ? 0 : 1];
    Three other = threes[player * -1 == -1 ? 0 : 1];

    int currDirX1 = current.x2 - current.x1 == 0 ? 0 : (current.x2 - current.x1 / std::abs(current.x2 - current.x1));
    int currDirY1 = current.y2 - current.y1 == 0 ? 0 : (current.y2 - current.y1 / std::abs(current.y2 - current.y1));
    int currDirX3 = current.x3 - current.x2 == 0 ? 0 : (current.x3 - current.x2 / std::abs(current.x3 - current.x2));
    int currDirY3 = current.y3 - current.y2 == 0 ? 0 : (current.y3 - current.y2 / std::abs(current.y3 - current.y2));

    int otherDirX1 = other.x2 - other.x1 == 0 ? 0 : (other.x2 - other.x1 / std::abs(other.x2 - other.x1));
    int otherDirY1 = other.y2 - other.y1 == 0 ? 0 : (other.y2 - other.y1 / std::abs(other.y2 - other.y1));
    int otherDirX3 = other.x3 - other.x2 == 0 ? 0 : (other.x3 - other.x2 / std::abs(other.x3 - other.x2));
    int otherDirY3 = other.y3 - other.y2 == 0 ? 0 : (other.y3 - other.y2 / std::abs(other.y3 - other.y2));

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (x + dirX == current.x1 && y + dirY == current.y1 && currDirX1 == dirX && currDirY1 == dirY) {
                threes[player == -1 ? 0 : 1] = null;
            } else if (x + dirX == current.x3 && y + dirY == current.y3 && currDirX3 == dirX && currDirY3 == dirY) {
                threes[player == -1 ? 0 : 1] = null;
            }
            if (x + dirX == other.x1 && y + dirY == other.y1 && otherDirX1 == dirX && otherDirY1 == dirY) {
                threes[player * -1 == -1 ? 0 : 1] = null;
            } else if (x + dirX == other.x3 && y + dirY == other.y3 && otherDirX3 == dirX && otherDirY3 == dirY) {
                threes[player * -1 == -1 ? 0 : 1] = null;
            }
        }
    }
}

void GomokuGame::activateThree(int x, int y, int player)
{
    int res = 0;
    Three tmp(-1,-1,-1,-1,-1,-1);

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (countAlignment(x, y, player, dirX, dirY) >= 4) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 2, player)
                    && !checkVal(x + 3 * dirX, y + 3 * dirY, player * -1)
                    && !checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                res += 1;
                tmp.x1 = x;
                tmp.y1 = y;
                tmp.x2 = x + dirX;
                tmp.y2 = y + dirY;
                tmp.x3 = x + (dirX * 2);
                tmp.y3 = y + (dirY * 2);
            } else if (checkVal(x + dirX, y + dirY, player)
                       && checkVal(x + 2 * dirX, y + 2 * dirY, 0)
                       && checkVal(x + 3 * dirX, y + 3 * dirY, player)
                       && !checkVal(x + 4 * dirX, y + 4 * dirY, player * -1)
                       && !checkVal(x + 4 * dirX, y + 4 * dirY, player)) {
                res += 1;
                tmp.x1 = x;
                tmp.y1 = y;
                tmp.x2 = x + dirX;
                tmp.y2 = y + dirY;
                tmp.x3 = x + (dirX * 3);
                tmp.y3 = y + (dirY * 3);
            } else if (checkVal(x + dirX, y + dirY, 0) && checkVal(x + 2 * dirX, y + 2 * dirY, player) && checkVal(x + 3 * dirX, y + 3 * dirY, player) && !checkVal(x + 4 * dirX, y + 4 * dirY, player * -1) && !checkVal(x + 4 * dirX, y + 4 * dirY, player)) {
                res += 1;
                tmp.x1 = x;
                tmp.y1 = y;
                tmp.x2 = x + (dirX * 2);
                tmp.y2 = y + (dirY * 2);
                tmp.x3 = x + (dirX * 3);
                tmp.y3 = y + (dirY * 3);
            } else if (checkVal(x + dirX, y + dirY, player) && checkVal(x + dirX * -1, y + dirY * -1, player) && !checkVal(x + dirX * 2, y + dirY, player) && !checkVal(x + dirX * -2, y + dirY * -2, player * -1)) {
                res += 1;
                tmp.x1 = x + dirX;
                tmp.y1 = y + dirY;
                tmp.x2 = x;
                tmp.y2 = y;
                tmp.x3 = x + dirX * -1;
                tmp.y3 = y + dirY * -1;
            }
        }
    }

    if (res >= 2 || res == 0) {
        return ;
    }

    threes[player == -1 ? 0 : 1] = tmp;
    threes[player == -1 ? 0 : 1].active = true;
}

bool GomokuGame::setMove(int x, int y, int player)
{
    if (x + 1 > GOBAN_SIZE || y + 1 > GOBAN_SIZE || x < 0 || y < 0) {
        return false;
    }

    if (matrix[y][x] != 0) {
        return false;
    }

    if (checkTwoThrees(x, y, player)) {
        return false;
    }

    matrix[y][x] = player;

    matrixChanged(x, y, player);

    checkPairRule(x, y, player);

    deactivateThree(x, y, player);
    activateThree(x, y, player);

    if (checkWin(x, y, player) == 2) {
        winnerChecked(player);
    }

    return true;
}

bool GomokuGame::checkVal(int x, int y, int val)
{
    if (x < 0 || y < 0 || x >= GOBAN_SIZE || y >= GOBAN_SIZE) {
        return false;
    }

    return (matrix[y][x] == val);
}

bool GomokuGame::moveAI(int player)
{
    all_variants ai_move;

    clock_t begin = clock();
    ai_move = _find_where_go(player, 5, 3, matrix, prooning);
    clock_t end = clock();

    if (!setMove(ai_move._x, ai_move._y, player)) {
        return false;
    }

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    reactionChanged(elapsed_secs);

    return true;
}

int GomokuGame::countAlignment(int x, int y, int val, int dirX, int dirY)
{
    int res = 0;
    int i = 1;

    while (x + i * dirX < GOBAN_SIZE
           && y + i * dirY < GOBAN_SIZE
           && x + i * dirX >= 0
           && y + i * dirY >= 0
           && matrix[y + i * dirY][x + i * dirX] == val) {
        res++;
        i++;
    }
    i = 1;
    dirX *= -1;
    dirY *= -1;
    while (x + i * dirX < GOBAN_SIZE
           && y + i * dirY < GOBAN_SIZE
           && x + i * dirX >= 0
           && y + i * dirY >= 0
           && matrix[y + i * dirY][x + i * dirX] == val) {
        res++;
        i++;
    }

    return res;
}

int GomokuGame::checkAlertRule(int x, int y, int player, int dirX, int dirY)
{
    int i = 0;

    while (x + i * dirX < GOBAN_SIZE
           && y + i * dirY < GOBAN_SIZE
           && x + i * dirX >= 0
           && y + i * dirY >= 0
           && matrix[y + i * dirY][x + i * dirX] == player) {
        for (int dirYN = -1; dirYN <= 1; dirYN++) {
            for (int dirXN = -1; dirXN <= 1; dirXN++) {
                if ((dirYN == 0 && dirXN == 0)
                        || (dirXN == dirX && dirYN == dirY)
                        || (dirXN == dirX * -1 && dirYN == dirY * -1)) {
                    continue;
                }

                int currX = x + i * dirX;
                int currY = y + i * dirY;

                if (checkVal(currX + dirXN, currY + dirYN, player)
                        && (checkVal(currX + dirXN * 2, currY + dirYN * 3, player * -1) || checkVal(currX + dirXN * 2, currY + dirYN * 3, 0))
                        && (checkVal(currX + dirXN * 2, currY + dirYN * 2, player * -1) || checkVal(currX + dirXN * -1, currY + dirYN * -1, player * -1))) {
                    return i;
                }
            }
        }
        i++;
    }

    return i;
}

int GomokuGame::checkWin(int x, int y, int player)
{
    if (twoRuleCount[player == -1 ? 0 : 1] >= 5) {
        return 2;
    }

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 4, player) || countAlignment(x, y, player, dirX, dirY) >= 4) {
                if (checkAlertRule(x, y, player, dirX, dirY) + checkAlertRule(x, y, player, dirX * -1, dirY * -1) >= 6) {
                    return 2;
                } else {
                    return 1;
                }
            }
        }
    }

    return 0;
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
    Three tmp(-1,-1,-1,-1,-1,-1);
    Three current = threes[player == -1 ? 0 : 1];

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if (countAlignment(x, y, player, dirX, dirY) >= 4) {
                continue ;
            }

            if (checkAlignment(x, y, dirX, dirY, 2, player) && !checkVal(x + 3 * dirX, y + 3 * dirY, player * -1) && !checkVal(x + 3 * dirX, y + 3 * dirY, player)) {
                res += 1;
                tmp.x1 = x;
                tmp.y1 = y;
                tmp.x2 = x + dirX;
                tmp.y2 = y + dirY;
                tmp.x3 = x + (dirX * 2);
                tmp.y3 = y + (dirY * 2);
            } else if (checkVal(x + dirX, y + dirY, player) && checkVal(x + 2 * dirX, y + 2 * dirY, 0) && checkVal(x + 3 * dirX, y + 3 * dirY, player) && !checkVal(x + 4 * dirX, y + 4 * dirY, player * -1) && !checkVal(x + 4 * dirX, y + 4 * dirY, player)) {
                res += 1;
                tmp.x1 = x;
                tmp.y1 = y;
                tmp.x2 = x + dirX;
                tmp.y2 = y + dirY;
                tmp.x3 = x + (dirX * 3);
                tmp.y3 = y + (dirY * 3);
            } else if (checkVal(x + dirX, y + dirY, 0) && checkVal(x + 2 * dirX, y + 2 * dirY, player) && checkVal(x + 3 * dirX, y + 3 * dirY, player) && !checkVal(x + 4 * dirX, y + 4 * dirY, player * -1) && !checkVal(x + 4 * dirX, y + 4 * dirY, player)) {
                res += 1;
                tmp.x1 = x;
                tmp.y1 = y;
                tmp.x2 = x + (dirX * 2);
                tmp.y2 = y + (dirY * 2);
                tmp.x3 = x + (dirX * 3);
                tmp.y3 = y + (dirY * 3);
            }
        }
    }

    if (res >= 2) {
        return true;
    }
    if (res == 0 || !current.active) {
        return false;
    }

    for (int dirY = -1; dirY <= 1; dirY++) {
        for (int dirX = -1; dirX <= 1; dirX++) {
            if (dirY == 0 && dirX == 0) {
                continue ;
            }

            if ((x + dirX == current.x1 && y + dirY == current.y1)
                    || (x + dirX == current.x2 && y + dirY == current.y2)
                    || (x + dirX == current.x3 && y + dirY == current.y3)) {
                return false;
            }
        }
    }

    return true;
}

bool GomokuGame::checkRules(int x, int y, int player)
{
    return !checkTwoThrees(x, y, player);
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
                twoRuleCount[player == -1 ? 0 : 1] += 1;
                result = true;
            }
        }
    }

    return result;
}





// Algorithm part
// Decision three with min-max algorithm







using namespace std;

size_t NUM_NODE;

bool compare_variants(all_variants i1, all_variants i2)
{
    return (i1.num > i2.num);
}


vector<vector<int> >	read_from_file();
vector<int>				check_not_you(vector<int>  tmp, node *now_node);
vector<int>				check(vector<int>  tmp, node *now_node);
void					_print(	vector<vector<int> > the_vector);
void					diagonal_left_up(node *nde, bool you);
void					diagonal_right_up(node *nde, bool you);
void					row(node *nde, bool you);
void					column(node *nde, bool you);
void					make_cross_map(node *parent);

void	most_best_variant(node *nde){
    int tmp_map, tmp_map_not_you, for_tmp;
    all_variants  tmpvar;
    for (int x = 0; x < nde->size; ++x)
        for (int y = 0; y < nde->size; ++y){
            tmp_map_not_you = nde->cross_map_not_you[x][y];
            tmp_map = nde->cross_map[x][y];
            // printf("x:%d y:%d %d %d\n",x, y,tmp_map, tmp_map_not_you );
            if (tmp_map != 0){
                for_tmp = 1;
                while(tmp_map-- > 1)
                    for_tmp *= 10;
                tmp_map = for_tmp + 1;
            }
            if (tmp_map_not_you != 0){
                for_tmp = 1;
                while(tmp_map_not_you-- > 1)
                    for_tmp *= 10;
                tmp_map_not_you = for_tmp;
            }
            if ((tmp_map + tmp_map_not_you) > 0){
                tmpvar.num = tmp_map + tmp_map_not_you;
                tmpvar._x = x;
                tmpvar._y = y;
                nde->variants.push_back(tmpvar);
            }
            nde->cross_map[x][y] = tmp_map + tmp_map_not_you;
        }

    sort(nde->variants.begin(), nde->variants.end(), compare_variants);
}

bool	checkRules(int x, int y, int player){
    return true;
}

node *	create_node(node *parent, int x, int y){
    node *child = new node;//create template width**depth
    child->map_in_node = parent->map_in_node;
    child->map_in_node[x][y] = parent->now_player;
    child->size = child->map_in_node.size();
    child->x = x;
    child->y = y;

    if (y > 2 and child->map_in_node[x][y - 1] != 0 and child->map_in_node[x][y - 1] == child->map_in_node[x][y - 2] and child->map_in_node[x][y - 2] != parent->now_player and child->map_in_node[x][y - 3] == parent->now_player){
        child->map_in_node[x][y-1] = 0;
        child->map_in_node[x][y-2] = 0;
    }//left
    if (y < (child->size - 3) and child->map_in_node[x][y + 1] != 0 and child->map_in_node[x][y + 1] == child->map_in_node[x][y + 2] and child->map_in_node[x][y + 2] != parent->now_player and child->map_in_node[x][y + 3] == parent->now_player){
        child->map_in_node[x][y+1] = 0;
        child->map_in_node[x][y+2] = 0;
    }//right
    if (x > 2 and child->map_in_node[x-1][y] != 0 and child->map_in_node[x-1][y] == child->map_in_node[x-2][y] and child->map_in_node[x-2][y] != parent->now_player and child->map_in_node[x-3][y] == parent->now_player){
        child->map_in_node[x-1][y] = 0;
        child->map_in_node[x-2][y] = 0;
    }//up
    if (x < (child->size - 3) and child->map_in_node[x+1][y] != 0 and child->map_in_node[x+1][y] == child->map_in_node[x+2][y] and child->map_in_node[x+2][y] != parent->now_player and child->map_in_node[x+3][y] == parent->now_player){
        child->map_in_node[x+1][y] = 0;
        child->map_in_node[x+2][y] = 0;
    }//right

    if (x > 2 and y > 2 and child->map_in_node[x-1][y-1] != 0 and child->map_in_node[x-1][y-1] == child->map_in_node[x-2][y-2] and child->map_in_node[x-2][y-2] != parent->now_player and child->map_in_node[x-3][y-3] == parent->now_player){
        child->map_in_node[x-1][y-1] = 0;
        child->map_in_node[x-2][y-2] = 0;
    }//left_up


    if (x < (child->size - 3) and y < (child->size - 3) and child->map_in_node[x+1][y+1] != 0 and child->map_in_node[x+1][y+1] == child->map_in_node[x+2][y+2] and child->map_in_node[x+2][y+2] != parent->now_player and child->map_in_node[x+3][y+3] == parent->now_player){
        child->map_in_node[x+1][y+1] = 0;
        child->map_in_node[x+2][y+2] = 0;
    }//right down


    if (x > 2 and y < (child->size - 3) and child->map_in_node[x-1][y+1] != 0 and child->map_in_node[x-1][y+1] == child->map_in_node[x-2][y+2] and child->map_in_node[x-2][y+2] != parent->now_player and child->map_in_node[x-3][y+3] == parent->now_player){
        child->map_in_node[x-1][y+1] = 0;
        child->map_in_node[x-2][y+2] = 0;
    }//right_up


    if (y > 2 and x < (child->size - 3) and child->map_in_node[x+1][y-1] != 0 and child->map_in_node[x+1][y-1] == child->map_in_node[x+2][y-2] and child->map_in_node[x+2][y-2] != parent->now_player and child->map_in_node[x+3][y-3] == parent->now_player){
        child->map_in_node[x+1][y-1] = 0;
        child->map_in_node[x+2][y-2] = 0;
    }//left down


    child->now_player = parent->other_player;
    child->other_player = parent->now_player;
    child->cross_map = child->map_in_node;
    child->level_depth = parent->level_depth + 1;
    child->win = false;
    for (int x = 0; x < child->size; ++x)
        for (int y = 0; y < child->size; ++y)
            child->cross_map[x][y] = 0;
    child->cross_map_not_you = child->cross_map;
    child->parent = parent;
    parent->nodes.push_back(child);
    return child;
}

int			return_heuristic(node *child, int AI_PLAYER){

    int 	sum = 0;
    int 	tmp_ai = 0;
    int 	tmp_rival = 0;
    vector<int> AI_player(5);
    vector<int> rival_player(5);
    for (int i = 0; i < 5; ++i)
    {
        AI_player[i] = 0;
        rival_player[i] = 0;
    }
    for (int i = 0; i < child->cross_map.size(); ++i){
        for (int j = 0; j < child->cross_map.size(); ++j){
            if (child->now_player == AI_PLAYER){
                tmp_ai = child->cross_map[i][j];
                tmp_rival = child->cross_map_not_you[i][j];
            }
            else{
                tmp_ai = child->cross_map_not_you[i][j];
                tmp_rival = child->cross_map[i][j];
            }
            if (tmp_ai > 0){
                if (tmp_ai > 5)
                    AI_player[4] += 1;
                else
                    AI_player[tmp_ai - 1] += 1;

            }
            if (tmp_rival > 0){
                if (tmp_rival > 5)
                    rival_player[4] += 1;
                else
                    rival_player[tmp_rival - 1] += 1;
            }
        }
    }
    for (int i = 0; i < 5; ++i)
        AI_player[i] = AI_player[i] - rival_player[i];
    sum = AI_player[0] +
    AI_player[1] * 15 +
    AI_player[2] * 300 +
    AI_player[3] * 6000 +
    AI_player[4] * 1000000;
    if (child->now_player == AI_PLAYER){
        if (AI_player[4] > 0)
            child->win = true;
    }
    else{
        if (AI_player[4] < 0)
            child->win = true;
    }
    child->heuristics = sum;
    // printf("heuristics:%d\n", sum);
    return sum;
}

int this_win_finally(int x,int y, int AI_PLAYER, int *_x_cap , int *_y_cap){

    return 0;
    // if (x == 4 and y == 2 and AI_PLAYER == 1){
    // 	(*_x_cap) = 4;
    // 	(*_y_cap) = 1;
    // 	return 2;
    // }
    // return 0;
}
void	make_cross_map(node *parent){
    row(parent, false);// if we have 2 free flangs its 2 point if 1 free flang - 1 point ?
    row(parent, true);
    column(parent, false);
    column(parent, true);
    diagonal_right_up(parent, false);
    diagonal_right_up(parent, true);
    diagonal_left_up(parent, false);
    diagonal_left_up(parent, true);
}

int 	choose_best_child(node *parent, bool maximizingPlayer)
{
    int tmp_heur, tmpx, tmpy;
    int heur = parent->nodes[0]->heuristics;
    int x = parent->nodes[0]->x;
    int y = parent->nodes[0]->y;


    for (int i = 0; i < parent->nodes.size(); ++i){
        tmp_heur = parent->nodes[i]->heuristics;
        tmpx = parent->nodes[i]->x;
        tmpy = parent->nodes[i]->y;
        if (maximizingPlayer){
            if (tmp_heur > heur){
                heur = tmp_heur;
                x = tmpx;
                y = tmpy;
            }
        }
        else{
            if (tmp_heur < heur){
                heur = tmp_heur;
                x = tmpx;
                y = tmpy;
            }
        }
    }
    // printf("heur %d %d %d\n", heur, x, y);
    parent->heuristics = heur;
    if (parent->level_depth == 0){
        parent->x = x;
        parent->y = y;
    }
    return heur;
}

int GomokuGame::minimax(node *parent, int MAX_DEPTH ,int MAX_WIDTH, int AI_PLAYER, int alpha, int beta, bool maximizingPlayer, bool USE_OPTIMIZATION)
{
    node	*child_tmp;
    int		width = MAX_WIDTH;
    int		child_num = 0;
    int		value = 2147483000;
    int		result;
    if (maximizingPlayer)
        value = -2147483000;

    make_cross_map(parent);

    NUM_NODE += 1;
    if (parent->level_depth == MAX_DEPTH)
        return return_heuristic(parent, AI_PLAYER);

    most_best_variant(parent);
    for (int i = 0; i < parent->variants.size(); ++i){
        if (width > 0 and checkRules(parent->variants[i]._y, parent->variants[i]._x, parent->now_player)){
            int		_x_cap = -1;
            int		_y_cap = -1;
//            int		res = this_win_finally(parent->variants[i]._x, parent->variants[i]._y, parent->now_player, &_x_cap, &_y_cap);

            if (checkWin(parent->variants[i]._y, parent->variants[i]._x, parent->now_player)){
                if (maximizingPlayer)
                    parent->heuristics = 200000000;
                else
                    parent->heuristics = -200000000;
                parent->x = parent->variants[i]._x;
                parent->y = parent->variants[i]._y;
                return parent->heuristics;
            }
            else {
                child_tmp = create_node(parent, parent->variants[i]._x, parent->variants[i]._y);
                width--;
                /*if (res == 2){
                    all_variants  tmpvar;
                    tmpvar.num = 10000000000;
                    tmpvar._x = _x_cap;
                    tmpvar._y = _y_cap;
                    child_tmp->variants.push_back(tmpvar);
                    printf("next must be   x:%d y:%x\n", _x_cap, _y_cap);
                }*/
                result = minimax(parent->nodes[child_num], MAX_DEPTH, MAX_WIDTH, AI_PLAYER, alpha, beta, !maximizingPlayer, USE_OPTIMIZATION);
                child_num += 1;

                if (maximizingPlayer){
                    value = max(value, result);
                    alpha = max(alpha, value);
                }
                else{
                    value = min(value, result);
                    beta = min(beta, value);
                }
                if (USE_OPTIMIZATION)
                    if (alpha >= beta)
                        break;
            }
        }
    }

    if (parent->nodes.size() <= 0)
        return parent->heuristics;
    return choose_best_child(parent, maximizingPlayer);
}

void	free_nodes(node *parent)
{
    if (parent->nodes.size() <= 0)
        return;
    for (int i = 0; i < parent->nodes.size(); ++i){
        free_nodes(parent->nodes[i]);
        delete parent->nodes[i];
    }
}

all_variants GomokuGame::_find_where_go(int AI_PLAYER, int MAX_DEPTH, int MAX_WIDTH, vector<vector<int> > map, bool USE_OPTIMIZATION){
    int 	OTHER_PLAYER = 2;
    int 	alpha = -2147483000;
    int 	beta = 2147483000;

    node *first_node = new node;
    first_node->parent = nullptr;
    first_node->map_in_node = map;
    first_node->size = first_node->map_in_node.size();
    first_node->level_depth = 0;
    first_node->x = -1;
    first_node->y = -1;
    first_node->now_player = AI_PLAYER;
    first_node->other_player = OTHER_PLAYER;
    first_node->win = false;
    first_node->cross_map = first_node->map_in_node;
    for (int x = 0; x < first_node->size; ++x)
        for (int y = 0; y < first_node->size; ++y)
            first_node->cross_map[x][y] = 0;
    first_node->cross_map_not_you = first_node->cross_map;
    minimax(first_node, MAX_DEPTH, MAX_WIDTH, AI_PLAYER, alpha, beta, true, USE_OPTIMIZATION);

    all_variants  result;
    result.num = NUM_NODE;
    result._x = first_node->y;
    result._y = first_node->x;

    free_nodes(first_node);
    delete first_node;

    return result;
}

vector<int>	check_not_you(vector<int>  tmp, node *now_node){
    int num = 0;
    int left_i = -1;
    vector<int>  _new(tmp.size());


    for (int i = 0; i < tmp.size(); ++i){
        if (tmp[i] != 0 and tmp[i] != now_node->now_player)
        {
            if (i > 0 and tmp[i-1] == 0)
                left_i = i-1;
            num++;
        }
        else
        {
            if(num)
            {
                if (tmp[i] == 0){
                    _new[i] = num;
                    if (i > 2 and left_i == -1 and num == 2){//need for -1 1 1 0, if we have flang not our
                        _new[i] = -3;
                    }
                }
                if (left_i != -1){
                    _new[left_i] = num;
                    if (tmp[i] != 0 and tmp[i] == now_node->now_player and num == 2){//need for 0 1 1 -1, if we have flang not our
                        _new[left_i] = -3;
                    }
                }
            }
            left_i = -1;
            num = 0;
        }
    }
    if(num and left_i != -1)
        _new[left_i] = num;
    return _new;
}

vector<int>	check(vector<int>  tmp, node *now_node){
    int num = 0;
    int left_i = -1;
    vector<int>  _new(tmp.size());

    for (int i = 0; i < tmp.size(); ++i){
        if (tmp[i] == now_node->now_player)
        {
            if (i > 0 and tmp[i-1] == 0)
                left_i = i-1;
            num++;
        }
        else
        {
            if(num)
            {
                if (tmp[i] == 0){
                    _new[i] = num;
                    if (i > 2 and left_i == -1 and num == 2){//need for -1 1 1 0, if we have flang not our
                        _new[i] = -3;
                    }
                }
                if (left_i != -1){
                    _new[left_i] = num;
                    if (tmp[i] != 0 and tmp[i] != now_node->now_player and num == 2){
                        _new[left_i] = -3;
                    }
                }
            }
            left_i = -1;
            num = 0;
        }
    }

    if(num and left_i != -1){
        _new[left_i] = num;
    }
    // for (int i = 0; i < tmp.size(); ++i)
    // {
    // 	printf("%d ", _new[i]);
    // }
    // printf("\n");
    return _new;
}


void	diagonal_left_up(node *nde, bool you){
    vector<int>  _new;
    int i;
    for (int x = 0; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = x; y >= 0; --y)
            tmp.push_back(nde->map_in_node[x-y][y]);
        i = 0;
        if (you){
            _new = check(tmp, nde);

            for (int y = x; y >= 0; --y, i++){
                if (_new[i] == -3){
                    nde->cross_map_not_you[x-y][y] = -_new[i] > nde->cross_map_not_you[x-y][y] ? -_new[i] : nde->cross_map_not_you[x-y][y];
                    _new[i] = 2;
                }
                nde->cross_map[x-y][y] = _new[i] > nde->cross_map[x-y][y] ? _new[i] : nde->cross_map[x-y][y];
            }
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = x; y >= 0; --y, i++){
                if (_new[i] == -3){
                    nde->cross_map[x-y][y] = -_new[i] > nde->cross_map[x-y][y] ? -_new[i] : nde->cross_map[x-y][y];
                    _new[i] = 2;
                }
                nde->cross_map_not_you[x-y][y] = _new[i] > nde->cross_map_not_you[x-y][y] ? _new[i] : nde->cross_map_not_you[x-y][y];
            }
        }
    }

    for (int x = 1; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = nde->size - 1; y >= x; --y)
            tmp.push_back(nde->map_in_node[nde->size - 1 + x - y][y]);

        i = 0;
        if (you){
            _new = check(tmp, nde);
            for (int y = nde->size - 1; y >= x; --y, i++){
                if (_new[i] == -3){
                    nde->cross_map_not_you[nde->size - 1 + x - y][y] = -_new[i] > nde->cross_map_not_you[nde->size - 1 + x - y][y] ? -_new[i] : nde->cross_map_not_you[nde->size - 1 + x - y][y];
                    _new[i] = 2;
                }
                nde->cross_map[nde->size - 1 + x - y][y] = _new[i] > nde->cross_map[nde->size - 1 + x - y][y] ? _new[i] : nde->cross_map[nde->size - 1 + x - y][y];
            }
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = nde->size - 1; y >= x; --y, i++){
                if (_new[i] == -3){
                    nde->cross_map[nde->size - 1 + x - y][y] = -_new[i] > nde->cross_map[nde->size - 1 + x - y][y] ? -_new[i] : nde->cross_map[nde->size - 1 + x - y][y];
                    _new[i] = 2;
                }
                nde->cross_map_not_you[nde->size - 1 + x - y][y] = _new[i] > nde->cross_map_not_you[nde->size - 1 + x - y][y] ? _new[i] : nde->cross_map_not_you[nde->size - 1 + x - y][y];
            }
        }

    }
}


void	diagonal_right_up(node *nde, bool you){
    vector<int>  _new;
    int i;
    for (int x = 0; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = nde->size - 1 - x; y <= nde->size - 1; ++y)
            tmp.push_back(nde->map_in_node[x - (nde->size - 1 - y)][y]);

        i = 0;
        if (you){
            _new = check(tmp, nde);
            for (int y = nde->size - 1 - x; y <= nde->size - 1; ++y, i++){
                if (_new[i] == -3){
                    nde->cross_map_not_you[x - (nde->size - 1 - y)][y] = -_new[i] > nde->cross_map_not_you[x - (nde->size - 1 - y)][y] ? -_new[i] : nde->cross_map_not_you[x - (nde->size - 1 - y)][y];
                    _new[i] = 2;
                }
                nde->cross_map[x - (nde->size - 1 - y)][y] = _new[i] > nde->cross_map[x - (nde->size - 1 - y)][y] ? _new[i] : nde->cross_map[x - (nde->size - 1 - y)][y];
            }
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = nde->size - 1 - x; y <= nde->size - 1; ++y, i++){
                if (_new[i] == -3){
                    nde->cross_map[x - (nde->size - 1 - y)][y] = -_new[i] > nde->cross_map[x - (nde->size - 1 - y)][y] ? -_new[i] : nde->cross_map[x - (nde->size - 1 - y)][y];
                    _new[i] = 2;
                }
                nde->cross_map_not_you[x - (nde->size - 1 - y)][y] = _new[i] > nde->cross_map_not_you[x - (nde->size - 1 - y)][y] ? _new[i] : nde->cross_map_not_you[x - (nde->size - 1 - y)][y];
            }
        }

    }
    for (int x = 1; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = 0; y <= nde->size - 1 - x; ++y)
            tmp.push_back(nde->map_in_node[x+y][y]);
        i = 0;
        if (you){
            _new = check(tmp, nde);
            for (int y = 0; y <= nde->size - 1 - x; ++y, i++){
                if (_new[i] == -3){
                    nde->cross_map_not_you[x+y][y] = -_new[i] > nde->cross_map_not_you[x+y][y] ? -_new[i] : nde->cross_map_not_you[x+y][y];
                    _new[i] = 2;
                }
                nde->cross_map[x+y][y] = _new[i] > nde->cross_map[x+y][y] ? _new[i] : nde->cross_map[x+y][y];
            }
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = 0; y <= nde->size - 1 - x; ++y, i++){
                if (_new[i] == -3){
                    nde->cross_map[x+y][y] = -_new[i] > nde->cross_map[x+y][y] ? -_new[i] : nde->cross_map[x+y][y];
                    _new[i] = 2;
                }
                nde->cross_map_not_you[x+y][y] = _new[i] > nde->cross_map_not_you[x+y][y] ? _new[i] : nde->cross_map_not_you[x+y][y];
            }
        }
    }
}


void	row(node *nde, bool you){
    vector<int>  _new;
    int i;
    for (int x = 0; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = 0; y < nde->size ; ++y)
            tmp.push_back(nde->map_in_node[x][y]);

        i = 0;
        if (you){
            _new = check(tmp, nde);
            for (int y = 0; y < nde->size ; ++y, i++){
                if (_new[i] == -3){
                    nde->cross_map_not_you[x][y] = -_new[i] > nde->cross_map_not_you[x][y] ? -_new[i] : nde->cross_map_not_you[x][y];
                    _new[i] = 2;
                }
                nde->cross_map[x][y] = _new[i] > nde->cross_map[x][y] ? _new[i] : nde->cross_map[x][y];
            }
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = 0; y < nde->size ; ++y, i++){
                if (_new[i] == -3){
                    nde->cross_map[x][y] = -_new[i] > nde->cross_map[x][y] ? -_new[i] : nde->cross_map[x][y];
                    _new[i] = 2;
                }
                nde->cross_map_not_you[x][y] = _new[i] > nde->cross_map_not_you[x][y] ? _new[i] : nde->cross_map_not_you[x][y];
            }
        }

    }
}


void	column(node *nde, bool you){
    vector<int>  _new;
    int i;
    for (int y = 0; y < nde->size ; ++y)
    {
        vector<int> tmp;
        for (int x = 0; x < nde->size ; ++x)
            tmp.push_back(nde->map_in_node[x][y]);
        i = 0;
        if (you){
            _new = check(tmp, nde);
            for (int x = 0; x < nde->size ; ++x, i++){
                if (_new[i] == -3){
                    nde->cross_map_not_you[x][y] = -_new[i] > nde->cross_map_not_you[x][y] ? -_new[i] : nde->cross_map_not_you[x][y];
                    _new[i] = 2;
                }
                nde->cross_map[x][y] = _new[i] > nde->cross_map[x][y] ? _new[i] : nde->cross_map[x][y];
            }
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int x = 0; x < nde->size ; ++x, i++){
                if (_new[i] == -3){
                    nde->cross_map[x][y] = -_new[i] > nde->cross_map[x][y] ? -_new[i] : nde->cross_map[x][y];
                    _new[i] = 2;
                }
                nde->cross_map_not_you[x][y] = _new[i] > nde->cross_map_not_you[x][y] ? _new[i] : nde->cross_map_not_you[x][y];
            }
        }
    }
}
