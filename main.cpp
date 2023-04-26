#include <iostream>
#include <fstream>
#include "parser.h"
#include "treePrint.h"
#include "staticSem.h"
using namespace std;

int main(int argc, char* argv[])
{
    string filename = "C:/Users/chris/CLionProjects/P4/random.txt";
    int level = 0;
    fstream inputFile;

    inputFile.open(filename);
//    if (argc > 2)
//    {
//        cout << "Error: arguments should be no more than 2.\n"
//             << "P1 file1 file2\nFatal: Improper usage\nUsage: P0 [filename]\nPlease restart program, terminating....";
//        exit(0);
//    }
//    else if (argc == 2) //file provided
//    {
//        filename = argv[1];
//        inputFile.open(filename, ios::in); //attempt to open file
//    }
//    else if (argc == 1) //no file provided take user input.
//    {
//        ofstream outFile;
//        outFile.open("userInput.txt");
//        string input;
//
//        if (outFile)
//        {
//            cout << "Enter your input (Type Ctrl+D when done): ";
//            while (getline(cin, input))
//            {
//                outFile << input;
//            }
//
//            outFile.close();
//
//            inputFile.open("userInput.txt", ios::in);//attempt to open new file created with userInput
//        }
//    }



    if (inputFile)
    {
        //cout << "File opened successfully\n";

        if(inputFile.get()== std::char_traits<char>::eof())
        {
            std::cout << "Error: File is Empty\nTerminating program...." << std:: endl;
            exit(0);
        }
        else
        {
            //cout << "file not empty\n";
            inputFile.unget();
        }

        node_t* parseTree = parser(inputFile); //needs to be replaced with parser.
        traversePreorder(parseTree, level);
        staticSem(parseTree);
        printTable();
    }
    else
    {
        cout << "Error in opening the file";
    }

    inputFile.close();

    return 0;
}
