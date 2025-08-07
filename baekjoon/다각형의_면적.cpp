#include <iostream>

using namespace std;

int N;
int arr[10000][2];

int main()
{
    cin >> N;

    for (int i = 0; i < N; ++i)
    {
        int x, y;
        cin >> x >> y;
        arr[i][0] = x;
        arr[i][1] = y;
    }

    double S = 0;
    for (int i = 0; i < N - 1; ++i)
    {
        S += (double)(arr[i][0] + arr[i + 1][0]) * (arr[i][1] - arr[i + 1][1]);
    }

    S += (double)(arr[N - 1][0] + arr[0][0]) * (arr[N - 1][1] - arr[0][1]);

    printf("%.1f", abs(S / 2.f));
}