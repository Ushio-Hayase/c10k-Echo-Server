#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

int N;

int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};

int main()
{
    cin >> N;
    while (N--)
    {
        int H, W;
        cin >> H >> W;
        vector<vector<char>> arr(H);
        vector<vector<bool>> visit(H);
        vector<pair<int, int>> door[26];
        bool keys[26];
        fill_n(keys, 26, false);
        queue<pair<int, int>> q;
        int result = 0;

        for (int i = 0; i < H; ++i)
        {
            arr[i].resize(W);
            visit[i].resize(W);
            string str;
            cin >> str;
            for (int j = 0; j < W; ++j)
            {
                if ((j == 0 && str[j] == '.') || (i == 0 && str[j] == '.') || (j == W - 1 && str[j] == '.') ||
                    (i == H - 1 && str[j] == '.'))
                {
                    q.push({i, j});
                }
                else if ((j == 0 && str[j] == '$') || (i == 0 && str[j] == '$') || (j == W - 1 && str[j] == '$') ||
                         (i == H - 1 && str[j] == '$'))
                {
                    str[j] = '.';
                    result++;
                    q.push({i, j});
                }
                else if ((j == 0 && 'a' <= str[j] && str[j] <= 'z') || (i == 0 && 'a' <= str[j] && str[j] <= 'z') ||
                         (j == W - 1 && 'a' <= str[j] && str[j] <= 'z') ||
                         (i == H - 1 && 'a' <= str[j] && str[j] <= 'z'))
                {
                    q.push({i, j});
                    keys[str[j] - 'a'] = true;
                }
                else if ((j == 0 && 'A' <= str[j] && str[j] <= 'Z') || (i == 0 && 'A' <= str[j] && str[j] <= 'Z') ||
                         (j == W - 1 && 'A' <= str[j] && str[j] <= 'Z') ||
                         (i == H - 1 && 'A' <= str[j] && str[j] <= 'Z'))
                {
                    door[str[j] - 'A'].push_back({i, j});
                }

                arr[i][j] = str[j];
            }
        }

        string hold_key;
        cin >> hold_key;
        for (int i = 0; i < hold_key.size(); ++i)
        {
            if (hold_key[i] != '0') keys[hold_key[i] - 'a'] = true;
        }

        for (int i = 0; i < 26; ++i)
        {
            if (keys[i])
            {
                for (auto coor : door[i])
                {
                    q.push(coor);
                }
            }
        }

        while (!q.empty())
        {
            int row = q.front().first;
            int col = q.front().second;

            q.pop();

            for (int i = 0; i < 4; ++i)
            {
                int new_row = row + dy[i];
                int new_col = col + dx[i];
                if (0 <= new_row && new_row < H && 0 <= new_col && new_col < W)
                {
                    if (!visit[new_row][new_col] && arr[new_row][new_col] == '.')
                    {
                        visit[new_row][new_col] = true;
                        q.push({new_row, new_col});
                    }
                    else if (!visit[new_row][new_col] && 'A' <= arr[new_row][new_col] && arr[new_row][new_col] <= 'Z')
                    {
                        door[arr[new_row][new_col] - 'A'].push_back({new_row, new_col});
                        if (keys[arr[new_row][new_col] - 'A'])
                        {
                            visit[new_row][new_col] = true;
                            q.push({new_row, new_col});
                        }
                    }
                    else if (!visit[new_row][new_col] && 'a' <= arr[new_row][new_col] && arr[new_row][new_col] <= 'z')
                    {
                        visit[new_row][new_col] = true;
                        keys[arr[new_row][new_col] - 'a'] = true;
                        q.push({new_row, new_col});
                        for (auto coor : door[arr[new_row][new_col] - 'a'])
                        {
                            if (!visit[coor.first][coor.second])
                            {
                                visit[coor.first][coor.second] = true;
                                q.push(coor);
                            }
                        }
                    }
                    else if (!visit[new_row][new_col] && arr[new_row][new_col] == '$')
                    {
                        visit[new_row][new_col] = true;
                        arr[new_row][new_col] = '.';
                        result++;
                        q.push({new_row, new_col});
                    }
                }
            }
        }

        cout << result << '\n';
    }
}