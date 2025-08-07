#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct Circle
{
    int x, y, r;
};

int T, N;

bool is_inside(Circle c1, Circle c2)
{
    return pow((c1.x - c2.x), 2) + pow((c1.y - c2.y), 2) <= pow(c1.r, 2) + 2 * c1.r * c2.r + pow(c2.r, 2);
}

int get_parent(vector<int>& parent, int x)
{
    if (parent[x] != x) return parent[x] = get_parent(parent, parent[x]);
    return parent[x];
}

void union_parent(vector<int>& parent, int x, int y)
{
    int root_x = get_parent(parent, x);
    int root_y = get_parent(parent, y);

    if (root_x == root_y) return;

    if (root_x < root_y)
        parent[root_y] = root_x;
    else
        parent[root_x] = root_y;
}

int main()
{
    cin >> T;
    while (T--)
    {
        cin >> N;
        vector<Circle> arr(N);
        vector<int> parent(N);
        int result = N;
        for (int i = 0; i < N; ++i)
        {
            cin >> arr[i].x >> arr[i].y >> arr[i].r;
            parent[i] = i;
        }

        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                if (is_inside(arr[i], arr[j]) && get_parent(parent, i) != get_parent(parent, j))
                {
                    union_parent(parent, parent[i], parent[j]);
                    result--;
                }

        cout << result << '\n';
    }
}