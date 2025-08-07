#include <iostream>

using namespace std;

size_t N, M;

int main()
{
    cin >> M >> N;

    size_t a = ((N + M) * (N - M + 1)) / 2;
    size_t b = 0x1000000000000000U;

    int result = 0;

    for (int i = 0; i < 64; ++i)
    {
        if ((a & b) == b) result++;
        a << 1;
    }

    cout << result;
}