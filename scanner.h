//
// Created by chris on 4/26/2023.
//

#ifndef P4_SCANNER_H
#define P4_SCANNER_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "token.h"
using namespace std;


Token scanner(fstream &file, int&);
int** buildFSATable();
int characterLookup(int);
Token checkTokens(int, string);



#endif //P4_SCANNER_H
