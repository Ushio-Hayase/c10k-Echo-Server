#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int N;
int arr[10001];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> N;

    while (N--)
    {
        int x;
        cin >> x;
        arr[x]++;
    }

    for (int i = 1; i <= 10000; ++i)
    {
        while (arr[i])
        {
            cout << i << '\n';
            arr[i]--;
        }
    }
}