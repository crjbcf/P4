//
// Created by chris on 4/28/2023.
//

#ifndef P4_CODEGEN_H
#define P4_CODEGEN_H

#include "node.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void genCode(node_t* node, fstream&);
void initVar(string, fstream&);
void writeArg(string arg, fstream& outFile);
void initFile(fstream* of);
#endif //P4_CODEGEN_H
