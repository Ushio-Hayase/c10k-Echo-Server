#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

int N;
pair<long long, long long> init_point;

long long ccw(pair<long long, long long>& a, pair<long long, long long>& b, pair<long long, long long>& c)
{
    return (b.first - a.first) * (c.second - a.second) - (c.first - a.first) * (b.second - a.second);
}

long long dist(pair<long long, long long>& a, pair<long long, long long>& b)
{
    return (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
}

bool cmp(pair<long long, long long>& a, pair<long long, long long>& b)
{
    long long tmp = ccw(init_point, a, b);
    return (tmp > 0 || (tmp == 0 && dist(init_point, a) < dist(init_point, b)));
}

int main()
{
    cin >> N;
    vector<pair<long long, long long>> points;

    for (int i = 0; i < N; ++i)
    {
        int px, py;
        char inBorder;
        cin >> px >> py >> inBorder;
        if (inBorder == 'Y') points.push_back({px, py});
    }

    sort(points.begin(), points.end());

    init_point = points[0];
    sort(points.begin() + 1, points.end(), cmp);

    int r = points.size() - 1;

    while (r > 1 && ccw(init_point, points[r], points[r - 1]) == 0) r--;
    reverse(points.begin() + r, points.end());

    cout << points.size() << '\n';

    for (auto i : points)
    {
        cout << i.first << ' ' << i.second << '\n';
    }
}