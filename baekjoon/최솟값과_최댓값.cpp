#include <iostream>
#include <limits>
#include <vector>

using namespace std;

class SegmentTree
{
   public:
    SegmentTree(vector<int>& arr)
    {
        size_ = arr.size();
        tree_max_.resize(4 * size_);
        tree_min_.resize(4 * size_);
        build(arr, 1, 0, size_ - 1);
    }

    pair<int, int> query(int left, int right) { return query(1, 0, size_ - 1, left, right); }

   private:
    void build(vector<int>& arr, int node, int start, int end)
    {
        if (start == end)
        {
            tree_max_[node] = arr[start];
            tree_min_[node] = arr[start];
        }
        else
        {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree_max_[node] = max(tree_max_[2 * node], tree_max_[2 * node + 1]);
            tree_min_[node] = min(tree_min_[2 * node], tree_min_[2 * node + 1]);
        }
    }

    pair<int, int> query(int node, int start, int end, int left, int right)
    {
        if (right < start || end < left) return {-1, numeric_limits<int>::max()};
        if (left <= start && end <= right) return {tree_max_[node], tree_min_[node]};

        int mid = (start + end) / 2;
        pair<int, int> p1 = query(node * 2, start, mid, left, right);
        pair<int, int> p2 = query(node * 2 + 1, mid + 1, end, left, right);
        return {max(p1.first, p2.first), min(p1.second, p2.second)};
    }

    vector<int> tree_max_;
    vector<int> tree_min_;
    int size_;
};

int N, M;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> N >> M;
    vector<int> arr(N);
    for (int i = 0; i < N; ++i) cin >> arr[i];

    SegmentTree st(arr);

    while (M--)
    {
        int l, r;
        cin >> l >> r;
        pair<int, int> result = st.query(l - 1, r - 1);

        cout << result.second << ' ' << result.first << '\n';
    }
}