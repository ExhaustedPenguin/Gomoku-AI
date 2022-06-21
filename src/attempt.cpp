#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include<vector>
#include<set>

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

#define TIMEOUT 10
#define INFINITY 10000000
#define SIZE 15

struct Point {
    int x, y;
    Point() : Point(0, 0) {}
    Point(float x, float y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};

class GomokuBoard {
public:
    int board[15][15];
    std::set<std::pair<int, int>> blackpos;
    std::set<std::pair<int, int>> whitepos;
    int empty_count;
    int cur_player;
    int thisplayer;
    Point nextstep=Point(5,4);
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        if (p.x < 0)
            return false;
        if (p.x >= SIZE) {
            return false;
        }
        if (p.y < 0)
            return false;
        if (p.y >= SIZE)
            return false;
        return true;
    }
    int get_disc(Point p) const {
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (!is_spot_on_board(center)) {
            return false;
        }
        if (get_disc(center) != EMPTY)
            return false;
        return true;
    }

public:
    GomokuBoard() {
        reset();
    }
    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        cur_player = BLACK;
        empty_count = SIZE * SIZE;
        thisplayer = BLACK;
    }
    bool put_disc(Point p) {
        if (!is_spot_valid(p)) {
            return false;
        }
        set_disc(p, cur_player);
        empty_count--;
        // Check Win
        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        return true;
    }
    int count_value(GomokuBoard state, int cur) {
        int consec;
        int value = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (state.board[i][j] == cur) {
                    consec++;
                }
                else if(state.board[i][j]==EMPTY){
                    switch(consec) {
                        case 4:
                            value += 100;
                            break;
                        case 3:
                            value += 40;
                            break;
                        case 2:
                            value += 10;
                            break;
                        case 1:
                            value += 2;
                            break;
                        case 0:
                            break;
                        default:
                            value += INFINITY;
                    }
                    consec = 0;
                }
                else if (state.board[i][j] == get_next_player(cur)) {
                    switch (consec) {
                    case 4:
                        value += 70;
                        break;
                    case 3:
                        value += 30;
                        break;
                    case 2:
                        value += 5;
                        break;
                    case 1:
                        value += 1;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                    consec = 0;
                }
            }
        }

        for (int j = 0; j < SIZE; j++) {
            for (int i = 0; i < SIZE; i++) {
                if (state.board[i][j] == cur) {
                    consec++;
                }
                else if (state.board[i][j] == EMPTY) {
                    switch (consec) {
                    case 4:
                        value += 100;
                        break;
                    case 3:
                        value += 40;
                        break;
                    case 2:
                        value += 10;
                        break;
                    case 1:
                        value += 2;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                    consec = 0;
                }
                else if (state.board[i][j] == get_next_player(cur)) {
                    switch (consec) {
                    case 4:
                        value += 70;
                        break;
                    case 3:
                        value += 30;
                        break;
                    case 2:
                        value += 5;
                        break;
                    case 1:
                        value += 1;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                    consec = 0;
                }
            }
        }

        for (int k = 0; k <= 2 * (SIZE - 1); k++) {
            int ibegin = 0, iend=SIZE-1;
            if (ibegin < k - SIZE + 1) {
                ibegin = k - SIZE + 1;
            }
            if (iend > k) {
                iend = k;
            }
            for (int i = ibegin; i <= iend; i++) {
                int j = k - i;

                if (state.board[i][j] == cur) {
                    consec++;
                }
                else if (state.board[i][j] == 0) {
                    switch (consec) {
                    case 4:
                        value += 100;
                        break;
                    case 3:
                        value += 40;
                        break;
                    case 2:
                        value += 10;
                        break;
                    case 1:
                        value += 2;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                }
                else if (state.board[i][j] == get_next_player(cur)) {
                    switch (consec) {
                    case 4:
                        value += 70;
                        break;
                    case 3:
                        value += 30;
                        break;
                    case 2:
                        value += 5;
                        break;
                    case 1:
                        value += 1;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                    consec = 0;
                }
            }
        }

        for (int k = 1-SIZE; k < SIZE; k++) {
            int ibegin = 0, iend = SIZE+k-1;
            if (ibegin < k) {
                ibegin = k;
            }
            if (iend > SIZE-1) {
                iend = SIZE-1;
            }
            for (int i = ibegin; i <= iend; i++) {
                int j = i-k;

                if (state.board[i][j] == cur) {
                    consec++;
                }
                else if (state.board[i][j] == 0) {
                    switch (consec) {
                    case 4:
                        value += 100;
                        break;
                    case 3:
                        value += 40;
                        break;
                    case 2:
                        value += 10;
                        break;
                    case 1:
                        value += 2;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                }
                else if (state.board[i][j] == get_next_player(cur)) {
                    switch (consec) {
                    case 4:
                        value += 70;
                        break;
                    case 3:
                        value += 30;
                        break;
                    case 2:
                        value += 5;
                        break;
                    case 1:
                        value += 1;
                        break;
                    case 0:
                        break;
                    default:
                        value += INFINITY;
                    }
                    consec = 0;
                }
            }

        }
        return value;
    }


    void read_board(std::ifstream& fin) {
        fin >> thisplayer;
        int temp;
        int black=0, white=0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                fin >> temp;
                board[i][j] = temp;
                if (temp == BLACK) {
                    blackpos.insert(std::make_pair(i, j));
                    black++;
                    empty_count--;
                }
                else if (temp == WHITE) {
                    whitepos.insert(std::make_pair(i, j));
                    white++;
                    empty_count--;
                }
            }
        }
        if (black > white) {
            thisplayer = WHITE;
        }
        else if (black == white) {
            thisplayer = BLACK;
        }
        else {
            thisplayer = 10000000;
        }
    }

    void next_step() {
        if (empty_count == SIZE * SIZE) {
            cur_player = BLACK;
            nextstep=Point(7,7);
        }
        else if (empty_count == (SIZE * SIZE) - 1) {
            cur_player = WHITE;
            auto it = blackpos.begin();
            Point p = Point((*it).first, (*it).second);
            if (put_disc(Point(p.x + 1, p.y))) {
                nextstep= Point(p.x + 1, p.y);
            }
            else if (put_disc(Point(p.x, p.y + 1))) {
                nextstep= Point(p.x, p.y + 1);
            }
            else if (put_disc(Point(p.x - 1, p.y))) {
                nextstep= Point(p.x - 1, p.y);
            }
            else if (put_disc(Point(p.x, p.y + 1))) {
                nextstep= Point(p.x, p.y + 1);
            }
            else if (put_disc(Point(p.x+1, p.y + 1))) {
                nextstep = Point(p.x+1, p.y + 1);
            }
            else if (put_disc(Point(p.x-1, p.y + 1))) {
                nextstep = Point(p.x-1, p.y + 1);
            }
            else if (put_disc(Point(p.x+1, p.y - 1))) {
                nextstep = Point(p.x+1, p.y - 1);
            }
            else if (put_disc(Point(p.x-1, p.y - 1))) {
                nextstep = Point(p.x-1, p.y - 1);
            }
        }
        Minimax(*this, 4, true);
        return ;
    }
        
    int Minimax(GomokuBoard state, int depth, bool isMax) {
        std::set<std::pair<int, int>> poscopy;
        if (thisplayer == BLACK) {
            poscopy = blackpos;
        }
        else {
            poscopy = whitepos;
        }
        int blackval, whiteval;
        int value;
        if (depth == 0) {
            blackval = count_value(state, BLACK);
            whiteval = count_value(state, WHITE);

            if (thisplayer == BLACK) {
                value = blackval - whiteval;
            }
            else {
                value = whiteval - blackval;
            }
            return value;
        }
        if (isMax) {
            value = 0 - INFINITY;
            int temp;
            while (!poscopy.empty()) {
                auto it = poscopy.begin();
                Point p = Point((*it).first, (*it).second);
                GomokuBoard tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x + 1, p.y))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x - 1, p.y))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x - 1, p.y);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x, p.y+1))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x, p.y + 1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x + 1, p.y-1))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y-1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x + 1, p.y+1))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y+1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x + 1, p.y-1))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y-1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x - 1, p.y - 1))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x - 1, p.y-1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = thisplayer;
                if (tempstate.put_disc(Point(p.x - 1, p.y + 1))) {
                    temp = Minimax(tempstate, depth - 1, false);
                    if (temp > value) {
                        value = temp;
                        nextstep = Point(p.x - 1, p.y+1);
                    }
                }
                poscopy.erase(it);
            }
        }
        else {
            value = INFINITY;
            int temp = 0;
            while (!poscopy.empty()) {
                auto it = poscopy.begin();
                Point p = Point((*it).first, (*it).second);
                GomokuBoard tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x + 1, p.y))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x - 1, p.y))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x - 1, p.y);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x, p.y + 1))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x, p.y+1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x, p.y - 1))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x, p.y-1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x + 1, p.y + 1))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y+1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x + 1, p.y - 1))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x + 1, p.y-1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x - 1, p.y - 1))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x - 1, p.y-1);
                    }
                }
                tempstate = state;
                tempstate.cur_player = get_next_player(thisplayer);
                if (tempstate.put_disc(Point(p.x - 1, p.y + 1))) {
                    temp = Minimax(tempstate, depth - 1, true);
                    if (temp < value) {
                        value = temp;
                        nextstep = Point(p.x - 1, p.y+1);
                    }
                }
                poscopy.erase(it);
            }
            return value;
        }
    }

};

const std::string file_log = "gamelog.txt";
const std::string file_state = "state";
const std::string file_action = "action";
const int timeout = TIMEOUT;

GomokuBoard game;

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    game.read_board(fin);
    fout << 5 << " " << 4 << std::endl;
    fout.flush();
    game.next_step();
    fout << game.nextstep.x << " " << game.nextstep.y << std::endl;
    fout.flush();
    fin.close();
    fout.close();
    return 0;
}
