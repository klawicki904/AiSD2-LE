#include <iostream>
#include <queue>
#include <map>
#include <fstream>
#include <cmath>
#include <sstream>
using namespace std;

class LetterFrequency {
public:
    char letter;
    unsigned int howMany;
    LetterFrequency* left;
    LetterFrequency* right;
    LetterFrequency* parent;
    LetterFrequency(char letter, unsigned int howMany, LetterFrequency* left, LetterFrequency* right, LetterFrequency* parent) {
        this->letter = letter;
        this->howMany = howMany;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
};
class Comparator {
public:
    bool operator()(LetterFrequency* first, LetterFrequency* second) {
        return first->howMany > second->howMany;
    }
};

// zewnetrzny destruktor klasy LetterFrequency
void deleteNode(LetterFrequency* node) {
    if (node->left != nullptr) {
        LetterFrequency* temp = node->left;
        node->left = nullptr;
        deleteNode(temp);
    }
    if (node->right != nullptr) {
        LetterFrequency* temp = node->right;
        node->right = nullptr;
        deleteNode(temp);
    }
    //cout<<"Usunieto wierzcholek "<<node->letter<<" "<<node->howMany<<endl;
    delete node;
}

// przejscie po drzewie huffmana budujace kod
string findPathToRoot(LetterFrequency* currentLetter, LetterFrequency* previousLetter) {
    bool isThisLastLetter = false;
    if (currentLetter == previousLetter)return findPathToRoot(currentLetter->parent, currentLetter);
    if (currentLetter->parent == nullptr)isThisLastLetter = true;
    if (currentLetter->left == previousLetter && isThisLastLetter)return "0";
    if (currentLetter->right == previousLetter && isThisLastLetter)return "1";
    if (currentLetter->left == previousLetter)return findPathToRoot(currentLetter->parent, currentLetter) + "0";
    if (currentLetter->right == previousLetter)return findPathToRoot(currentLetter->parent, currentLetter) + "1";
}

// tworzymy kod na podstawie drzewa kodow huffmana
string createCode(map<char, pair<unsigned int, LetterFrequency*>>& characterMap, char letter) {
    return findPathToRoot(characterMap[letter].second, characterMap[letter].second);
}

// sluzy do budowy drzewa kodow huffmana ze stringa
LetterFrequency* readToTreeFromText(map<char, pair<unsigned int, LetterFrequency*>>& characterMap, string& text) {
    for (int i = 0; i < text.length(); i++)characterMap[text[i]].first++;
    priority_queue<LetterFrequency*, vector<LetterFrequency*>, Comparator> kolejka;
    for (auto it = characterMap.begin(); it != characterMap.end(); it++) {
        LetterFrequency* temp = new LetterFrequency(it->first, it->second.first, nullptr, nullptr, nullptr);
        kolejka.push(temp);
        it->second.second = temp;
    }

    while (kolejka.size() != 1) {
        LetterFrequency* first = kolejka.top();
        kolejka.pop();
        LetterFrequency* second = kolejka.top();
        kolejka.pop();
        LetterFrequency* temp = new LetterFrequency('*', first->howMany + second->howMany, first, second, nullptr);
        first->parent = temp;
        second->parent = temp;
        kolejka.push(temp);
    }
    return kolejka.top();
}

// zamiana z dziesietnego do ciagu znakow reprezentujacego zapis binarny
string decToBin(unsigned int code, short codeLength) {
    string result = "";
    for (int i = codeLength-1; i >= 0; i--) {
        if ((code >> i) & 1)result += "1";
        else result += "0";
    }
    return result;
}

// zamiana z ciagu znakow reprezentujacego zapis binarny do dziesietnego
unsigned int binToDec(string code){
    unsigned int result = 0;
    for (int i = 0; i < code.length(); i++) {
        result=result << 1;
        if(code[i]=='1')result=result|1;
    }
    return result;
}

// funckja ta zwraca ilosc bitow w w kodach huffmana potrzebnych do zapisania tekstu
int countBitsToWrite(map<char, pair<unsigned int, LetterFrequency*>>& characterMap, string& text) {
    int result=0;
    for (int i = 0; i < text.size(); i++)result += createCode(characterMap, text[i]).size();
    return result;
}

void compression(string filename, map<char, pair<unsigned int, LetterFrequency*>>& characterMap, string& text){
    ofstream outputFile(filename, ios::binary);

    // zapisuje liczbe wszystkich unikalnych znakow
    unsigned short letterN = characterMap.size();
    outputFile.write(reinterpret_cast<char*>(&letterN), sizeof(letterN));

    for (auto it = characterMap.begin(); it != characterMap.end(); it++) {
        // zapisuje znak w pliku
        outputFile.put(it->first);
        // tworze kod dla odpowiedniego znaku
        string code = createCode(characterMap, it->first);
        // zapisuje dlugosc kodu dla danego znaku
        short codeLength = code.length();
        outputFile.put(codeLength);
        // zapisuje kod znaku do pliku
        unsigned int codeBits = binToDec(code);
        outputFile.write(reinterpret_cast<char*>(&codeBits), sizeof(unsigned int));
    }
    // sprawdzam ile bajtow tekstu zapisze, aby moc poprawnie odczytac ostatni zapisany bajt
    unsigned int bytesToWrite = countBitsToWrite(characterMap, text);
    if (bytesToWrite % 8 == 0)bytesToWrite /= 8;
    else bytesToWrite = ceil((float)bytesToWrite / 8);
    outputFile.write(reinterpret_cast<char*>(&bytesToWrite), sizeof(bytesToWrite));

    // kompresja, staram sie upchac jak najwiecej bitow do bajta
    int k = 0;
    int letterCount = 0;
    unsigned char letter = 0;
    for (int i = 0; i < text.size(); i++) {
        string code = createCode(characterMap, text[i]);
        for (int j = 0; j < code.size(); j++) {
            k++;
            if (code[j] == '1') {
                letter = letter << 1;
                letter = letter | 1;
            }
            else letter = letter << 1;
            if (k % 8 == 0) {
                letterCount++;
                outputFile.put(letter);
                letter = 0;
            }
        }
    }
    // zapis ostatniego niedokonczonego bajta
    if (k % 8 != 0) {
        outputFile.put(letter);
        outputFile.put(k % 8);
        letterCount++;
    }
    outputFile.close();
}

// funkcja ta nie korzysta juz z klasy LetterFrequency
// zakladam ze sluzy tylko do odczytywania wiec nie ma sensu bawic sie w liczenie wystapien wszystkich znakow
// oraz budowanie drzewa huffmana na nowo
// interesuje nas tylko zkompresowana tresc
map<string,char> decompression(const string& filename) {
    ifstream inputFile(filename, ios::binary);
    if (!inputFile) {
        cerr << "Problem z otwieraniem pliku wejsciowego" << endl;
        return {};
    }
    map<string,char> codeMap;

    // analogicznie jak w funkcji kompresujacej odczytuje kolejne pola metadanych
    // czytam ilosc unikalnych liter
    unsigned short letterN;
    inputFile.read(reinterpret_cast<char*>(&letterN), sizeof(letterN));

    // w tej petli czytam znak, dlugosc jego kodu huffmana a nastepnie mapuje odczytany kod i zamieniam na stringa
    for (int i = 0; i < letterN; i++) {
        char character = inputFile.get();
        short codeLength = inputFile.get();
        unsigned int codeBits;
        inputFile.read(reinterpret_cast<char*>(&codeBits), sizeof(unsigned int));
        codeMap[decToBin(codeBits, codeLength)] = character;
    }

    string compressedText;
    char offsetChar;
    int offset;
    char byte;
    unsigned char uByte;
    string readByte;
    string result;
    unsigned int bytesToRead;
    bool isItLastLetter = false;
    int i;

    // czytam ilosc bajtow do odczytania
    inputFile.read(reinterpret_cast<char*>(&bytesToRead), sizeof(bytesToRead));

    // czytanie pliku
    // z jakiegos powodu byte nie moze byc unsigned (???)
    while (inputFile.get(byte)) {
        // sprawdzam czy to ostatni bajt, aby go poprawnie odczytac
        // ostatni bajt moze byc wypelniony zerami (zero-padding) co psuje odczyt danych i czytane sa niepoprawne kody (zera)
        // aby tego uniknac odczytuje offset, czyli ile interesujacych nas bitow zawiera ostatni bajt
        bytesToRead--;
        if (bytesToRead == 0) {
            inputFile.get(offsetChar);
            offset = (int)offsetChar;
        }
        if (bytesToRead == 0)i = offset - 1;
        else i = 7;

        // tak jak pisalem byte nie moze byc unsigned podczas czytania wiec go tu zamieniam
        uByte = byte;

        // dekompresja bajta, czytam bity i tworze z nich kody, jesli powstanie kod, to dodaje do result odpowiednia litere znajdujaca sie w mapie
        for (; i >= 0; i--) {
            if (uByte & (1 << i))readByte += "1";
            else readByte += "0";
            if (auto search = codeMap.find(readByte); search != codeMap.end()) {
                result += search->second;
                readByte = "";
            }
        }
    }

    inputFile.close();
    cout << endl<<"Odczytany tekst: ";
    cout << result << endl;
    return codeMap;
}

int main() {

    // czytanie z pliku surowego tekstu
    ifstream file("dane.txt");
    if (!file.is_open()) {
        cerr << "Problem z otwieraniem pliku wejsciowego" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string text = buffer.str();
    file.close();

    // tworzenie drzewa huffmana do kompresji
    map<char, pair<unsigned int, LetterFrequency*>> characterMap;
    LetterFrequency* letterTree = readToTreeFromText(characterMap, text);
    cout << "kody huffmana znakow: " << endl;
    for (auto it = characterMap.begin(); it != characterMap.end(); it++) {
        cout << it->first << " " << createCode(characterMap, it->first) << endl;
    }
    compression("output.txt", characterMap, text);

    // odczyt i dekompresja
    map<string, char> result = decompression("output.txt");

    // zwolnienie pamieci
    deleteNode(letterTree);
    return 0;
}
