#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int N, M;
vector<vector<int>> edge;
int parent[100'001];

int Find(int v)
{
    if (parent[v] == v || parent[v] == 0) return v;
    return Find(parent[v]);
}

void Union(int a, int b)
{
    int p1 = Find(a);
    int p2 = Find(b);

    if (p1 < p2)
        parent[p2] = p1;
    else
        parent[p1] = p2;
}

int main()
{
    cin >> N >> M;

    for (int i = 0; i < M; ++i)
    {
        int a, b, c;
        cin >> a >> b >> c;
        edge.push_back({c, a, b});
    }

    int result = 0;
    int max_edge = 0;

    sort(edge.begin(), edge.end());

    for (auto& i : edge)
    {
        if (Find(i[1]) != Find(i[2]))
        {
            Union(i[1], i[2]);
            result += i[0];
            max_edge = max(max_edge, i[0]);
        }
    }

    cout << result - max_edge;
}