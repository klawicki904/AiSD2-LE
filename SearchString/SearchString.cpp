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

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 4)
    {
        cerr << "Internal error (missing parameters for the external program)" << endl;
        return 1;
    }

    wstring inputPath = argv[2]; // path of the file to read
    ifstream file(inputPath);
    if (!file.is_open())
    {
        wcerr << "Cannot open file: " << inputPath << endl;
        return 2;
    }

    wstring wpattern = argv[3];
    if (wpattern == L"") // return normally when pattern empty
    {
        return 0;
    }
    
    int pattern_len = wpattern.length();

    string line;
    int line_number = 0;

    wstring_convert<codecvt_utf8<wchar_t>> converter; // converter to wstring
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
