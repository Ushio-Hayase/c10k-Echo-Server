#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int N, M;

int entry[32001];
vector<int> quest[32001];

int main()
{
    cin >> N >> M;

    for (int i = 0; i < M; ++i)
    {
        int x, y;

        cin >> x >> y;

        entry[y]++;
        quest[x].push_back(y);
    }

    priority_queue<int, vector<int>, greater<int>> pq;

    for (int i = 1; i <= N; ++i)
        if (entry[i] == 0) pq.push(i);

    while (!pq.empty())
    {
        const int node = pq.top();
        cout << node << " ";
        pq.pop();

        for (auto i : quest[node])
        {
            entry[i]--;
            if (entry[i] == 0) pq.push(i);
        }
    }
}