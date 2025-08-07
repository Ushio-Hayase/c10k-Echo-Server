#include <algorithm>
#include <iostream>
using namespace std;

struct Point
{
    int x, y;
    bool operator<(const Point& other) const
    {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

int ccw(Point a, Point b, Point c)
{
    long long result = (long long)(b.x - a.x) * (c.y - a.y) - (long long)(b.y - a.y) * (c.x - a.x);
    if (result > 0) return 1;
    if (result < 0) return -1;
    return 0;
}

// 두 선분의 교차 판정
bool isIntersect(Point a, Point b, Point c, Point d)
{
    int ab = ccw(a, b, c) * ccw(a, b, d);
    int cd = ccw(c, d, a) * ccw(c, d, b);

    if (ab == 0 && cd == 0)
    {
        if (a.x > b.x || (a.x == b.x && a.y > b.y)) swap(a, b);
        if (c.x > d.x || (c.x == d.x && c.y > d.y)) swap(c, d);
        return !(b < c || d < a);
    }
    return ab <= 0 && cd <= 0;
}

bool isInside(Point p, int x1, int y1, int x2, int y2)
{
    return p.x >= min(x1, x2) && p.x <= max(x1, x2) && p.y >= min(y1, y2) && p.y <= max(y1, y2);
}

int main()
{
    int T;
    cin >> T;

    while (T--)
    {
        Point line1, line2;
        int x1, y1, x2, y2;

        cin >> line1.x >> line1.y >> line2.x >> line2.y;
        cin >> x1 >> y1 >> x2 >> y2;

        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);

        Point rect[4] = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};

        bool intersect = false;
        for (int i = 0; i < 4; i++)
            if (isIntersect(line1, line2, rect[i], rect[(i + 1) % 4]))
            {
                intersect = true;
                break;
            }

        if (!intersect)
            if (isInside(line1, x1, y1, x2, y2) || isInside(line2, x1, y1, x2, y2)) intersect = true;

        cout << (intersect ? "T" : "F") << "\n";
    }

    return 0;
}