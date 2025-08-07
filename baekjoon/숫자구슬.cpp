#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int N, M;

int dp[300][300];
vector<int> ans;

pair<int, vector<int>> func(vector<int> cnt, int n, int depth)
{
    int rst = 0;
    vector<int> rstVector;
    if (depth >= M)
    {
        if (n < N) return {numeric_limits<int>::max(), {}};

        int result = 0;
        int i = 0;
        for (auto iter : cnt)
        {
            result = max(dp[i][i + iter + 1], result);
            i += iter;
        }

        return {result, cnt};
    }

    for (int i = 1; i < N - n; ++i)
    {
        vector<int> cp{cnt};
        cp.push_back(i);
        auto tmp = func(cp, n + i, depth + 1);
        if (rst < tmp.first)
        {
            rst = tmp.first;
            rstVector = tmp.second;
        }
    }
    return {rst, rstVector};
}

int main()
{
    cin >> N >> M;

    for (int i = 0; i < N; ++i)
    {
        int x;
        cin >> x;
        dp[i][i] = x;
    }

    fill(*dp, *dp + 300 * 300, numeric_limits<int>::max());
    ans.resize(M);

    for (int len = 1; len < N; ++len)
    {
        for (int i = 0; i < N - len; ++i)
        {
            int j = i + len;

            for (int k = i; k < j; ++k)
            {
                const int cost = dp[i][k + 1] + dp[k + 1][j + 1];
                dp[i][j] = min(cost, dp[i][j]);
            }
        }
    }
    auto tmp = func({}, 0, 0).second;

    for (auto i : tmp) cout << i << " ";
}