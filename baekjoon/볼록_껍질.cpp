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
    return a.first * b.second + b.first * c.second + a.second * c.first - a.second * b.first - b.second * c.first -
           c.second * a.first;
}

bool cmp_first(pair<long long, long long>& a, pair<long long, long long>& b)
{
    if (a.second == b.second) return a.first < b.first;
    return a.second < b.second;
}

bool cmp(pair<long long, long long>& a, pair<long long, long long>& b)
{
    long long tmp = ccw(init_point, a, b);
    if (tmp != 0)
        return tmp > 0;
    else if (a.second != b.second)
        return a.second < b.second;
    else
        return a.first < b.first;
}

int main()
{
    cin >> N;
    vector<pair<long long, long long>> points(N);

    for (int i = 0; i < N; ++i)
    {
        cin >> points[i].first >> points[i].second;
    }

    sort(points.begin(), points.end(), cmp_first);

    init_point = points[0];
    sort(points.begin() + 1, points.end(), cmp);

    stack<pair<long long, long long>, vector<pair<long long, long long>>> st;

    st.push(points[0]);
    st.push(points[1]);

    for (int i = 2; i < N; ++i)
    {
        while (st.size() >= 2)
        {
            auto second = st.top();
            st.pop();
            auto first = st.top();

            if (ccw(first, second, points[i]) > 0)
            {
                st.push(second);
                break;
            }
        }
        st.push(points[i]);
    }

    cout << st.size();
}