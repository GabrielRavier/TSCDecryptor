#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <cstring>

using std::cout;
using std::cin;
using std::ofstream;
using std::endl;
using std::string;


char Current_Script[1000000];

int __cdecl Get_File_Size(const char *path)
{
    int result;
    HANDLE hObject;

    hObject = CreateFileA(path, 0, 1u, NULL, 3u, 0x80u, NULL);
    if ( hObject == (HANDLE)-1 )
    {
        result = -1;
    }
    else
    {
        int File_Size = GetFileSize(hObject, NULL);
        CloseHandle(hObject);
        result = File_Size;
    }
    return result;
}

void __cdecl Decrypt_Encrypted_TSC_Script(char *pData, signed int size)
{
    int val1;
    signed int i;
    int half;

    half = size / 2;
    if ( pData[size / 2] )
        val1 = -(pData[half] % 256);
    else
        val1 = -7;
    for ( i = 0; i < size; ++i )
    {
        if ( i != half )
            pData[i] += val1;
    }
}

int main()
{
    string path;
    int head_size;
	int result = 0;

    cout << "Enter complete path to file : ";
    std::getline(cin, path);
    head_size = Get_File_Size(path.c_str());
    if ( head_size == -1 )
    {
        cout << "Get_File_Size failed" << endl;
        cout << GetLastError();
        result = -1;
    }
    else
    {
        FILE *stream = fopen(path.c_str(), "rb");
        if (stream)
        {
            fread(Current_Script, 1, head_size, stream);
            Decrypt_Encrypted_TSC_Script(Current_Script, head_size);
            Current_Script[head_size] = 0;
            cout << Current_Script;
            cout << "\nSave to file ? (1 for yes, 0 for no) ";
			bool decision;
            cin >> decision;
            if (decision)
            {
                cout << "Enter name for file : ";
                string path2;
                std::getline(cin, path2);
                ofstream MyFile(path2.c_str());
                if (!(MyFile))
                {
                    cout << "ERROR : Couldn't open file." << endl;
                    result = -2;
                }
                else
                {
                    MyFile << Current_Script;
                    cout << "Outputted TSC script to " << path2;
                }
            }
			fclose(stream);
        }
		else
		{
			cout << "ERROR : Couldn't open file." << endl;
			result = -2;
		}
    }
    return result;
}
