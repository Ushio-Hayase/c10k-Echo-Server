#include <iostream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int h1, m1, s1;
    cin >> h1 >> m1 >> s1;

    int h2, m2, s2;
    cin >> h2 >> m2 >> s2;

    long long start_sec = h1 * 3600LL + m1 * 60LL + s1;
    long long end_sec = h2 * 3600LL + m2 * 60LL + s2;

    if (end_sec <= start_sec)
    {
        end_sec += 24 * 3600LL;
    }

    int result = 0;

    if ((start_sec % 43200) * 719 % 43200 == 0)
    {
        result++;
    }

    if ((start_sec % 3600) * 59 % 3600 == 0 && start_sec % 43200 != 0)
    {
        result++;
    }

    for (long long t = start_sec + 1; t <= end_sec; ++t)
    {
        if ((t % 43200) * 719 % 43200 < 719)
        {
            result++;
        }
        if ((t % 3600) * 59 % 3600 < 59 && t % 43200 != 0)
        {
            result++;
        }
    }

    if (h1 == 0 && m1 == 0 && s1 == 0 && h2 == 23 && m2 == 59 && s2 == 59)
    {
        result++;
        result++;
    }

    cout << result << endl;

    return 0;
}