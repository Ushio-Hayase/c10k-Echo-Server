#include <iostream>
#include <string>

using namespace std;

int main()
{
    string str;
    string explosion_str;

    cin >> str;
    cin >> explosion_str;

    string result;

    for (char& ch : str)
    {
        result.push_back(ch);
        if (result.length() < explosion_str.length()) continue;

        int idx = result.length() - explosion_str.length();

        if (result.substr(idx) == explosion_str)
        {
            for (int i = 0; i < explosion_str.length(); ++i) result.pop_back();
        }
    }

    if (result == "")
        cout << "FRULA";
    else
        cout << result;
}