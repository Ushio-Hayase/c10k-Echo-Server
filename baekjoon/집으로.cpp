#include <cmath>
#include <iostream>

using namespace std;

double X, Y, D, T;

int main()
{
    cout << fixed;
    cout.precision(9);

    cin >> X >> Y >> D >> T;
    double distance = sqrt(pow(X, 2) + pow(Y, 2));
    double ans = distance;

    for (int i = 1;; ++i)
    {
        if (i == 1)
        {
            distance = abs(D - distance);
            ans = min(ans, distance + T * i);
        }
        else
        {
            distance -= D;
            if (distance <= 0)
            {
                ans = min(ans, T * i);
                break;
            }
            ans = min(ans, abs(distance) + T * i);
        }
    }
    cout << ans;
}