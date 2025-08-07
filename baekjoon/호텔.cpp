#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int C, N;
    cin >> C >> N;

    vector<int> cost(N), customer(N);
    for (int i = 0; i < N; i++)
    {
        cin >> cost[i] >> customer[i];
    }

    // dp[i] = i명 이상의 고객을 확보하는 최소 비용
    vector<int> dp(C + 101, INT_MAX);  // 약간의 여유공간
    dp[0] = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = customer[i]; j <= C + 100; j++)
        {
            if (dp[j - customer[i]] != INT_MAX)
            {
                dp[j] = min(dp[j], dp[j - customer[i]] + cost[i]);
            }
        }
    }

    int result = INT_MAX;
    for (int i = C; i <= C + 100; i++)
    {
        result = min(result, dp[i]);
    }

    cout << result << endl;
    return 0;
}