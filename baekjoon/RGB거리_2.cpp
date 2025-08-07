#include <iostream>
#include <limits>

using namespace std;

int N;
int arr[1001][3];
int dp[1001][3];
int ans = 1000 * 1000 + 1;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> N;

    for (int i = 1; i <= N; i++) cin >> arr[i][0] >> arr[i][1] >> arr[i][2];

    for (int rgb = 0; rgb <= 2; rgb++)
    {
        for (int i = 0; i <= 2; i++)
        {
            if (i == rgb)
                dp[1][i] = arr[1][i];
            else
                dp[1][i] = 1000 * 1000 + 1;
        }

        for (int i = 2; i <= N; i++)
        {
            dp[i][0] = arr[i][0] + min(dp[i - 1][1], dp[i - 1][2]);
            dp[i][1] = arr[i][1] + min(dp[i - 1][0], dp[i - 1][2]);
            dp[i][2] = arr[i][2] + min(dp[i - 1][0], dp[i - 1][1]);
        }

        for (int i = 0; i <= 2; i++)
        {
            if (i == rgb)
                continue;
            else
                ans = min(ans, dp[N][i]);
        }
    }
    cout << ans;
}