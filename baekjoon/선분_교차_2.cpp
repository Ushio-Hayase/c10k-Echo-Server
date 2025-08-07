#include <iostream>

using namespace std;

int ccw(long long x1, long long y1, long long x2, long long y2, long long x3, long long y3)
{
    long long result = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    if (result == 0) return 0;
    return result > 0 ? 1 : -1;
}

bool on_line(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return (x3 <= max(x1, x2) && x3 >= min(x1, x2) && y3 <= max(y1, y2) && y3 >= min(y1, y2));
}

int main()
{
    int x1, x2, y1, y2, x3, y3, x4, y4;
    cin >> x1 >> y1 >> x2 >> y2;
    cin >> x3 >> y3 >> x4 >> y4;

    int ccw1 = ccw(x1, y1, x2, y2, x3, y3);
    int ccw2 = ccw(x1, y1, x2, y2, x4, y4);
    int ccw3 = ccw(x3, y3, x4, y4, x1, y1);
    int ccw4 = ccw(x3, y3, x4, y4, x2, y2);

    if (ccw1 * ccw2 < 0 && ccw3 * ccw4 < 0)
        cout << 1;
    else if ((x1 == x3 && y1 == y3) || (x2 == x3 && y2 == y3) || (x1 == x4 && y1 == y4) || (x2 == x4 && y2 == y4))
        cout << 1;
    else if (ccw1 == 0 && on_line(x1, y1, x2, y2, x3, y3))
        cout << 1;
    else if (ccw2 == 0 && on_line(x1, y1, x2, y2, x4, y4))
        cout << 1;
    else if (ccw3 == 0 && on_line(x3, y3, x4, y4, x1, y1))
        cout << 1;
    else if (ccw4 == 0 && on_line(x3, y3, x4, y4, x2, y2))
        cout << 1;
    else
        cout << 0;
}