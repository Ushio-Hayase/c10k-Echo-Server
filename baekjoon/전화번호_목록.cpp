#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class TrieNode
{
   public:
    TrieNode(char v) : value(v) {}

    void insert(string& str, size_t idx)
    {
        if (idx == str.length() - 1) is_finish = true;

        char current_char = str[idx];

        if (nodes.find(current_char) == nodes.end()) nodes[current_char] = new TrieNode(current_char);

        if (!is_finish) nodes[current_char]->insert(str, idx + 1);
    }

    bool find(string& str, size_t idx)
    {
        char current_char = str[idx];
        if (idx == str.length() - 1) return nodes.find(current_char) != nodes.end();

        if (nodes.find(current_char) == nodes.end()) return false;

        return nodes[current_char]->find(str, idx + 1);
    }

    bool remove(string& str, size_t idx)
    {
        char current_char = str[idx];
        if (idx == str.length() - 1) return is_finish;

        if (nodes.find(current_char) != nodes.end() && nodes[current_char]->remove(str, idx + 1))
        {
            delete nodes[current_char];
            nodes.erase(current_char);
            if (nodes.empty()) return true;
        }

        return false;
    }

   private:
    std::map<char, TrieNode*> nodes;
    char value;
    bool is_finish = false;
};

class Trie
{
   public:
    Trie() : root(' ') {}

    void insert(string str) { root.insert(str, 0); }

    bool find(string str) { return root.find(str, 0); }

    void remove(string str) { root.remove(str, 0); }

   private:
    TrieNode root;
};

int N, T;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> T;

    while (T--)
    {
        bool result = true;
        cin >> N;
        Trie strs;
        vector<string> cont(N);
        for (int i = 0; i < N; ++i)
        {
            string tmp;
            cin >> tmp;
            cont[i] = tmp;
        }

        sort(cont.begin(), cont.end(), [](string& left, string& right) { return left.length() > right.length(); });

        for (int i = 0; i < N; ++i)
        {
            if (strs.find(cont[i]))
            {
                result = false;
                break;
            }
            strs.insert(cont[i]);
        }

        cout << (result ? "YES" : "NO") << '\n';
    }
}