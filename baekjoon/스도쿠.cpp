#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> arr;
vector<pair<int, int>> empty_space;

bool is_square_able(int row, int col)
{
    int row_start = (row / 3) * 3;
    int col_start = (col / 3) * 3;

    bool check[10];
    fill(check, check + 10, false);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            if (arr[row_start + i][col_start + j] == 0) continue;
            if (check[arr[row_start + i][col_start + j]]) return false;
            check[arr[row_start + i][col_start + j]] = true;
        }
    return true;
}

bool is_row_able(int row)
{
    bool check[10];
    fill(check, check + 10, false);

    for (int i = 0; i < 9; ++i)
    {
        if (arr[row][i] == 0) continue;
        if (check[arr[row][i]]) return false;
        check[arr[row][i]] = true;
    }

    return true;
}

bool is_col_able(int col)
{
    bool check[10];
    fill(check, check + 10, false);

    for (int i = 0; i < 9; ++i)
    {
        if (arr[i][col] == 0) continue;
        if (check[arr[i][col]]) return false;
        check[arr[i][col]] = true;
    }

    return true;
}

void dfs(int num)
{
    if (num > empty_space.size())
    {
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j) cout << arr[i][j] << ' ';
            cout << '\n';
        }
        exit(0);
    }
    pair<int, int> coor = empty_space[num];
    for (int i = 1; i <= 9; ++i)
    {
        arr[coor.first][coor.second] = i;
        if (is_row_able(coor.first) && is_col_able(coor.second) && is_square_able(coor.first, coor.second))
            dfs(num + 1);
        arr[coor.first][coor.second] = 0;
    }
}

int main()
{
    arr.resize(9);
    for (int i = 0; i < 9; ++i)
    {
        arr[i].resize(9);
        for (int j = 0; j < 9; ++j)
        {
            cin >> arr[i][j];
            if (arr[i][j] == 0) empty_space.push_back({i, j});
        }
    }

    dfs(0);
}