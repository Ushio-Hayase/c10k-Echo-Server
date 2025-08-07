#include <iostream>
#include <numeric>

using namespace std;

int N;
int cost[16][16];
int dp[16][65536];

int func(int now, int cites)
{
    if (cites == (1 << N) - 1)
    {
        return (cost[now][0] != 0) ? cost[now][0] : 1e+8;
    }
    if (dp[now][cites] != -1) return dp[now][cites];
    dp[now][cites] = 1e+8;

    for (int i = 0; i < N; ++i)
    {
        if (!static_cast<bool>(cites & (1 << i)) && cost[now][i] != 0)
        {
            int new_cost = func(i, static_cast<int>(cites | (1 << i))) + cost[now][i];
            if (new_cost < dp[now][cites]) dp[now][cites] = new_cost;
        }
    }
    return dp[now][cites];
}

int main()
{
    cin >> N;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) cin >> cost[i][j];

    for (int i = 0; i < N; ++i) fill_n(dp[i], size(dp[i]), -1);
    cout << func(0, 1);
}