#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

/// <summary>
/// Je�li pojawiaj� si� b��dy, trzeba zmieni� wersj� C++ na min. C++17:
/// Projekt > W�a�ciwo�ci > C/C++ > J�zyk > Standard J�zyka C++
/// 
/// na cerr da� to, co ma si� wy�wietli� w GUI na czerwono (jak chcecie polskie znaki, to trzeba u�y� wcout << L"..."; , ale proponuj� po angielsku a ja to przet�umacz� w GUI)
/// 
/// 4 argumenty uruchomienia:
/// 1. "-c" lub "-d" - -c kompresja, -d dekompresja
/// 2. "-x" opcjonalny, usu� plik wej�ciowy po pomy�lnym zako�czeniu operacji
/// 3. �cie�ka do pliku czytanego
/// 4. �cie�ka do pliku skompresowanego/zdekompresowanego
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
    // te b��dy nie wy�wietl� si� w GUI
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
    // ten b��d jest obs�u�ony ju� w GUI
    if (inputFilePath == outputFilePath)
    {
        cerr << "Input file path cannot be the same as output file path" << endl;
    }

    /// kompresja/dekompresja...

    // na koniec, po udanej kompresji wypisa� TYLKO 2 liczby w jednej linii oddzielone spacj�:
    cout << filesystem::file_size(inputFilePath) << " " << // rozmiar pliku wej�ciowego
        filesystem::file_size(outputFilePath); // rozmiar pliku wyj�ciowego

    // dopiero po wypisaniu usun�� plik, je�li trzeba

    return 0;
}
