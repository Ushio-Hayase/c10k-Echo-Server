#include <iostream>
#include <vector>

using namespace std;

int N, M, sum;
int cost[101];
int mem[101];
int dp[101][10001];

int main()
{
    cin >> N >> M;

    for (int i = 1; i <= N; ++i)
    {
        cin >> mem[i];
    }
    for (int i = 1; i <= N; ++i)
    {
        cin >> cost[i];
        sum += cost[i];
    }

    for (int i = 1; i <= N; ++i)
    {
        for (int j = 0; j <= sum; ++j)
        {
            if (j >= cost[i]) dp[i][j] = max(mem[i] + dp[i - 1][j - cost[i]], dp[i][j]);

            dp[i][j] = max(dp[i][j], dp[i - 1][j]);
        }
    }

    for (int i = 0; i <= sum; ++i)
        if (dp[N][i] >= M)
        {
            cout << i;
            return 0;
        }
}