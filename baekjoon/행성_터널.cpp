#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int N;
vector<pair<int, int>> x;
vector<pair<int, int>> y;
vector<pair<int, int>> z;

vector<vector<int>> edges;
int parent[100'000];

int find(int i)
{
    if (parent[i] == i) return i;
    return parent[i] = find(parent[i]);
}

void Union(int a, int b)
{
    int a_parent = find(a);
    int b_parent = find(b);

    if (a_parent < b_parent)
        parent[b_parent] = a_parent;
    else
        parent[a_parent] = b_parent;
}

int main()
{
    cin >> N;

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int i = 0; i < N; ++i)
    {
        int a, b, c;
        cin >> a >> b >> c;

        x.emplace_back(a, i);
        y.emplace_back(b, i);
        z.emplace_back(c, i);
        parent[i] = i;
    }

    sort(x.begin(), x.end());
    sort(y.begin(), y.end());
    sort(z.begin(), z.end());

    int result = 0;

    for (int i = 0; i < N - 1; ++i)
    {
        edges.push_back({abs(x[i + 1].first - x[i].first), x[i].second, x[i + 1].second});
        edges.push_back({abs(y[i + 1].first - y[i].first), y[i].second, y[i + 1].second});
        edges.push_back({abs(z[i + 1].first - z[i].first), z[i].second, z[i + 1].second});
    }

    sort(edges.begin(), edges.end());

    for (int i = 0; i < edges.size() - 1; ++i)
    {
        if (find(edges[i][1]) != find(edges[i][2]))
        {
            result += edges[i][0];
            Union(edges[i][1], edges[i][2]);
        }
    }
    cout << result;
}