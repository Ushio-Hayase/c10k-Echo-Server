#include <iostream>
#include <vector>

using namespace std;

int N, M;

int drow[4] = {-1, 0, 1, 0};
int dcol[4] = {0, 1, 0, -1};

int main()
{
    cin >> N >> M;
    pair<int, int> robot;
    int direction;
    cin >> robot.first >> robot.second >> direction;

    vector<vector<int>> arr(N, vector<int>(M));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) cin >> arr[i][j];

    int result = 0;

    while (true)
    {
        if (arr[robot.first][robot.second] == 0)
        {
            arr[robot.first][robot.second] = -1;
            ++result;
        }

        bool can_cleaning = false;

        for (int i = 0; i < 4; ++i)
        {
            int row = robot.first + drow[i];
            int col = robot.second + dcol[i];

            if (arr[row][col] == 0) can_cleaning = true;
        }

        if (!can_cleaning)
        {
            int row = robot.first + drow[(direction + 2) % 4];
            int col = robot.second + dcol[(direction + 2) % 4];

            if (arr[row][col] != 1)
            {
                robot.first = row;
                robot.second = col;
            }
            else
                break;
        }
        else
        {
            direction += 3;
            direction %= 4;

            int row = robot.first + drow[direction];
            int col = robot.second + dcol[direction];
            if (arr[row][col] == 0)
            {
                robot.first = row;
                robot.second = col;
            }
        }
    }

    cout << result;
}