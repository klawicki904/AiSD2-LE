#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <cwctype> // dla towlower
#include <chrono>

using namespace std;
using namespace std::chrono;

enum SearchAlgorithm
{
    naive,
    rabin_karp,
    kmp,
    boyer_moore,
    trie
};

wstring toLower(const wstring& input) {
    wstring result = input;
    transform(result.begin(), result.end(), result.begin(), ::towlower);
    return result;
}

// --------- NAIWNY ----------
void naiveSearch(const wstring& text, const wstring& pattern) {
    size_t n = text.size(), m = pattern.size();
    if (m > n) return;
    for (size_t i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) ++j;
        // Wyniki nie s¹ wypisywane
    }
}

// --------- RABIN-KARP ----------
void rabinKarp(const wstring& text, const wstring& pattern) {
    size_t n = text.length(), m = pattern.length();
    if (m > n) return;
    const int d = 256;
    const int q = 101;

    int h = 1, p = 0, t = 0;
    for (int i = 0; i < (int)m - 1; i++)
        h = (h * d) % q;

    for (int i = 0; i < (int)m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (size_t i = 0; i <= n - m; i++) {
        if (p == t) {
            bool match = true;
            for (int j = 0; j < (int)m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
        }
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
}

// --------- KMP ----------
vector<size_t> computeLPS(const wstring& pattern) {
    size_t m = pattern.length(), len = 0;
    vector<size_t> lps(m, 0);
    for (int i = 1; i < (int)m;) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        }
        else {
            if (len != 0) len = lps[len - 1];
            else lps[i++] = 0;
        }
    }
    return lps;
}

void kmpSearch(const wstring& text, const wstring& pattern) {
    size_t n = text.size(), m = pattern.size(), i = 0, j = 0;
    if (m > n) return;
    vector<size_t> lps = computeLPS(pattern);

    while (i < n) {
        if (pattern[j] == text[i]) { i++; j++; }
        if (j == m) {
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
}

// --------- BOYER-MOORE ----------
vector<int> preprocessBadChar(const wstring& pattern) {
    vector<int> badChar(65536, -1);
    for (int i = 0; i < (int)pattern.size(); i++) {
        badChar[pattern[i]] = i;
    }
    return badChar;
}

void boyerMoore(const wstring& text, const wstring& pattern) {
    int n = text.size(), m = pattern.size();
    if (m > n) return;
    vector<int> badChar = preprocessBadChar(pattern);

    int shift = 0;

    while (shift <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;
        if (j < 0) {
            shift += (shift + m < n) ? m - badChar[text[shift + m]] : 1;
        }
        else {
            shift += max(1, j - badChar[text[shift + j]]);
        }
    }
}

// --------- TRIE ----------
struct TrieNode {
    unordered_map<wchar_t, TrieNode*> children;
    vector<int> positions;
};

class Trie {
public:
    TrieNode* root = new TrieNode();
    wstring pattern;

    void insert(const wstring& text) {
        for (size_t i = 0; i <= text.size() - pattern.size(); ++i) {
            TrieNode* node = root;
            for (size_t j = 0; j < pattern.size(); ++j) {
                wchar_t c = text[i + j];
                if (!node->children[c])
                    node->children[c] = new TrieNode();
                node = node->children[c];
            }
            node->positions.push_back(i + 1);
        }
    }

    void search(const wstring& pattern) const {
        TrieNode* node = root;
        for (wchar_t c : pattern) {
            if (!node->children.count(c))
                return;
            node = node->children.at(c);
        }
        for (int pos : node->positions) {
            // Wyniki nie s¹ wypisywane
        }
    }

    void clear(TrieNode* node = nullptr) {
        if (!node)
            node = root;
        for (auto& p : node->children) {
            clear(p.second);
            delete p.second;
        }
        node->children.clear();
    }

    ~Trie() {
        clear();
        delete root;
    }
};

int wmain() {
    setlocale(LC_ALL, "");

    // --- Ustawienia ---
    wstring fileName = L"large_text.txt";
    wstring pattern = L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    bool ignoreCase = true;
    vector<SearchAlgorithm> algorithms = { naive, rabin_karp, kmp, boyer_moore, trie };

    wifstream file(fileName);
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    if (!file.is_open()) {
        wcout << L"Nie znaleziono pliku: " << fileName << endl;
        return 1;
    }

    vector<wstring> lines;
    wstring line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    if (pattern.empty()) {
        wcout << L"Pusty wzorzec." << endl;
        return 0;
    }

    wstring patternToUse = ignoreCase ? toLower(pattern) : pattern;

    for (auto alg : algorithms) {
        wcout << L"----- Algorytm: ";
        switch (alg) {
        case naive: wcout << L"naiwny"; break;
        case rabin_karp: wcout << L"rabin-karp"; break;
        case kmp: wcout << L"kmp"; break;
        case boyer_moore: wcout << L"boyer-moore"; break;
        case trie: wcout << L"trie"; break;
        }
        wcout << L" -----" << endl;

        auto start = high_resolution_clock::now();

        if (alg == trie) {
            Trie trie;
            trie.pattern = patternToUse;
            for (const auto& l : lines) {
                wstring lineToUse = ignoreCase ? toLower(l) : l;
                trie.insert(lineToUse);
                trie.search(trie.pattern);
            }
        }
        else {
            for (const auto& l : lines) {
                wstring lineToUse = ignoreCase ? toLower(l) : l;
                switch (alg) {
                case naive: naiveSearch(lineToUse, patternToUse); break;
                case rabin_karp: rabinKarp(lineToUse, patternToUse); break;
                case kmp: kmpSearch(lineToUse, patternToUse); break;
                case boyer_moore: boyerMoore(lineToUse, patternToUse); break;
                default: break;
                }
            }
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        wcout << L"Czas wykonania (ms): " << duration.count() << endl << endl;
    }

    return 0;
}
