#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

int shapes[26][2] =  // Δrow, Δcol
    {{9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {4, 1}, {3, 0}, {3, 1},
     {2, 0}, {2, 1}, {2, 2}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {0, 1},
     {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}};

struct Node
{
    Node(int v, Node* up, Node* down, Node* left, Node* right)
        : value_(v), up_(up), down_(down), left_(left), right_(right)
    {
    }

    int value_;
    Node* up_ = nullptr;
    Node* down_ = nullptr;
    Node* left_ = nullptr;
    Node* right_ = nullptr;
};

class Game
{
   private:
    vector<vector<Node>> board_;
    vector<vector<bool>> enemy_occupy_;
    vector<vector<bool>> my_occypy_;

   public:
    Game() = default;

    Game(const vector<vector<Node>>& arr)
        : board_(arr),
          enemy_occupy_(10, vector<bool>(17, false)),
          my_occypy_(10, vector<bool>(17, false))
    {
    }

    int calcul_score(const int row1, const int col1, const int row2,
                     const int col2)
    {
        int sum = 0;
        bool l = false, r = false, u = false, d = false;

        int score = 0;

        for (int i = row1; i <= row2; ++i)
            for (int j = col1; j <= col2; ++j)
            {
                int content = board_[i][j].value_;
                sum += content;
                if (content != 0)
                {
                    if (i == row1) u = true;
                    if (i == row2) d = true;
                    if (j == col1) l = true;
                    if (j == col2) r = true;
                    score++;
                }
                else
                {
                    if (enemy_occupy_[i][j]) score += 2;
                }
            }

        if ((sum == 10) && l && r && u && d)
        {
            return score;
        }
        else
            return (int)0x80000000;
    }

    vector<int> estimate()
    {
        int score = 0x80000000;
        int row1, row2, col1, col2;

        for (int k = 0; k < 26; ++k)
            for (int i = 0; i < 10; ++i)
                for (int j = 0; j < 17; ++j)
                {
                    int r = i + shapes[k][0];
                    int c = j + shapes[k][1];
                    if (r < 10 && c < 17)
                    {
                        int cur_score = calcul_score(i, j, r, c);
                        if (cur_score > score)
                        {
                            row1 = i;
                            row2 = r;
                            col1 = j;
                            col2 = c;
                            score = cur_score;
                        }
                    }
                }

        if (score <= 0)
            return {-1, -1, -1, -1};
        else
            return {row1, col1, row2, col2};
    }

    void update(int row1, int col1, int row2, int col2, bool isMe)
    {
        if (row1 == -1) return;
        if (isMe)
        {
            for (int i = row1; i <= row2; ++i)
                for (int j = col1; j <= col2; ++j)
                {
                    board_[i][j] = 0;
                    my_occypy_[i][j] = true;
                    enemy_occupy_[i][j] = false;
                }
        }
        else
        {
            for (int i = row1; i <= row2; ++i)
                for (int j = col1; j <= col2; ++j)
                {
                    board_[i][j] = 0;
                    my_occypy_[i][j] = false;
                    enemy_occupy_[i][j] = true;
                }
        }
    }
};

int main()
{
    Game game;
    bool first = false;

    while (true)
    {
        string command;
        if (!(cin >> command)) continue;

        if (command == "READY")
        {
            string turn;
            cin >> turn;
            first = (turn == "FIRST");
            cout << "OK" << endl;
        }
        else if (command == "INIT")
        {
            vector<vector<Node>> board(10, vector<Node>(17));

            for (int i = 0; i < 10; ++i)
            {
                string row;
                cin >> row;
                for (int j = 0; j < 17; ++j)
                {
                    Node* up = i > 0 ? &board[i - 1][j] : nullptr;
                    Node* down = i < 9 ? &board[i + 1][j] : nullptr;
                    Node* left = j > 0 ? &board[i][j - 1] : nullptr;
                    Node* right = j < 16 ? &board[i][j + 1] : nullptr;

                    board[i][j] = {row[j] - '0', up, down, left, right};
                }
            }
            game = Game(board);
        }
        else if (command == "TIME")
        {
            int my_time, enemy_time;
            cin >> my_time >> enemy_time;

            vector<int> result = game.estimate();
            game.update(result[0], result[1], result[2], result[3], true);

            cout << result[0] << ' ' << result[1] << ' ' << result[2] << ' '
                 << result[3] << endl;
        }
        else if (command == "OPP")
        {
            int r1, c1, r2, c2, time;
            cin >> r1 >> c1 >> r2 >> c2 >> time;
            game.update(r1, c1, r2, c2, false);
        }
        else if (command == "FINISH")
        {
            break;
        }
        else
        {
            cerr << "Invalid command: " << command << endl;
            break;
        }
    }

    return 0;
}