#include <algorithm>
#include <iostream>
using namespace std;

struct Point
{
    long long x, y;
};

int ccw(Point p1, Point p2, Point p3)
{
    long long result = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
    if (result == 0) return 0;
    return result > 0 ? 1 : -1;
}

void intersection(Point p1, Point p2, Point p3, Point p4)
{
    long double detA = (p2.x - p1.x) * (p4.y - p3.y) - (p2.y - p1.y) * (p4.x - p3.x);

    if (detA == 0) return;

    long double t = ((p3.x - p1.x) * (p4.y - p3.y) - (p3.y - p1.y) * (p4.x - p3.x)) / detA;

    long double x = p1.x + t * (p2.x - p1.x);
    long double y = p1.y + t * (p2.y - p1.y);

    cout << '\n' << x << ' ' << y;
}

bool onSegment(Point p1, Point p2, Point p3)
{
    return p3.x >= min(p1.x, p2.x) && p3.x <= max(p1.x, p2.x) && p3.y >= min(p1.y, p2.y) && p3.y <= max(p1.y, p2.y);
}

int main()
{
    Point p1, p2, p3, p4;
    cin >> p1.x >> p1.y >> p2.x >> p2.y;
    cin >> p3.x >> p3.y >> p4.x >> p4.y;

    cout << fixed;
    cout.precision(9);

    int ccw1 = ccw(p1, p2, p3);
    int ccw2 = ccw(p1, p2, p4);
    int ccw3 = ccw(p3, p4, p1);
    int ccw4 = ccw(p3, p4, p2);

    if (ccw1 * ccw2 <= 0 && ccw3 * ccw4 <= 0)
    {
        if (ccw1 == 0 && ccw2 == 0 && ccw3 == 0 && ccw4 == 0)
        {
            if (max(min(p1.x, p2.x), min(p3.x, p4.x)) <= min(max(p1.x, p2.x), max(p3.x, p4.x)) &&
                max(min(p1.y, p2.y), min(p3.y, p4.y)) <= min(max(p1.y, p2.y), max(p3.y, p4.y)))
            {
                cout << 1;

                Point start = {max(min(p1.x, p2.x), min(p3.x, p4.x)), max(min(p1.y, p2.y), min(p3.y, p4.y))};
                Point end = {min(max(p1.x, p2.x), max(p3.x, p4.x)), min(max(p1.y, p2.y), max(p3.y, p4.y))};

                if (start.x == end.x && start.y == end.y)
                {
                    cout << '\n' << start.x << ' ' << start.y;
                }
            }
            else
            {
                cout << 0;
            }
        }
        else
        {
            cout << 1;
            intersection(p1, p2, p3, p4);
        }
    }
    else
    {
        cout << 0;
    }

    return 0;
}