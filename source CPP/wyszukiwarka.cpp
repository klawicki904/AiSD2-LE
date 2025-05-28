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

//pomocnicze
wstring toLower(const std::wstring& input) {
    std::wstring result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::towlower);
    return result;
}

wstring removePunctuation(const std::wstring& input) {
    std::wregex punct(LR"([.,:;!?()\[\]\"'])");
    return std::regex_replace(input, punct, L"");
}


// --------------------- NAIWNY -----------------------------
vector<int> naiveSearch(const wstring& text, const wstring& pattern) {
    vector<int> result;
    int n = text.size(), m = pattern.size();
    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) ++j;
        if (j == m) result.push_back(i);
    }
    return result;
}

// --------------------- RABIN-KARP -----------------------------
// Oblicza hash wzorca i porownuje go z hashami fragmentow tekstu tej samej dlugosci
// Jezeli hashe sie zgadzaja, robi dokladne porownanie znak po znaku
vector<int> rabinKarp(const wstring& text, const wstring& pattern) {
    const int d = 256;
    const int q = 101;
    vector<int> result;
    int n = text.length(), m = pattern.length();
    if (m > n) return result;

    int h = 1, p = 0, t = 0;

    for (int i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) result.push_back(i);
        }
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
    return result;
}

// --------------------- KMP -----------------------------
// Wstepnie analizuje wzorzec i tworzy tablice LPS (Longest Prefix Suffix)
//Jesli wiem, ze poczatek juz nie pasuje, nie sprawdzam go znowu
vector<int> computeLPS(const wstring& pattern) {
    int m = pattern.length(), len = 0;
    vector<int> lps(m, 0);
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

vector<int> kmpSearch(const wstring& text, const wstring& pattern) {
    vector<int> result;
    vector<int> lps = computeLPS(pattern);
    int n = text.size(), m = pattern.size(), i = 0, j = 0;

    while (i < n) {
        if (pattern[j] == text[i]) { i++; j++; }
        if (j == m) {
            result.push_back(i - j);
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return result;
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

vector<int> boyerMoore(const wstring& text, const wstring& pattern) {
    vector<int> result;
    vector<int> badChar = preprocessBadChar(pattern);
    int n = text.size(), m = pattern.size();
    int shift = 0;

    while (shift <= (n - m)) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;
        if (j < 0) {
            result.push_back(shift);
            shift += (shift + m < n) ? m - badChar[text[shift + m]] : 1;
        }
        else {
            shift += max(1, j - badChar[text[shift + j]]);
        }
    }
    return result;
}

// ------------------------- TRIE -----------------------------
// Przeszukiwanie odbywa sie znak po znaku w strukturze drzewa.
struct TrieNode {
    bool isEnd = false;
    unordered_map<wchar_t, TrieNode*> children;
};

class Trie {
public:
    TrieNode* root = new TrieNode();
    unordered_map<wstring, vector<int>> wordPositions;

    void insert(const wstring& word, int position) {
        TrieNode* node = root;
        for (wchar_t c : word) {
            if (!node->children[c]) node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->isEnd = true;
        wordPositions[word].push_back(position);
    }

    vector<int> search(const wstring& word) {
        TrieNode* node = root;
        for (wchar_t c : word) {
            if (!node->children[c]) return {};
            node = node->children[c];
        }
        if (node->isEnd) return wordPositions[word];
        return {};
    }
};

// --------------------- MAIN -----------------------------
int main() {
    //ustawiamy domyslne kodowanie systemu operacyjnego
    //konwertuje char na wchat_t zgodnie z lokalna tablica kodowania (czyli np utf-8)
    // CZYLI NIE WYPISUJE KRZACZKÓW LUB ?

    setlocale(LC_ALL, ""); //ustawia obsluge polskich znakow w konsoli
    wstring text;

    wifstream file("plik.txt"); // TU TRZEBA DAC ODPOWIEDNI PLIK Z ODPOWIEDZIA !!!!

    //zeby wifstream moglo czytac poprawnie plik zapisany jako utf-8
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>)); //otwieramy plik jako utf-8 i dekodujemy go jako wstring

    if (file) {
        wstringstream bufor;
        bufor << file.rdbuf();
        text = bufor.str();
    }
    else {
        wcout << "Nie znaleziono pliku 'plik.txt'. WorowadŸ tekst rêcznie:\n";
        getline(wcin, text);
    }


    wstring option, pattern;

    wcout << L"Wybierz algorytm (naiwny / rabin-karp / kmp / trie / boyer-moore): ";
    getline(wcin, option);

    wcout << L"Podaj slowo do wyszukania: ";
    getline(wcin, pattern);

    wstring cleanedText = toLower(removePunctuation(text));
    wstring cleanedPattern = toLower(removePunctuation(pattern));

    vector<int> positions;

    if (option == L"naiwny") {
        positions = naiveSearch(cleanedText, cleanedPattern);
    }
    else if (option == L"rabin-karp") {
        positions = rabinKarp(cleanedText, cleanedPattern);
    }
    else if (option == L"kmp") {
        positions = kmpSearch(cleanedText, cleanedPattern);
    }
    else if (option == L"boyer-moore") {
        positions = boyerMoore(cleanedText, cleanedPattern);
    }
    else if (option == L"trie") {
        Trie trie;
        wistringstream ss(cleanedText);
        wstring word;
        int pos = 0;

        while (ss >> word) {
            int found = text.find(word, pos);
            if (found != wstring::npos) {
                trie.insert(word, found);
                pos = found + word.length();
            }
        }
        positions = trie.search(cleanedPattern);
    }
    else {
        wcout << L"Nieznana opcja." << endl;
        return 1;
    }

    if (!positions.empty()) {
        for (int p : positions) {
            wcout << L"Znaleziono na pozycji: " << p << endl;
        }
    }
    else {
        wcout << L"Nie znaleziono wzorca.\n" << endl;
    }

    return 0;
}
