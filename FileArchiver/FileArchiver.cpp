#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

/// <summary>
/// Jeœli pojawiaj¹ siê b³êdy, trzeba zmieniæ wersjê C++ na min. C++17:
/// Projekt > W³aœciwoœci > C/C++ > Jêzyk > Standard Jêzyka C++
/// 
/// na cerr daæ to, co ma siê wyœwietliæ w GUI na czerwono (jak chcecie polskie znaki, to trzeba u¿yæ wcout << L"..."; , ale proponujê po angielsku a ja to przet³umaczê w GUI)
/// 
/// 4 argumenty uruchomienia:
/// 1. "-c" lub "-d" - -c kompresja, -d dekompresja
/// 2. "-x" opcjonalny, usuñ plik wejœciowy po pomyœlnym zakoñczeniu operacji
/// 3. œcie¿ka do pliku czytanego
/// 4. œcie¿ka do pliku skompresowanego/zdekompresowanego
/// </summary>

using namespace std;

ifstream::pos_type filesize(const char* filename)
{
    ifstream in(filename, ifstream::ate | ifstream::binary);
    return in.tellg();
}

int main(int argc, char* argv[])
{
    bool compressFile = false;
    bool decompressFile = false;
    bool deleteInputFile = false;
    string inputFilePath = "";
    string outputFilePath = "";

    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " (-c or -d) [-x] <input_file> <output_file>" << endl;
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
        else if (arg == "-x")
        {
            deleteInputFile = true;
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

    /// kompresja/dekompresja...

    // na koniec, po udanej kompresji wypisaæ TYLKO 2 liczby w jednej linii oddzielone spacj¹:
    cout << filesystem::file_size(inputFilePath) << " " << // rozmiar pliku wejœciowego
        filesystem::file_size(outputFilePath); // rozmiar pliku wyjœciowego

    // dopiero po wypisaniu usun¹æ plik, jeœli trzeba

    return 0;
}
