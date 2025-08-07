#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

using namespace std;

int N, M;
vector<int> entry;
vector<list<int>> adj;
list<int> result;

int main()
{
    cin >> N >> M;

    entry.resize(N + 1);
    adj.resize(N + 1);

    int remain = 0;

    for (int i = 0; i < M; ++i)
    {
        int a, x;
        cin >> a;
        cin >> x;
        for (int j = 1; j < a; ++j)
        {
            int y;
            cin >> y;
            adj[x].push_back(y);
            entry[y]++;
            remain++;
            x = y;
        }
    }

    priority_queue<int, vector<int>, greater<int>> pq;

    for (int i = 1; i <= N; ++i)
    {
        if (entry[i] == 0)
        {
            pq.push(i);
            result.push_back(i);
        }
    }

    while (!pq.empty())
    {
        const int singer = pq.top();
        pq.pop();

        for (auto i : adj[singer])
        {
            entry[i]--;
            remain--;
            if (entry[i] == 0)
            {
                pq.push(i);
                result.push_back(i);
            }
        }
    }

    if (remain != 0)
    {
        cout << 0;
        return 0;
    }

    for (auto i : result) cout << i << '\n';
}