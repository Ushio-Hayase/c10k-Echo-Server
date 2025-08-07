#include <iostream>
#include <vector>

using namespace std;

int N, R, Q;
vector<vector<int>> adj;
vector<int> child_cnt;
vector<bool> visit;

int get_child_cnt(int idx)
{
    visit[idx] = true;
    int result = 1;
    if (child_cnt[idx] != 0) return child_cnt[idx];
    for (auto i : adj[idx])
    {
        if (visit[i]) continue;
        result += get_child_cnt(i);
    }
    return child_cnt[idx] = result;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> N >> R >> Q;

    adj.resize(N + 1);
    child_cnt.resize(N + 1);
    visit.resize(N + 1);

    for (int i = 0; i < N - 1; ++i)
    {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    get_child_cnt(R);

    for (int n = 0; n < Q; ++n)
    {
        int query;
        cin >> query;

        cout << child_cnt[query] << '\n';
    }
}