#include <iostream>
#include <fstream>
#include <string> // getline()
#include <codecvt> // for wstring conversion

/// <summary>
/// launch parameters: "-n" file_to_read_path pattern_file_path
/// cout: only "line column" format
/// cerr: errors with show up in the GUI
/// </summary>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cerr << "Internal error (missing parameters for the external program)" << endl;
        return 1;
    }

    string inputPath = argv[2]; // path of the file to read
    ifstream file(inputPath);
    if (!file.is_open())
    {
        cerr << "Cannot open file: " << inputPath << endl;
        return 2;
    }

    ifstream patternFile(argv[3]); // path to the file with pattern
    if (!patternFile.is_open())
    {
        return 3;
    }
    string pattern;
    patternFile >> pattern; // save file with pattern to string
    if (pattern == "") // return normally when pattern empty
    {
        return 0;
    }
    
    wstring_convert<codecvt_utf8<wchar_t>> converter; // converter to wstring
    wstring wpattern = converter.from_bytes(pattern); // pattern in wide string
    int pattern_len = wpattern.length();

    string line;
    int line_number = 0;

    while (getline(file, line))
    {
        line_number++;
        // convert line to wide string (otherwise Polish characters count as 2 characters)
        wstring wline = converter.from_bytes(line);
        int line_len = wline.length();
        if (line_len == 0) { continue; } // skip empty lines in the input file

        // naive pattern search
        for (int i = 0; i <= line_len - pattern_len; ++i)
        {
            bool match = true;
            for (int j = 0; j < pattern_len; ++j)
            {
                if (wline[i + j] != wpattern[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                // print 1-indexed line and column number
                cout << line_number << " " << (i + 1) << endl;
            }
        }
    }

    file.close();
    return 0;
}
