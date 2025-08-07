#include <iostream>
#include <vector>

using namespace std;

class SegmentTree
{
   public:
    SegmentTree(vector<long long>& arr)
    {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 1, 0, n - 1);
    }

    void update(int idx, long long val) { update(1, 0, n - 1, idx, val); }

    long long query(int l, int r) { return query(1, 0, n - 1, l, r); }

   private:
    vector<long long> tree;
    int n;

    void build(vector<long long>& arr, int node, int start, int end)
    {
        if (start == end)
        {
            tree[node] = arr[start];
        }
        else
        {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    void update(int node, int start, int end, int idx, long long val)
    {
        if (start == end)
        {
            tree[node] = val;
        }
        else
        {
            int mid = (start + end) / 2;
            if (idx <= mid)
                update(2 * node, start, mid, idx, val);
            else
                update(2 * node + 1, mid + 1, end, idx, val);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    long long query(int node, int start, int end, int l, int r)
    {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];

        int mid = (start + end) / 2;
        long long p1 = query(2 * node, start, mid, l, r);
        long long p2 = query(2 * node + 1, mid + 1, end, l, r);
        return p1 + p2;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    cin >> N >> M >> K;

    vector<long long> arr(N);
    for (int i = 0; i < N; ++i)
    {
        cin >> arr[i];
    }

    SegmentTree seg_tree(arr);

    for (int i = 0; i < M + K; ++i)
    {
        int a;
        cin >> a;

        if (a == 1)
        {
            int b;
            long long c;
            cin >> b >> c;
            seg_tree.update(b - 1, c);
        }
        else
        {
            int b, c;
            cin >> b >> c;
            cout << seg_tree.query(b - 1, c - 1) << '\n';
        }
    }

    return 0;
}