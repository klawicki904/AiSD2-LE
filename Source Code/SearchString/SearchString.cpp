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
/*Porownuje wzorzec od konca. Gdy znajdzie niedopasowanie, używa heurystyk przesunięcia, by pominąć jak najwięcej niepotrzebnych porównań.
*/
vector<int> preprocessBadChar(const wstring& pattern) {
    vector<int> badChar(65536, -1);
    for (int i = 0; i < pattern.size(); i++) {
        badChar[pattern[i]] = i;
    }
    return badChar;
}

vector<int> preprocessGoodSuffix(const wstring& pattern) {
    int m = pattern.size();
    vector<int> goodSuffix(m + 1, m);
    vector<int> border(m + 1, 0);

    int i = m, j = m + 1;
    border[i] = j;

    while (i > 0) {
        while (j <= m && pattern[i - 1] != pattern[j - 1]) {
            if (goodSuffix[j] == m)
                goodSuffix[j] = j - i;
            j = border[j];
        }
        i--;
        j--;
        border[i] = j;
    }

    j = border[0];
    for (i = 0; i <= m; i++) {
        if (goodSuffix[i] == m)
            goodSuffix[i] = j;
        if (i == j)
            j = border[j];
    }

    return goodSuffix;
}

void boyerMoore(const wstring& text, const wstring& pattern, int lineNumber) {
    int n = text.size(), m = pattern.size();
    if (m > n) return;

    vector<int> badChar = preprocessBadChar(pattern);
    vector<int> goodSuffix = preprocessGoodSuffix(pattern);

    int shift = 0;

    while (shift <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;
        if (j < 0) {
            cout << lineNumber << " " << shift + 1 << endl;
            shift += goodSuffix[0];
        }
        else {
            shift += max(goodSuffix[j + 1], j - badChar[text[shift + j]]);
        }
    }
}
/*
// ------------------------- TRIE -----------------------------
// Przeszukiwanie odbywa sie znak po znaku w strukturze drzewa.
// (jeden wezel triea to taka galaz w drzewie znakow)
struct TrieNode {
    unordered_map<wchar_t, TrieNode*> children; //mapowanie znakow na kolejne wezly
    vector<int> positions; //lista pozycji w linii, gdzie konczy sie wzorzec
};

class Trie {
public:
    TrieNode* root = new TrieNode(); //korzen trie

    wstring pattern; // nasz wzorzec ktory chcemy wyszukiwac

    //dodajemy cala linie tekstu do drzewa ale tylko podciagi o dlugosci takiej jak wzorzec
    void insert(const wstring& text, int lineNumber) {
        for (size_t i = 0; i <= text.size() - pattern.size(); ++i) {
            TrieNode* node = root;

            //dla każdego znaku wzorca tworzymy sciezke w drzewie
            for (size_t j = 0; j < pattern.size(); ++j) {
                wchar_t c = text[i + j];
                if (!node->children[c])
                    node->children[c] = new TrieNode();
                node = node->children[c];
            }

            //zapisujemy pozycje (indeks startu wzorca w linii)
            node->positions.push_back(i + 1); // +1 bo chcemy indeksy od 1, nie od 0
        }
    }

    //szuka wzorca w drzewie i wypisuje wszystkie zapisane pozycje
    void search(const wstring& pattern, int lineNumber) const {
        TrieNode* node = root;

        //przechodzimy przez wezly drzewa zgodnie ze znakami wzorca
        for (wchar_t c : pattern) {
            if (!node->children.count(c))
                return; //brak ścieżki czyli brak wzorca
            node = node->children.at(c);
        }

        //wypisujemy wszystkie znalezione pozycje wzorca w tej linii
        for (int pos : node->positions) {
            wcout << lineNumber << " " << pos << endl;
        }
    }

    //usuwanie wszystkich wezlow drzewa
    void clear(TrieNode* node = nullptr) {
        if (!node)
            node = root;
        for (auto& p : node->children) {
            clear(p.second);
            delete p.second;
        }
        node->children.clear();
    }

    //usuwanie drzewa
    ~Trie() {
        clear();
        delete root;
    }
};


*/

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

    if (option == L"-i") // sprawdza, czy pierwszy (opcjonalny) argument wystepuje
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
    /* else if (option == L"trie") {
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
            naiveSearch(ignoreCase ? toLower(line) : line, ignoreCase ? toLower(pattern) : pattern, lineNumber++);
        }
        break;
    }
    case rabin_karp:
    {
        while (getline(file, line))
        {
            rabinKarp(ignoreCase ? toLower(line) : line, ignoreCase ? toLower(pattern) : pattern, lineNumber++);
        }
        break;
    }
    case kmp:
    {
        while (getline(file, line))
        {
            kmpSearch(line, pattern, lineNumber++);
        }
        break;
    }
    case boyer_moore:
    {
        while (getline(file, line))
            boyerMoore(ignoreCase ? toLower(line) : line, ignoreCase ? toLower(pattern) : pattern, lineNumber++);
        break;
    }
    /*case trie:
    {
        Trie trie;
        trie.pattern = pattern; //ustawiamy wzorzec bo bedziemy wycinac slowa o odpowiedniej długosci

        while (getline(file, line)) {
            //przetwarzamy linie:
            wstring lineToUse = ignoreCase ? toLower(line) : line;

            //dodajemy wszystkie mozliwe slowa o długości wzorca z tej linii do drzewa
            trie.insert(lineToUse, lineNumber);

            //szukamy wystapien wzorca w tej linii i wypisujemy gdzie jest
            trie.search(ignoreCase ? toLower(pattern) : pattern, lineNumber);

            lineNumber++; //przechodzimy do nastepnej linii
        }
        break;
    }*/
    }


    return 0;
}