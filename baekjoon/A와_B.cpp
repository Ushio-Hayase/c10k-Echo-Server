#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int N;

int main()
{
    string src, dst;

    cin >> src >> dst;

    while (src.compare(dst) != 0)
    {
        if (dst[dst.size() - 1] == 'A')
        {
            dst.pop_back();
        }
        else
        {
            dst.pop_back();
            reverse(dst.begin(), dst.end());
        } 
        if (dst.size() == src.size())
        {
            if (dst == src)
                cout << 1;
            else
                cout << 0;
            break;
        }
    }
}