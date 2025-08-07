#include <iostream>
#include <numeric>

using namespace std;

int N;
pair<int, int> matrix[500];
int dp[500][500];

int main()
{
    cin >> N;

    fill(*dp, *dp + 500 * 500, numeric_limits<int>::max());

    for (int i = 0; i < N; ++i)
    {
        cin >> matrix[i].first >> matrix[i].second;
        dp[i][i] = 0;  // 자기 자신끼리는 연산 횟수 0
    }

    for (int i = 1; i < N; ++i)
        for (int j = 0; j < N - i; ++j)
        {
            int tmp = i + j;
            for (int k = j; k < tmp; ++k)
            {
                const int cost = dp[j][k] + dp[k + 1][tmp] + matrix[j].first * matrix[k].second * matrix[tmp].second;
                dp[j][tmp] = min(dp[j][tmp], cost);
            }
        }

    cout << dp[0][N - 1];
}