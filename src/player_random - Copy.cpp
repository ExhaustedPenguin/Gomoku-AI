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
    enum SPOT_STATE {
        EMPTY = 0,
        BLACK = 1,
        WHITE = 2
    };
    static const int SIZE = 15;
    std::array<std::array<int, SIZE>, SIZE> board;
    std::set<std::pair<int, int>> blackpos;
    std::set<std::pair<int, int>> whitepos;
    int empty_count;
    int cur_player;
    bool done;
    int winner;
    int thisplayer;
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
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
        done = false;
        winner = -1;
    }
    bool put_disc(Point p) {
        if (!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        empty_count--;
        // Check Win
        if (checkwin(cur_player)) {
            done = true;
            winner = cur_player;
        }
        if (empty_count == 0) {
            done = true;
            winner = EMPTY;
        }

        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        return true;
    }
    bool checkwin(int disc) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (is_disc_at(Point(i, j), disc)) {
                    bool iswin = true;
                    if (i + 4 < SIZE) {                                 //HORIZONTAL
                        for (int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i + k, j), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (j + 4 < SIZE) {                                 //VERTICAL
                        for (int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i, j + k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (i + 4 < SIZE && j + 4 < SIZE) {                 //DIAGONAL TO RIGHT
                        for (int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i + k, j + k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (i - 4 >= 0 && j + 4 < SIZE) {                   //DIAGONAL TO LEFT
                        for (int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i - k, j + k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                }
            }
        }
        return false;
    }
    std::string encode_player(int state) {
        if (state == BLACK) return "O";
        if (state == WHITE) return "X";
        return "Draw";
    }
    std::string encode_spot(int x, int y) {
        if (is_spot_valid(Point(x, y))) return ".";
        if (board[x][y] == BLACK) return "O";
        if (board[x][y] == WHITE) return "X";
        return " ";
    }

    int count_value() {
        int value = 0;
        int whitevalue, blackvalue;
        bool iscounted=true;
        while (!whitepos.empty()) {
            auto it = whitepos.begin(); 
            if ((*it.first - 1) > 0 && (*it.first + 1) < SIZE * SIZE) {
                for (int k = -2; k < 3; k++)
                    if (!is_disc_at(Point(*it.first + k, *it.second), GomokuBoard::WHITE)) {
                        if (k != 2) {
                            iscounted = false;
                        }
                        break;
                    }
                if (iscounted) return true;
            }
            if ((*it.second - 1) > 0 && (*it.second + 1) < SIZE * SIZE) {
                for (int k = -2; k < 3; k++)
                    if (!is_disc_at(Point(*it.first, *it.second+k), GomokuBoard::WHITE)) {
                        if (k != 2) {
                            iscounted = false;
                        }
                        break;
                    }
                if (iscounted) return true;
            }
        }
    }

};

const std::string file_log = "gamelog.txt";
const std::string file_state = "state";
const std::string file_action = "action";
const int timeout = TIMEOUT;

void launch_executable(std::string filename) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    size_t pos;
    std::string command = "start /min " + filename + " " + file_state + " " + file_action;
    if ((pos = filename.rfind("/")) != std::string::npos || (pos = filename.rfind("\\")) != std::string::npos)
        filename = filename.substr(pos + 1, std::string::npos);
    std::string kill = "timeout /t " + std::to_string(timeout) + " > NUL && taskkill /im " + filename + " > NUL 2>&1";
    system(command.c_str());
    system(kill.c_str());
#elif __linux__
    std::string command = "timeout " + std::to_string(timeout) + "s " + filename + " " + file_state + " " + file_action;
    system(command.c_str());
#elif __APPLE__
    // May require installing the command by:
    // brew install coreutils
    std::string command = "gtimeout " + std::to_string(timeout) + "s " + filename + " " + file_state + " " + file_action;
    system(command.c_str());
#endif
}






void read_board(std::ifstream& fin) {
    fin >> player;
    int temp;
    int black, white;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> temp;
            game.board[i][j] = temp;
            if (temp == 1) {
                game.blackpos.insert(std::pair<i, j>);
                black++;
            }
            else if (temp == 2) {
                game.whitepos.insert(std::pair<i, j>);
                white++;
            }
        }
    }
    if (black > white) {
        player = GomokuBoard::WHITE;
    }
    else if (black == white) {
        player = GomokuBoard::BLACK;
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (game.board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}

void next_step(std::ofstream& fout) {
    if (empty_count == SIZE * SIZE) {
        game.thisplayer = GomokuBoard::BLACK;
        fout << 7 << " " << 7 << std::endl;
        fout.flush();
        return;
    }
    else if (empty_count == (SIZE * SIZE) - 1) {
        game.thisplayer = GomokuBoard::WHITE;
        auto it = game.blackpos.begin();
        p = Point(*it.first, *it.second);
        if (game.put_disc(Point(p.x + 1, p.y))) {
            fout << p.x + 1 << " " << p.y << std::endl;
            fout.flush();
            return;
        }
    }
}

GomokuBoard game;

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    next_step(fout);
    fin.close();
    fout.close();
    return 0;
}
