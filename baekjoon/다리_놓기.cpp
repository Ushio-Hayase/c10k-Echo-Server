#include <cmath>
#include <iostream>

using namespace std;

size_t dp[31];

size_t factorial(size_t x)
{
    if (x == 0) return 1;
    if (dp[x] != 0) return dp[x];
    return dp[x] = x * factorial(x - 1);
}

int main()
{
    size_t T, N, M;
    std::cin >> T;
    dp[1] = 1;
    dp[2] = 2;

    while (T--)
    {
        std::cin >> N >> M;
        std::cout << factorial(M) / (factorial(N) * factorial(M - N)) << '\n';
    }
}