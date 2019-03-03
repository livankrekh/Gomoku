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

    all_variants ai_move;
    ai_move = _find_MF(player * -1, 5, 3, matrix);
    setMove(ai_move._x, ai_move._y, ai_move.num);
    matrixChanged(ai_move._x, ai_move._y, ai_move.num);

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

    std::cout << "Check threes - " << res << std::endl;
    std::cout << "x - " << x << ", x2 - " << x2 << ", y - " << y << ", y2 - " << y2 << std::endl;

    if (res == 0) return false;
    if (res >= 2) return true;

//    if (!threes[player == -1 ? 0 : 1].active) {
//        return false;
//    }

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

bool GomokuGame::checkRules(int x, int y, int player)
{
    return true;
//    return checkTwoThrees(x, y, player);
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

using namespace std;

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
void 					checkH(vector<int>  tmp, int now_player, vector<int> &how_many_nums);
void					iterate_all_variants(node *nde, vector<int> &how_many_nums);

void	most_best_variant(node *nde){
    int tmp_map, tmp_map_not_you, for_tmp;
    all_variants  tmpvar;
    for (int x = 0; x < nde->size; ++x)
        for (int y = 0; y < nde->size; ++y){
            tmp_map_not_you = nde->cross_map_not_you[x][y];
            tmp_map = nde->cross_map[x][y];
            // printf("%d %d\n",tmp_map, tmp_map_not_you );
            if (tmp_map != 0){
                for_tmp = 1;
                while(tmp_map-- > 1)
                    for_tmp *= 10;
                tmp_map = for_tmp;
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

node *	create_node(node *parent, int x, int y){
    node *child = new node;//create template width**depth

    child->map_in_node = parent->map_in_node;
    child->map_in_node[x][y] = parent->now_player;
    child->x = x;
    child->y = y;
    child->size = child->map_in_node.size();
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


void			return_heuristic(node *child, int START_PLAYER){

    size_t 	sum = 0;
    vector<int> how_many_nums(5);
    iterate_all_variants(child, how_many_nums);

    sum = how_many_nums[0] + how_many_nums[1] * 30 +
    how_many_nums[2]*300 + how_many_nums[3]*10000 +
    how_many_nums[4]*10000000;
    if (how_many_nums[4] != 0)
        child->win = true;
    child->heuristics = sum;
}
void	GomokuGame::make_childs(node *parent, int MAX_DEPTH ,int MAX_WIDTH, int START_PLAYER){
    if (parent->level_depth == MAX_DEPTH){
        return_heuristic(parent, START_PLAYER);
//         printf("return_heuristic %lu\n", parent->heuristics);
        return;
    }
//     printf("make_childs1\n");
    node *child_tmp;
    int width = MAX_WIDTH;
    row(parent, false);// if we have 2 free flangs its 2 point if 1 free flang - 1 point ?
    row(parent, true);
    column(parent, false);
    column(parent, true);
    diagonal_right_up(parent, false);
    diagonal_right_up(parent, true);
    diagonal_left_up(parent, false);
    diagonal_left_up(parent, true);
    most_best_variant(parent);

    for (int i = 0; i < parent->variants.size(); ++i){
        if (width > 0 and this->checkRules(parent->variants[i]._x, parent->variants[i]._y, parent->now_player)){
            child_tmp = create_node(parent, parent->variants[i]._x, parent->variants[i]._y);
            width--;
            // printf("\ndep = %d   x=%d y=%d\n\n", child_tmp->level_depth, parent->variants[i]._x, parent->variants[i]._y);
            // _print(child_tmp->map_in_node);
        }
    }

    for (int i = 0; i < parent->nodes.size(); ++i){
        // printf("child num %d create child-child\n", i);
        // printf("in parent childe num %d\n", i);
        // _print(parent->nodes[i]->map_in_node);
        make_childs(parent->nodes[i], MAX_DEPTH, MAX_WIDTH, START_PLAYER);
    }

    bool maximaze = true;
    int limit_tmp, tmpx, tmpy;
    int limit = parent->nodes[0]->heuristics;
    int x = parent->nodes[0]->x;
    int y = parent->nodes[0]->y;


    if (parent->now_player == START_PLAYER)
        maximaze = false;
    // printf("find maxH\n");
    for (int i = 0; i < parent->nodes.size(); ++i){
        // check have we heuristics
        limit_tmp = parent->nodes[i]->heuristics;
        tmpx = parent->nodes[i]->x;
        tmpy = parent->nodes[i]->y;
        if (maximaze){
            if (limit_tmp > limit){
                limit = limit_tmp;
                x = tmpx;
                y = tmpy;
            }
        }
        else{
            if (limit_tmp < limit){
                limit = limit_tmp;
                x = tmpx;
                y = tmpy;
            }
        }
    }
    // printf("limit %d %d %d\n", limit, x, y);
    parent->heuristics = limit;
    if (parent->level_depth == 0){
        parent->x = x;
        parent->y = y;
    }
}

all_variants	GomokuGame::_find_MF(int START_PLAYER, int MAX_DEPTH, int MAX_WIDTH, std::vector<std::vector<int>> MAP){
    int OTHER_PLAYER = -1*START_PLAYER;
    node *first_node = new node;

    first_node->parent = nullptr;
    first_node->map_in_node = MAP;//read_from_file();
    first_node->size = first_node->map_in_node.size();
    first_node->level_depth = 0;
    first_node->x = 0;
    first_node->y = 0;
    first_node->now_player = START_PLAYER;
    first_node->other_player = OTHER_PLAYER;
    first_node->win = false;
    first_node->cross_map = first_node->map_in_node;
    for (int x = 0; x < first_node->size; ++x)
        for (int y = 0; y < first_node->size; ++y)
            first_node->cross_map[x][y] = 0;
    first_node->cross_map_not_you = first_node->cross_map;
    printf("\nmakechild start\n");
    make_childs(first_node, MAX_DEPTH, MAX_WIDTH, START_PLAYER);
    all_variants ret;
    ret.num = START_PLAYER;
    ret._x = first_node->x;
    ret._y = first_node->y;
    printf("HER %lu\n", first_node->heuristics);
    printf("x:%d y:%d\n", first_node->x, first_node->y);
//    printf("AOjv isdhjfbvokhdfbohadoibjis\n");
    return ret;
}

void 	checkH(vector<int>  tmp, int now_player, vector<int> &how_many_nums){
    int num = 0;
    int left_i = -1;
    for (int i = 0; i < tmp.size(); ++i){
        if (tmp[i] == now_player){
            if (i > 0 and tmp[i-1] == 0)
                left_i = i-1;
            num++;
        }
        else
        {
            if(num and (tmp[i] == 0 or left_i != -1))
                how_many_nums[num - 1] += 1;
            left_i = -1;
            num = 0;
        }
    }

    if(num and left_i != -1)
        how_many_nums[num - 1] += 1;

}

vector<vector<int> > 	read_from_file()
{

    vector<vector<int> > the_vector;
    string s;
    string tmp_int;
    string tmp_s;

    ifstream file("map.txt");
    while(getline(file, s)){
        vector<int> tmp;
        tmp_s = "";
        for (int i = 0; i < s.size(); ++i)
        {
            if (s[i] != ' '){
                tmp_s += s[i];
            }
            else{
                tmp.push_back(stoi(tmp_s));
                tmp_s = "";
            }
        }
        tmp.push_back(stoi(tmp_s));
        tmp_s = "";
        the_vector.push_back(tmp);

    }

    file.close();
    return the_vector;
}

void 	_print(	vector<vector<int> > the_vector){
    int size;
    printf("\n");
    size = the_vector.size();
    for (int i = 0; i < size; ++i)
    {
        printf("%d| ", i);
        for (int i2 = 0; i2 < size; ++i2)
            printf("%4d", the_vector[i][i2]);
        printf("\n");
    }

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
                if (tmp[i] == 0)
                    _new[i] = num > _new[i] ? num : _new[i];
                if (left_i != -1)
                    _new[left_i] = num > _new[left_i] ? num : _new[left_i];
            }
            left_i = -1;
            num = 0;
        }
    }

    if(num and left_i != -1)
        _new[left_i] = num > _new[left_i] ? num : _new[left_i];
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
                if (tmp[i] == 0)
                    _new[i] = num > _new[i] ? num : _new[i];
                if (left_i != -1)
                    _new[left_i] = num > _new[left_i] ? num : _new[left_i];
            }
            left_i = -1;
            num = 0;
        }
    }

    if(num and left_i != -1)
        _new[left_i] = num > _new[left_i] ? num : _new[left_i];
    return _new;
}

void	iterate_all_variants(node *nde, vector<int> &how_many_nums){

    // column
    for (int y = 0; y < nde->size ; ++y)
    {
        vector<int> tmp;
        for (int x = 0; x < nde->size ; ++x)
            tmp.push_back(nde->map_in_node[x][y]);
        checkH(tmp, nde->now_player, how_many_nums);
    }
    // row
    for (int x = 0; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = 0; y < nde->size ; ++y)
            tmp.push_back(nde->map_in_node[x][y]);
        checkH(tmp, nde->now_player, how_many_nums);
    }
    //diagonal_right_up
    for (int x = 0; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = nde->size - 1 - x; y <= nde->size - 1; ++y)
            tmp.push_back(nde->map_in_node[x - (nde->size - 1 - y)][y]);
        checkH(tmp, nde->now_player, how_many_nums);

    }
    for (int x = 1; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = 0; y <= nde->size - 1 - x; ++y)
            tmp.push_back(nde->map_in_node[x+y][y]);
        checkH(tmp, nde->now_player, how_many_nums);
    }
    // diagonal_left_up
    for (int x = 0; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = x; y >= 0; --y)
            tmp.push_back(nde->map_in_node[x-y][y]);
        checkH(tmp, nde->now_player, how_many_nums);
    }
    for (int x = 1; x < nde->size ; ++x)
    {
        vector<int> tmp;
        for (int y = nde->size - 1; y >= x; --y)
            tmp.push_back(nde->map_in_node[nde->size - 1 + x - y][y]);
        checkH(tmp, nde->now_player, how_many_nums);
    }


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
            for (int y = x; y >= 0; --y, i++)
                nde->cross_map[x-y][y] = _new[i] > nde->cross_map[x-y][y] ? _new[i] : nde->cross_map[x-y][y];
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = x; y >= 0; --y, i++)
                nde->cross_map_not_you[x-y][y] = _new[i] > nde->cross_map_not_you[x-y][y] ? _new[i] : nde->cross_map_not_you[x-y][y];
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
            for (int y = nde->size - 1; y >= x; --y, i++)
                nde->cross_map[nde->size - 1 + x - y][y] = _new[i] > nde->cross_map[nde->size - 1 + x - y][y] ? _new[i] : nde->cross_map[nde->size - 1 + x - y][y];
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = nde->size - 1; y >= x; --y, i++)
                nde->cross_map_not_you[nde->size - 1 + x - y][y] = _new[i] > nde->cross_map_not_you[nde->size - 1 + x - y][y] ? _new[i] : nde->cross_map_not_you[nde->size - 1 + x - y][y];
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
            for (int y = nde->size - 1 - x; y <= nde->size - 1; ++y, i++)
                nde->cross_map[x - (nde->size - 1 - y)][y] = _new[i] > nde->cross_map[x - (nde->size - 1 - y)][y] ? _new[i] : nde->cross_map[x - (nde->size - 1 - y)][y];
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = nde->size - 1 - x; y <= nde->size - 1; ++y, i++)
                nde->cross_map_not_you[x - (nde->size - 1 - y)][y] = _new[i] > nde->cross_map_not_you[x - (nde->size - 1 - y)][y] ? _new[i] : nde->cross_map_not_you[x - (nde->size - 1 - y)][y];
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
            for (int y = 0; y <= nde->size - 1 - x; ++y, i++)
                nde->cross_map[x+y][y] = _new[i] > nde->cross_map[x+y][y] ? _new[i] : nde->cross_map[x+y][y];
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = 0; y <= nde->size - 1 - x; ++y, i++)
                nde->cross_map_not_you[x+y][y] = _new[i] > nde->cross_map_not_you[x+y][y] ? _new[i] : nde->cross_map_not_you[x+y][y];
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
            for (int y = 0; y < nde->size ; ++y, i++)
                nde->cross_map[x][y] = _new[i] > nde->cross_map[x][y] ? _new[i] : nde->cross_map[x][y];
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int y = 0; y < nde->size ; ++y, i++)
                nde->cross_map_not_you[x][y] = _new[i] > nde->cross_map_not_you[x][y] ? _new[i] : nde->cross_map_not_you[x][y];
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
            for (int x = 0; x < nde->size ; ++x, i++)
                nde->cross_map[x][y] = _new[i] > nde->cross_map[x][y] ? _new[i] : nde->cross_map[x][y];
        }
        else{
            _new = check_not_you(tmp, nde);
            for (int x = 0; x < nde->size ; ++x, i++)
                nde->cross_map_not_you[x][y] = _new[i] > nde->cross_map_not_you[x][y] ? _new[i] : nde->cross_map_not_you[x][y];
        }
    }
}
