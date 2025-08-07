#include <iostream>

using namespace std;

int N;
int dp[101][10][1 << 10];
constexpr int mod = 1'000'000'000;

int main()
{
    cin >> N;

    for (int i = 0; i < 10; ++i) dp[1][i][1 << i] = 1;

    for (int i = 1; i < N; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            for (int k = 0; k < (1 << 10); ++k)
            {
                if (j >= 0 && j < 9)
                {
                    int tmp = k | 1 << (j + 1);
                    dp[i + 1][j + 1][tmp] += dp[i][j][k];
                    dp[i + 1][j + 1][tmp] %= mod;
                }
                if (j > 0 && j <= 9)
                {
                    int tmp = k | 1 << (j - 1);
                    dp[i + 1][j - 1][tmp] += dp[i][j][k];
                    dp[i + 1][j - 1][tmp] %= mod;
                }
            }
        }
    }

    int total = 0;
    for (int i = 1; i < 10; ++i)
    {
        total += dp[N][i][0b1111111111];
        total %= mod;
    }
    cout << total;
}