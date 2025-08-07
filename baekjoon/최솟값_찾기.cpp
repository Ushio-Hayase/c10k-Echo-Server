#include <deque>
#include <iostream>

using namespace std;

int N, L;
int arr[5000000];
deque<pair<int, int>> dq;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin.tie(nullptr);

    cin >> N >> L;

    for (int i = 0; i < N; ++i) cin >> arr[i];

    for (int i = 0; i < N; ++i)
    {
        if (!dq.empty())
        {
            if (dq.front().second < i - L + 1) dq.pop_front();
        }
        while (!dq.empty() && dq.back().first > arr[i]) dq.pop_back();
        dq.push_back(make_pair(arr[i], i));
        cout << dq.front().first << " ";
    }
}