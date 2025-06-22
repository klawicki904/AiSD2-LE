#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <bitset>
#include <cmath>
#include <locale>

#include <filesystem> // do zwracania rozmiaru plików

using namespace std;

ifstream::pos_type filesize(const char* filename)
{
    ifstream in(filename, ifstream::ate | ifstream::binary);
    return in.tellg();
}

//reprezentuje wierzcholek drzewa huffmana
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left, * right;

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    HuffmanNode(HuffmanNode* l, HuffmanNode* r) : ch(0), freq(l->freq + r->freq), left(l), right(r) {}
};

//ustawiamy drzewo wedlug najmniejszej czestotliwosci
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

class Huffman {
public:
    //buduje drzewo huffmana na podstawie czestotliwosci znakow
    void build(const string& data) {
        unordered_map<char, int> freq;
        for (char c : data) freq[c]++; //zliczanie czestotliwosci kazdego znaku
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

        for (auto it = freq.begin(); it != freq.end(); ++it)
            pq.push(new HuffmanNode(it->first, it->second));

        // tworzenie drzewa Huffmana
        while (pq.size() > 1) {
            HuffmanNode* l = pq.top(); pq.pop();
            HuffmanNode* r = pq.top(); pq.pop();
            pq.push(new HuffmanNode(l, r));
        }

        root = pq.top(); //mamy gotowe drzewo
        codes.clear();
        generateCodes(root, ""); // lecimy po drzewie i tworzymy kod binarny dla kazdego znaku
    }

    //koduje tekst na ciag bitow w postaci stringa
    string encode(const string& data) {
        string encoded;
        for (char c : data)
            encoded += codes[c];
        return encoded;
    }

    //dekoduje ciag bitow do oryginalnego tekstu
    string decode(const string& data) {
        string result;
        HuffmanNode* node = root;
        for (char bit : data) {
            node = (bit == '0') ? node->left : node->right;
            if (!node->left && !node->right) { //jak dotrzemy, to dodajemy znak
                result += node->ch;
                node = root; //wracamy na gore drzewa
            }
        }
        return result;
    }

    //zapisuje zakodowane dane oraz slownik kodow do pliku binarnego
    void writeToFile(const string& filename, const string& data) {
        ofstream output(filename, ios::binary);
        unsigned short symbolCount = codes.size();
        output.write(reinterpret_cast<char*>(&symbolCount), sizeof(symbolCount));

        //zapis slownika kodow (znak, dlugosc, kod binarny)
        for (auto it = codes.begin(); it != codes.end(); ++it) {
            char ch = it->first;
            const string& code = it->second;

            output.put(ch);
            uint8_t codeLength = static_cast<uint8_t>(code.size());
            output.put(codeLength);
            bitset<32> bits(code);
            unsigned int codeBits = static_cast<unsigned int>(bits.to_ulong());
            output.write(reinterpret_cast<char*>(&codeBits), sizeof(codeBits));
        }

        string encoded = encode(data);
        unsigned int bitLength = static_cast<unsigned int>(encoded.size());
        unsigned int byteLength = ceil(bitLength / 8.0);
        output.write(reinterpret_cast<char*>(&bitLength), sizeof(bitLength));

        //zapis zakodowanego tekstu bit po bicie
        unsigned char buffer = 0;
        int count = 0;
        for (char bit : encoded) {
            buffer <<= 1;
            if (bit == '1') buffer |= 1;
            count++;
            if (count == 8) {
                output.put(buffer);
                count = 0;
                buffer = 0;
            }
        }
        if (count > 0) {
            buffer <<= (8 - count);
            output.put(buffer);
            //output.put(static_cast<unsigned char>(count)); //liczba istotnych bitow w ostatnim bajcie
        }

        output.close();
    }

    //odczytuje dane z pliku binarnego i dekoduje tekst
    string readFromFile(const string& filename) {
        ifstream input(filename, ios::binary);
        if (!input) {
            cerr << "Nie mozna otworzyc pliku." << endl;
            return "";
        }

        unsigned short symbolCount;
        input.read(reinterpret_cast<char*>(&symbolCount), sizeof(symbolCount));

        unordered_map<string, char> reverseCodes;

        //wczytanie slownika kodow (odwrotnie: kod -> znak)
        for (int i = 0; i < symbolCount; ++i) {
            char ch = input.get();
            uint8_t length = static_cast<uint8_t>(input.get());
            unsigned int codeBits;
            input.read(reinterpret_cast<char*>(&codeBits), sizeof(codeBits));
            bitset<32> bits(codeBits);
            string code = bits.to_string().substr(32 - length); //bierzemy tylko potrzebna dlugosc
            reverseCodes[code] = ch;
        }

        unsigned int bitLength;
        input.read(reinterpret_cast<char*>(&bitLength), sizeof(bitLength));

        //odczyt zakodowanego ciagu bitow
        string bitStream;
        char byte;
        while (bitStream.size() < bitLength && input.get(byte)) {
            bitset<8> bits(static_cast<unsigned char>(byte));
            bitStream += bits.to_string();
        }

        bitStream = bitStream.substr(0, bitLength); //!!ucinamy nadmiar

        //dekodowanie bitow na oryginalny tekst
        string result, current;
        for (char bit : bitStream) {
            current += bit;
            if (reverseCodes.count(current)) {
                result += reverseCodes[current];
                current.clear();
            }
        }

        input.close();
        return result;
    }

    ~Huffman() {
        destroy(root);
    }

private:
    HuffmanNode* root = nullptr;
    unordered_map<char, string> codes;

    //generowanie kodow binarnych dla znakow
    void generateCodes(HuffmanNode* node, const string& prefix) {
        if (!node) return;
        if (!node->left && !node->right)
            codes[node->ch] = prefix;
        generateCodes(node->left, prefix + "0");
        generateCodes(node->right, prefix + "1");
    }

    //zwolnienie pamieci drzewa huffmana
    void destroy(HuffmanNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};

int main(int argc, char** argv) {
    //ustawienie lokalnej lokalizacji (dla poprawnej obslugi polskich znakow)
    locale::global(locale(""));

    bool compressFile = false;
    bool decompressFile = false;
    string inputFilePath = "";
    string outputFilePath = "";

    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " (-c or -d) <input_file> <output_file>" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];

        if (arg == "-c")
        {
            compressFile = true;
        }
        else if (arg == "-d")
        {
            decompressFile = true;
        }
        else if (arg[0] == '-')
        {
            cerr << "Unknown option: " << arg << endl;
            return 1;
        }
        else
        {
            if (inputFilePath == "")
            {
                inputFilePath = arg;
            }
            else
            {
                outputFilePath = arg;
            }
        }
    }
    // te b³êdy nie wyœwietl¹ siê w GUI
    if (!compressFile && !decompressFile) {
        cerr << "Error: use -c (compress) or -d (decompress)." << endl;
        return 1;
    }
    if (compressFile && decompressFile) {
        cerr << "Error: can't use -c and -d simultaneously." << endl;
        return 1;
    }
    if (inputFilePath.empty()) {
        cerr << "Missing input file path." << endl;
        return 2;
    }
    if (outputFilePath == "") {
        cerr << "Missing output file path." << endl;
        return 3;
    }
    // ten b³¹d jest obs³u¿ony ju¿ w GUI
    if (inputFilePath == outputFilePath)
    {
        cerr << "Input file path cannot be the same as output file path" << endl;
    }

    ifstream input(inputFilePath);
    if (!input) {
        cerr << "Nie mozna otworzyc pliku wejsciowego." << endl;
        return 1;
    }

    //wczytanie calego pliku do stringa
    string data((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    Huffman huff;

    if (compressFile)
    {
        huff.build(data); //budowanie drzewa huffmana
        huff.writeToFile(outputFilePath, data); //zapis zakodowanych danych do pliku
    }
    else if (decompressFile)
    {
        string decoded = huff.readFromFile(inputFilePath); //odczyt i dekodowanie
        //cout << decoded << endl;

        //zapis zdekodowanego tekstu do pliku tekstowego z polskimi znakami
        ofstream outputText(outputFilePath);
        outputText.imbue(locale("")); //zapewnia obsluge polskich znakow
        outputText << decoded;
        outputText.close();
    }

    cout << filesystem::file_size(inputFilePath) << " " << // rozmiar pliku wejœciowego
        filesystem::file_size(outputFilePath); // rozmiar pliku wyjœciowego


    return 0;
}
