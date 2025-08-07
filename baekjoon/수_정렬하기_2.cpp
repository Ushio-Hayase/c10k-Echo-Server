#include <algorithm>
#include <iostream>

using namespace std;

int N;
int arr[1'000'000];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;

    for (int i = 0; i < N; ++i) cin >> arr[i];

    sort(arr, arr + N);

    for (int i = 0; i < N; ++i) cout << arr[i] << "\n";
}