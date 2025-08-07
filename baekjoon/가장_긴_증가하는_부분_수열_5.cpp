#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

using namespace std;

int N;
int arr[1'000'001];
vector<int> lis;
vector<int> trace;
vector<int> result;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    for (int i = 1; i <= N; ++i)
    {
        cin >> arr[i];
    }
    lis.push_back(numeric_limits<int>::min() + 1);
    trace.resize(N + 1);

    for (int i = 1; i <= N; ++i)
    {
        if (lis[lis.size() - 1] < arr[i])
        {
            lis.push_back(arr[i]);
            trace[i] = lis.size() - 1;
        }
        else
        {
            auto iter = lower_bound(lis.begin() + 1, lis.end(), arr[i]);
            trace[i] = distance(lis.begin(), iter);
            *iter = arr[i];
        }
    }
    int cur_lis_len = lis.size() - 1;
    int before = numeric_limits<int>::max() - 1;

    cout << lis.size() - 1 << "\n";
    for (int i = N; i > 0; --i)
    {
        if (trace[i] == cur_lis_len)
            if (arr[i] < before)
            {
                result.push_back(arr[i]);
                before = arr[i];
                cur_lis_len--;
            }
    }

    for (auto i = result.rbegin(); i != result.rend(); i++)
    {
        cout << *i << ' ';
    }
}