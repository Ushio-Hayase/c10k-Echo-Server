#pragma GCC optimize("O3,unroll-loops")

#include <immintrin.h>

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int N;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> N;

    vector<vector<int>> arr;

    arr.resize(4);

    for (int i = 0; i < N; ++i)
    {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        arr[0].push_back(a);
        arr[1].push_back(b);
        arr[2].push_back(c);
        arr[3].push_back(d);
    }

    size_t result = 0;

    vector<int> sum[2];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
        {
            sum[0].push_back(arr[0][i] + arr[1][j]);
        }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) sum[1].push_back(arr[2][i] + arr[3][j]);

    sort(sum[1].begin(), sum[1].end());
    sort(sum[0].begin(), sum[0].end());

    for (size_t i = 0; i < N * N; ++i)
    {
        auto iter_low = lower_bound(sum[1].begin(), sum[1].end(), -sum[0][i]);
        auto iter_high = upper_bound(sum[1].begin(), sum[1].end(), -sum[0][i]);
        if (iter_low != sum[1].end() && sum[0][i] + *iter_low == 0) result += iter_high - iter_low;
    }
    cout << result;
}