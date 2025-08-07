#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int N;
    cin >> N;
    vector<long long> arr(N + 1);

    for (int i = 1; i <= N; i++) cin >> arr[i];
    int ans = 0;

    for (int i = 1; i <= N; i++)
    {
        int cnt = 0;

        for (int j = 1; j < i; j++)
        {
            bool visible = true;
            for (int k = j + 1; k < i; k++)
            {
                if ((arr[i] - arr[j]) * (k - j) <= (arr[k] - arr[j]) * (i - j))
                {
                    visible = false;
                    break;
                }
            }
            if (visible) cnt++;
        }

        for (int j = i + 1; j <= N; j++)
        {
            bool visible = true;
            for (int k = i + 1; k < j; k++)
            {
                if ((arr[j] - arr[i]) * (k - i) <= (arr[k] - arr[i]) * (j - i))
                {
                    visible = false;
                    break;
                }
            }
            if (visible) cnt++;
        }

        ans = max(ans, cnt);
    }

    cout << ans << endl;
    return 0;
}