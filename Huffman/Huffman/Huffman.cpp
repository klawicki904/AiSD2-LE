#include <iostream>
#include <queue>
#include <map>
#include <fstream>
#include <bitset>
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

string findPathToRoot(LetterFrequency* currentLetter, LetterFrequency* previousLetter) {
    bool isThisLastLetter = false;
    if (currentLetter == previousLetter)return findPathToRoot(currentLetter->parent, currentLetter);
    if (currentLetter->parent == nullptr)isThisLastLetter = true;
    if (currentLetter->left == previousLetter && isThisLastLetter)return "0";
    if (currentLetter->right == previousLetter && isThisLastLetter)return "1";
    if (currentLetter->left == previousLetter)return findPathToRoot(currentLetter->parent, currentLetter) + "0";
    if (currentLetter->right == previousLetter)return findPathToRoot(currentLetter->parent, currentLetter) + "1";
}

string createCode(map<char, pair<unsigned int, LetterFrequency*>>& characterMap, char letter) {
    return findPathToRoot(characterMap[letter].second, characterMap[letter].second);
}

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

int countBytesToWrite(map<char, pair<unsigned int, LetterFrequency*>>& characterMap, string& text) {
    int result=0;
    for (int i = 0; i < text.size(); i++)result += createCode(characterMap, text[i]).size();
    return result;
}

void writeToFile(string filename, map<char, pair<unsigned int, LetterFrequency*>>& characterMap, string& text) {
    ofstream outputFile(filename, ios::binary);

    // liczba wszystkich liter
    unsigned short letterN = characterMap.size();
    outputFile.write(reinterpret_cast<char*>(&letterN), sizeof(letterN));

    for (auto it = characterMap.begin(); it != characterMap.end(); it++) {
        // tworze kod dla odpowiedniej litery
        string code = createCode(characterMap, it->first);
        // zapisuje litere w pliku
        outputFile.put(it->first);
        // zapisuje dlugosc kodu znaku
        uint8_t codeLength = code.size();
        outputFile.put(codeLength);
        // zapisuje kod znaku do pliku
        unsigned int codeBits = bitset<32>(code).to_ulong();
        outputFile.write(reinterpret_cast<char*>(&codeBits), sizeof(unsigned int));
    }
    unsigned int bytesToWrite = countBytesToWrite(characterMap, text);
    if (bytesToWrite % 8 == 0)bytesToWrite /= 8;
    else bytesToWrite = ceil((float)bytesToWrite / 8);
    outputFile.write(reinterpret_cast<char*>(&bytesToWrite), sizeof(bytesToWrite));
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
    if (k % 8 != 0) {
        outputFile.put(letter);
        outputFile.put(k % 8);
        letterCount++;
    }
    outputFile.close();
}

map<string, char> readFromFile(const string& filename) {
    ifstream inputFile(filename, ios::binary);
    if (!inputFile) {
        cerr << "Problem z otwieraniem pliku wejsciowego" << endl;
        return {};
    }

    map<string, char> codeMap;
    short letterN;
    inputFile.read(reinterpret_cast<char*>(&letterN), sizeof(letterN));

    for (size_t i = 0; i < letterN; ++i) {
        char character = inputFile.get();
        uint8_t codeLength = inputFile.get();
        unsigned int codeBits;
        inputFile.read(reinterpret_cast<char*>(&codeBits), sizeof(unsigned int));
        bitset<32> bits(codeBits);
        codeMap[bits.to_string().substr(32 - codeLength)] = character;
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
    inputFile.read(reinterpret_cast<char*>(&bytesToRead), sizeof(bytesToRead));
    int i;
    while (inputFile.get(byte)) {
        bytesToRead--;
        if (bytesToRead == 0) {
            inputFile.get(offsetChar);
            offset = (int)offsetChar;
        }
        uByte = byte;
        if (bytesToRead == 0)i = offset - 1;
        else i = 7;
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
    cout << result << endl;
    return codeMap;
}

int main() {
    ifstream file("dane.txt");
    if (!file.is_open()) {
        cerr << "Problem z otwieraniem pliku wejsciowego" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string text = buffer.str();
    file.close();

    map<char, pair<unsigned int, LetterFrequency*>> characterMap;
    LetterFrequency* letterTree = readToTreeFromText(characterMap, text);

    writeToFile("output.txt", characterMap, text);
    map<string, char> result = readFromFile("output.txt");

    deleteNode(letterTree);

    return 0;
}
