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

#include <cwctype> //dla tolower

#include <regex>

using namespace std;

//L"" - dla unicode szerokiego
//u8"" - dla utf-8 windows

enum SearchAlgorithm
{
    unknown,
    naive,
    rabin_karp,
    kmp,
    boyer_moore,
    //trie
};

//pomocnicze
wstring toLower(const std::wstring& input) {
    std::wstring result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::towlower);
    return result;
}

// --------------------- NAIWNY -----------------------------
void naiveSearch(const wstring& text, const wstring& pattern, int lineNumber) {
    //vector<size_t> result;
    size_t n = text.size(), m = pattern.size();
    if (m > n) return; // pomin za krotkie linie
    for (size_t i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) ++j;
        if (j == m) { cout << lineNumber << " " << i + 1 << endl; }
    }
    //return result;
}

// --------------------- RABIN-KARP -----------------------------
// Oblicza hash wzorca i porownuje go z hashami fragmentow tekstu tej samej dlugosci
// Jezeli hashe sie zgadzaja, robi dokladne porownanie znak po znaku
void rabinKarp(const wstring& text, const wstring& pattern, int lineNumber) {
    size_t n = text.length(), m = pattern.length();
    if (m > n) return;
    const int d = 256;
    const int q = 101;

    int h = 1, p = 0, t = 0;

    for (int i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (size_t i = 0; i <= n - m; i++) {
        if (p == t) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) { cout << lineNumber << " " << i + 1 << endl; }
        }
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
}

// --------------------- KMP -----------------------------
// Wstepnie analizuje wzorzec i tworzy tablice LPS (Longest Prefix Suffix)
//Jesli wiem, ze poczatek juz nie pasuje, nie sprawdzam go znowu
vector<size_t> computeLPS(const wstring& pattern) {
    size_t m = pattern.length(), len = 0;
    vector<size_t> lps(m, 0);
    for (int i = 1; i < m;) {
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

void kmpSearch(const wstring& text, const wstring& pattern, int lineNumber) {
    size_t n = text.size(), m = pattern.size(), i = 0, j = 0;
    if (m > n) return;
    vector<size_t> lps = computeLPS(pattern);

    while (i < n) {
        if (pattern[j] == text[i]) { i++; j++; }
        if (j == m) {
            { cout << lineNumber << " " << i - j + 1 << endl; }
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
}

// --------------------- BOYER-MOORE -----------------------------
/*Porownuje wzorzec od konca.
Jesli wystapi niedopasowanie, przesuwa wzorzec bardziej inteligentnie niz o 1 znak.
Uzywa tablicy Bad Character Heuristic, czyli: jak daleko mo¿na przesunac wzorzec w zaleznosci od niedopasowanego znaku.*/
vector<int> preprocessBadChar(const wstring& pattern) {
    vector<int> badChar(65536, -1);             //wchar_t ma taki range
    for (int i = 0; i < pattern.size(); i++) {
        badChar[pattern[i]] = i;
    }
    return badChar;
}

void boyerMoore(const wstring& text, const wstring& pattern, int lineNumber) {
    int n = text.size(), m = pattern.size();
    if (m > n) return;
    vector<int> badChar = preprocessBadChar(pattern);
    
    int shift = 0;
    
    while (shift <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;
        if (j < 0) {
            cout << lineNumber << " " << shift + 1 << endl;
            shift += (shift + m < n) ? m - badChar[text[shift + m]] : 1;
        }
        else {
            shift += max(1, j - badChar[text[shift + j]]);
        }
    }
}

// ------------------------- TRIE -----------------------------
// Przeszukiwanie odbywa sie znak po znaku w strukturze drzewa.
//struct TrieNode {
//    bool isEnd = false;
//    unordered_map<wchar_t, TrieNode*> children;
//};
//
//class Trie {
//public:
//    TrieNode* root = new TrieNode();
//    unordered_map<wstring, vector<size_t>> wordPositions;
//
//    void insert(const wstring& word, size_t position) {
//        TrieNode* node = root;
//        for (wchar_t c : word) {
//            if (!node->children[c]) node->children[c] = new TrieNode();
//            node = node->children[c];
//        }
//        node->isEnd = true;
//        wordPositions[word].push_back(position);
//    }
//
//    vector<size_t> search(const wstring& word) {
//        TrieNode* node = root;
//        for (wchar_t c : word) {
//            if (!node->children[c]) return {};
//            node = node->children[c];
//        }
//        if (node->isEnd) return wordPositions[word];
//        return {};
//    }
//};

// --------------------- MAIN -----------------------------
int wmain(int argc, wchar_t* argv[]) {
    //ustawiamy domyslne kodowanie systemu operacyjnego
    //konwertuje char na wchat_t zgodnie z lokalna tablica kodowania (czyli np utf-8)
    // CZYLI NIE WYPISUJE KRZACZKÓW LUB ?
    setlocale(LC_ALL, ""); //ustawia obsluge polskich znakow w konsoli

    if (argc < 4)
    {
        wcout << "Brakuje parametrów \n";
        wcout << "Uzycie: <algorytm> <sciezka do pliku> <wzorzec> \n";
        return 1;
    }
    bool ignoreCase = false;
    wstring option = argv[1], pattern = argv[3];
    wstring fileName = argv[2];

    if (option == L"-i") // sprawdza, czy pierwszy (opcjonalny) argument wystêpuje
    {
        ignoreCase = true;
        option = argv[2]; fileName = argv[3]; pattern = argv[4];
    }
    wifstream file(fileName);

    //zeby wifstream moglo czytac poprawnie plik zapisany jako utf-8
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>)); //otwieramy plik jako utf-8 i dekodujemy go jako wstring

    if (!file.is_open()) {
        wcout << "Nie znaleziono pliku " << argv[2] << endl;
        return 2;
    }

    if (pattern == L"") // nie trzeba szukac wzorca jesli wzorzec jest pusty
    {
        return 0;
    }

    SearchAlgorithm selectedAlgorithm = unknown;
    if (option == L"naiwny") {
        selectedAlgorithm = naive;
    }
    else if (option == L"rabin-karp") {
        selectedAlgorithm = rabin_karp;
    }
    else if (option == L"kmp") {
        selectedAlgorithm = kmp;
    }
    else if (option == L"boyer-moore") {
        selectedAlgorithm = boyer_moore;
    }
    /*else if (option == L"trie") {
        selectedAlgorithm = trie;
    }*/
    else {
        wcout << L"Nieznana opcja dla wyboru algorytmu." << endl;
        return 1;
    }
    wstring line;
    int lineNumber = 1;
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    switch (selectedAlgorithm)
    {
    case unknown:
    {
        return 3;
    }
    case naive:
    {
        while (getline(file, line))
        {
            if (ignoreCase)
            {
                naiveSearch(toLower(line), pattern, lineNumber++);
            }
            else
            {
                naiveSearch(line, pattern, lineNumber++);
            }
        }
        break;
    }
    case rabin_karp:
    {
        while (getline(file, line))
        {
            if (ignoreCase)
            {
                rabinKarp(toLower(line), pattern, lineNumber++);
            }
            else
            {
                rabinKarp(line, pattern, lineNumber++);
            }
        }
        break;
    }
    case kmp:
    {
        while (getline(file, line))
        {
            if (ignoreCase)
            {
                kmpSearch(toLower(line), pattern, lineNumber++);
            }
            else
            {
                kmpSearch(line, pattern, lineNumber++);
            }
        }
        break;
    }
    case boyer_moore:
    {
        while (getline(file, line))
        {
            if (ignoreCase)
            {
                boyerMoore(toLower(line), pattern, lineNumber++);
            }
            else
            {
                boyerMoore(line, pattern, lineNumber++);
            }
        }
        break;
    }
    /*case trie:
    {
        wstringstream bufor;
        bufor << file.rdbuf();
        wstring text = bufor.str();
        Trie trie;
        wistringstream ss(text);
        wstring word;
        size_t pos = 0;

        while (ss >> word) {
            size_t found = text.find(word, pos);
            if (found != wstring::npos) {
                trie.insert(word, found);
                pos = found + word.length();
            }
        }
        break;
    }*/
    }
    return 0;
}
